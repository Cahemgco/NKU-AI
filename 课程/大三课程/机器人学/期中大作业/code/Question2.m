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
% L(1) = Link('d', 0, 'a', 0, 'alpha', 0, 'qlim', [-pi,pi], 'modified');
% L(2) = Link('d', 0, 'a', l1, 'alpha', 0, 'qlim', [-pi,pi], 'modified');
% L(3) = Link('theta', 0, 'a', l2, 'alpha', 0, 'qlim', [0, 0.2], 'modified');

RRP_robot = SerialLink(L(1:3),'name','RRP');
RRP_robot.plotopt = {'workspace',[-1,1,-1,1,-1,1]};
% RRP_robot.teach;
%% 正运动学
% 指定的关节角和位移
q1 = [pi/4, pi/3, 0]';  
q2 = [pi/2, -pi/4, 0]';
q3 = [-pi/2, pi/4, 0.15]';

% 函数求解
% 内置函数：fkine解正运动学函数，根据q，求解出末端位姿p
p1 = RRP_robot.fkine(q1);    
p2 = RRP_robot.fkine(q2);
p3 = RRP_robot.fkine(q3);

% 编写函数：根据推导的正运动学方程自己编写
my_p1 = myfkine(q1, l1, l2);
my_p2 = myfkine(q2, l1, l2);
my_p3 = myfkine(q3, l1, l2);

% 验证
display(p1);
display(my_p1);
display(p2);
display(my_p2);
display(p3);
display(my_p3);

% figure(1)
% RRP_robot.plot(q1);
% figure(2)
% RRP_robot.plot(q2);
% figure(3)
% RRP_robot.plot(q3);
%% 逆运动学
% 函数求解
% 内置函数：ikine逆解函数，根据末端位姿p，求解q
% q0 = [0, 0, 0]; % 初始化关节
% q1 = RRP_robot.ikine(p1, 'q0', q0, 'mask', [1, 1, 1, 0, 0, 0], 'search', true);
% q2 = RRP_robot.ikine(p2, 'q0', q0, 'mask', [1, 1, 1, 0, 0, 0], 'search', true);      
% q3 = RRP_robot.ikine(p3, 'q0', q0, 'mask', [1, 1, 1, 0, 0, 0], 'search', true);

% 编写函数：根据推导的逆运动学方程自己编写
my_q1 = myikine(p1, l1, l2);
my_q2 = myikine(p2, l1, l2);
my_q3 = myikine(p3, l1, l2);

% 验证模型
display(q1);
display(my_q1);
display(q2);
display(my_q2);
display(q3);
display(my_q3);

%% 正雅可比
dot_q1 = [-pi/8, pi/4, 0.05]';
dot_q2 = [-pi/3, pi/8, 0.1]';
dot_q3 = [pi/5, -pi/6, -0.1]';
% 内置函数
J1 = RRP_robot.jacob0(q1);
J2 = RRP_robot.jacob0(q2);
J3 = RRP_robot.jacob0(q3);
dot_X1 = J1*dot_q1;
dot_X2 = J2*dot_q2;
dot_X3 = J3*dot_q3;
% 编写函数
my_J1 = myjacob(q1, l1, l2);
my_J2 = myjacob(q2, l1, l2);
my_J3 = myjacob(q3, l1, l2);
my_dot_X1 = my_J1*dot_q1;
my_dot_X2 = my_J2*dot_q2;
my_dot_X3 = my_J3*dot_q3;

display(J1);
display(my_J1);
display(J2);
display(my_J2)
display(J3);
display(my_J3);

display(dot_X1);
display(my_dot_X1);
display(dot_X2);
display(my_dot_X2)
display(dot_X3);
display(my_dot_X3);

%% 逆雅可比
my_dot_q1 = myinvjacob(my_J1, my_dot_X1);
my_dot_q2 = myinvjacob(my_J2, my_dot_X2);
my_dot_q3 = myinvjacob(my_J3, my_dot_X3);

display(my_dot_q1)
display(my_dot_q2)
display(my_dot_q3)
%% 动力学
% 内置函数
G = RRP_robot.gravload(q3');         % 重力矢量
M = RRP_robot.inertia(q3');          % 质量矩阵
C = RRP_robot.coriolis(q3', dot_q3');      % 离心力和科氏力矩阵
% 编写函数
[my_M, my_C, my_G] = mydynamics(l1,l2,r1,r2,m1,m2,m3,i1,i2,q3(2),dot_q3(1),dot_q3(2));

display(M);
display(my_M);
display(C);
display(my_C);
display(G);
display(my_G);

% 给定力/力矩，绘制位移、速度、加速度
[t,q,qd] = RRP_robot.fdyn(5, @(robot, t, q, qd) [0.01,-0.01,5.455]);    % 给定恒定力/力矩[0.01,-0.01,5.455]
qdd = zeros(size(q));

for i=1:length(qdd)
    qdd(i,:)=RRP_robot.accel(q(i,:),qd(i,:),[0.01,-0.01,5.455]);    %求解角加速度
end

% figure('Name','theta1');
% plot(t,q(:,1));
% xlabel('时间（s）')
% ylabel('角度（rad）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','theta2');
% plot(t,q(:,2));
% xlabel('时间（s）')
% ylabel('角度（rad）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','d3');
% plot(t,q(:,3));
% xlabel('时间（s）')
% ylabel('位移（m）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','dtheta1');
% plot(t,qd(:,1));
% xlabel('时间（s）')
% ylabel('角速度（rad/s）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','dtheta2');
% plot(t,qd(:,2));
% xlabel('时间（s）')
% ylabel('角速度（rad/s）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','dd3');
% plot(t,qd(:,3));
% xlabel('时间（s）')
% ylabel('线速度（m/s）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','ddtheta1');
% plot(t,qdd(:,1));
% xlabel('时间（s）')
% ylabel('角加速度（rad/s^2）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','ddtheta2');
% plot(t,qdd(:,2));
% xlabel('时间（s）')
% ylabel('角加速度（rad/s^2）')
% formatPlot(); % 添加函数调用以统一绘图格式
% 
% figure('Name','ddd3');
% plot(t,qdd(:,3),'linewidth',1.2);
% xlabel('时间（s）')
% ylabel('线加速度（m/s^2）')
% ylim([0,0.05]);
% formatPlot(); % 添加函数调用以统一绘图格式

%% 动力学仿真绘图
% 需要先运行simulink程序Scara.slxc
% figure('Name','theta1');
% plot(out.out.time,out.out.signals.values(:,1));
% xlabel('时间（s）')
% ylabel('角度（rad）')
% formatPlot();
% 
% figure('Name','theta2');
% plot(out.out.time,out.out.signals.values(:,2));
% xlabel('时间（s）')
% ylabel('角度（rad）')
% formatPlot();
% 
% figure('Name','d3');
% plot(out.out.time,out.out.signals.values(:,3));
% xlabel('时间（s）')
% ylabel('位移（m）')
% formatPlot();
% 
% figure('Name','dtheta1');
% plot(out.out.time,out.out.signals.values(:,4));
% xlabel('时间（s）')
% ylabel('角速度（rad/s）')
% formatPlot();
% 
% figure('Name','dtheta2');
% plot(out.out.time,out.out.signals.values(:,5));
% xlabel('时间（s）')
% ylabel('角速度（rad/s）')
% formatPlot();
% 
% figure('Name','dd3');
% plot(out.out.time,out.out.signals.values(:,6));
% xlabel('时间（s）')
% ylabel('线速度（m/s）')
% formatPlot();
% 
% figure('Name','ddtheta1');
% plot(out.out.time,out.out.signals.values(:,7));
% xlabel('时间（s）')
% ylabel('角加速度（rad/s^2）')
% formatPlot();
% 
% figure('Name','ddtheta2');
% plot(out.out.time,out.out.signals.values(:,8));
% xlabel('时间（s）')
% ylabel('角加速度（rad/s^2）')
% formatPlot();
% 
% figure('Name','ddd3');
% plot(out.out.time,out.out.signals.values(:,9),'linewidth',1.2);
% xlabel('时间（s）')
% ylabel('线加速度（m/s^2）')
% ylim([0,0.05]);
% formatPlot();