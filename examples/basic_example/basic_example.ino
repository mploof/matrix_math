#include <MemoryFree.h>
#include "Arduino.h"
#include "matrix_math.h"

matrix myMatrix;

void setup() {
  Serial.begin(9600);
  myMatrix.init(3, 3, 0);
  myMatrix.print("Test Matrix");
  myMatrix.set141();
  myMatrix.print("141 Matrix");
  myMatrix.setValue(0, 0, 5.6);
  myMatrix.setValue(1, 1, 3.3);
  myMatrix.setValue(2, 2, 4.2);
  myMatrix.print("New values");

}

void loop() {
}
