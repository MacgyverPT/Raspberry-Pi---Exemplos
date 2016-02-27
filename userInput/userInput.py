#!/usr/bin/python
import os
import time
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(17, GPIO.OUT)

#variables
count = 0

os.system('clear')

#ask user how many time the led blinks
count = input("How many times would you like it to blink? ")
while count > 0:
	GPIO.output(17, GPIO.HIGH)
	time.sleep(1)
	GPIO.output(17, GPIO.LOW)
	time.sleep(1)
	count -= 1
