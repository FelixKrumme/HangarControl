#include <Arduino.h>

// Put here Interupt Service Routines for Endtriggers

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper1.movesteps(1000, 1000);
  stepper1.move(1000);

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}