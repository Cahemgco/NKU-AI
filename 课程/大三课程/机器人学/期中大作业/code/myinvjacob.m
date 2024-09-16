function dot_q = myinvjacob(J, dot_X)
    v = dot_X(1:3);
    J_v = J(1:3,:);
    dot_q = inv(J_v)*v;
end
