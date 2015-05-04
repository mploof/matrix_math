#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"



Spline spline;
bool debug = true;
enum{WAIT, RECEIVE, SEND};
int state = WAIT;

void setup() {
	Serial.begin(9600);

	//float points[] = {
	//	  0,  0,
	//	//2, 0.25,
	//	//5,  3,
	//	  7,  5,
	//	  9,  9,
	//	  -1
	//};


	//spline.setInterpPts(points);
	//spline.printInterpPts(true);
	////spline.printCtrlPts("Control points");
	//
	//int curve_points = 60;
	//spline.getCurvePts(curve_points);
	////spline.printCurvePts("Curve points");

	//// Send code to indicate Arduino is ready to send data
	////Serial.print(1);

	////spline.printCurvePts(true);
}

void loop() {

	// Wait for initial command receive / send command
	switch (state){
	case WAIT:
		wait();
		break;
	case RECEIVE:
		receive();
		break;
	case SEND:
		send();
		break;
	}

	// Delay between checks
	delay(10);
}

void wait(){

	if (Serial.available() > 0){
		int val = Serial.read();
		blink(val);

		// Flush the serial buffer
		while (Serial.available()){
			Serial.read();
		}

		switch (val) {
		case '1':
			
			for (byte i = 0; i < 10; i++){
				delay(100);
				Serial.print(9);
			}
			state = RECEIVE;
		case '2':
			Serial.print(1);
			state = SEND;
		default:
			state = WAIT;
			break;
		}

		// Flush the serial buffer
		while (Serial.available()){
			Serial.read();
		}
	}
}

void receive(){
	
	while (1){
		if (Serial.available()){
			char val = Serial.read();
		}
	}

}

	
void send(){
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