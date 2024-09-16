import numpy as np
import cv2
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def solve(X, Y):
    # 求权重
    w = 1 / np.linalg.norm(Y, axis=1)
    w = w.transpose()
    W = np.diag(w)

    # 求加权重心
    cx = (w.transpose() @ X) /w.sum(axis=0)
    cy = (w.transpose() @ Y) /w.sum(axis=0)

    # 去中心化
    x, y = np.subtract(X, cx), np.subtract(Y, cy)

    # 构造SVD矩阵
    S = x.transpose()@ W @ y

    # SVD分解
    u, s, v = np.linalg.svd(S)

    # 求旋转矩阵
    R = v.transpose() @ u.transpose()

    # 求平移向量
    t = cy - R @ cx

    return R.transpose(), t.transpose()

def is_unitary_orthogonal(matrix, tol=1e-10):
    # 判断单位性
    unitary_condition = np.allclose(matrix.T @ matrix, np.eye(matrix.shape[1]), atol=tol)

    # 判断正交性
    orthogonal_condition = np.allclose(matrix @ matrix.T, np.eye(matrix.shape[0]), atol=tol)

    return unitary_condition and orthogonal_condition


if __name__ == '__main__':
    R, _ = cv2.Rodrigues(np.array([-1, 2, -3], dtype=float))
    print("矩阵R是否为单位正交矩阵:", is_unitary_orthogonal(R))

    t = np.array ([0.5, 1, 3])
    X = np.random.randint(0, 10, [100, 3])
    Y = X @ R + t
    Y += np.random.normal(0, 0.01, Y.shape)  # Y增加高斯噪声

    R_est, t_est = solve(X, Y)
    # print((R_est @ X.T).T + t_est.T)

    print("设定旋转矩阵\n", R, '\n')
    print("设定平移向量\n", t, '\n')
    print("求得旋转矩阵\n", R_est, '\n')
    print("求得平移向量\n", t_est, '\n')

    diff_R = np.linalg.norm(R - R_est)
    diff_t = np.linalg.norm(t - t_est)

    print("旋转矩阵误差\n", diff_R, '\n')
    print("平移向量误差\n", diff_t, '\n')

    # 绘制世界坐标系中的点
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(Y[:,0], Y[:,1], Y[:,2], c='r', marker='o', label='point cloud')

    # 绘制传感器坐标系
    origin = np.zeros((1, 3))
    axes_endpoints = np.array([[5, 0, 0], [0, 5, 0], [0, 0, 5]])
    transformed_axes_origin = (origin @ R) + t
    transformed_axes_endpoints = (axes_endpoints @ R) + t
    for i in range(3):
        ax.plot([transformed_axes_origin[0,0], transformed_axes_endpoints[i,0]], 
                [transformed_axes_origin[0,1], transformed_axes_endpoints[i,1]], 
                [transformed_axes_origin[0,2], transformed_axes_endpoints[i,2]], c='b')

    # 设置图例和标签
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.legend()

    # 显示图形
    plt.show()