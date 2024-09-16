clear;
close all;
clc;
clear L;
%% 参数定义 
m1 = 2.4312;    m2 = 3.7860;    m3 = 0.5552;
l1 = 0.250;     l2 = 0.250;     l3 = 0.245;
r1 = 0.11860;   r2 = 0.14925;   r3 = 0;
i1 = 0.03219;   i2 = 0.03376;

%% DH法建立模型
L(1)= Link('d', 0, 'a', 0, 'alpha', 0,'modified', ...
    'I', [0 0 0;
          0 0 0;
          0 0 i1], ...
    'r', [r1 0 0], ...
    'm', m1, ...
    'qlim', [-pi,pi] );
L(2)= Link('d', 0, 'a', l1, 'alpha', 0,'modified', ...
    'I', [0 0 0;
          0 0 0;
          0 0 i2], ...
    'r', [r2 0 0], ...
    'm', m2, ...
    'qlim', [-pi,pi] );
L(3)= Link('theta', 0, 'a', l2, 'alpha', 0,'modified', ...
    'r', [r3 0 0], ...
    'm', m3, ...
    'qlim', [0, 0.2] );

RRP_robot = SerialLink(L(1:3),'name','RRP');
RRP_robot.plotopt = {'workspace',[-1,1,-1,1,-1,1]};

%% 阻抗控制器
% 定义控制参数
Kx=[500 0 0; 0 500 0;0 0 500];
Kb=[50 0 0; 0 50 0; 0 0 30];

% 时间
tf = 5;
step = 0.01;
t = 0:step:tf;

% 关节初始位置和期望位置、速度、加速度
q0 = [0; 0; 0];
qd = [deg2rad(30); deg2rad(60); 0.15];
dqd = [0; 0; 0];

% 关节位置、速度和加速度的初值
q = zeros(3,length(t));
q_dot = zeros(3,length(t));
q_ddot = zeros(3,length(t));

% 初始条件
q(:, 1) = q0;

% 重力矩阵
G = [0; 0; m3 * 9.81];

% % 控制力矩
% tau_list = [];

% 求实际跟踪位置和速度
[t, x] = ode45(@(t, x) impedance_control_add_force(t, x, RRP_robot, qd, dqd, Kx, Kb), 0:0.01:10, zeros(6, 1));
% [t, x] = ode45(@(t, x) PD_control_add_force(t, x, RRP_robot, qd, dqd, Kx, Kb), 0:0.01:10, zeros(6, 1));
% 求实际加速度
a = zeros(length(x),3);
for i = 1 : length(x)
    if i==1
        a(i,:) = x(i, 4:6)/step;
    else
        a(i,:) = (x(i, 4:6)-x(i-1, 4:6))/step;
    end
end
%% 求控制量
tau = zeros(length(x),3);

for i = 1 : length(x)
    M = RRP_robot.inertia(x(i,1:3));          % 质量矩阵
    C = RRP_robot.coriolis(x(i,1:3), x(i,4:6));      % 离心力和科氏力矩阵
    J = RRP_robot.jacob0(x(i,1:3));
    Kq = J(1:3,:)'*Kx*J(1:3,:);
    Kw = J(1:3,:)'*Kb*J(1:3,:);
    tau(i,:) = Kq*(qd-x(i, 1:3)') + Kw*(dqd-x(i, 4:6)') + G;
end

%% 绘制笛卡尔空间位置曲线
p = [];
for i = 1 : length(x)
    % 计算当前的机械臂末端位姿
    T = RRP_robot.fkine([x(i,1), x(i,2), x(i,3)]);
    p = [p,T];
end
% tranimate(p) % 动图

p_ = double(p);
p_ = p_(:,4,:);
p_ = squeeze(p_);

x_ = p_(1,:);
y_ = p_(2,:);
z_ = p_(3,:);

figure;
plot(t,x_);
formatPlot();
hold on
plot(t,y_);
formatPlot();
hold on
plot(t,z_);
formatPlot();
xlabel('时间（s）')
ylabel('位置（m）')
ylim([0,0.6])
legend('X','Y','Z')
formatPlot();

%% 实际跟踪结果
figure('Name','Q1');
plot(t, x(:,1));
xlabel('时间（s）')
ylabel('弧度（rad）')
ylim([-0.2,1.5])
formatPlot();

figure('Name','Q2');
plot(t, x(:,2));
xlabel('时间（s）')
ylabel('弧度（rad）')
ylim([-0.5, 1.5])
formatPlot();

figure('Name','Q3');
plot(t, x(:,3));
xlabel('时间（s）')
ylabel('位置（m）')
ylim([0,0.165])
formatPlot();

figure('Name','QD1');
plot(t, x(:,4));
xlabel('时间（s）')
ylabel('角速度（rad/s）')
formatPlot();

figure('Name','QD2');
plot(t, x(:,5));
xlabel('时间（s）')
ylabel('角速度（rad/s）')
formatPlot();

figure('Name','QD3');
plot(t, x(:,6));
xlabel('时间（s）')
ylabel('线速度（m/s）')
ylim([-0.2,2])
formatPlot();

figure('Name','QDD1');
plot(t, a(:,1));
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
formatPlot();

figure('Name','QDD2');
plot(t, a(:,2));
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
formatPlot();

figure('Name','QDD3');
plot(t, a(:,3));
xlabel('时间（s）')
ylabel('线加速度（m/s^2）')
formatPlot();

figure('Name','tau1');
plot(t, tau(:,1));
xlabel('时间（s）')
ylabel('力矩（N \cdot m）')
formatPlot();

figure('Name','tau2');
plot(t, tau(:,2));
xlabel('时间（s）')
ylabel('力矩（N \cdot m）')
formatPlot();

figure('Name','tau3');
plot(t, tau(:,3));
xlabel('时间（s）')
ylabel('力（N）')
formatPlot();