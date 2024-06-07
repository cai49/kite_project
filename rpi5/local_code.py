import time
import paho.mqtt.publish as publish
import paho.mqtt.client as receive

mqtt_broker_address = "192.168.1.184"
default_topic = "default.channel"

lrotate_topic = "move.linear.rotate"
lmove_topic = "move.linear.traverse"

commands = ["LOCATE", "FORWARD", "ROTATE|RIGHT", "ROTATE|LEFT", "WAIT|C", "END"]

# mul = [{'topic':lmove_topic, 'payload':"LOCATE"},
#     {'topic':lrotate_topic, 'payload':"ROTATE"}]

mul = [(lmove_topic,"LOCATE"),
        (lmove_topic,"LOCATE"),
       (lrotate_topic,"ROTATE")]

message = "Hello, Raspberry Pi!"
publish.single(default_topic, message, hostname=mqtt_broker_address)
time.sleep(1)

# for i, command in enumerate(commands, start=0):
# 	publish.single(mqtt_channel, command, hostname=mqtt_broker_address)

# 	time.sleep(1)


print(mul[0][0])
publish.multiple(mul, hostname=mqtt_broker_address)

