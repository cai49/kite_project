# Mosquitto Broker Utils

To check for the mosquitto status, within the CLI\
```systemctl status mosquitto```

To start / stop the mosquitto broker, within the CLI
- ```sudo /etc/init.d/mosquitto start```
- ```sudo /etc/init.d/mosquitto stop```

Appended the following lines of code with ```sudo vim /etc/mosquitto/mosquitto.conf```\
_listener 1883_ \
_allow_anonymous true_

