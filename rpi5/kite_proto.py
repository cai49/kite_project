import serial
import time
import paho.mqtt.client as mqtt

default_topic = "default.channel"

linear_rotation_topic = "move.linear.rotate"
linear_move_topic = "move.linear.traverse"
wait_directive_topic = "directive.wait"
locate_directive_topic = "directive.locate"
end_directive_topic = "directive.end"

serial = serial.Serial('/dev/ttyACM0', 115200, timeout=1.0)
time.sleep(0.1)
serial.reset_input_buffer()
print("Serial OK")


def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(default_topic)
    client.subscribe(linear_rotation_topic)
    client.subscribe(linear_move_topic)
    client.subscribe(wait_directive_topic)
    client.subscribe(locate_directive_topic)
    client.subscribe(end_directive_topic)


def on_message(client, userdata, msg):
    byte_payload = msg.payload
    topic = msg.topic

    payload = byte_payload.decode("utf-8")

    if topic == default_topic:
        process_default(payload)
    elif topic == linear_rotation_topic:
        process_linear_rotation(payload)
    elif topic == linear_move_topic:
        process_linear_motion(payload)
    elif topic == wait_directive_topic:
        process_wait_directive(payload)
    elif topic == locate_directive_topic:
        process_locate_directive(payload)
    elif topic == end_directive_topic:
        process_end_directive(payload)
    else:
        print(f"Topic not found! {topic}")


def on_disconnect(client, userdata, rc):
    print(f"Disconnected from client {client} with result code {rc}")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

client.connect("localhost", 1883, 60)
print("Listening Forever")


def process_default(payload):
    print(f"Processed default: {payload}")

    if "#DISCONNECT" in payload:
        client.disconnect()
        print("Successfully Disconnected from Client")

        serial.close()
        print("Successfully Closed Serial COM")


def process_linear_rotation(payload):
    print(f"Processed linear rotation: {payload}")

    serial.write(f"ROTATE|{payload}\n".encode('utf-8'))

    while serial.in_waiting <= 0:
        time.sleep(0.01)

    answer = serial.readline().decode('utf-8').rstrip()
    print(answer)


def process_linear_motion(payload):
    print(f"Processed linear motion: {payload}")

    serial.write(f"{payload}\n".encode('utf-8'))

    while serial.in_waiting <= 0:
        time.sleep(0.01)

    answer = serial.readline().decode('utf-8').rstrip()
    print(answer)


def process_wait_directive(payload):
    print(f"Processed wait directive with parameter: {payload}")

    serial.write(f"WAIT|{payload}\n".encode('utf-8'))

    while serial.in_waiting <= 0:
        time.sleep(0.01)

    answer = serial.readline().decode('utf-8').rstrip()
    print(answer)


def process_locate_directive(payload):
    print(f"Processed locate directive: {payload}")

    serial.write(f"{payload}\n".encode('utf-8'))

    while serial.in_waiting <= 0:
        time.sleep(0.01)

    answer = serial.readline().decode('utf-8').rstrip()
    print(answer)


def process_end_directive(payload):
    print(f"Processed end directive: {payload}")

    serial.write(f"{payload}\n".encode('utf-8'))

    while serial.in_waiting <= 0:
        time.sleep(0.01)

    answer = serial.readline().decode('utf-8').rstrip()
    print(answer)


try:
    client.loop_forever()
except:
    print("Something Happened Connecting to the Broker!")
