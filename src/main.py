"""
Created By: Ismael Castro
"""

import sys

# PyQt6
from PyQt6.QtGui import QPixmap
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt6.QtCore import QPoint
import qtUi.main_window as UImain
import qtUi.connection_wizard as UIwzcom

# Paho
import paho.mqtt.publish as publish

app = QApplication(sys.argv)
MainWindow = QMainWindow()
ConnectionWizard = QWidget()

uiMain = UImain.Ui_MainWindow()
uiMain.setupUi(MainWindow)

MainWindow.show()

uiComWiz = UIwzcom.Ui_ConnectionWizard()
uiComWiz.setupUi(ConnectionWizard)

mqtt_broker_address = "192.168.1.184"
default_topic = "default.channel"

linear_rotation_topic = "move.linear.rotate"
linear_move_topic = "move.linear.traverse"
wait_directive_topic = "directive.wait"
locate_directive_topic = "directive.locate"
end_directive_topic = "directive.end"

package = []


def on_add_button_clicked():
    text_edit = uiMain.item_textedit.toPlainText().upper()
    on_add_command(text_edit)


def on_rotate_right_command_button_clicked():
    text_edit = "ROTATE|RIGHT"
    on_add_command(text_edit)


def on_rotate_left_command_button_clicked():
    text_edit = "ROTATE|LEFT"
    on_add_command(text_edit)


def on_front_command_button_clicked():
    text_edit = "FORWARD"
    on_add_command(text_edit)


def on_locate_command_button_clicked():
    text_edit = "LOCATE"
    on_add_command(text_edit)


def on_end_command_button_clicked():
    text_edit = "END"
    on_add_command(text_edit)


def on_add_command(text_edit):
    if text_edit == "":
        return

    indexes = uiMain.listWidget.selectedItems()

    if len(indexes) > 0:
        y = 0
        for i, k in enumerate(indexes):
            index = uiMain.listWidget.indexFromItem(k).row()
            if index > y:
                y = index

        print(f"Added item {text_edit} at index {y + 1}")
        uiMain.listWidget.insertItem(y + 1, text_edit)
    else:
        print(f"Added item {text_edit}")
        uiMain.listWidget.addItem(text_edit)

    # uiMain.item_textedit.clear()


def on_remove_button_clicked():
    indexes = uiMain.listWidget.selectedItems()

    num_indexes = len(indexes)

    if num_indexes > 1:
        for i, k in enumerate(indexes):
            index = uiMain.listWidget.indexFromItem(k).row()
            uiMain.listWidget.takeItem(index)
    elif num_indexes == 1:
        y = 0
        for i, k in enumerate(indexes):
            index = uiMain.listWidget.indexFromItem(k).row()
            if index > y:
                y = index

        print(f"Removed item: {uiMain.listWidget.item(y).text()}")
        uiMain.listWidget.takeItem(y)
    elif num_indexes == 0 and uiMain.listWidget.count() != 0:
        print(f"Removed item: {uiMain.listWidget.item(uiMain.listWidget.count() - 1).text()}")
        uiMain.listWidget.takeItem(uiMain.listWidget.count() - 1)


def on_clear_button_clicked():
    uiMain.listWidget.clear()


def on_close():
    app.quit()
    ConnectionWizard.close()


def on_open_com_wizard():
    point = MainWindow.rect().bottomRight()
    global_point = MainWindow.mapToGlobal(point)
    width = QPoint(-10, MainWindow.height() + 30)

    ConnectionWizard.move(global_point - width)

    ConnectionWizard.show()


def on_connection_connect():
    uiComWiz.icon_connection_status.setPixmap(
        QPixmap("C:/Users/CAI_4/Projects/Python/kite_project_proto/icons/led_green.png"))
    message = "Hello, Raspberry Pi!"
    publish.single(default_topic, message, hostname=mqtt_broker_address)


def on_disconnection():
    uiComWiz.icon_connection_status.setPixmap(
        QPixmap("C:/Users/CAI_4/Projects/Python/kite_project_proto/icons/led_red.png"))
    message = "#DISCONNECT"
    publish.single(default_topic, message, hostname=mqtt_broker_address)


def on_compile():
    on_open_com_wizard()
    on_connection_connect()

    items_text = [str(uiMain.listWidget.item(i).text()) for i in range(uiMain.listWidget.count())]

    actions = []
    for i, item in enumerate(items_text):
        if "ROTATE" in item:
            action = item.split("|")[1]
            item_topic = linear_rotation_topic
        elif "FORWARD" in item:
            action = item
            item_topic = linear_move_topic

        elif "WAIT" in item:
            action = item.split("|")[1]
            item_topic = wait_directive_topic
        elif "LOCATE" in item:
            action = item
            item_topic = locate_directive_topic
        elif "END" in item:
            action = item
            item_topic = end_directive_topic
        else:
            action = item
            item_topic = default_topic

        actions.append((item_topic, action))

    global package
    package = actions

    uiComWiz.package_viewer.setPlainText(list_to_str(items_text))


def list_to_str(items):
    return "\n".join(items)


def on_package_send():
    global package
    if package:
        publish.multiple(package, hostname=mqtt_broker_address)


uiMain.actionQuit.triggered.connect(on_close)
uiMain.action_open_connection_wizard.triggered.connect(on_open_com_wizard)
uiMain.item_add_button.clicked.connect(on_add_button_clicked)
uiMain.item_remove_button.clicked.connect(on_remove_button_clicked)
uiMain.clear_list_button.clicked.connect(on_clear_button_clicked)

uiMain.rright_command_button.clicked.connect(on_rotate_right_command_button_clicked)
uiMain.rleft_command_button.clicked.connect(on_rotate_left_command_button_clicked)
uiMain.front_command_button.clicked.connect(on_front_command_button_clicked)
uiMain.locate_command_button.clicked.connect(on_locate_command_button_clicked)
uiMain.end_command_button.clicked.connect(on_end_command_button_clicked)

uiMain.compile_button.clicked.connect(on_compile)

uiComWiz.connect_button.clicked.connect(on_connection_connect)
uiComWiz.disconnect_button.clicked.connect(on_disconnection)
uiComWiz.send_packages_button.clicked.connect(on_package_send)

sys.exit(app.exec())
