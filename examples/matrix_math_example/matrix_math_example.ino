#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"


const int XY_SIZE = 2;

Spline spline;
//float* KF_locations = NULL;
float KF_locations[17];
bool debug = true;
enum{WAIT, RECEIVE, SEND};
int state = WAIT;
int KF_count = 0;


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

/************************* State Functions *************************/

void wait(){

	if (Serial.available() > 0){
		int val = Serial.read();		

		//// Flush the serial buffer
		//while (Serial.available()){
		//	Serial.read();
		//}

		switch (val) {
		case 1:				
			response(true);
			state = RECEIVE;
			break;
		case 2:
			response(true);
			state = SEND;
			break;
		default:
			state = WAIT;
			break;
		}
	}
}

void receive(){

	// Get the number of incoming points	
	while (true){
		if (Serial.available()){
			KF_count = Serial.read();
			response(KF_count);
			break;
		}
	}
	
	//// Free the location array if it's already been allocated
	//if (KF_locations != NULL)
	//	delete KF_locations;
	//// Allocate new memory for the locations. Add an extra location to hold delimiter
	//KF_locations = new float[(KF_count * XY_SIZE) + 1];

	// Store the XY coordinates for each of the points
	for (byte i = 0; i < KF_count * XY_SIZE; i++){
		delay(50);
		float val = readFloat();
		KF_locations[i] = val;
		response(round(val));
		// Check whether the calculated value was really an error code
		//if (val > -5000){
		//	//blink(2);
		//	response(val);
		//}
		//else{
		//	blink(10);
		//	response(false);
		//	state = WAIT;
		//	return;
		//}			
		
	}
	// Add end of array delimiter
	KF_locations[KF_count * XY_SIZE + 1] = -1;

	// Set the interpolation points from the array values
	spline.setInterpPts(KF_locations);
	int curve_points = 10;
	spline.getCurvePts(curve_points);

	// Signal to Processing that computations are completed
	response(true);

	// Go back to waiting state
	state = SEND;
}
	
void send(){	
	spline.printCurvePts(false);
	state = WAIT;
}




/************************* Helper Functions *************************/

void response(int p_ok){
	Serial.println(p_ok);
}

void blink(int p_count){
	for (byte i = 0; i < p_count; i++){
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
		delay(100);
	}
}

/** Read four bytes from serial buffer and return converted float value

@return
A float
*/

float readFloat() {
	const int SIZE = 4;
	const int TIMEOUT = 4000;
	uint8_t buf[SIZE];	
	unsigned long time = millis();	
	while (millis() - time < TIMEOUT){
		if (Serial.available() == SIZE){			
			for (byte i = 0; i < SIZE; i++){
				buf[SIZE - 1 - i] = Serial.read();
			}
			float ret;
			unsigned long * _fl = (unsigned long *)(&ret);
			*_fl = (unsigned long)(((unsigned long)buf[0] << 8) + (unsigned long)buf[1]) << 16;
			*_fl |= (unsigned long)(((unsigned long)buf[2] << 8) + ((unsigned long)buf[3]));
			return(ret);
		}
	}
	return -5001;
}