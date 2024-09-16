function q = myikine(position, l1, l2)
    position = double(position);
    d3 = position(3,4);
    theta1 = atan2((position(2,4)-l2*position(2,1))/l1, (position(1,4)-l2*position(1,1))/l1);
    theta2 = atan2(position(2,1), position(1,1)) - theta1;

    q = [theta1, theta2, d3];
end