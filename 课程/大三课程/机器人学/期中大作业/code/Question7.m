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
%% 轨迹规划
% 关节空间
ratio = pi/180; % 角度转弧度
q0 = [0 0 0]';   % 初始位姿
qmid = [20*ratio 40*ratio 0.1]';    % 中间点位姿
qf = [30*ratio 60*ratio 0.15]';     % 终点位姿
t0 = 0;
tmid = 5;
tf = 10;
%%  求解三次函数 中间点速度、加速度连续的情况
A = [1 0 0 0 0 0 0 0;
    0 0 0 0 1 0 0 0;
    1 1 1 1 0 0 0 0;
    0 0 0 0 1 1 1 1;
    0 1 0 0 0 0 0 0;
    0 0 0 0 0 1 2 3;
    0 1 2 3 0 -1 0 0;
    0 0 2 6 0 0 -2 0];
b = [q0';qmid';qmid';qf';0 0 0;0 0 0;0 0 0;0 0 0];
a = A \ b;
a1 = a(:,1);    % 关节1
a2 = a(:,2);    % 关节2
a3 = a(:,3);    % 关节3

step = 0.01;
time = 0:step:tf;
% t = linspace(0,tf,tf/step);
% 0-5s
T1 = time/tmid;
% 位移
y11 = a1(1)+a1(2)*T1+a1(3)*T1.^2+a1(4)*T1.^3;  % 关节1
y21 = a2(1)+a2(2)*T1+a2(3)*T1.^2+a2(4)*T1.^3;  % 关节2
y31 = a3(1)+a3(2)*T1+a3(3)*T1.^2+a3(4)*T1.^3;  % 关节3
% 速度
v11 = (a1(2)+2*a1(3)*T1+3*a1(4)*T1.^2)/tmid;
v21 = (a2(2)+2*a2(3)*T1+3*a2(4)*T1.^2)/tmid;
v31 = (a3(2)+2*a3(3)*T1+3*a3(4)*T1.^2)/tmid;
% 加速度
a11 = (2 * a1(3) + 6 * a1(4) * T1)/tmid^2;
a21 = (2 * a2(3) + 6 * a2(4) * T1)/tmid^2;
a31 = (2 * a3(3) + 6 * a3(4) * T1)/tmid^2;

% 5-10s
T2 = (time-tmid)/(tf-tmid);
% 位移
y12= a1(5)+a1(6)*T2+a1(7)*T2.^2+a1(8)*T2.^3;  % 关节1
y22 = a2(5)+a2(6)*T2+a2(7)*T2.^2+a2(8)*T2.^3;  % 关节2
y32 = a3(5)+a3(6)*T2+a3(7)*T2.^2+a3(8)*T2.^3;  % 关节3
% 速度
v12 = (a1(6)+2*a1(7)*T2+3*a1(8)*T2.^2)/(tf-tmid);
v22 = (a2(6)+2*a2(7)*T2+3*a2(8)*T2.^2)/(tf-tmid);
v32 = (a3(6)+2*a3(7)*T2+3*a3(8)*T2.^2)/(tf-tmid);
% 加速度
a12 = (2 * a1(7) + 6 * a1(8) * T2)/(tf-tmid)^2;
a22 = (2 * a2(7) + 6 * a2(8) * T2)/(tf-tmid)^2;
a32 = (2 * a3(7) + 6 * a3(8) * T2)/(tf-tmid)^2;

% 合并0-5s和5-10s
% 位移
q1 = [y11(1:5/step),y12(5/step+1:end)];
q2 = [y21(1:5/step),y22(5/step+1:end)];
q3 = [y31(1:5/step),y32(5/step+1:end)];

% 速度
qd1 = [v11(1:5/step),v12(5/step+1:end)];
qd2 = [v21(1:5/step),v22(5/step+1:end)];
qd3 = [v31(1:5/step),v32(5/step+1:end)];
% 加速度
qdd1 = [a11(1:5/step),a12(5/step+1:end)];
qdd2 = [a21(1:5/step),a22(5/step+1:end)];
qdd3 = [a31(1:5/step),a32(5/step+1:end)];

%% 逆动力学控制器
% 跟踪轨迹
qd = [q1;q2;q3];
dqd = [qd1;qd2;qd3];
ddqd = [qdd1;qdd2;qdd3];

% 定义控制参数
K0=[1.5 0 0; 0 1.5 0;0 0 1.5];
K1=[2 0 0; 0 2 0; 0 0 2];

% 求实际跟踪位置和速度
[t, x] = ode45(@(t, x) inverse_dynamic(t, x, qd, dqd, ddqd, K0, K1), 0:0.01:tf, zeros(6, 1));

% 求实际加速度
a = zeros(length(x),3);
for i = 1 : length(x)
    if i==1
        a(i,:) = x(i, 4:6)/0.01;
    else
        a(i,:) = (x(i, 4:6)-x(i-1, 4:6))/0.01;
    end
end
%% 求控制量
eta = zeros(length(x),3);
tau = zeros(length(x),3);
G = [0;0;m3 * 9.81];
for i = 1 : length(x)
    eta(i,:) = K0 * (qd(:, i)-x(i, 1:3)') + K1 * (dqd(:,i)-x(i, 4:6)') + ddqd(:,i);
end

for i = 1 : length(x)
    G = RRP_robot.gravload(qd(:,i)');         % 重力矢量
    M = RRP_robot.inertia(qd(:,i)');          % 质量矩阵
    C = RRP_robot.coriolis(qd(:,i)', dqd(:,i)');      % 离心力和科氏力矩阵
    tau(i,:) = (M*eta(i,:)')' + (C*x(i, 4:6)')' + G;
end

%% 实际跟踪结果

figure('Name','Q1');
plot(t, x(:,1));
xlabel('时间（s）')
ylabel('弧度（rad）')
xlim([0,10]);
hold on;
plot(t, q1);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','Q2');
plot(t, x(:,2));
xlabel('时间（s）')
ylabel('弧度（rad）')
xlim([0,10]);
hold on;
plot(t, q2);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','Q3');
plot(t, x(:,3));
xlabel('时间（s）')
ylabel('位移（m）')
xlim([0,10]);
hold on;
plot(t, q3);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QD1');
plot(t, x(:,4));
xlabel('时间（s）')
ylabel('角速度（rad/s）')
xlim([0,10]);
hold on;
plot(t, qd1);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QD2');
plot(t, x(:,5));
xlabel('时间（s）')
ylabel('角速度（rad/s）')
xlim([0,10]);
hold on;
plot(t, qd2);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QD3');
plot(t, x(:,6));
xlabel('时间（s）')
ylabel('线速度（m/s）')
xlim([0,10]);
hold on;
plot(t, qd3);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QDD1');
plot(t, a(:,1));
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
xlim([0,10]);
hold on;
plot(t, qdd1);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QDD2');
plot(t, a(:,2));
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
xlim([0,10]);
hold on;
plot(t, qdd2);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','QDD3');
plot(t, a(:,3));
xlabel('时间（s）')
ylabel('线加速度（m/s）')
xlim([0,10]);
hold on;
plot(t, qdd3);
formatPlot();
legend("控制结果","跟踪信号")
hold off;

figure('Name','tau1');
plot(t, tau(:,1));
xlabel('时间（s）')
ylabel('力矩（N \cdot m）')
xlim([0,10]);
formatPlot();

figure('Name','tau2');
plot(t, tau(:,2));
xlabel('时间（s）')
ylabel('力矩（N \cdot m）')
xlim([0,10]);
formatPlot();

figure('Name','tau3');
plot(t, tau(:,1));
xlabel('时间（s）')
ylabel('力（N）')
xlim([0,10]);
formatPlot();