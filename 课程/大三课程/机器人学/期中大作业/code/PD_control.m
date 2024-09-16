function dxdt = PD_control(t, x, RRP_robot, qd, dqd, Kp, Kd)
    % 获得当前状态
    n = fix(t*100)+1;
    q = x(1:3);
    dq = x(4:6);

    % 计算期望位置和期望速度的偏差
    e = qd(:,n) - q;
    de = dqd(:,n) - dq;
%     e = qd - q;
%     de = dqd - dq;

    % 计算控制力矩
    tol = Kp * e + Kd * de;

    % 计算动力学方程
    M = RRP_robot.inertia(q'); 
    C = RRP_robot.coriolis(q', dq');

    ddq = inv(M) * (tol - C * dq);

    % 构造状态向量的导数
    dxdt = [dq;ddq];
end