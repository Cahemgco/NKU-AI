import os
import gym
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from torch.distributions import Categorical
from gym import wrappers
from time import time

hidden_size = 1024          # 神经元个数
update_timestep = 1200      # 每多少补跟新策略
lr = 1e-4                   # learning rate
eps = 1e-4
gamma = 0.99                # discount factor
K_epochs = 4                # update policy for K epochs
eps_clip = 0.2              # clip parameter for PPO  论文中表明0.2效果不错

random_seed = 42
torch.random.manual_seed(random_seed)
np.random.seed(random_seed)

# 检查是否有可用的GPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
env = gym.make('LunarLander-v2', render_mode='human')

# 录制视频
# env = gym.make('LunarLander-v2', render_mode='rgb_array_list')
# env = wrappers.RecordVideo(env,video_folder='./videos')

observation_space = env.observation_space.shape[0]
action_space = env.action_space.n

class Memory:
    def __init__(self):
        self.actions = []
        self.states = []
        self.logprobs = []
        self.rewards = []
        self.is_terminals = []

    def clear_memory(self):
        del self.actions[:]
        del self.states[:]
        del self.logprobs[:]
        del self.rewards[:]
        del self.is_terminals[:]

class ActorCriticDiscrete(nn.Module):
    def __init__(self, input_size, output_size, hidden_size):
        super(ActorCriticDiscrete, self).__init__()

        # actor
        self.action_layer = nn.Sequential(
                nn.Linear(input_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, output_size),
                nn.Softmax(dim=-1)
                )

        # critic
        self.value_layer = nn.Sequential(
               nn.Linear(input_size, hidden_size),
               nn.ReLU(),
               nn.Linear(hidden_size, 1)
               )

    def act(self, state, memory):
        state = torch.from_numpy(state).float()
        action_probs = self.action_layer(state)
        dist = Categorical(action_probs)
        action = dist.sample()

        memory.states.append(state)
        memory.actions.append(action)
        memory.logprobs.append(dist.log_prob(action))

        return action.item()

    def evaluate(self, state, action):
        action_probs = self.action_layer(state)
        dist = Categorical(action_probs)

        action_logprobs = dist.log_prob(action)
        dist_entropy = dist.entropy()

        state_value = self.value_layer(state)

        return action_logprobs, torch.squeeze(state_value), dist_entropy

class PPOAgent:
    def __init__(self, input_size, output_size, hidden_size, lr, eps, gamma, K_epochs, eps_clip):
        self.lr = lr
        self.gamma = gamma
        self.eps_clip = eps_clip
        self.K_epochs = K_epochs
        self.timestep = 0
        self.memory = Memory()

        self.policy = ActorCriticDiscrete(input_size, output_size, hidden_size)
        self.optimizer = torch.optim.Adam(self.policy.parameters(), lr=lr, eps=eps)
        self.policy_old = ActorCriticDiscrete(input_size, output_size, hidden_size)
        self.policy_old.load_state_dict(self.policy.state_dict())

        self.MseLoss = nn.MSELoss()

    def update(self):   
        # Monte Carlo estimate of state rewards:
        rewards = []
        discounted_reward = 0
        for reward, is_terminal in zip(reversed(self.memory.rewards), reversed(self.memory.is_terminals)):
            if is_terminal:
                discounted_reward = 0
            discounted_reward = reward + (self.gamma * discounted_reward)
            rewards.insert(0, discounted_reward)

        # Normalizing the rewards:
        rewards = torch.tensor(rewards, dtype=torch.float32)
        rewards = (rewards - rewards.mean()) / (rewards.std() + 1e-5)

        # convert list to tensor
        old_states = torch.stack(self.memory.states).detach()
        old_actions = torch.stack(self.memory.actions).detach()
        old_logprobs = torch.stack(self.memory.logprobs).detach()

        # Optimize policy for K epochs:
        for _ in range(self.K_epochs):
            # Evaluating old actions and values : 新策略 重用 旧样本进行训练 
            logprobs, state_values, dist_entropy = self.policy.evaluate(old_states, old_actions)

            # Finding the ratio (pi_theta / pi_theta__old): 
            ratios = torch.exp(logprobs - old_logprobs.detach())

            # Finding Surrogate Loss:计算优势值
            advantages = rewards - state_values.detach()
            surr1 = ratios * advantages ###  重要性采样的思想，确保新的策略函数和旧策略函数的分布差异不大
            surr2 = torch.clamp(ratios, 1-self.eps_clip, 1+self.eps_clip) * advantages ### 采样clip的方式过滤掉一些新旧策略相差较大的样本
            loss = -torch.min(surr1, surr2)  + 0.5*self.MseLoss(state_values, rewards) - 0.01*dist_entropy

            # take gradient step
            self.optimizer.zero_grad()
            loss.mean().backward()
            self.optimizer.step()

        # Copy new weights into old policy:
        self.policy_old.load_state_dict(self.policy.state_dict())

    def step(self, reward, done):
        self.timestep += 1 
        # Saving reward and is_terminal:
        self.memory.rewards.append(reward)
        self.memory.is_terminals.append(done)

        # update if its time
        if self.timestep % update_timestep == 0:
            self.update()
            self.memory.clear_memory()
            self.timstamp = 0

    def act(self, state):
        return self.policy_old.act(state, self.memory)

    def save_checkpoint(self, directory, episode):
        if not os.path.exists(directory):
            os.makedirs(directory)
        filename = os.path.join(directory, 'checkpoint_{}.pth'.format(episode))
        torch.save(self.policy.state_dict(), f=filename)
        print('保存当前模型至 \'{}\''.format(filename))
        
    def load_checkpoint(self, directory, filename):
        self.policy.load_state_dict(torch.load(os.path.join(directory, filename)))
        self.policy_old.load_state_dict(torch.load(os.path.join(directory, filename)))
        print('加载模型 \'{}\'.'.format(filename))
        return int(filename[11:-4])


# 加载训练好的权重
save_directory = "./ppo_weight"
load_filename = 'checkpoint_2500.pth'
agent = PPOAgent(observation_space ,action_space,hidden_size,lr,eps,gamma,K_epochs,eps_clip)
agent.load_checkpoint(save_directory, load_filename)

# 测试
total_reward = 0
observation = env.reset()[0]
while True:
    env.render()
    action = agent.act(observation)
    observation, reward, done, _, _ = env.step(action)
    total_reward += reward
    if done:
        print("Total reward:", total_reward)
        break

env.close()
