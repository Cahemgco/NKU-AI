import os
import gym
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from torch.distributions import Categorical
from gym import wrappers
from time import time

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


class LunarLanderSolver:
    def __init__(self, hidden_size, input_size, output_size):
        self.model = torch.nn.Sequential(
            nn.Linear(input_size, hidden_size, bias=False),
            nn.ReLU(),
            nn.Linear(hidden_size, output_size, bias=False),
            nn.Softmax(dim=-1)
        ).to(device)  # 将模型移动到 GPU 上

    def forward(self, x):
        return self.model(x)

    def load_checkpoint(self, directory, filename):
        self.model.load_state_dict(torch.load(os.path.join(directory, filename)))
        print('Loading weights from checkpoint \'{}\'.'.format(filename))

    def action(self, observation):
        observation = torch.tensor(observation, dtype=torch.float32, device=device)
        action_probs = self.forward(observation)
        distribution = Categorical(action_probs)
        action = distribution.sample()
        return action.item()


# 加载训练好的权重
save_directory = "./weight"
load_filename = 'checkpoint_25000.pth'
model = LunarLanderSolver(hidden_size=1024, input_size=observation_space, output_size=action_space)
model.load_checkpoint(save_directory, load_filename)

# 测试
total_reward = 0
observation = env.reset()[0]
while True:
    env.render()
    action = model.action(observation)
    observation, reward, done, _, _ = env.step(action)
    total_reward += reward
    if done:
        print("Reward:", total_reward)
        break

env.close()
