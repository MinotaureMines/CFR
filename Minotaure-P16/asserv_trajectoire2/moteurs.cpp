#include "Arduino.h"
#include "moteurs.h"

//Initialisation des pins des moteurs
void Moteurs::init()
{
  pinMode(DA, OUTPUT);
  pinMode(DR, OUTPUT);
  pinMode(GA, OUTPUT);
  pinMode(GR, OUTPUT);
  pinMode(PWD, OUTPUT);
  pinMode(PWG, OUTPUT);
  avancent(0, 0);
}

//Fonction de commande des moteurs
void Moteurs::avancent(int pwmD, int pwmG)
{
  //pwmG*=1.33;
  //Commande analogique si possible
  if(pwmD<0)
  {
    digitalWrite(DA, HIGH);
    digitalWrite(DR, LOW);
    analogWrite(PWD, -pwmD);
  }
  else
  {
    digitalWrite(DA, LOW);
    digitalWrite(DR, HIGH);
    analogWrite(PWD, pwmD);
  }
  
  if(pwmG<0)
  {
    digitalWrite(GA, HIGH);
    digitalWrite(GR, LOW);
    analogWrite(PWG, -pwmG);
  }
  else
  {
    digitalWrite(GA, LOW);
    digitalWrite(GR, HIGH);
    analogWrite(PWG, pwmG);
  }
  //Sinon, on sature
  if(abs(pwmD)>=255)
    digitalWrite(PWD, HIGH);
  if(abs(pwmG)>=255)
    digitalWrite(PWG, HIGH);
}

//On arrête les moteurs à la fin
void Moteurs::stopper()
{
    digitalWrite(DA, LOW);
    digitalWrite(DR, LOW);
    digitalWrite(GA, LOW);
    digitalWrite(GR, LOW);
    digitalWrite(PWD,LOW);
    digitalWrite(PWG,LOW);  
}


