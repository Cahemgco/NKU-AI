clc
clear
% 设置轨迹
T = 30;
t0 = 0.01;
lamda = T / t0;
time = linspace(0, T, lamda);

% 直线轨迹
% x = time;
% y = zeros(1,lamda);
% theta = zeros(1,lamda);

% 曲线轨迹(圆弧)
theta = -time * pi/60; 
theta = flip(theta);
x = 30 * cos(theta);
y = 30 * sin(theta) + 30;

% 叉车参数
delta_phi = 0.11;  % 轮子转速
beta = -pi/1.3;   % 转角
r = 0.05; % 轮子半径
L2 = 2;
%%
% 初始误差
error_p = zeros(3,3);
k_phi = 2;
k_beta = 5;
error_d = [k_phi, 0; 0, k_beta];

% 世界坐标系下
for i = 1:lamda - 2
    delta_s = delta_phi*r*sin(beta);
    delta_theta = -delta_phi*r*cos(beta)/L2;

    Fp = [1, 0, -delta_s * sin(theta(i) + delta_theta / 2);
     0, 1, delta_s * cos(theta(i) + delta_theta / 2);
     0, 0, 1];

    A = [r*sin(beta)*(cos(theta(i) - (r*delta_phi*cos(beta))/(2*L2)) + (r*cos(beta)/(2*L2))*delta_phi*sin(theta(i) - (r*delta_phi*cos(beta))/(2*L2)));
     r*sin(beta)*(sin(theta(i) - (r*delta_phi*cos(beta))/(2*L2)) - (r*cos(beta)/(2*L2))*delta_phi*cos(theta(i) - (r*delta_phi*cos(beta))/(2*L2)));
     -r*cos(beta)/2];

    B = [r*delta_phi*(cos(beta)*cos(theta(i) - (r*delta_phi*cos(beta))/(2*L2)) - (sin(beta))^2*(r*delta_phi)/(2*L2)*sin(theta(i) - (r*delta_phi*cos(beta))/(2*L2)));
     r*delta_phi*(cos(beta)*sin(theta(i) - (r*delta_phi*cos(beta))/(2*L2)) + (sin(beta))^2*(r*delta_phi)/(2*L2)*cos(theta(i) - (r*delta_phi*cos(beta))/(2*L2)));
     r*delta_phi*sin(beta)/2];

    Fd = [A,B];

    error_p = Fp*error_p*Fp'+Fd*error_d*Fd';
    error_d = [k_phi*delta_phi, 0; 0, k_beta];

    % 绘制椭圆
    if mod(i,500)==0
        scatter(x(i),y(i),"blue","*");
        % 求解特征向量和特征值
        [V, D] = eig(error_p);
    
        a = D(1,1); % 长轴
        b = D(2,2); % 短轴
    
        % 特征向量
        v1 = V(1:2,1);  % 第一列的特征向量
        v2 = V(1:2,2);  % 第二列的特征向量
        
        % 椭圆中心点
        x0 = x(i);
        y0 = y(i);
        
        % 生成椭圆的角度
        e_theta = linspace(0, 2 * pi);
        
        % 计算椭圆上每个点的坐标
        e_x = x0 + a * cos(e_theta) * v1(1) + b * sin(e_theta) * v2(1);
        e_y = y0 + a * cos(e_theta) * v1(2) + b * sin(e_theta) * v2(2);
        
        % 绘制椭圆
        plot(e_x, e_y,"r");
        axis equal;
        grid on;
        xlim([0,35]);
        ylim([0,30]);
        hold on;
    end
end
%%
plot(x,y,"--b")
xlabel("X方向位移")
ylabel("Y方向位移")










