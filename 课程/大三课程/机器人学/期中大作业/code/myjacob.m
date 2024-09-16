function J = myjacob(q, l1, l2)
    theta1 = q(1); theta2 = q(2); d3 = q(3);
    J = [-l1*sin(theta1) - l2*sin(theta1 + theta2), -l2*sin(theta1 + theta2), 0;
     l1*cos(theta1) + l2*cos(theta1 + theta2), l2*cos(theta1 + theta2), 0;
     0, 0, 1;
     0, 0, 0;
     0, 0, 0;
     1, 1, 0];
end