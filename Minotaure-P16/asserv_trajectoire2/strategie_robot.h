#ifndef STRATEGIE_ROBOT
#define STRATEGIE_ROBOT

#include "commande.h"
#include "ParamTraj.h"

class StrategieRobot
{
  private :
    ParamTraj parametres;
    ParamTraj parametresRapides;
    ParamTraj parametresLents;
    ParamTraj parametresMegaLents;
  
    double decalageCube;

    void sPousserNoirVersJaune(Deplacement& deplace);
    void sPousserVertVersJaune(Deplacement& deplace);
    void sPousserOrangeVersJaune(Deplacement& deplace);
    void sAttraperVert(Deplacement& deplace);
    void sAttraperNoir(Deplacement& deplace);
    void sAttraperOrange(Deplacement& deplace);
    void sAttraperBleu(Deplacement& deplace);

    void sRamassageCubes1(Deplacement& deplace);
    void sRamassageCubes2(Deplacement& deplace);
    void sLargageCubes1(Deplacement& deplace);
    void sLargageCubes2(Deplacement& deplace);

  public :
    StrategieRobot();
    ~StrategieRobot();
    void strategie_robot(Deplacement& deplace, int const typeRobot);
    void positionInitiale(Deplacement& depalce, int const typeRobot);
};
#endif
