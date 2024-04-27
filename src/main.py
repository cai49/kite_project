import sys
from PyQt6 import uic
from PyQt6.QtWidgets import QApplication, QMainWindow
import qtUi.add_items as UI

app = QApplication(sys.argv)
MainWindow = QMainWindow()
ui = UI.Ui_MainWindow()
ui.setupUi(MainWindow)
MainWindow.show()


def on_add_button_clicked():
    text_edit = ui.item_textedit.toPlainText()

    indexes = ui.listWidget.selectedItems()

    if len(indexes) > 0:
        y = 0
        for i, k in enumerate(indexes):
            index = ui.listWidget.indexFromItem(k).row()
            if index > y:
                y = index

        print(f"Added item {text_edit} at index {y+1}")
        ui.listWidget.insertItem(y+1, text_edit)
    else:
        print(f"Added item {text_edit}")
        ui.listWidget.addItem(text_edit)


def on_remove_button_clicked():
    indexes = ui.listWidget.selectedItems()

    num_indexes = len(indexes)

    if num_indexes > 1:
        for i, k in enumerate(indexes):
            index = ui.listWidget.indexFromItem(k).row()
            ui.listWidget.takeItem(index)
    elif num_indexes == 1:
        y = 0
        for i, k in enumerate(indexes):
            index = ui.listWidget.indexFromItem(k).row()
            if index > y:
                y = index

        ui.listWidget.takeItem(y)
    else:
        ui.listWidget.takeItem(ui.listWidget.count()-1)


def on_close():
    app.quit()


ui.actionQuit.triggered.connect(on_close)
ui.item_add_button.clicked.connect(on_add_button_clicked)
ui.item_remove_button.clicked.connect(on_remove_button_clicked)

sys.exit(app.exec())
