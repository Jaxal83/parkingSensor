#!/usr/bin/python

import random
import simpy
import sys
from datetime import datetime, timedelta
import paho.mqtt.client as mqtt
import socket

def car(client, spotTracking, env, name, spaces, startTime, arrival_time, findSpotTimeReq, park_duration):
     # Simulate arriving at the spot
     yield env.timeout(arrival_time)

     # Request one of the spots
     print('Car {} arriving at {}'.format(name, startTime + timedelta(minutes = env.now)))
     with spaces.put(name) as req:
         yield req
     
         yield env.timeout(findSpotTimeReq)

         for i in range(len(spotTracking)):
             if spotTracking[i] == None:
                 spotTracking[i] = name
                 parkingSpot = i+1
                 break
         dt_str = str(startTime + timedelta(minutes = env.now))
         dt_obj = datetime.strptime(dt_str, '%Y-%m-%d %H:%M:%S.%f')
         msg_time = str(dt_obj.timestamp())        
         client.publish(mqtt_topic, "\'{}7{:0>3d}{}{}\'".format(gsm_num, parkingSpot, 1, msg_time))
         print('\t\t\t\t\t\t\t\tCar {} parked in spot {} at {}'.format(name, parkingSpot, startTime + timedelta(minutes = env.now)))
         yield env.timeout(park_duration)

         dt_str = str(startTime + timedelta(minutes = env.now))
         dt_obj = datetime.strptime(dt_str, '%Y-%m-%d %H:%M:%S.%f')
         msg_time = str(dt_obj.timestamp())
         client.publish(mqtt_topic, "\'{}7{:0>3d}{}{}\'".format(gsm_num, parkingSpot, 0, msg_time))
         print('\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tCar {} leaving spot {} at {}'.format(name, parkingSpot, startTime + timedelta(minutes = env.now)))
         yield spaces.get()
         spotTracking[parkingSpot-1] = None

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

    gsm_num = "42"

    startTime = datetime.now()
    print('Starting simulation at {} for {} cars at a rate of {} sec = 1 min\n\n'.format(startTime, numCars, rate))

    env = simpy.rt.RealtimeEnvironment(factor=rate)
    spaces = simpy.Store(env, capacity=spots)   #this should be about 300 for the entire garage
    spotTracking = [None] * spots

    for i in range(1, numCars + 1):                         #instantiate many cars
        env.process(car(client, spotTracking, env, i, spaces, startTime, random.randint(1, 1020), random.randint(20,300)/60.0, random.randint(random.randint(1, 240), random.randint(241,600))))
    env.run()
