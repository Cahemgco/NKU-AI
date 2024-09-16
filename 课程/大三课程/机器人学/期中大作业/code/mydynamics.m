function [M, C, G] = mydynamics(l1, l2, r1, r2, m1, m2, m3, I1, I2, theta2, theta1_dot, theta2_dot)
%% 质量矩阵M
% 初始化质量矩阵 M
M = zeros(3, 3);

% 计算 M11
M11 = r1^2 * m1 + (l1^2 + r2^2 + 2*l1*r2*cos(theta2)) * m2 + (l1^2 + l2^2 + 2*l1*l2*cos(theta2)) * m3 + I1 + I2;

% 计算 M12 和 M21
M12 = (r2^2 + l1*r2*cos(theta2)) * m2 + (l2^2 + l1*l2*cos(theta2)) * m3 + I2;
M21 = M12;

% 计算 M22
M22 = r2^2 * m2 + l2^2 * m3 + I2;

% 计算 M33
M33 = m3;

% 将结果填入质量矩阵 M 中
M(1, 1) = M11;
M(1, 2) = M12;
M(2, 1) = M21;
M(2, 2) = M22;
M(3, 3) = M33;

%% 离心力与科氏力矩阵C
% 初始化离心力与科氏力矩阵项 C
C = zeros(3, 3);

% 计算 C11
C11 = -(m2 * l1 * r2 + m3 * l1 * l2) * sin(theta2) * theta2_dot;

% 计算 C12
C12 = -(m2 * l1 * r2 + m3 * l1 * l2) * sin(theta2) * (theta1_dot + theta2_dot);

% 计算 C21
C21 = (m2 * l1 * r2 + m3 * l1 * l2) * sin(theta2) * theta1_dot;

% 将结果填入C 中
C(1, 1) = C11;
C(1, 2) = C12;
C(2, 1) = C21;

%% 重力矢量
% 初始化重力矢量 G
G = zeros(3, 1);

% 计算重力项的第三个分量
g = 9.81;
G(3) = m3 * g;

end