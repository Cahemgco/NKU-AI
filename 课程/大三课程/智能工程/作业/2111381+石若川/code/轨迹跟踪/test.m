clear all;clc;

%% define constant
l=0.2;
R=1;
K=0.2;
delta_T=0.5;

%% define initialization
Pos=[0,12,3/2*pi]; %X,Y,Theta
T=linspace(0,2*pi,200);
% desire_Pos=[16*power(sin(T),3);13*cos(T)-5*cos(2*T)-2*cos(3*T)-cos(4*T)];
desired_point = [10, 5];

%%  Graphics
f3=figure;
xlabel('x (m)')
ylabel('y (m)')
grid on

%%  robot dimensions
A.R_w = 1/2; % robot width/2
A.R_l=  2/2;   % robot length/2
A.a1 = [-A.R_l -A.R_w]';
A.b1 = [A.R_l -A.R_w/2]';
A.b2 = [A.R_l A.R_w/2]';
A.c = [-A.R_l A.R_w]';
A.P = [A.a1 A.b1 A.b2 A.c];

A.Rot = [ cos(Pos(3)) -sin(Pos(3)); sin(Pos(3)) cos(Pos(3))]*A.P; %rotated car
A.Prot_trasl = A.Rot + [ ones(1,4)*Pos(1); ones(1,4)*Pos(2)]; % add offset of car's center

A.P_robot=patch(A.P(1,:),A.P(2,:),'b');
A.P_robot.XData=A.Prot_trasl(1,:)';
A.P_robot.YData=A.Prot_trasl(2,:)';


%% define controller


%% draw picture
h= animatedline('color','r','LineStyle','--');
h_car= animatedline('color','b');
h_car_model = animatedline('Marker','o','MaximumNumPoints',1);     %只显示1个新的点
axis([-20 20 -20 15])

for k = 1:length(T)
    addpoints(h_car,Pos(1),Pos(2));
    addpoints(h_car_model,Pos(1),Pos(2));
    
%     e_x=Pos(1)-desire_Pos(1,k);
%     e_y=Pos(2)-desire_Pos(2,k);
    e_x=Pos(1)-desired_point(1);
    e_y=Pos(2)-desired_point(2);
    u_x=-K*e_x;
    u_y=-K*e_y;
    A_mat=[cos(Pos(3)) -l*sin(Pos(3));sin(Pos(3)) l*cos(Pos(3))];
    P=[cos(Pos(3)) 0;sin(Pos(3)) 0;0 1];
    U_mat=[u_x,u_y]';
    v_w_mat=A_mat\U_mat;
    Pos(1)=Pos(1)+v_w_mat(1)*cos(Pos(3))-v_w_mat(2)*l*sin(Pos(3));
    Pos(2)=Pos(2)+v_w_mat(1)*sin(Pos(3))+v_w_mat(2)*l*cos(Pos(3));
    
    Pos(3)=Pos(3)+v_w_mat(2)*delta_T;
    addpoints(h,desired_point(1),desired_point(2));
    
    A.Rot = [cos(Pos(3)) -sin(Pos(3)); sin(Pos(3)) cos(Pos(3))]*A.P; %rotated car
    A.Prot_trasl = A.Rot + [ones(1,4)*Pos(1); ones(1,4)*Pos(2)]; % add offset of car's center
    A.P_robot.XData=A.Prot_trasl(1,:)';
    A.P_robot.YData=A.Prot_trasl(2,:)';
    frame=getframe(gcf);
    im = frame2im(frame); 
    [imind,cm] = rgb2ind(im,256);
    if k==1
         imwrite(imind,cm,'experiment1.gif','gif', 'LoopCount',inf,'DelayTime',0.000001);
    end
    if rem(k,2)==0
         imwrite(imind,cm,'experiment1.gif','gif','WriteMode','append','DelayTime',0.000001);
    end
    drawnow
end


