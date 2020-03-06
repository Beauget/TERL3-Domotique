#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include <Arduino.h>

#define PIN_AMOUNT 4
#define STEP_AMOUNT 4

#define FORWARD 1
#define BACKWARD -1

#define DELAY_BETWEEN_STEP 2

class StepMotor
{
  public:
    StepMotor(byte in1, byte in2, byte in3, byte in4);
    void begin();

    void steps(signed int amount);
    
    void forwardStep();
    void backwardStep();
    void step(signed char direction);
    void stepById(int id);

  private:
    signed int currentStep;
    byte pins[PIN_AMOUNT];

    static const bool ENERGIZING_MATRIX[STEP_AMOUNT][PIN_AMOUNT];
};

#endif
