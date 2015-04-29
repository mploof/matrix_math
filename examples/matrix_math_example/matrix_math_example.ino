#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"



Spline spline;

void setup() {
	Serial.begin(9600);

	float points[] = {
		  0,  0,
		  2, 0.25,
		  //5,  3,
		  //7,  5,
		  9,  9
	};

	
	spline.setInterpPts(points, 4);
	spline.printInterpPts(true);
	//spline.printCtrlPts("Control points");
	
	int curve_points = 60;
	//spline.getCurvePts(curve_points);
	//spline.printCurvePts("Curve points");

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
				spline.printInterpPts(false);
				blink(1);
				break;
			}
			case '2':
			{
				spline.printCtrlPts(false);
				blink(2);
				break;
			}
			case '3':
			{
				spline.printCurvePts(false);
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