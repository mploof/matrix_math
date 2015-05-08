#include <MemoryFree.h>
#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"


const int XY_SIZE = 2;

Spline spline;
float* KF_locations = (float *)malloc(17 * sizeof(float));
//float KF_locations[17];
bool debug = true;
enum{WAIT = 2, RECEIVE, SEND};
int state = WAIT;
int KF_count = 0;


void setup() {
	Com.begin(9600);

	for (byte i = 2; i < 13; i++){
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}

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
	checkMem();
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

	if (Com.available() > 0){
		int val = Com.read();		

		//// Flush the serial buffer
		//while (Com.available()){
		//	Com.read();
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

void checkMem(){
	int mem = freeMemory();
	int which = floor(mem / 25);
	if (which > 12)
		led(12);
	else
		led(which);
}

void receive(){

	int spline_pnt_count = 0;

	// Get the number of incoming points	
	while (true){		
		if (Com.available()){
			KF_count = Com.read();
			response(KF_count);
			break;
		}
	}

	// Get the number of curve points to generate
	while (true){		
		if (Com.available()){
			spline_pnt_count = Com.read();
			response(spline_pnt_count);
			break;
		}
	}
	
	// Deallocate KF array memory
	free(KF_locations);	

	// Reallocate with new size
	KF_locations = (float *)malloc((KF_count * 2 + 1) * sizeof(float));
	delay(100);
	
	// Store the XY coordinates for each of the points
	for (byte i = 0; i < KF_count * XY_SIZE; i++){				
		float val = readFloat();
		KF_locations[i] = val;	
		response(round(val));			
	}	
	
	// Add end of array delimiter
	KF_locations[KF_count * XY_SIZE] = -1;
	
	// Set the interpolation points from the array values	
	spline.setInterpPts(KF_locations);		
	
	spline.getCurvePts(spline_pnt_count);
	
	// Signal to Processing that computations are completed
	response(true);
	
	// Go back to waiting state
	state = SEND;
	
}
	
void send(){	
	spline.printCurvePts(false);
	clearSerial();
	state = WAIT;
}


/************************* Helper Functions *************************/

void led(int p_which){
	for (byte i = 2; i < 13; i++)
		digitalWrite(i, LOW);
	digitalWrite(p_which, HIGH);
}
void response(int p_ok){
	Com.println(p_ok);
}


/** Read four bytes from serial buffer and return converted float value

@return
A float
*/

float readFloat() {
	const int SIZE = 4;
	const int MAX_TIME = 4000;
	uint8_t buf[SIZE];	
	unsigned long time = millis();	
	while (millis() - time < MAX_TIME){
		if (Com.available() == SIZE){			
			for (byte i = 0; i < SIZE; i++){
				buf[SIZE - 1 - i] = Com.read();
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

void clearSerial(){
	while (Com.available()){
		Com.read();
	}
}