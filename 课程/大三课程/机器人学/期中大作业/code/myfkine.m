function position = myfkine(q, l1, l2)
    theta1 = q(1);
    theta2 = q(2);
    d3 = q(3);
    position = [cos(theta1+theta2) -sin(theta1+theta2) 0 l2*cos(theta1+theta2)+l1*cos(theta1);
        sin(theta1+theta2) cos(theta1+theta2) 0 l2*sin(theta1+theta2)+l1*sin(theta1);
        0 0 1 d3;
        0 0 0 1];
end