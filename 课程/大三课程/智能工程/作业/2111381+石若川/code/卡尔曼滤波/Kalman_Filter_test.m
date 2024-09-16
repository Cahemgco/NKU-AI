clc;clear;close all

t = linspace(0, 100, 101); % 在1~100s内采样100次
size = length(t);

% 设置噪声的协方差矩阵
Q = [0.1, 0; 0, 0.1]; % 过程噪声的协方差矩阵
R = [5, 0; 0, 5]; % 观测噪声的协方差矩阵

% 初始化
X = [0; 0]; % 估计的初始状态
P = [0.001, 0; 0, 0.001]; % 误差协方差矩阵的初始值
real_positions = zeros(2, size);
real_positions(:,1) = X;
X_true = X;

% 已知的线性变换矩阵（B=0）
A = [1, 0; 0, 1]; % 状态转移矩阵
B = [1;1];  % 控制矩阵
u = 1;  % 输入
H = [1, 0; 0, 1]; % 观测矩阵

% 实际观测值，通过理论值加上观测噪声模拟获得
measure_positions = zeros(2, size);
measure_positions(:,1) = real_positions(:,1) + [randn(1) * sqrt(R(1,1)); randn(1) * sqrt(R(2,2))]; % 初始值

% 最优估计值
optim_positions = zeros(2, size);
optim_positions(:,1) = measure_positions(:,1);  % 初始值
%%
% 卡尔曼滤波
for i = 2:length(t)
    w = [randn(1) * sqrt(Q(1,1)); randn(1) * sqrt(Q(2,2))]; % 过程噪声
    v = [randn(1) * sqrt(R(1,1)); randn(1) * sqrt(R(2,2))]; % 观测噪声

    % 计算实际值
    X_true = A * X_true + B * u + w;
    Z = H * X_true + v;

    % 计算先验
    X_ = A * X + B * u;
    P_ = A * P * A' + Q;

    % 修正
    K = P_ * H' * inv(H * P_ * H' + R);
    X = X_ + K * (Z - H * X_);
    P = (eye(2) - K * H ) * P_;

    % 记录结果
    real_positions(:, i) = X_true;
    optim_positions(:,i) = X;
    measure_positions(:,i) = Z;
end

% 轨迹
figure;
plot(real_positions(1,:), real_positions(2,:), 'DisplayName', 'real positions');
hold on;
scatter(measure_positions(1,:), measure_positions(2,:), 'DisplayName', 'measured positions','Marker','.');
plot(optim_positions(1,:), optim_positions(2,:), 'DisplayName', 'kalman filtered positions');
legend('Location','southeast');
grid on;
xlabel("X方向位置")
ylabel("Y方向位置")
xlim([-5,105])
ylim([-5,105])
hold off;
magnify;

% X方向
figure;
plot(t, real_positions(1,:), 'DisplayName', 'real positions');
hold on;
scatter(t, measure_positions(1,:), 'DisplayName', 'measured positions','Marker','.');
plot(t, optim_positions(1,:), 'DisplayName', 'kalman filtered positions');
legend('Location','southeast');
grid on;
xlabel("时间t")
ylabel("X方向位置")
xlim([0,100])
ylim([-5,105])
hold off;
magnify;

% Y方向
figure;
plot(t, real_positions(2,:), 'DisplayName', 'real positions');
hold on;
scatter(t, measure_positions(2,:), 'DisplayName', 'measured positions','Marker','.');
plot(t, optim_positions(2,:), 'DisplayName', 'kalman filtered positions');
legend('Location','southeast');
grid on;
xlabel("时间t")
ylabel("Y方向位置")
xlim([0,100])
ylim([-5,105])
hold off;
magnify;
