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

% % 笛卡尔空间
% p0 = RRP_robot.fkine(q0);
% pmid = RRP_robot.fkine(qmid);
% pf = RRP_robot.fkine(qf);
% 
% % 生成轨迹
% Q1 = RRP_robot.jtraj(p0,pmid,[0:0.1:5]','mask', [1, 1, 1, 0, 0, 0]);
% Q2 = RRP_robot.jtraj(pmid,pf,[0:0.1:5]','mask', [1, 1, 1, 0, 0, 0]);
% Q = [Q1;Q2];
% 
% %% 速度曲线
% figure('Name','Q1');
% plot([0:0.1:10.1]',Q(:,1));
% xlabel('时间（s）')
% ylabel('弧度（rad）')
% xlim([0,10]);
% hold on;
% plot(5, qmid(1), 'ro');
% 
% % 绘制垂直虚线
% line([5 5], [0 qmid(1)], 'LineStyle', '--', 'Color', 'k');
% line([0 5], [qmid(1) qmid(1)], 'LineStyle', '--', 'Color', 'k');
% formatPlot();
% hold off; % 结束保持
% 
% 
% figure('Name','Q2');
% plot([0:0.1:10.1]',Q(:,2));
% xlabel('时间（s）')
% ylabel('弧度（rad）')
% xlim([0,10]);
% hold on;
% plot(5, qmid(2), 'ro');
% 
% % 绘制垂直虚线
% line([5 5], [0 qmid(2)], 'LineStyle', '--', 'Color', 'k');
% line([0 5], [qmid(2) qmid(2)], 'LineStyle', '--', 'Color', 'k');
% formatPlot();
% hold off; % 结束保持
% 
% figure('Name','Q3');
% plot([0:0.1:10.1]',Q(:,3));
% xlabel('时间（s）')
% ylabel('位移（m）')
% xlim([0,10]);
% hold on;
% plot(5, qmid(3), 'ro');
% 
% % 绘制垂直虚线
% line([5 5], [0 qmid(3)], 'LineStyle', '--', 'Color', 'k');
% line([0 5], [qmid(3) qmid(3)], 'LineStyle', '--', 'Color', 'k');
% formatPlot();
% hold off; % 结束保持

%% 求解三次函数 中间点速度为0的情况
% % 0-5s
% A1 = [t0^3 t0^2 t0 1;
%     3*t0^2 2*t0 1 0;
%     tmid^3 tmid^2 tmid 1;
%     3*tmid^2 2*tmid 1 0];
% % 5-10s
% A2 = [tmid^3 tmid^2 tmid 1;
%     3*tmid^2 2*tmid 1 0;
%     tf^3 tf^2 tf 1;
%     3*tf^2 2*tf 1 0];
% 
% % 关节1
% b1 = [0;0;20*ratio;0];
% b2 = [20*ratio;0;30*ratio;0];
% % 关节2
% b3 = [0;0;40*ratio;0];
% b4 = [40*ratio;0;60*ratio;0];
% % 关节3
% b5 = [0;0;0.1;0];
% b6 = [0.1;0;0.15;0];
% % 求解多项式系数
% a1 = A1 \ b1;    % 关节1 0-5
% a2 = A2 \ b2;    % 关节1 5-10
% a3 = A1 \ b3;    % 关节2 0-5
% a4 = A2 \ b4;    % 关节2 5-10
% a5 = A1 \ b5;    % 关节3 0-5
% a6 = A2 \ b6;    % 关节3 5-10
% 
% step = 0.05;
% t = linspace(0,10,10/step);
% 
% % 计算多项式函数
% y1 = polyval(a1, t);
% y2 = polyval(a2, t);
% y3 = polyval(a3, t);
% y4 = polyval(a4, t);
% y5 = polyval(a5, t);
% y6 = polyval(a6, t);
% 
% % 计算一阶导数
% dy1 = polyval(polyder(a1), t);
% dy2 = polyval(polyder(a2), t);
% dy3 = polyval(polyder(a3), t);
% dy4 = polyval(polyder(a4), t);
% dy5 = polyval(polyder(a5), t);
% dy6 = polyval(polyder(a6), t);
% 
% % 计算二阶导数
% d2y1 = polyval(polyder(polyder(a1)), t);
% d2y2 = polyval(polyder(polyder(a2)), t);
% d2y3 = polyval(polyder(polyder(a3)), t);
% d2y4 = polyval(polyder(polyder(a4)), t);
% d2y5 = polyval(polyder(polyder(a5)), t);
% d2y6 = polyval(polyder(polyder(a6)), t);
% 
% % 关节1 全过程
% q1 = [y1(1:5/step),y2(5/step+1:end)];
% qd1 = [dy1(1:5/step),dy2(5/step+1:end)];
% qdd1 = [d2y1(1:5/step),d2y2(5/step+1:end)];
% % 关节2 全过程
% q2 = [y3(1:5/step),y4(5/step+1:end)];
% qd2 = [dy3(1:5/step),dy4(5/step+1:end)];
% qdd2 = [d2y3(1:5/step),d2y4(5/step+1:end)];
% % 关节3 全过程
% q3 = [y5(1:5/step),y6(5/step+1:end)];
% qd3 = [dy5(1:5/step),dy6(5/step+1:end)];
% qdd3 = [d2y5(1:5/step),d2y6(5/step+1:end)];

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
% 加速度
a11 = (2 * a1(3) + 6 * a1(4) * T1)/tmid^2;
a21 = (2 * a2(3) + 6 * a2(4) * T1)/tmid^2;
a31 = (2 * a3(3) + 6 * a3(4) * T1)/tmid^2;

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

%% 绘图
figure('Name','Q1');
plot(t,q1);
xlabel('时间（s）')
ylabel('弧度（rad）')
xlim([0,10]);
hold on;
plot(5, qmid(1), 'ro');
line([5 5], [0 qmid(1)], 'LineStyle', '--', 'Color', 'k');
line([0 5], [qmid(1) qmid(1)], 'LineStyle', '--', 'Color', 'k');
formatPlot();
hold off;

figure('Name','Q2');
plot(t,q2);
xlabel('时间（s）')
ylabel('弧度（rad）')
xlim([0,10]);
hold on;
plot(5, qmid(2), 'ro');
line([5 5], [0 qmid(2)], 'LineStyle', '--', 'Color', 'k');
line([0 5], [qmid(2) qmid(2)], 'LineStyle', '--', 'Color', 'k');
formatPlot();
hold off;

figure('Name','Q3');
plot(t,q3);
xlabel('时间（s）')
ylabel('位移（m）')
xlim([0,10]);
hold on;
plot(5, qmid(3), 'ro');
line([5 5], [0 qmid(3)], 'LineStyle', '--', 'Color', 'k');
line([0 5], [qmid(3) qmid(3)], 'LineStyle', '--', 'Color', 'k');
formatPlot();
hold off;

figure('Name','QD1');
plot(t,qd1);
xlabel('时间（s）')
ylabel('角速度（rad/s）')
xlim([0,10]);
% ylim([0 0.11])
formatPlot();

figure('Name','QD2');
plot(t,qd2);
xlabel('时间（s）')
ylabel('角速度（rad/s）')
xlim([0,10]);
% ylim([0,0.22]);
formatPlot();

figure('Name','QD3');
plot(t,qd3);
xlabel('时间（s）')
ylabel('线速度（m/s）')
xlim([0,10]);
% ylim([0,0.032]);
formatPlot();

figure('Name','QDD1');
plot(t,qdd1);
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
xlim([0,10]);
% ylim([0 0.11])
formatPlot();

figure('Name','QDD2');
plot(t,qdd2);
xlabel('时间（s）')
ylabel('角加速度（rad/s^2）')
xlim([0,10]);
% ylim([0,0.22]);
formatPlot();

figure('Name','QDD3');
plot(t,qdd3);
xlabel('时间（s）')
ylabel('线加速度（m/s^2）')
xlim([0,10]);
% ylim([0,0.032]);
formatPlot();

