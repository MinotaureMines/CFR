#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "utility.h"
#include "arduino.h"
#include "ParamTraj.h"

class Deplacement;

#define HAUT 30
#define LARG 20

#define HAUT_CELLULE 0.1
#define LARG_CELLULE 0.1


enum FlagCellule:byte{
    LIBRE =0x00, CUBES = 0x01, NON = 0x02, ROBOT= 0x04
};

typedef pair<byte, byte> Pair;

class PathFinder{
public:
    /**
     * Attention : lourd, à appeler le moins souvent possible (1 fois dans l'idéal)
     */
    PathFinder();

    PathFinder(const PathFinder&)=delete;
    PathFinder& operator=(const PathFinder&)=delete;

    stack<Pair> chercherChemin(Pair src, Pair dest, FlagCellule=LIBRE);

/**
*Remplie le déplacement avec chemin trouvé par pathfinding entre X et Y
*return : true si un chemin existe, false sinon (dans ce cas, out n'est pas modifié)
**/
    bool findGoTo(double departX, double departY, double finX, double finY, ParamTraj parametres, Deplacement& out,bool finForcee = false, FlagCellule=LIBRE);

    static void afficherPath(stack<Pair>& path);

    void setPositionAdversaire(double posX, double posY, double largeur);

private:

    void reinitialiserGrille();
    stack<Pair> construireChemin(Pair src, Pair dest);

    static inline Pair realCoordsToPair(double x, double y){
        return make_pair((byte)(x/LARG_CELLULE), (byte)(y/HAUT_CELLULE));
    }
    static inline pair<double, double> pairToRealCoords(Pair& coordCellule){
        return make_pair((double)coordCellule.first * LARG_CELLULE + LARG_CELLULE/2, (double)coordCellule.second * HAUT_CELLULE + HAUT_CELLULE/2);
    }

public:
    static constexpr byte FLAG_MASQUE_VISITE = 0b10000000; // le dernier bit du flag sert de booleen, qui marque quand une case est visitee.
    static constexpr byte FLAG_MASQUE_CELLULE = 0b01111111;// faire un & avec les flag pour avoir la bone valeur
    static constexpr byte FLAG_MASQUE_ROBOT = 0b00000100;

private:
    struct Cellule{
        byte flag;
        byte distance =0;
        byte parent_x =0 ;
        byte parent_y =0 ;

        constexpr Cellule():flag(LIBRE){}
        explicit Cellule(FlagCellule flag):flag(flag){}

        Cellule operator=(Cellule & autre) = delete;

        void reinitialiser(){
            distance = 0;
            parent_x = 0;
            parent_y = 0;
            flag &= ~FLAG_MASQUE_VISITE;
            flag &= ~FLAG_MASQUE_ROBOT;
        }
    };

    Cellule grille[LARG][HAUT];

};

#endif
