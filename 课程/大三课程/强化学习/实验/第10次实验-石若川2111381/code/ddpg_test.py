import os
import gym
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from gym import wrappers
from time import time
from torch.distributions import Normal
from collections import deque, namedtuple
import random

# random_seed = 42
# torch.manual_seed(random_seed)
# np.random.seed(random_seed)

# 检查是否有可用的GPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
env = gym.make('LunarLanderContinuous-v2', render_mode='human')

# 录制视频
# env = gym.make('LunarLanderContinuous-v2', render_mode='rgb_array_list')
# env = wrappers.RecordVideo(env, video_folder='./videos')

observation_space = env.observation_space.shape[0]  # 状态空间
action_space = env.action_space.shape[0]  # 动作空间
action_bound_low = env.action_space.low
action_bound_high = env.action_space.high
action_bound_tensor = [torch.tensor(action_bound_low, device=device), torch.tensor(action_bound_high, device=device)]


class ReplayBuffer:
    def __init__(self, buffer_size, batch_size):
        self.memory = deque(maxlen=buffer_size)
        self.batch_size = batch_size
        self.experience = namedtuple("Experience", field_names=["state", "action", "reward", "next_state", "done"])

    def add(self, state, action, reward, next_state, done):
        e = self.experience(state, action, reward, next_state, done)
        self.memory.append(e)

    def sample(self):
        experiences = random.sample(self.memory, k=self.batch_size)

        states = torch.from_numpy(np.vstack([e.state for e in experiences if e is not None])).float().to(device)
        actions = torch.from_numpy(np.vstack([e.action for e in experiences if e is not None])).float().to(device)
        rewards = torch.from_numpy(np.vstack([e.reward for e in experiences if e is not None])).float().to(device)
        next_states = torch.from_numpy(np.vstack([e.next_state for e in experiences if e is not None])).float().to(device)
        dones = torch.from_numpy(np.vstack([e.done for e in experiences if e is not None]).astype(np.uint8)).float().to(device)

        return (states, actions, rewards, next_states, dones)

    def __len__(self):
        return len(self.memory)


class Actor(nn.Module):
    def __init__(self, input_size, output_size, hidden_size):
        super(Actor, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, output_size)
        self.reset_parameters()

    def reset_parameters(self):
        self.fc1.weight.data.uniform_(*hidden_init(self.fc1))
        self.fc2.weight.data.uniform_(*hidden_init(self.fc2))
        self.fc3.weight.data.uniform_(-3e-3, 3e-3)

    def forward(self, state):
        x = F.relu(self.fc1(state))
        x = F.relu(self.fc2(x))
        return torch.tanh(self.fc3(x))


class Critic(nn.Module):
    def __init__(self, input_size, output_size, hidden_size):
        super(Critic, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size + output_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, 1)
        self.reset_parameters()

    def reset_parameters(self):
        self.fc1.weight.data.uniform_(*hidden_init(self.fc1))
        self.fc2.weight.data.uniform_(*hidden_init(self.fc2))
        self.fc3.weight.data.uniform_(-3e-3, 3e-3)

    def forward(self, state, action):
        xs = F.relu(self.fc1(state))
        x = torch.cat((xs, action), dim=1)
        x = F.relu(self.fc2(x))
        return self.fc3(x)


def hidden_init(layer):
    fan_in = layer.weight.data.size()[0]
    lim = 1. / np.sqrt(fan_in)
    return (-lim, lim)


class DDPGAgent:
    def __init__(self, input_size, action_size, hidden_size, lr_actor, lr_critic, gamma, tau, buffer_size, batch_size):
        self.gamma = gamma
        self.tau = tau
        self.memory = ReplayBuffer(buffer_size, batch_size)

        self.actor_local = Actor(input_size, action_size, hidden_size).to(device)
        self.actor_target = Actor(input_size, action_size, hidden_size).to(device)
        self.actor_optimizer = optim.Adam(self.actor_local.parameters(), lr=lr_actor)

        self.critic_local = Critic(input_size, action_size, hidden_size).to(device)
        self.critic_target = Critic(input_size, action_size, hidden_size).to(device)
        self.critic_optimizer = optim.Adam(self.critic_local.parameters(), lr=lr_critic, weight_decay=0.0001)

        self.noise = OUNoise(action_size)

        self.update_targets(1.0)  # 硬更新目标网络

    def act(self, state, add_noise=True):
        state = torch.from_numpy(state).float().to(device)
        self.actor_local.eval()
        with torch.no_grad():
            action = self.actor_local(state).cpu().data.numpy()
        self.actor_local.train()
        if add_noise:
            action += self.noise.sample()
        return np.clip(action, action_bound_low, action_bound_high)

    def step(self, state, action, reward, next_state, done):
        self.memory.add(state, action, reward, next_state, done)

        if len(self.memory) > self.memory.batch_size:
            experiences = self.memory.sample()
            self.learn(experiences)

    def learn(self, experiences):
        states, actions, rewards, next_states, dones = experiences

        # 更新Critic
        actions_next = self.actor_target(next_states)
        Q_targets_next = self.critic_target(next_states, actions_next)
        Q_targets = rewards + (self.gamma * Q_targets_next * (1 - dones))
        Q_expected = self.critic_local(states, actions)
        critic_loss = F.mse_loss(Q_expected, Q_targets)
        self.critic_optimizer.zero_grad()
        critic_loss.backward()
        self.critic_optimizer.step()

        # 更新Actor
        actions_pred = self.actor_local(states)
        actor_loss = -self.critic_local(states, actions_pred).mean()
        self.actor_optimizer.zero_grad()
        actor_loss.backward()
        self.actor_optimizer.step()

        # 软更新目标网络
        self.update_targets(self.tau)

    def update_targets(self, tau):
        for target_param, local_param in zip(self.actor_target.parameters(), self.actor_local.parameters()):
            target_param.data.copy_(tau * local_param.data + (1.0 - tau) * target_param.data)
        for target_param, local_param in zip(self.critic_target.parameters(), self.critic_local.parameters()):
            target_param.data.copy_(tau * local_param.data + (1.0 - tau) * target_param.data)

    def save_checkpoint(self, directory, episode):
        if not os.path.exists(directory):
            os.makedirs(directory)
        filename = os.path.join(directory, 'checkpoint_{}.pth'.format(episode))
        torch.save({
            'actor_state_dict': self.actor_local.state_dict(),
            'critic_state_dict': self.critic_local.state_dict()
        }, filename)
        print('保存当前模型至 \'{}\''.format(filename))

    def load_checkpoint(self, directory, filename):
        checkpoint = torch.load(os.path.join(directory, filename))
        self.actor_local.load_state_dict(checkpoint['actor_state_dict'])
        self.critic_local.load_state_dict(checkpoint['critic_state_dict'])
        print('重新开始训练 checkpoint \'{}\'.'.format(filename))
        return int(filename.split('_')[1].split('.')[0])


class OUNoise:
    def __init__(self, size, mu=0., theta=0.15, sigma=0.2):
        self.mu = mu * np.ones(size)
        self.theta = theta
        self.sigma = sigma
        self.size = size
        self.reset()

    def reset(self):
        self.state = np.ones(self.size) * self.mu

    def sample(self):
        x = self.state
        dx = self.theta * (self.mu - x) + self.sigma * np.random.randn(len(x))
        self.state = x + dx
        return self.state


# 加载训练好的权重
hidden_size = 256  # 神经元个数
lr_actor = 1e-4  # Actor网络的学习率

lr_critic = 1e-3           # Critic网络的学习率
gamma = 0.99               # 折扣因子
tau = 1e-3                 # 软更新参数
buffer_size = int(1e6)     # 经验回放缓冲区大小
batch_size = 128           # 批量大小
save_directory = "./ddpg_weight"
load_filename = 'checkpoint_200.pth'
model = DDPGAgent(observation_space, action_space, hidden_size, lr_actor, lr_critic, gamma, tau, buffer_size, batch_size)
model.load_checkpoint(save_directory, load_filename)

# 测试
total_reward = 0
state = env.reset()[0]
while True:
    env.render()
    action = model.act(state)
    next_state, reward, done, _, _ = env.step(action)
    model.step(state, action, reward, next_state, done)
    state = next_state
    total_reward += reward
    if done:
        print("Reward:", total_reward)
        break

env.close()
