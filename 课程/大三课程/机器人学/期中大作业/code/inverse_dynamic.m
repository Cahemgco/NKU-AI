function dxdt = inverse_dynamic(t, x, qd, dqd, ddqd, K0, K1)
    % 获得当前状态
    n = fix(t*100)+1;
    q = x(1:3);
    dq = x(4:6);

    % 计算期望位置和期望速度的偏差
    e = qd(:,n) - q;
    de = dqd(:,n) - dq;

    ddq = K0 * e + K1 * de + ddqd(:,n);

    % 构造状态向量的导数
    dxdt = [dq;ddq];
end