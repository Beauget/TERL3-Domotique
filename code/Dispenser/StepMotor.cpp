#include "StepMotor.h"

const bool StepMotor::ENERGIZING_MATRIX[STEP_AMOUNT][PIN_AMOUNT] = {
                                                                    {HIGH, HIGH, LOW , LOW }, // Step 1
                                                                    {LOW , HIGH, HIGH, LOW }, // Step 2
                                                                    {LOW , LOW , HIGH, HIGH}, // Step 3
                                                                    {HIGH, LOW , LOW , HIGH}  // Step 4
                                                                  };

StepMotor::StepMotor(byte in1, byte in2, byte in3, byte in4) : currentStep(0), pins({in1, in2, in3, in4})
{
  /*this->pins[0] = in1;
  this->pins[1] = in2;
  this->pins[2] = in3;
  this->pins[3] = in4;*/
}

void StepMotor::begin()
{
  for(byte i = 0; i < STEP_AMOUNT; i++)
  {
    pinMode(this->pins[i], OUTPUT);
    digitalWrite(this->pins[i], LOW);
  }
}

void StepMotor::steps(signed int amount)
{
  if(amount > 0)
  {
    for(unsigned int i = 0; i < amount; i++)
    {
      this->forwardStep();
    }
  }
  else if(amount < 0)
  {
    unsigned int absolute = abs(amount);
    for(unsigned int i = 0; i < absolute; i++)
    {
      this->backwardStep();
    }
  }
}

void StepMotor::forwardStep()
{
  this->step(FORWARD);
}

void StepMotor::backwardStep()
{
  this->step(BACKWARD);
}

void StepMotor::step(signed char direction)
{
  this->stepById(this->currentStep);
  
  this->currentStep += direction;

  if(direction == FORWARD && this->currentStep >= 4)
  {
    this->currentStep = 0;
  }
  else if(direction == BACKWARD && this->currentStep < 0)
  {
    this->currentStep = PIN_AMOUNT - 1;
  }
  
  delay(DELAY_BETWEEN_STEP);
}

void StepMotor::stepById(int id)
{
  for(int i = 0; i < PIN_AMOUNT; i++)
  {
    digitalWrite(this->pins[i], StepMotor::ENERGIZING_MATRIX[id][i]);
  }
}
