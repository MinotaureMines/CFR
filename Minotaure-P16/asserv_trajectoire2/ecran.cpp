#include <LiquidCrystal.h>
#include "Arduino.h"
#include "ecran.h"

Ecran::Ecran() : lcd(0,0,0,0,0,0)
{
  
}

Ecran::~Ecran()
{
  
}

Ecran& Ecran::getInstance()
{
    static Ecran instance;

    return instance;
}

void Ecran::init(int rs, int en, int d4, int d5, int d6, int d7)
{
  LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);
  lcd = lcd2;
  lcd.begin(16, 2);
}

void Ecran::afficherTexte(String texte)
{

  
  texte += " ";
  String ligne1 = "", ligne2 = "";
  int lenLigne1 = 0, lenLigne2 = 0;
  bool ligne1Pleine = false;

  int const lenLigne = 16;
  int lenMot = 0, lenMessage = texte.length();
  
  int lastSpace = 0, newSpace = 0;

  String mot;

  while(lastSpace <= lenMessage - 1)
  {
    newSpace = texte.indexOf(" ", lastSpace);
    lenMot = newSpace - lastSpace;

    mot = texte.substring(newSpace, lastSpace);
    lastSpace = newSpace + 1;

    if(! ligne1Pleine)
    {
      if(lenLigne1 == 0)
      {
        lenLigne1 = lenMot;
        ligne1 = mot;
      }
      else if(lenLigne1 + lenMot + 1 <= lenLigne)
      {
        ligne1 += String(" ") + mot;
        lenLigne1 += lenMot + 1;
      }
      else
      {
        ligne1Pleine = true;
      }
    }
    
    if(ligne1Pleine)
    {
      if(lenLigne2 == 0)
      {
        lenLigne2 = lenMot;
        ligne2 = mot;
      }
      else if(lenLigne2 + lenMot + 1 <= lenLigne)
      {
        ligne2 += String(" ") + mot;
        lenLigne2 += lenMot + 1;
      }
    }
  }
  int espacesAAjouter;
  espacesAAjouter = lenLigne - ligne1.length();
  for(int i = 0; i < espacesAAjouter; i++)
  {
    ligne1 += " ";
  }

  espacesAAjouter = lenLigne - ligne2.length();
  for(int i = 0; i < espacesAAjouter; i++)
  {
    ligne2 += " ";
  }
  
  lcd.setCursor(0,0);
  lcd.print(ligne1);
  lcd.setCursor(0,1);
  lcd.print(ligne2);
}

void Ecran::test()
{
  lcd.setCursor(0,0);
  lcd.print("Ca ne rentrera jamais sur une seule ligne");
}

