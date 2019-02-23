#include "pathfinding.h"

#include "arduino.h"

using namespace std;

constexpr byte MAX_PATH = 60; //longueur maximale d'un chemin

static const byte TERRAIN[] PROGMEM = 
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 2,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 2,
                                  2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  2, 2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
                                  2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const byte NB_DIR = 8;//nombre de directions
constexpr char directionsX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr char directionsY[] = {-1, 0, 1, -1, 1, -1, 0, 1};


PathFinder::PathFinder(){
    for(byte x=0; x<LARG; x++){
      for(byte y=0; y<HAUT; y++) {
          grille[x][y].flag= pgm_read_byte(&(TERRAIN[x+y*LARG]));
      }
    }
}

inline bool estValide(int x, int y)
{
    return (x >= 0) && (x < LARG) &&
           (y >= 0) && (y < HAUT);
}

inline byte norme(byte a, byte b){
    if(abs(a)==1 && abs(b)==1){
        return 3;
    }else{
        return 2;//approximation de sqrt(2)
    }
}


stack<Pair> PathFinder::construireChemin(Pair src, Pair dest){
    stack<Pair> res;
    if(! (grille[dest.first][dest.second].flag & FLAG_MASQUE_VISITE))return res;
    byte x = dest.first;
    byte y = dest.second;
    Cellule* courant;
    while(x!=src.first || y != src.second){
      courant = &grille[x][y];
      res.push(Pair(x, y));
      x = courant->parent_x;
      y = courant->parent_y;
    }
    return res;
}


stack<Pair> PathFinder::chercherChemin(Pair src, Pair dest, FlagCellule flag_autorise){
  if(src == dest){
    return stack<Pair>();
  }
  bool grilleChangee = true;
  grille[src.first][src.second].flag |= 0x80; //on marque le depart comme visité
  int x_tmp;
  int y_tmp;
  byte cpt = 0;
  byte tmp_distance;
  while(grilleChangee && cpt<MAX_PATH){
    cpt++;
    grilleChangee = false;
    for(byte x=0; x<LARG; x++){
      for(byte y=0; y<HAUT; y++){
         if(grille[x][y].flag & FLAG_MASQUE_VISITE){
            for(byte i=0; i<NB_DIR; i++){ // pour tous les voisins
               x_tmp = x + directionsX[i];
               y_tmp = y + directionsY[i];
               if(estValide(x_tmp, y_tmp) // si on est dansla grille
                && ((FLAG_MASQUE_CELLULE & grille[x_tmp][y_tmp].flag)<=flag_autorise)){ // si on peut allr sur cette case
                    tmp_distance = grille[x][y].distance + norme(directionsX[i], directionsY[i]);
                    // si on a deja vu la case, et que le chemin est meilleur, ou si on ne l'a jamais atteinte
                    if(!(grille[x_tmp][y_tmp].flag & FLAG_MASQUE_VISITE) || ((grille[x_tmp][y_tmp].flag & FLAG_MASQUE_VISITE) && tmp_distance<grille[x_tmp][y_tmp].distance)){
                        //TODO : vérifier si le résultat reste correcte en norme absolue,et si oui simplifier le programme
                        grille[x_tmp][y_tmp].distance = tmp_distance;
                        grille[x_tmp][y_tmp].parent_x = x;
                        grille[x_tmp][y_tmp].parent_y = y;
                        grille[x_tmp][y_tmp].flag |= FLAG_MASQUE_VISITE;
                        grilleChangee = true;
                    }
               }
            }
         }
      }
    }
  }
  if(cpt==MAX_PATH){
    return stack<Pair>();
  }

  auto res = construireChemin(src, dest);
  reinitialiserGrille();
  return res;

}

void PathFinder::reinitialiserGrille(){
    for(int x=0; x<LARG; x++){
        for(int y=0; y<HAUT; y++){
            grille[x][y].reinitialiser();
        }
    }
}

void PathFinder::afficherPath(stack<Pair>& path){
  while(!path.empty()){
    Pair& courant = path.top();
    Serial.println(F("Chemin : "));//une économie de 10o !
    Serial.print(" (");
    Serial.print(courant.first);
    Serial.print(" ; ");
    Serial.print(courant.second);
    Serial.print(")\n");
    path.pop();
    delay(100);
  }
}

