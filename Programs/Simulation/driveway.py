#!/usr/bin/python

import random
import sys
from datetime import datetime, timedelta
import paho.mqtt.client as mqtt
import socket
import time

def on_publish(client, payload, result):
    pass

if __name__ == "__main__":
    args = sys.argv
    if '-r' in args:
        rate = float(args[args.index('-r') + 1])
    else:
        rate = 0.5

    if '-c' in args:
        numCars = int(args[args.index('-c') + 1])
    else:
        numCars = 50

    if '-s' in args:
        spots = int(args[args.index('-s') + 1])
    else:
        spots = 10

    mqtt_username = "#########"
    mqtt_password = "#########"
    mqtt_topic = "#########"
    mqtt_broker_ip = "#########"
    client = mqtt.Client()
    client.username_pw_set(mqtt_username, mqtt_password)
    client.on_publish = on_publish
    client.connect(mqtt_broker_ip, 1883)

    gsm_num = "99"

    while(1):
        client.publish(mqtt_topic, "\'{}111{}\'".format(gsm_num, datetime.now()))
        time.sleep(1.5)
        client.publish(mqtt_topic, "\'{}120{}\'".format(gsm_num, datetime.now()))
        time.sleep(2.5)
        client.publish(mqtt_topic, "\'{}110{}\'".format(gsm_num, datetime.now()))
        time.sleep(0.5)
        client.publish(mqtt_topic, "\'{}121{}\'".format(gsm_num, datetime.now()))
        time.sleep(1)
