import serial
import time


serial = serial.Serial('/dev/ttyACM0', 115200, timeout=1.0)
#time.sleep(3)
serial.reset_input_buffer()
print("Serial OK")

try:
    while True:
        if serial.in_waiting > 0:
            line = serial.readline().decode('utf-8').rstrip()
            print(line)
        
        time.sleep(0.01)
except KeyboardInterrupt:
    serial.close()
    print("Successfully Closed Serial COM")
