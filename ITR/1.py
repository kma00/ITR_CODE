from PyQt5 import QtCore, QtGui, QtWidgets
import random
import time
import serial
import asyncio
from aiohttp import web
import socket
import sys
import threading
#received_data = None
HOST = '192.168.87.210' # replace with the IP address of your Python PC
PORT = 9999 # replace with the port number used by the NodeMCU
#def socket_listener():
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    global received_data 
    s.bind((HOST, PORT))
    s.listen(1)
    print('Listening on', HOST, PORT)
    sys.stdout.flush()

    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        sys.stdout.flush()
        while True:
            data = conn.recv(1024)
            if not data:
                break
            received_data = int (data.decode())
            print('Received data:', data.decode())
            sys.stdout.flush()
#threading.Thread(target=socket_listener).start()
#socket_thread = threading.Thread(target=socket_listener)
#socket_thread.start()

async def handle_data(request):
    data = await request.text()
    print("Received data:", data)
    return web.Response(text="OK")

app = web.Application()
app.router.add_post('/', handle_data)

# Replace 'your_ip_address' and 'your_port' with the IP address and port of your Arduino
#ser = serial.serial_for_url('socket://192.168.1.8:9999', timeout=1)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(0, 0, 801, 601))
        self.label_6.setText("")
        self.label_6.setPixmap(QtGui.QPixmap("back.png"))
        self.label_6.setScaledContents(True)
        self.label_6.setObjectName("label_6")
        self.label_product = QtWidgets.QLabel(self.centralwidget)
        self.label_product.setGeometry(QtCore.QRect(240, 40, 331, 101))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label_product.setFont(font)
        self.label_product.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignTrailing | QtCore.Qt.AlignVCenter)
        self.label_product.setObjectName("label_product")
        self.label_obstacle = QtWidgets.QLabel(self.centralwidget)
        self.label_obstacle.setGeometry(QtCore.QRect(160, 170, 151, 141))
        self.label_obstacle.setStyleSheet("background-color: red;")
        self.label_obstacle.setText("")
        self.label_obstacle.setObjectName("label_obstacle")
        self.label_transport = QtWidgets.QLabel(self.centralwidget)
        self.label_transport.setGeometry(QtCore.QRect(450, 170, 151, 141))
        self.label_transport.setStyleSheet("background-color: green;")
        self.label_transport.setText("")
        self.label_transport.setObjectName("label_transport")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(60, 340, 671, 201))
        self.label_4.setText("")
        self.label_4.setPixmap(QtGui.QPixmap("noproduct.png"))
        self.label_4.setScaledContents(True)
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(60, 30, 151, 81))
        self.label_5.setText("")
        self.label_5.setPixmap(QtGui.QPixmap("logo.png"))
        self.label_5.setScaledContents(True)
        self.label_5.setObjectName("label_5")
        MainWindow.setCentralWidget(self.centralwidget)

    def readData(self):
        global received_data
        try:
            data = ser.read().decode('utf-8').strip()
        except Exception as e:
            print(f"Error reading data: {e}")
            data = ""
        if  received_data == 0:
            self.label_transport.setStyleSheet("background-color: transparent;")
            self.label_4.setPixmap(QtGui.QPixmap("noproduct.png"))
            self.label_product.setText("No product")
            self.label_obstacle.setStyleSheet("background-color: transparent;")
        elif received_data == 45:
            #product_num = int(data[1])
            self.label_transport.setStyleSheet("background-color: green;border-radius: 75px;")
            self.label_4.setPixmap(QtGui.QPixmap(f"received{1}.png"))
            self.label_product.setText(f"Product {1} received")
            QtCore.QTimer.singleShot(2000, lambda: self.label_transport.setStyleSheet("background-color: transparent;"))
        elif received_data == 5:
            #product_num = int(data[1])
            self.label_transport.setStyleSheet("background-color: green;border-radius: 75px;")
            self.label_4.setPixmap(QtGui.QPixmap(f"delivered{2}.png"))
            self.label_product.setText(f"Product {2} delivered")
            QtCore.QTimer.singleShot(2000, lambda: self.label_transport.setStyleSheet("background-color: transparent;"))
        elif received_data == 35:
            self.label_transport.setStyleSheet("background-color: transparent;")
            self.label_obstacle.setStyleSheet("background-color: red;")
            self.label_4.setPixmap(QtGui.QPixmap("noproduct.png"))
            self.label_product.setText("Obstacle")

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Product Transport"))
        self.label_product.setText(_translate("MainWindow", "No product"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    timer = QtCore.QTimer()
    timer.timeout.connect(ui.readData)
    timer.start(1000)
    sys.exit(app.exec_())