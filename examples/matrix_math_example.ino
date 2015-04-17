#include "Arduino.h"
#include "matrix_math.h"

void setup() {
	Com.begin(9600);

	int determinant = 0;
	
	matrix m(3, 3, 1);
	m.setValue(0, 1, 2);
	m.setValue(0, 2, 0);
	m.setValue(1, 0, -1);
	m.setValue(2, 1, 2);
	m.setValue(2, 2, 3);
	m.print("m Matrix");
	Com.println("");
	matrix i;
	matrix::inverse(m, i);
	i.print("Inverse matrix");
	matrix target;
	matrix::mult(m, i, target);
	target.print("Identity matrix?");

	
	
}

void loop() {

}

int determ(matrix& p_input){
	//Com.println("Current matrix");
	//p_input.print();
	Com.println("");
	unsigned long time;
	time = micros();
	int deter = matrix::determinant(p_input);
	time = micros() - time;
	Com.print("Order ");
	Com.println(p_input.colCount());
	Com.print("Determinant = ");
	Com.println(deter);	
}