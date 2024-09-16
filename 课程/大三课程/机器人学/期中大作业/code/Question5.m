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
t = linspace(0,tf,tf/step);
% 0-5s
T1 = t/tmid;
% 位移
y11 = a1(1)+a1(2)*T1+a1(3)*T1.^2+a1(4)*T1.^3;  % 关节1
y21 = a2(1)+a2(2)*T1+a2(3)*T1.^2+a2(4)*T1.^3;  % 关节2
y31 = a3(1)+a3(2)*T1+a3(3)*T1.^2+a3(4)*T1.^3;  % 关节3
% 速度
v11 = (a1(2)+2*a1(3)*T1+3*a1(4)*T1.^2)/tmid;
v21 = (a2(2)+2*a2(3)*T1+3*a2(4)*T1.^2)/tmid;
v31 = (a3(2)+2*a3(3)*T1+3*a3(4)*T1.^2)/tmid;
% 5-10s
T2 = (t-tmid)/(tf-tmid);
% 位移
y12= a1(5)+a1(6)*T2+a1(7)*T2.^2+a1(8)*T2.^3;  % 关节1
y22 = a2(5)+a2(6)*T2+a2(7)*T2.^2+a2(8)*T2.^3;  % 关节2
y32 = a3(5)+a3(6)*T2+a3(7)*T2.^2+a3(8)*T2.^3;  % 关节3
% 速度
v12 = (a1(6)+2*a1(7)*T2+3*a1(8)*T2.^2)/(tf-tmid);
v22 = (a2(6)+2*a2(7)*T2+3*a2(8)*T2.^2)/(tf-tmid);
v32 = (a3(6)+2*a3(7)*T2+3*a3(8)*T2.^2)/(tf-tmid);

% 合并0-5s和5-10s
% 位移
q1 = [y11(1:5/step),y12(5/step+1:end)];
q2 = [y21(1:5/step),y22(5/step+1:end)];
q3 = [y31(1:5/step),y32(5/step+1:end)];
% 速度
qd1 = [v11(1:5/step),v12(5/step+1:end)];
qd2 = [v21(1:5/step),v22(5/step+1:end)];
qd3 = [v31(1:5/step),v32(5/step+1:end)];

%% 绘图
num_steps = length(qd1);
v = [];
for i = 1:num_steps
    % 计算当前的机械臂末端位姿
    J = RRP_robot.jacob0([q1(i), q2(i), q3(i)]);
    dotq = J*[qd1(i), qd2(i), qd3(i)]';
    v = [v,dotq];
end


vx = v(1,:);
vy = v(2,:);
vz = v(3,:);
wx = v(4,:);
wy = v(5,:);
wz = v(6,:);

figure;
plot(t,vx);
formatPlot();
hold on
plot(t,vy);
formatPlot();
hold on
plot(t,vz);
formatPlot();
xlabel('时间（s）')
ylabel('线速度（m/s）')
legend('v_x','v_y','v_z')
formatPlot();

figure;
plot(t,wx);
formatPlot();
hold on
plot(t,wy);
formatPlot();
hold on
plot(t,wz);
formatPlot();
xlabel('时间（s）')
ylabel('角速度（rad/s）')
legend('w_x','w_y','w_z')
formatPlot();
