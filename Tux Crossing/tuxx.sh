#!/bin/bash

#
# TUX CROSSING:
# 
# Tux Crossing. A variant on the UK "Pelican" crossing for pedestrians
# going over roads.
# 
# There is a set of Red, Yellow (sometimes called amber) and Green
# traffic lights to control the traffic, and a "Red Man" and "Green Man"
# indicators for pedestrians, and a button for them push.
# Push the button and the lights cycle to red, then the Green Man comes
# on(often with a beeping sound), then after a short while the Green Man
# starts to flash, meaning to not start crossing, and the yellow traffic
# light flashes too - meaning that if the crossing is clear, traffic
# can pass... The after a few more seconds the flashing stops and it
# revers to Go for traffic and Stop for pedestrians.

#our lamps (using wiringPi pin numbers)
red = 0
yellow = 1
green = 2
redMan = 3
greenMan = 4
button = 8


#Setup
setup(){
	echo Setup
	
	for i in $red $yellow $green $redMan $greenMan ; do gpio mode $i out ; done
	for i in $red $yellow $green $redMan $greenMan ; do gpio mode $i 0 ; done
	
	gpio write $green 1
	gpio write $redMan 1
	gpio mode $button in
}


#WaitButton
waitButton(){
	echo -n "Waiting for button..."
	while['gpio read $button' = 1]; do
		sleep 0.1
	done
	echo "Got it"
}


#StopTraffic
stopTraffic(){
	echo -n "Stopping traffic..."
	gpio write $green 0
	gpio write $yellow 1
	sleep 2
	gpio write $yellow 0
	gpio write $red 1
	sleep 2
	echo "stopped"
}


#walk
walk(){
	echo "Walk now..."
	gpio write $redMan 0
	gpio write greenMan 1
	sleep 10
	gpio write $red 0
	gpio write $yellow 
	echo "Walked"
}


#graceTime
graceTime(){
	echo "Grace time..."
	for i in 'seq 0 7' ; do
		sleep 0.5
		gpio write $greenMan 0
		gpio write $yellow 0
		sleep 0.5
		gpio write $greenMan 1
		gpio write $yellow 1
	done
	echo "time up!"
}


#startTraffic
startTraffic(){
	echo "starting traffic..."
	gpio write $greenMan 0
	gpio write $redMan 1
	sleep 0.5
	gpio write $yellow 0
	gpio write $green 1
	echo "going"
}


#MAIN PROGAM
setup
while true; do
	waitButton
	stopTraffic
	walk
	graceTime
	startTraffic
done
