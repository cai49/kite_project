# Form implementation generated from reading ui file '.\add_items.ui'
#
# Created by: PyQt6 UI code generator 6.7.0
#
# WARNING: Any manual changes made to this file will be lost when pyuic6 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt6 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(420, 500)
        self.centralwidget = QtWidgets.QWidget(parent=MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.listWidget = QtWidgets.QListWidget(parent=self.centralwidget)
        self.listWidget.setGeometry(QtCore.QRect(20, 60, 181, 251))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.listWidget.setFont(font)
        self.listWidget.setContextMenuPolicy(QtCore.Qt.ContextMenuPolicy.PreventContextMenu)
        self.listWidget.setFrameShape(QtWidgets.QFrame.Shape.StyledPanel)
        self.listWidget.setDragDropMode(QtWidgets.QAbstractItemView.DragDropMode.DragDrop)
        self.listWidget.setDefaultDropAction(QtCore.Qt.DropAction.MoveAction)
        self.listWidget.setSelectionMode(QtWidgets.QAbstractItemView.SelectionMode.ContiguousSelection)
        self.listWidget.setObjectName("listWidget")
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        self.item_add_button = QtWidgets.QPushButton(parent=self.centralwidget)
        self.item_add_button.setGeometry(QtCore.QRect(20, 380, 181, 61))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.item_add_button.setFont(font)
        self.item_add_button.setObjectName("item_add_button")
        self.items_label = QtWidgets.QLabel(parent=self.centralwidget)
        self.items_label.setGeometry(QtCore.QRect(20, 30, 181, 21))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.items_label.setFont(font)
        self.items_label.setObjectName("items_label")
        self.item_textedit = QtWidgets.QPlainTextEdit(parent=self.centralwidget)
        self.item_textedit.setGeometry(QtCore.QRect(20, 330, 181, 41))
        self.item_textedit.setObjectName("item_textedit")
        self.item_remove_button = QtWidgets.QPushButton(parent=self.centralwidget)
        self.item_remove_button.setGeometry(QtCore.QRect(220, 380, 181, 61))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.item_remove_button.setFont(font)
        self.item_remove_button.setObjectName("item_remove_button")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(parent=MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 420, 21))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtWidgets.QMenu(parent=self.menubar)
        self.menuFile.setObjectName("menuFile")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(parent=MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionQuit = QtGui.QAction(parent=MainWindow)
        self.actionQuit.setObjectName("actionQuit")
        self.menuFile.addAction(self.actionQuit)
        self.menubar.addAction(self.menuFile.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        MainWindow.setTabOrder(self.listWidget, self.item_textedit)
        MainWindow.setTabOrder(self.item_textedit, self.item_add_button)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        __sortingEnabled = self.listWidget.isSortingEnabled()
        self.listWidget.setSortingEnabled(False)
        item = self.listWidget.item(0)
        item.setText(_translate("MainWindow", "1"))
        item = self.listWidget.item(1)
        item.setText(_translate("MainWindow", "2"))
        item = self.listWidget.item(2)
        item.setText(_translate("MainWindow", "3"))
        item = self.listWidget.item(3)
        item.setText(_translate("MainWindow", "4"))
        item = self.listWidget.item(4)
        item.setText(_translate("MainWindow", "5"))
        item = self.listWidget.item(5)
        item.setText(_translate("MainWindow", "6"))
        self.listWidget.setSortingEnabled(__sortingEnabled)
        self.item_add_button.setText(_translate("MainWindow", "Add Item"))
        self.items_label.setText(_translate("MainWindow", "Items List"))
        self.item_textedit.setPlainText(_translate("MainWindow", "Haha"))
        self.item_remove_button.setText(_translate("MainWindow", "Remove Item"))
        self.menuFile.setTitle(_translate("MainWindow", "File"))
        self.actionQuit.setText(_translate("MainWindow", "Quit"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec())
