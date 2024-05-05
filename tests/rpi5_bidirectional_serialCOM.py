from serial import Serial
import time


serial = Serial('/dev/ttyACM0', 115200, timeout=1.0)
time.sleep(1)
serial.reset_input_buffer()
print("Serial OK")

try:
    while True:
        print("Send message to arduino")
        serial.write("Hello, Arduino!".encode('utf-8'))

        # Wait infinetly until the serial buffer has any data
        while serial.in_waiting <= 0:
            time.sleep(0.01)
        
        answer = serial.readline().decode('utf-8').rstrip()
        print(answer)
        time.sleep(1)
except KeyboardInterrupt:
    print("Successfully Closed Serial COM")
    serial.close()