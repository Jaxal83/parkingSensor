#!/usr/bin/python

import paho.mqtt.client as mqtt
import socket
from datetime import datetime
import time
import mysql.connector
mydb = mysql.connector.connect(
    host='#########',
    user='#########',
    password='#########',
    database='#########'
)
dbcursor=mydb.cursor()
sqlInsert = "INSERT INTO parkingSpots (spotId, status, tStamp) VALUES (%s, %s, %s)"
garageStatus = []

mqtt_username = "#########"
mqtt_password = "#########"
mqtt_listen_topic = "#########"
mqtt_status_topic = "#########"
mqtt_broker_ip = "#########"

client = mqtt.Client()
client.username_pw_set(mqtt_username, mqtt_password)

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_listen_topic)

def on_publish(client, payload, result):
    pass

def on_message(client, userdata, message):
    topic = message.topic
    if topic  == "parkitcdaStatus":
	     #send current garage status
        client.publish(mqtt_status_topic, "".join(garageStatus))
    elif topic == "parkitcda":
        msg = str(message.payload.decode("utf-8"))
        origin = msg[1:3]
        if origin == "99":
            spotId = int(msg[4])
            status = int(msg[5])
            tStamp = msg[6:-1]
#           print("id: {}    s: {}    t: {}".format(spotId, status, tStamp))
#           tStamp2 = datetime.datetime.fromtimestamp(tStamp*1000)
#           print(tStamp2)
        elif origin == "42":
            spotId = int(msg[4:7])
            status = int(msg[7])
            tStamp = msg[8:-1]
#           print("m: {}    id: {}    s: {}    t: {}".format(msg, spotId, status, tStamp))
#           tStamp2 = datetime.fromtimestamp(float(tStamp))
#           print(tStamp2)
        else:
            print(msg)

        if len(garageStatus) < spotId-1:
            for i in range(len(garageStatus)+1, spotId):
                garageStatus.append(i)

        sqlVal = (spotId, status, datetime.fromtimestamp(float(tStamp)))
        dbcursor.execute(sqlInsert, sqlVal)
        mydb.commit()
#       print(dbcursor.rowcount, 'record inserted.')

        garageStatus[spotId-1] = status

if __name__ == "__main__":
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_publish = on_publish
    client.connect(mqtt_broker_ip, 1883)
    client.loop_start()

    try:
        while True:
            time.sleep(1)

    except KeyboardInterrupt:
        print("exiting")
        client.disconnect()
        client.loop_stop()
