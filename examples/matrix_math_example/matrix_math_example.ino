#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"



Spline spline;

void setup() {
	Serial.begin(9600);

	int points[] = {
		-9,  0,
		-8,  3,
		-5,  4,
		 0,  3,
		 2,  4,
		 4,  3,
		 7,  5
	};

	
	spline.setInterpPts(points, 7);

	
	int curve_points = 50;
	spline.getCurvePts(curve_points);


	// Send code to indicate Arduino is ready to send data
	//Serial.print(1);

	//spline.printCurvePts(true);
}

void loop() {

	// Wait till a command is received
	if (Serial.available() > 0){
		
		char val = Serial.read();

		// Flush the serial buffer
		while (Serial.available()){
			Serial.read();
		}

		switch (val) {
			case '1':
			{
				spline.printInterpPts();
				blink(1);
				break;
			}
			case '2':
			{
				spline.printCtrlPts();
				blink(2);
				break;
			}
			case '3':
			{
				spline.printCurvePts();
				blink(3);
				break;
			}
			default:
				break;
		}		
		
		// Flush the serial buffer
		while (Serial.available()){
			Serial.read();
		}
		
	}

	// Delay between checks of the serial buffer
	delay(10);
}

void blink(int p_count){
	for (byte i = 0; i < p_count; i++){
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
		delay(100);
	}
}