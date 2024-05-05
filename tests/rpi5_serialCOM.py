import serial
import time


serial = serial.Serial('/dev/ttyACM0', 115200, timeout=1.0)
#time.sleep(3)
serial.reset_input_buffer()
print("Serial OK")

try:
    while True:
        serial.write("Hello, Arduino!\n".encode('utf-8'))
        print("Message sent")
        time.sleep(1)
except KeyboardInterrupt:
    serial.close()
    print("Successfully Closed Serial COM")
