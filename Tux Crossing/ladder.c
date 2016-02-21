
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <wiringPi.h>

#ifndef TRUE
# define TRUE (1==1)
# define FALSE (1==2)
#endif

#undef DEBUG

//input button
#define Button 8

//map the LEDs to the hardware pins using wiringPi pin numbers
cont int ledMap[12] = {13, 12, 11, 10, 7, 6, 5, 4, 3, 2, 1, 0};

//Some constants
const double vBatt		=	   9.0; //volts
const double capacitor	=	   0.001; //1000uF
const double rCharge	=	2200.0; //ohms
const double rDischarge =  68000.0; //ohms
const double timeInc	=	   0.01; //seconds
double vCharge, vCap, vCapLast;



/*
 * Setup: Program de GPIO correctly and initialise the lamps
 */
void setup(void){
	int i;
	
	if(geteuid() != 0){
		fprintf(stderr, "ladder: Need to be root to run (sudo?)\n";
		exit(0);
	}
	
	if(wiringPiSetup() == -1){
		exit(1);
	}
	
	for(i = 0; i < 12; i++){
		digitalWrite(ledMap[i], 0);
		pinMode(ledMap[i], OUTPUT);
	}
	
	pinMode(BUTTON; OUTPUT);
	
	vCharge = rDischarge / (rCharge + rDischarge) * vBatt;
	vCap = vCapLast = 0.0;
}

/*
 * IntroLeds: Put a little pattern on the LEDs to start with
 */
 void introLeds(void){
	 int i, j;
	 
	 printf("Pi Ladder\n");
	 printf(" ========\n\n");
	 printf("	  vBatt: %6.2f volts\n", vBatt);
	 printf("	rCharge: %6.0f ohms\n", rCharge);
	 printf("rDischarge: %6.0f ohms\n", rDischarge);
	 printf("	vCharge: %6.2f volts\n", vCharge);
	 printf(" capacitor: %6.0f uF\n", capacitor * 1000.0);
	 
	 //flash 3x
	 for (j = 0; j < 3; j++){
		 for(i = 0; i < 12; i++)
			digitalWrite(ledMap[i], 1);
		 delay(500);
		 for(i = 0; i < 12; i++)
			digitalWrite(ledMap[i], 0);
		 delay(100);
	 }
	 
	 //All ON
	 for (i = 0; i < 12; i++)
		 digitalWrite(ledMap[i], 1);
	 delay(500);
	 
	 //countdown...
	 for ( i = 11; i >= 0; i--){
		 digitalWrite(ledMap[i], 0);
		 delay(100);
	 }
	 delay(500);
}


/*
 * WinningLeds: Put a little patterns on the LEDs to start with
 */
void winningLeds(void){
	int i, j;
	
	//flash 3x
	 for (j = 0; j < 3; j++){
		 for(i = 0; i < 12; i++)
			digitalWrite(ledMap[i], 1);
		 delay(500);
		 for(i = 0; i < 12; i++)
			digitalWrite(ledMap[i], 0);
		 delay(100);
	 }
	 
	 //All ON
	 for (i = 0; i < 12; i++)
		 digitalWrite(ledMap[i], 1);
	 delay(500);
	 
	 //countup...
	 for(i = 0; i < 12; i++){
		 digitalWrite(ledMap[i], 0);
		 delay(100);
	 }
	 delay(500);
}


/*
 * Charge capacitor: dischargeCapacitor
 * 		Add or remove charge to the capacitor
 * 		Standard capacitor formulae
 */
 void chargeCapacitor(void){
	vCap = (vCapLast - vCharge) * exp(-timeInc / (rCharge * Capacitor)) + vCharge;
	
	#ifdef DEBUG
		printf("vCap: %7.4f\n", vCap);
	#endif
	
	vCapLast = vCap;
}

void dischargeCapacitor(void){
	vCap = vCapLast * exp(-timeInc / (rDischarge * capacitor));
	
	#ifdef DEBUG
		printf("-vCap: %7.4f\n", vCap);
	#endif
	
	vCapLast = vCap;
}


/*
 * ledBarGraph: Output the supplied number as a bargraph on the LEDs
 */
void ledBargraph(double value, int topLedOn){
	int topLed = (int)floor (value / rCharge * 12.0) + 1;
	int i;
	
	if(topLed > 12)
		topLed = 12;
	
	if(!topLedOn)
		--topLed;
		
	for(i = 0; i < topLed; i++){
		digitalWrite(ledMap[i], 1);
	}
	
	for(i = topLed; i < 12; i++){
		digitalWrite(ledMap[i], 0);
	}
}


/*
 * ledOnAction: Make sure the leading LED is on and check the button
 */
 void ledOnAction(void){
	 if(digitalRead (BUTTON) == LOW){
		 chargeCapacitor();
		 ledBargraph(vCap, TRUE);
	 }
}


/*
 * ledOffAction: Make sure the leading LED is off and check the button
 */
void ledOffAction(void){
	dischargeCapacitor();
	
	//are we still pushing the button?
	if(digitalRead (BUTTON) == LOW){
		vCap = vCapLast = 0.0;
		ledBargraph(vCap, FALSE);
	
	//wait until we release the button
	while(digitalRead (BUTTON) == LOW)
		delay(10);
	}
}


/*
 * MAIN PROGRAM
 */
int main(void){
	unsigned int then, ledOnTime, ledOffTime;
	unsigned int ourDelay = (int)(1000.0 * timeInc);
	
	setup();
	introLeds();
	
	//setup the LED times - TODO reduce the ON time as the game progress
	ledOnTime = 1000;
	ledOffTime = 1000;
	
	//this is our Gate/Squarewave loop
	for(;;){
		//LED ON
		(void)ledBargraph(vCap, TRUE);
		then = millis() + ledOnTime;
		while(mills() < then){
			ledOnAction();
			delay(ourDelay);
		}
		
		//Have you won yet?
		//We need vCap to be in the loop 12th of the vCharge
		if(vCap > (11.0 / 12.0 * vCharge)){ //Woo hoo!!
			winningLeds();
			while(digitalRead(BUTTON) == HIGH)
				delay(10);
			while(digitalRead (BUTTON) == LOW)
				delay(10);
			vCap = vCapLast = 0.0;
		}
		
		//LED OFF
		(void)ledBargraph(vCap, FALSE);
		then = millis() + ledOffTime;
		while(millis() < then){
			ledOff();
			delay(ourDelay);
		}
	}
	
	return 0;
}






















