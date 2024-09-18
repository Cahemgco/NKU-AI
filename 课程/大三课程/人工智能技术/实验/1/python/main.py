import sys
import time
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QMessageBox, QLabel, QLineEdit
from PyQt5.QtGui import QPainter, QColor, QPen, QPixmap
from PyQt5.QtCore import Qt

class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

        self.Num_of_Queens = 8
        self.Draw_Flag = 0
        self.results = dict()
        self.current_result = []

    def initUI(self):
        self.label_numberbox = QLabel('皇后数量：', self)
        self.label_numberbox.move(440, 20)
        self.label_numberbox.resize(60, 20)

        # 皇后的数量N
        self.number_input = QLineEdit(self)
        self.number_input.move(500, 20)
        self.number_input.resize(40, 20)

        # 耗时
        self.time_text = QLabel('搜索耗时：', self)
        self.time_used = QLabel(self)
        self.time_text.move(440, 100)
        self.time_used.move(530, 100)

        # 开始与退出
        Start_Button = QPushButton('开始搜索',self)
        Exit_Button = QPushButton('退出',self)
        Start_Button.clicked.connect(self.Start)
        Exit_Button.clicked.connect(self.Exit)
        Start_Button.move(475, 140)
        Exit_Button.move(475, 200)

        # 输出所有解
        OutputButton = QPushButton('输出所有解', self)
        OutputButton.pressed.connect(self.solution)
        OutputButton.move(475, 260)

        self.setGeometry(300, 300, 620, 410)
        self.setWindowTitle('N-皇后问题')
        self.show()

    def paintEvent(self, e):
        if self.Draw_Flag == 0:
            self.Draw_Board()
        else:
            self.Draw_Chess()


    def Start(self):
        queen_input = self.number_input.text()
        try:
            Num = int(queen_input)
            if Num <= 0:
                # 输入不是正整数，弹出警告窗口
                QMessageBox.warning(self, '警告', '您输入的皇后数量非法，请输入一个正整数！')
                self.number_input.clear()  # 清空文本框内容
                return
            else:
                self.Draw_Flag = 1
                self.Num_of_Queens = Num


                time_begin = time.time()    # 开始计时
                self.results = self.Get_All_Results(self.Num_of_Queens)  # 求解
                time_end = time.time()  # 结束计时

                Time_cost = round(time_end - time_begin, 5)
                Time_cost = str(Time_cost)
                self.time_used.setText((Time_cost) + 's')
                self.time_used.adjustSize()

                Num_of_results = len(self.results)

                self.Draw_Flag = 0

                QMessageBox.information(self, '结果', '共有' + str(Num_of_results) + '种解')
        except ValueError:
            # 无法转换为整数，弹出警告窗口
            QMessageBox.warning(self, '警告', '您输入的皇后数量非法，请输入一个正整数！')

    def Get_All_Results(self, num):
        all_results = []
        current_board = []
        self.Get_All_Results_Current(current_board, all_results)
        return all_results

    def Get_All_Results_Current(self, current_board, all_results):
        next_x = len(current_board)
        for next_y in range(self.Num_of_Queens):
            flag = False
            # 遍历下一行可摆的位置
            for i in range(next_x):
                if next_y == current_board[i] or next_y == current_board[i] + next_x - i or next_y == current_board[i] - next_x + i:
                    flag = True
                    break

            # 判断是否与之前的棋子冲突
            if not flag:
                new_board = current_board.copy()
                new_board.append(next_y)
                if len(new_board) == self.Num_of_Queens:
                    all_results.append(new_board.copy())
                    self.current_result = new_board.copy()
                    self.repaint()
                else:
                    self.Get_All_Results_Current(new_board, all_results)

    def Exit(self):
        self.close()

    def Draw_Board(self):
        qp = QPainter()
        qp.begin(self)

        pen = QPen(Qt.black, 2, Qt.SolidLine)
        qp.setPen(pen)
        qp.drawLine(5, 5, 405, 5)
        qp.drawLine(5, 5, 5, 405)
        qp.drawLine(5, 405, 405, 405)
        qp.drawLine(405, 5, 405, 405)

        temp = int(400 / self.Num_of_Queens)    # 棋盘格大小

        for i in range(self.Num_of_Queens):
            for j in range(self.Num_of_Queens):
                if (i % 2 == 0 and j % 2 == 0) or (i % 2 == 1 and j % 2 == 1):
                    qp.setBrush(QColor(128, 128, 128))  # 设置棋盘格为灰色
                    qp.drawRect(int(5 + i * temp), int(5 + j * temp), temp, temp)
                else:
                    qp.setBrush(QColor(255, 255, 255))  # 设置棋盘格为白色
                    qp.drawRect(int(5 + i * temp), int(5 + j * temp), temp, temp)

        qp.end()

    def Draw_Chess(self):
        temp = int(400 / self.Num_of_Queens)
        qp = QPainter()
        qp.begin(self)
        pen = QPen(Qt.black, 2, Qt.SolidLine)
        qp.setPen(pen)
        qp.drawLine(5, 5, 405, 5)
        qp.drawLine(5, 5, 5, 405)
        qp.drawLine(5, 405, 405, 405)
        qp.drawLine(405, 5, 405, 405)

        for i in range(self.Num_of_Queens):
            for j in range(self.Num_of_Queens):
                if (i % 2 == 0 and j % 2 == 0) or (i % 2 == 1 and j % 2 == 1):
                    qp.setBrush(QColor(128, 128, 128))
                    qp.drawRect(int(5 + i * temp), int(5 + j * temp), temp, temp)
                else:
                    qp.setBrush(QColor(255, 255, 255))
                    qp.drawRect(int(5 + i * temp), int(5 + j * temp), temp, temp)

        queen = QPixmap('queen.jpg')

        for i in range(self.Num_of_Queens):
            x = 5 + temp * i
            y = 5 + temp * (self.current_result[i])
            A = int(x + 0.1 * temp)
            B = int(y + 0.1 * temp)
            C = int(0.8 * temp)
            D = int(0.8 * temp)
            qp.drawPixmap(A, B, C, D, queen)

        qp.end()

    def solution(self):
        if len(self.results) == 0:
            QMessageBox.information(self, '提示', '没有搜索出可行解')
        else:
            self.Draw_Flag = 1

            for i in range(len(self.results)):
                self.current_result = self.results[i]
                self.repaint()
                time.sleep(0.5)


    def closeEvent(self, event):
        reply = QMessageBox.question(self, '提示',
                                     "确定退出?", QMessageBox.Yes |
                                     QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = Window()
    sys.exit(app.exec_())



