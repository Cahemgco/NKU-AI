figure;
plot(out.target.time, out.target.signals.values(:,1), '--', 'LineWidth', 1, 'Color', 'b');
hold on;
plot(out.x_I.time, out.x_I.signals.values, 'LineWidth', 1, 'Color', 'r');
legend('x目标值', 'x实际值');
% ylim([0,16]);
xlim([0,8]);
xlabel('时间（s）')
ylabel('位移')
grid on;
hold off;
%%
figure;
plot(out.target.time, out.target.signals.values(:,2), '--', 'LineWidth', 1, 'Color', 'b');
hold on;
plot(out.y_I.time, out.y_I.signals.values, 'LineWidth', 1, 'Color', 'r');
legend('y目标值', 'y实际值');
ylim([-1.1,1.1]);
xlim([0,8]);
xlabel('时间（s）')
ylabel('位移')
grid on;
hold off;
%%
figure;
plot(out.target.time, out.target.signals.values(:,3), '--', 'LineWidth', 1, 'Color', 'b');
hold on;
plot(out.theta_I.time, out.theta_I.signals.values, 'LineWidth', 1, 'Color', 'r');
legend('theta目标值', 'theta实际值');
ylim([-1.1,1.1]);
xlim([0,8]);
xlabel('时间（s）')
ylabel('角度（弧度）')
grid on;
hold off;
%%
figure;
plot(out.diffrence.time, out.diffrence.signals.values(:,1), '-', 'LineWidth', 1, 'Color', 'r');
hold on
plot(out.diffrence.time, out.diffrence.signals.values(:,2), '--', 'LineWidth', 1, 'Color', 'b');
hold on
plot(out.diffrence.time, out.diffrence.signals.values(:,3), '-.', 'LineWidth', 1, 'Color', 'k');
grid on
xlim([0,8]);
legend("x误差","y误差","theta误差","Location","southeast")
xlabel('时间（s）')
ylabel('误差')
hold off;
%%  轨迹
% 插值以得到光滑的曲线
xx = linspace(min(out.x_I.signals.values), max(out.x_I.signals.values), 100); % 生成更多的 x 值，以获得光滑曲线
yy = spline(out.x_I.signals.values, out.y_I.signals.values, xx); % 使用样条插值生成光滑曲线
set_y = sin(xx);

% 绘制散点图和光滑曲线
scatter(out.x_I.signals.values, out.y_I.signals.values, "b*"); % 绘制散点
hold on; % 保持图形并在同一图形中绘制下一个曲线
plot(xx, yy, 'b-', 'LineWidth', 0.5); % 绘制光滑曲线
grid on
hold on; % 关闭图形保持，以便下次的绘图不会受影响
plot(xx,set_y,'r--','LineWidth', 1)
% scatter(out.target.signals.values(:,1), out.target.signals.values(:,2), "b*");
legend("实际轨迹点","拟合的实际轨迹","设定轨迹")
xlim([0,8])
ylim([-1.1,1.1])
%  绘制小车
% l_1 = 0.5;
% l_2 = 2;
% n = 100; % 采样数
% Wheel = [0,0,l_2;l_1,-l_1,0]; % 初始状态三个轮子的位置坐标
% 
% % 获取终点位姿
% x = out.x_I.signals.values(length(out.x_I.signals.values));
% y = out.y_I.signals.values(length(out.y_I.signals.values));
% theta = out.theta_I.signals.values(length(out.theta_I.signals.values));
% 
% R_theta = [cos(theta), -sin(theta);sin(theta), cos(theta)]; % 旋转矩阵
% R_Wheel = R_theta * Wheel; % 经过旋转后的轮子坐标
% 
% L1 = R_theta * [-1 * ones(1, n);linspace(-l_1-1, l_1+1, n)] + [x;y]; 
% L2 = R_theta * [(l_2+2) * ones(1, n);linspace(-l_1-1, l_1+1, n)] + [x;y]; 
% L3 = R_theta * [linspace(-1, l_2+2, n);(l_1+1) * ones(1, n)] + [x;y];
% L4 = R_theta * [linspace(-1, l_2+2, n);(-l_1-1) * ones(1, n)] + [x;y]; 
% 
% plot(L1(1, :), L1(2, :), 'r');
% plot(L2(1, :), L2(2, :), 'r');
% plot(L3(1, :), L3(2, :), 'r');
% plot(L4(1, :), L4(2, :), 'r');
% plot(ones(1,3)*x+R_Wheel(1,:),ones(1,3)*y+R_Wheel(2,:),'bo');
xlabel('X方向位移')
ylabel('Y方向位移')
% 
% hold off;
