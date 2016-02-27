#!/usr/bin/python

import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

#setup
GPIO.setup (17, GPIO.OUT)

#Main loop:
#turn LED on, wait a second and turn off
timeSleep = 1

#for i in range(0,10):
GPIO.output(17, GPIO. HIGH)
time.sleep(timeSleep)
GPIO.output(17, GPIO.HIGH)
time.sleep(timeSleep)

#clean
GPIO.cleanup()
