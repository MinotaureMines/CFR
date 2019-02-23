#include "pathfinding.h"

void setup(){
  
  Serial.begin(115200);
  delay(500);
  byte Libre = 0;
  byte Flag = 0x02;
  PathFinder pathfinder;
  auto var = pathfinder.chercherChemin(make_pair((byte)19, (byte)29), make_pair((byte)10, (byte)10));
  PathFinder::afficherPath(var);
}

void loop(){

}
