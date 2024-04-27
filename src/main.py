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
    ui.listWidget.addItem(text_edit)
    print(text_edit)


def on_close():
    app.quit()


ui.actionQuit.triggered.connect(on_close)
ui.item_add_button.clicked.connect(on_add_button_clicked)

sys.exit(app.exec())
