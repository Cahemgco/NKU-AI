from tkinter import *
import enum
import heapq
import time
import _thread
from tkinter import messagebox

class PointState(enum.Enum):
    # 障碍物
    BARRIER = 'black'
    # 未使用
    UNUSED = 'white'
    # 在open list的方格
    OPEN = 'green'
    # 在close list的方格
    CLOSED = 'gray'
    # 路径
    PATH = 'red'
    # 起点
    START = 'yellow'
    # 终点
    TARGET = 'purple'


class MiniMap:
    class Point:
        def __init__(self, x, y, f, g, father, state, rectangle):
            # x坐标
            self.x = x
            # y坐标
            self.y = y
            # f = g + h
            self.f = f
            # 从寻路起点到这个点的代价
            self.g = g
            # 父节点
            self.father = father
            # 当前点状态
            self.state = state
            # 当前点对应画布上的矩形
            self.rectangle = rectangle

        def __lt__(self, other):
            if self.f < other.f:
                return True
            else:
                return False

    def __init__(self, *args):
        # 高
        self.height = args[0]
        # 宽
        self.width = args[1]
        # 方格尺寸
        self.size = args[2]
        # 起点，初始为（0,0）
        self.start = (0, 0)
        # 终点，初始为（19,19）
        self.end = (19, 19)
        # 每次绘制的延迟时间
        self.delay = args[3]

        self.root = Tk()
        self.root.title('A star')
        self.canvas = Canvas(self.root, width=self.width * self.size + 3, height=self.height * self.size + 3)
        # 生成方格集合
        self.points = self.generatePoints()
        # 生成网格
        self.generateMesh()

        self.canvas.bind('<Button-1>', self.setBarrier)  # 鼠标左键设置障碍物
        self.canvas.bind('<Button-2>', self.setStart)   # 鼠标中键设置起点
        self.canvas.bind('<Button-3>', self.setTarget)  # 鼠标右键设置终点

        self.canvas.bind('<KeyPress-c>', self.cleanMap)  # 键盘c键清空网格
        self.canvas.bind('<KeyPress-n>', self.navigation)   # 键盘n键开始寻找路径
        self.canvas.focus_set()

        self.canvas.pack(side=TOP, expand=YES, fill=BOTH)
        self.root.resizable(False, False)
        self.root.mainloop()

    def generatePoints(self):
        points = [[self.Point(x, y, 0, 0, None, PointState.UNUSED.value,
                              self.canvas.create_rectangle((x * self.size + 3, y * self.size + 3),
                                                           ((x + 1) * self.size + 3, (y + 1) * self.size + 3),
                                                           fill=PointState.UNUSED.value)) for y in range(self.height)]
                  for x in range(self.width)]
        return points

    def generateMesh(self):
        for i in range(self.height + 1):
            self.canvas.create_line((3, i * self.size + 3), (self.width * self.size + 3, i * self.size + 3))
        for i in range(self.width + 1):
            self.canvas.create_line((i * self.size + 3, 3), (i * self.size + 3, self.height * self.size + 3))

    def setBarrier(self, event):
        x = int((event.x + 3) / self.size)
        y = int((event.y + 3) / self.size)
        if x < self.width and y < self.height:
            if self.points[x][y].state == PointState.BARRIER.value:    # 重复点击
                self.changeState(self.points[x][y], PointState.UNUSED.value)
            else:
                self.changeState(self.points[x][y], PointState.BARRIER.value)

    def setStart(self, event):
        x = int((event.x + 3) / self.size)
        y = int((event.y + 3) / self.size)
        self.start = (x, y)
        if x < self.width and y < self.height:
            if self.points[x][y].state == PointState.START.value:   # 重复点击
                self.changeState(self.points[x][y], PointState.UNUSED.value)
            else:
                self.changeState(self.points[x][y], PointState.START.value)


    def setTarget(self, event):
        x = int((event.x + 3) / self.size)
        y = int((event.y + 3) / self.size)
        self.end = (x, y)
        if x < self.width and y < self.height:
            if self.points[x][y].state == PointState.TARGET.value:  # 重复点击
                self.changeState(self.points[x][y], PointState.UNUSED.value)
            else:
                self.changeState(self.points[x][y], PointState.TARGET.value)

    def cleanMap(self, event):
        for i in range(self.width):
            for j in range(self.height):
                self.changeState(self.points[i][j], PointState.UNUSED.value)

    def navigation(self, event):
        _thread.start_new_thread(self.generatePath, (self.start, self.end))

    def changeState(self, point, state):
        point.state = state
        self.canvas.itemconfig(point.rectangle, fill=state)

    def generatePath(self, start, end):
        xStart = start[0]
        yStart = start[1]
        xEnd = end[0]
        yEnd = end[1]

        heap = []

        close_list = set()
        open_list = set()

        heapq.heappush(heap, self.points[xStart][yStart])
        open_list.add((xStart, yStart))
        # 寻路循环
        while 1:
            # 从open list中取出代价最小点
            pMin = heapq.heappop(heap)
            open_list.remove((pMin.x, pMin.y))
            # 将这个点放入close list中
            close_list.add((pMin.x, pMin.y))
            self.changeState(self.points[pMin.x][pMin.y], PointState.CLOSED.value)
            # 遍历八个方向
            for i in range(-1, 2):
                for j in range(-1, 2):
                    if i == 0 and j == 0:
                        continue
                    # 当前要判断的点的坐标
                    xCur = pMin.x + i
                    yCur = pMin.y + j
                    # 如果这个点越界则跳过
                    if xCur >= self.width or xCur < 0 or yCur >= self.height or yCur < 0:
                        continue
                    pCur = self.points[xCur][yCur]
                    isCorner = (i != 0 and j != 0)
                    if isCorner:
                        # 如果将要判断的斜角方向被阻挡则跳过（如将要判断东南方向，若东方或南方被阻挡，则跳过）
                        if self.points[xCur][pMin.y].state == PointState.BARRIER.value or \
                                self.points[pMin.x][yCur].state == PointState.BARRIER.value:
                            continue

                    # 欧氏距离
                    if (xCur, yCur) not in close_list and self.points[xCur][yCur].state != PointState.BARRIER.value:
                        # 在open list中
                        if (xCur, yCur) in open_list:
                            # 起点到pMin再到pCur的代价比起点到pCur的代价小
                            if ((14 if isCorner else 10) + pMin.g) < pCur.g:
                                pCur.g = pMin.g + (14 if isCorner else 10)
                                pCur.f = pCur.g + 10 * (abs(xEnd - xCur) + abs(yEnd - yCur))    # 更新pCur的代价
                                pCur.father = pMin  # 将pMin设置为pCur的父节点
                        # 不在open list中
                        else:
                            pCur.g = pMin.g + (14 if isCorner else 10)
                            pCur.f = pCur.g + 10 * (abs(xEnd - xCur) + abs(yEnd - yCur))
                            pCur.father = pMin  # 将pMin设置为pCur的父节点
                            self.changeState(pCur, PointState.OPEN.value)
                            # 将这个点加入open list
                            open_list.add((xCur, yCur))
                            heapq.heappush(heap, pCur)

                    # # 曼哈顿距离
                    # if (xCur, yCur) not in close_list and self.points[xCur][yCur].state != PointState.BARRIER.value:
                    #     # 在open list中
                    #     if (xCur, yCur) in open_list:
                    #         # 起点到pMin再到pCur的代价比起点到pCur的代价小
                    #         if ((20 if i*j != 0 else 10) + pMin.g) < pCur.g:
                    #             pCur.g = pMin.g + (20 if i*j != 0 else 10)
                    #             pCur.f = pCur.g + 10 * (abs(xEnd - xCur) + abs(yEnd - yCur))  #  更新pCur的代价
                    #             pCur.father = pMin    # 将pMin设置为pCur的父节点
                    #     # 不在open list中
                    #     else:
                    #         pCur.g = pMin.g + (20 if i*j != 0 else 10)
                    #         pCur.f = pCur.g + 10 * (abs(xEnd - xCur) + abs(yEnd - yCur))
                    #         pCur.father = pMin    # 将pMin设置为pCur的父节点
                    #         self.changeState(pCur, PointState.OPEN.value)
                    #         # 将这个点加入open list
                    #         open_list.add((xCur, yCur))
                    #         heapq.heappush(heap, pCur)

            # 检测是否寻路完成
            if (xEnd, yEnd) in open_list:
                pNext = self.points[xEnd][yEnd]
                self.changeState(pNext, PointState.PATH.value)
                while pNext.father:
                    pNext = pNext.father
                    self.changeState(self.points[pNext.x][pNext.y], PointState.PATH.value)
                break
            # 如果寻路不完成但open list长度为0，则没有可达路径
                if len(open_list) == 0:
                    print('Unreachable!')
                    # 弹出一个消息框来提示没有路径
                    messagebox.showinfo("提示", "没有找到路径")
                    break

            # 等待绘制
            time.sleep(self.delay)


# 参数为地图高、宽、方格尺寸、延迟时间
demo = MiniMap(20, 20, 30, 0.05)
