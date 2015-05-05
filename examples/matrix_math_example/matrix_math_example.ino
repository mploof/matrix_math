#include "Arduino.h"
#include "matrix_math.h"
#include "spline_calc.h"



Spline spline;
bool debug = true;
enum{WAIT, RECEIVE, SEND};
int state = WAIT;
uint8_t buf[32];

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

		//// Flush the serial buffer
		//while (Serial.available()){
		//	Serial.read();
		//}

		switch (val) {
		case 1:				
			Serial.println(1);
			state = RECEIVE;
			break;
		case 2:
			Serial.println(1);
			state = SEND;
			break;
		default:
			state = WAIT;
			break;
		}
	}
}

void receive(){
	
	memset(buf, 0, 10);
	int TIMEOUT = 2000;
	int count = Serial.available();

	for (int i = 0; i < count; i++){
		uint8_t* p = (uint8_t*)(void*)&buf[i];
		// If data is immediately available, write it
		if (Serial.available() > 0){
			*p = (uint8_t)Serial.read();
		}
		// Otherwise wait till some shows up
		else{
			unsigned long timer = millis();
			// wait until we have data ready
			while (Serial.available() < 1) {
				if (millis() - timer > TIMEOUT) {
					// timed out waiting for complete input
					return;
				}
			}
			*p = (uint8_t)Serial.read();
		}
	}

	Serial.println(ntoi(buf));
	
	//while (true){
	//	if (Serial.available()){
	//		blink(1);
	//		char buffer[10];
	//		for (int i = 0; i < 10; i++)
	//			buffer[i]= '\0';
	//		Serial.readBytesUntil('\n', buffer, Serial.available());
	//		char* point = buffer;
	//		int val = atoi(point);
	//		blink(val);
	//		Serial.print(val);

			//int size = Serial.available();
			//char* in_byte = new char[size];
			//int i = 0;
			//while (Serial.available()){
			//	in_byte[i] = Serial.read();
			//	i++;
			//}
			//char* point = in_byte;
			//for (int j = 0; j < size; j++){
			//	Serial.println(in_byte[j]);
			//}
			////Serial.println(444);
			////Serial.println(atof(point));
			//delete in_byte;
	/*		break;
		}
	}*/
	state = WAIT;

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


/** Convert Network-order Bytes to Integer

@param p_dat
A pointer to an array of bytes

@return
A signed integer
*/

int ntoi(uint8_t* p_dat) {
	int ret = ((int)p_dat[0] << 8) + (int)p_dat[1];
	return(ret);
}

/** Convert Network-order Bytes to Unsigned Integer

@param p_dat
A pointer to an array of bytes

@return
An unsigned integer
*/

unsigned int ntoui(uint8_t* p_dat) {
	unsigned int ret = ((unsigned int)p_dat[0] << 8) + (unsigned int)p_dat[1];
	return(ret);
}

/** Convert Network-order Bytes to Unsigned Long

@param p_dat
A pointer to an array of bytes

@return
An unsigned long
*/

unsigned long ntoul(uint8_t* p_dat) {
	unsigned long ret = (unsigned long)(((unsigned long)p_dat[0] << 24) + (unsigned long)p_dat[1]) << 16;
	ret |= (unsigned long)(((unsigned long)p_dat[2] << 8) + ((unsigned long)p_dat[3]));
	return(ret);
}

/** Convert Network-order Bytes to Long

@param p_dat
A pointer to an array of bytes

@return
A long
*/

long ntol(uint8_t* p_dat) {
	unsigned long ret = (long)(((long)p_dat[0] << 24) + ((long)p_dat[1] << 16));
	ret |= (long)(((long)p_dat[2] << 8) + ((long)p_dat[3]));
	return(ret);
}

/** Convert Network-order Bytes to Float

@param p_dat
A pointer to an array of bytes

@return
A float
*/

float ntof(uint8_t* p_dat) {
	float ret;
	unsigned long * _fl = (unsigned long *)(&ret);
	*_fl = (unsigned long)(((unsigned long)p_dat[0] << 8) + (unsigned long)p_dat[1]) << 16;
	*_fl |= (unsigned long)(((unsigned long)p_dat[2] << 8) + ((unsigned long)p_dat[3]));
	return(ret);
}