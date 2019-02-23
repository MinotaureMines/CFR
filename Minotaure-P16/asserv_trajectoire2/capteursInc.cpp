#include "Arduino.h"
#include "capteursInc.h"

//Fonctions d'interruption des capteurs incrementaux

volatile int CapteursInc::d = 0;
volatile int CapteursInc::g = 0;

void CapteursInc::inc1D()
{
  if(PIND & (1<<PD3))
    d+=1;
  else
    d-=1;
}
void CapteursInc::inc2D()
{
  if(PIND & (1<<PD2))
    d-=1;
  else
    d+=1;
}
void CapteursInc::inc1G()
{
  if(PINE & (1<<PE5))
    g+=1;
  else
    g-=1;
}
void CapteursInc::inc2G()
{
  if(PINE & (1<<PE4))
    g-=1;
  else
    g+=1;
}

void CapteursInc::init()
{
  pinMode(INC_1_D, INPUT);
  pinMode(INC_2_D, INPUT);
  pinMode(INC_1_G, INPUT);
  pinMode(INC_2_G, INPUT);
  attachInterrupt(digitalPinToInterrupt(INC_1_D), inc1D, RISING);
  attachInterrupt(digitalPinToInterrupt(INC_2_D), inc2D, RISING);
  attachInterrupt(digitalPinToInterrupt(INC_1_G), inc1G, RISING);
  attachInterrupt(digitalPinToInterrupt(INC_2_G), inc2G, RISING);
}
