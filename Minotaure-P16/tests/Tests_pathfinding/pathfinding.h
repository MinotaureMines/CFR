#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "utility.h"

#include "arduino.h"

#define HAUT 30
#define LARG 20


enum FlagCellule:byte{
    LIBRE =0x00, CUBES = 0x01, NON = 0x02
};

typedef pair<byte, byte> Pair;

class PathFinder{
public:
    PathFinder();

    PathFinder(const PathFinder&)=delete;
    PathFinder& operator=(const PathFinder&)=delete;

    stack<Pair> chercherChemin(Pair src, Pair dest, FlagCellule=LIBRE);

    static void afficherPath(stack<Pair>& path);

private:

    void reinitialiserGrille();
    stack<Pair> construireChemin(Pair src, Pair dest);

public:
    static constexpr byte FLAG_MASQUE_VISITE = 0b10000000; // le dernier bit du flag sert de booleen, qui marque quand une case est visitee.
    static constexpr byte FLAG_MASQUE_CELLULE = 0b01111111;// faire un & avec les flag pour avoir la bone valeur

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
            flag &= !FLAG_MASQUE_VISITE;
        }
    };

    Cellule grille[LARG][HAUT];

};

#endif
