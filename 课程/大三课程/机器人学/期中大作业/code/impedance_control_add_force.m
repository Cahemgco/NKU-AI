function dxdt = impedance_control_add_force(t, x, RRP_robot, qd, dqd, Kx, Kb)
    % 获得当前状态
    q = x(1:3);
    dq = x(4:6);

    % 计算期望位置和期望速度的偏差
    e = qd - q;
    de = dqd - dq;

    % 计算动力学方程
    M = RRP_robot.inertia(q'); 
    C = RRP_robot.coriolis(q', dq');

    % 计算雅可比
    J = RRP_robot.jacob0(q');

    % 计算刚度系数和阻尼项
    Kq = J(1:3,:)'*Kx*J(1:3,:);
    Kw = J(1:3,:)'*Kb*J(1:3,:);
    
    % 二阶导
    ddq = inv(M)*(Kq * e + Kw * de - C * dq);

    if t>=4 && t<=6
        ddq = ddq + inv(M) * [2 2 3]';
    end


    % 构造状态向量的导数
    dxdt = [dq;ddq];
end