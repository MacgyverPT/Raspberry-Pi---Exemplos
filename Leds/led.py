#!/user/bin/python
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.cleanup()
GPIO.setwarnings(False)

GPIO.setup(17, GPIO.OUT)

print "Lights ON"
GPIO.output(17, GPIO.HIGH)
