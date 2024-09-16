code文件夹下包含问题2-9中的代码，包括：
1. Question2.m至Question9.m——各问题的主函数，完成了所有问题的求解，运行将调用本文件夹中的函数。
2. 自行编写的函数包括：
impedance_control.m——求解阻抗控制器函数（问题8）
impedance_control_add_force.m——求解有外力的情况下的阻抗控制器函数（问题9）
inverse_dynamic.m——求解逆动力学控制器的函数（问题7）
myfkine.m——求解正运动学函数（问题2）
myikine.m——求解逆运动学函数（问题2）
myjacob.m——求解雅可比矩阵函数（问题2）
myinvjacob.m——求解逆雅可比函数（问题2）
mydynamics.m——求解动力学方程函数（问题2）
PD_control.m——求解PD控制器的函数（问题6）
PD_control_add_force.m——求解有外力的情况下的PD控制器函数（问题9）
formatPlot.m——控制绘图格式函数
3. Scara.slx——动力学仿真所用到的的Simulink模型（问题2）

注意：进行动力学仿真曲线绘制时，需要运行Question2.m后，再运行Scara.slx仿真。运行Scara.slx后，将Question2.m中210行及以后的代码解注释，单独运行该部分代码，即可完成绘图。