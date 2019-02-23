#include "strategie_robot.h"
#include "commande.h"
#include "ParamTraj.h"
#include "pathfinding.h"
#include "ecran.h"

#define ROBOT_PRINCIPAL 0
#define ROBOT_SECONDAIRE 1
#define ROBOT_TEST 2

StrategieRobot::StrategieRobot() : parametres(0.8,2,0.25,2), parametresRapides(1.2,2,0.5,2),parametresLents(0.6,1.5,0.2,1.5), parametresMegaLents (0.3,1,0.1,0.5)
{

}

StrategieRobot::~StrategieRobot()
{

}


void StrategieRobot::positionInitiale(Deplacement& deplace, int const typeRobot)
{
  Serial.println("Position initiale");

  switch(typeRobot)
  {
    case ROBOT_PRINCIPAL :
      Serial3.println("Robot principal");
      deplace.pAbeilleRangee();
      deplace.pFermerBarriere();
      deplace.pInitialiserCroix();
      deplace.pInitialiserDistributeur();
      deplace.attendre(1);
      break;
    case ROBOT_SECONDAIRE :
      deplace.sFauxHaute();
      deplace.sGripFerme();
      deplace.sPincesPresqueHaute();
      Serial.println("Robot secondaire");
      break;
    case ROBOT_TEST :
      Serial.println("Robot test");
      break;
    default :
      Serial.println("Grosse merde");
  }

  deplace.executionCommandes();
}

void StrategieRobot::sRamassageCubes1(Deplacement& deplace)
{
  deplace.sPincesInterrupteur();
  deplace.attendre(0.2);
  deplace.sGripMid();
  deplace.attendre(0.5);
  deplace.sGripOuvert();
  deplace.sPincesPresqueBasse();
  deplace.attendre(0.2);
}

void StrategieRobot::sRamassageCubes2(Deplacement& deplace)
{
  deplace.sPincesBasse();
  deplace.attendre(0.5);
  deplace.sGripFerme();
  deplace.attendre(1.4);
  deplace.sPincesHaute();
  deplace.attendre(0.8);
}

void StrategieRobot::sLargageCubes1(Deplacement& deplace)
{
  deplace.attendre(0.2);
  deplace.sGripMid();
  deplace.sFauxBasse();
  deplace.attendre(1.2);
}

void StrategieRobot::sLargageCubes2(Deplacement& deplace)
{
  deplace.sGripFerme();
  deplace.sFauxHaute();
}

void StrategieRobot::sPousserNoirVersJaune(Deplacement& deplace)
{
  deplace.goTo(0.2,0.85,parametres, true);
  deplace.goTo(0.2,0.85,parametres, false,Angle(PI),true);
  deplace.sPincesPresqueHaute();
  deplace.goTo(0.42,0.85,parametresMegaLents, false, true);
  deplace.attendre(0.5);
  deplace.goTo(0.2, 0.85, parametres, true);
  /*deplace.goTo(0.2, 0.5, parametres, false, Angle(0), true);
  deplace.attendre(0.1);
  deplace.recalage(0.095,0.5, Angle(0));*/
}

void StrategieRobot::sPousserVertVersJaune(Deplacement& deplace)
{
  deplace.goTo(0.54,0.5,parametres, true);
  deplace.goTo(0.54,0.5,parametres, false,Angle(PI/2),true);
  deplace.sPincesPresqueHaute();
  deplace.goTo(0.54,0.72,parametresMegaLents, false, true);
  deplace.attendre(0.5);
  deplace.goTo(0.54, 0.45, parametres, true);
  deplace.sPincesHaute();
}

void StrategieRobot::sPousserOrangeVersJaune(Deplacement& deplace)
{
  deplace.goTo(0.2,0.9,parametres, true);
  deplace.goTo(0.54, 1.25, parametres, true);
  deplace.goTo(0.54, 1.25,parametres, false,Angle(PI/2),true);
  deplace.sPincesPresqueHaute();
  deplace.goTo(0.54, 0.97,parametresMegaLents, false, true);
  deplace.attendre(0.5);
  deplace.goTo(0.54, 1.2, parametres, true);
  deplace.sPincesHaute();
  deplace.goTo(0.2, 1.2, parametres, true);
}

void StrategieRobot::sAttraperVert(Deplacement& deplace)
{
  deplace.goTo(0.54 - decalageCube, 0.5, parametres, true);
  deplace.goTo(0.54 - decalageCube, 0.5, parametres, false, Angle(PI/2), true);
  sRamassageCubes1(deplace);
  deplace.goTo(0.54 - decalageCube,0.86,parametresLents, false);
  sRamassageCubes2(deplace);
  deplace.goTo(0.54 - decalageCube, 0.65, parametresMegaLents, false, true);
  deplace.goTo(0.3, 0.85 + decalageCube, parametres, true);
  deplace.goTo(0.3, 0.85 + decalageCube, parametres, false, Angle(0), true);
}

void StrategieRobot::sAttraperOrange(Deplacement& deplace)
{
  deplace.goTo(0.2,0.85,parametres, true);
  deplace.goTo(0.54 + decalageCube, 1.2, parametres, true);
  deplace.goTo(0.54 + decalageCube, 1.2, parametres, false, Angle(-PI/2), true);
  sRamassageCubes1(deplace);
  deplace.goTo(0.54 + decalageCube,0.84,parametresLents, false);
  sRamassageCubes2(deplace);
  deplace.goTo(0.54 + decalageCube, 1, parametresMegaLents, false, true);
  deplace.goTo(0.3, 0.85 + decalageCube, parametres, true);
  deplace.goTo(0.3, 0.85 + decalageCube, parametres, false, Angle(0), true);
}

void StrategieRobot::sAttraperBleu(Deplacement& deplace)
{
  deplace.goTo(0.95, 0.85 - decalageCube, parametres, true);
  deplace.goTo(0.95, 0.85 - decalageCube, parametres, false, Angle(PI), true);
  sRamassageCubes1(deplace);
  deplace.goTo(0.60,0.85 - decalageCube,parametresLents, false);
  sRamassageCubes2(deplace);
  deplace.goTo(0.68, 0.85 - decalageCube, parametresMegaLents, false, true);
  deplace.goTo(0.6, 0.6, parametres, true);
  deplace.goTo(0.25, 0.6, parametres, true);
  deplace.goTo(0.25, 0.85 + decalageCube, parametres, true);
  deplace.goTo(0.25, 0.85 + decalageCube, parametres, false, Angle(0), true);
}

void StrategieRobot::sAttraperNoir(Deplacement& deplace)
{
  deplace.goTo(0.2,0.85 + decalageCube,parametres, true);
  deplace.goTo(0.2,0.85 + decalageCube,parametres, false,Angle(0),true);
  sRamassageCubes1(deplace);
  deplace.goTo(0.55,0.85 + decalageCube,parametresMegaLents, false);
  sRamassageCubes2(deplace);
}

void StrategieRobot::strategie_robot(Deplacement& deplace, int const typeRobot)
{

  Serial.println("Chargement stratégie");

  switch(typeRobot)
  {
    case ROBOT_PRINCIPAL :

      //Sortie de la zone de depart
      Serial.println("Robot principal");
      deplace.commandeScore(5);
      deplace.goTo(0.17,1.150,parametres,false);
      deplace.goTo(0.690,1.150,parametres,true);
      deplace.goTo(1.60,0.450,parametres,false);


      //Pousssee de l'abeille en fonction du cote
      if(deplace.getCoteVert())
      {
        Serial3.println("Cote vert");
        //deplace.goTo(1.60, 0.450, parametres, false, Angle(-PI/2), true);
        deplace.goTo(1.60, 0.450, parametres, false, Angle(PI), true);
        deplace.goTo(1.82, 0.450, parametresMegaLents, false, true);
        deplace.goTo(1.82, 0.28, parametresLents, false);
        deplace.pAbeilleGauche();
        deplace.attendre(3);
        deplace.goTo(1.82, 0.52, parametresLents, false, true);
        deplace.pAbeilleRangee();

        deplace.commandeScore(50);
        
        deplace.goTo(1.82, 0.32, parametres, false);
        deplace.goTo(1.82, 0.32, parametresLents, false, Angle(PI), true);
        deplace.goTo(1.70, 0.32, parametresMegaLents, false);
        deplace.goTo(1.70, 0.32, parametres, false, Angle(3.*PI/4), true);

        
        deplace.recalage(1.70,0.06, Angle(PI/2), 4);
        deplace.attendre(0.5);
        deplace.goTo(1.70,0.11, parametresLents, false);
      }
      else
      {
        Serial3.println("Cote orange");
        deplace.goTo(1.60, 0.450, parametres, false, Angle(PI/2), true);
        deplace.pAbeilleDeriere();
        deplace.attendre(0.5);
        deplace.goTo(1.6, 0.155, parametresMegaLents, false, true);
        deplace.goTo(1.715, 0.155, parametresLents, false);
        deplace.pAbeilleRangeeLent();
        deplace.attendre(4);
        deplace.commandeScore(50);
        deplace.goTo(1.690,0.155,parametres,false,true);
        deplace.goTo(1.690,0.155,parametres, false, Angle(PI/2), true);
        deplace.recalage(1.69, 0.06, Angle(PI/2), 5);//0.06
        deplace.attendre(2);
        deplace.goTo(1.69, .10, parametresLents, true);
      }
   
      //On va de l'autre cote du terrain pour vider la tour

        deplace.goTo(1.15,1.1,parametresRapides, true);
        if(deplace.getCoteVert())
        {
        deplace.goTo(1.15,2.39+0.05,parametresRapides, true);
        deplace.goTo(1.9-0.265,2.39+0.05,parametresMegaLents,false);
        deplace.ajouterCommande(new CommandeAvancer(-0.01, parametresMegaLents, &deplace));
        deplace.attendre(1);
        deplace.pSpinCroix(true, 400);
        deplace.attendre(2.5);
        }
        else
        {
          
        deplace.goTo(1.15,2.39,parametresLents, true);
        deplace.goTo(1.8,2.39,parametresMegaLents,false);
        deplace.avancer(-0.01, parametresMegaLents);
        deplace.attendre(1);
        deplace.pSpinCroix(true, 400);
        deplace.attendre(2.5);
        deplace.avancer(-0.06 ,parametresLents);
        }

        deplace.commandeScore(10);

      //On met les balles dans la station d'epuration

      if(deplace.getCoteVert())
      {      
      deplace.avancer(-0.20, parametresMegaLents);
      deplace.pSpinCroix(false);
      deplace.goTo(2.-.23-.29/2, 2.17, parametresLents, false, true);
      deplace.goTo(2.-.23-.29/2, 2.17, parametresMegaLents, false, Angle(PI), true);
      deplace.avancer(-.04, parametresMegaLents);
      deplace.pOuvrirBarriere();
      deplace.commandeScore(20);
      }
      else
      {
      deplace.commandeScore(10);
      deplace.goTo(1.685,2.39,parametres, false, true);
      deplace.goTo(1.685,2.39,parametres,false,Angle(PI/2),true);
      deplace.goTo(1.685,2.115,parametresLents, false,true);
      deplace.pOuvrirBarriere();
      deplace.commandeScore(20);
      }

      deplace.attendre(100);



      //Lancement balles
//     deplace.pEnvoyerBalles(8);
/*
      deplace.goTo(2.390, 0.400, parametres, false);
      deplace.goTo(2.390, 1.6, parametres, false);
      deplace.goTo(2.39, 1.65, parametresMegaLents, false);
      deplace.goTo(2.39, 1.75, parametresMegaLents, false);
      deplace.goTo(1.87, 1.75, parametres, false);
      deplace.goTo(1.87, 1.71, parametresMegaLents, false);
      deplace.pOuvrirBarriere();
      */
      
      break;


/*===================================================
 *|              Fin Robot Principal              |*
 ==================================================*/





      
    case ROBOT_SECONDAIRE :
      Serial3.println("photo");
      Serial.println("photo");
      if(deplace.getCoteVert())
      {
        Serial3.println("Cote vert");
        decalageCube = -0.015;
      }
      else
      {
        Serial3.println("Cote rouge");
        decalageCube = 0.015;
      }
      int organisationCubes;
      long debutAttente;
      debutAttente = micros();
      long tempsPhoto;
      tempsPhoto = 8 * 1000000;
      while (!Serial3.available() && micros() - debutAttente < tempsPhoto)
      {
        delay(10);
      }
      long tempsAttente;
      tempsAttente = micros() - debutAttente;

      if(tempsAttente < tempsPhoto)
      {
        organisationCubes = Serial3.parseInt();
        deplace.attendre((tempsPhoto - tempsAttente)/ 1000000);
      }
      else
      {
        organisationCubes = -1;
      }
      Ecran::getInstance().afficherTexte("Strategie numero " + String(organisationCubes));
      deplace.goTo(0.505,0.15,parametresLents, false);
      deplace.commandeScore(5);
      deplace.sPincesHaute();

      //organisationCubes = 2;
      int score;

      double xInterrupteur;
      xInterrupteur = 1.17;

      double yTube;
      yTube = 0.17;

      bool cubesMilieu;
      cubesMilieu = true;

      switch(organisationCubes)
      {
        /*
        case 1 :
          // Orange - Noir - Vert
          sPousserNoirVersJaune(deplace);
          sAttraperVert(deplace);
          xInterrupteur = 1.17;
          score = 15;
          yTube = 0.17;
          cubesMilieu = true;
          break;
*/
        case 2 :
          // Jaune - Noir - Bleu
          sAttraperNoir(deplace);
          xInterrupteur = 1.175;
          score = 15;
          yTube = 0.15;
          cubesMilieu = true;
          break;
/*
        case 4 :
          // Jaune - Vert - Noir
          sPousserNoirVersJaune(deplace);
          sAttraperVert(deplace);
          xInterrupteur = 1.17;
          score = 15;
          yTube = 0.17;
          cubesMilieu = true;
          break;
        case 7 :
          // Bleu - Orange - Noir
          sPousserOrangeVersJaune(deplace);
          sAttraperNoir(deplace);
          xInterrupteur = 1.17;
          score = 15;
          yTube = 0.17;
          cubesMilieu = false;
          break;
*/
        case 8 :
          // Vert - Orange - Jaune
          sAttraperOrange(deplace);
          xInterrupteur = 1.17;
          score = 15;
          yTube = 0.16;
          cubesMilieu = false;
          break;
/*
        case 9 :
          //Noir - Bleu - Vert
          // A vérifier
          sPousserVertVersJaune(deplace);
          sAttraperBleu(deplace);
          xInterrupteur = 1.17;
          score = 15;
          cubesMilieu = true;
          break;
*/
        case 10 :
          // Orange - Bleu - Jaune
          // A vérifier
          deplace.goTo(0.95, 0.5, parametres, false);
          sAttraperBleu(deplace);
          xInterrupteur = 1.17;
          score = 15;
          cubesMilieu = true;
          break;

        default :
          // On récupère les trois cubes à partir du bord
          sAttraperNoir(deplace);
          score = 15;
          xInterrupteur = 1.17;
          yTube = 0.15;
          cubesMilieu = true;
      }

      //Largage des cubes
      deplace.goTo(0.135,0.85 + decalageCube,parametresMegaLents, false,true);
      deplace.goTo(0.175, 0.85 + decalageCube, parametresMegaLents, false);
      sLargageCubes1(deplace);
      deplace.commandeScore(score);
      deplace.goTo(0.45,0.85 + decalageCube,parametresMegaLents, true);
      sLargageCubes2(deplace);

/*
      if(recalageNecessaire)
      {
        deplace.goTo(0.45,0.3, parametres, false);
        deplace.sPincesPresqueHaute();
        deplace.goTo(0.45,0.3, parametres, false, Angle(PI/2), true);
        deplace.recalage(0.45, 0.095, Angle(PI/2));
        deplace.goTo(0.45,0.15, parametres, false);
        deplace.sPincesHaute();
        deplace.attendre(1);
      }
*/

      //Interrupteur
      deplace.goTo(0.45,xInterrupteur, parametres, true);
      if(cubesMilieu)
      {
        deplace.goTo(0.45,xInterrupteur, parametresLents, false, Angle(-PI/4), true);
      }
      deplace.goTo(0.45,xInterrupteur, parametresLents, false, Angle(PI), true);
      deplace.sPincesInterrupteur();
      deplace.attendre(0.3);
      deplace.goTo(0.09,xInterrupteur,parametresMegaLents, false);
      deplace.attendre(0.2);
      deplace.commandeScore(25);
      deplace.goTo(0.25,xInterrupteur,parametresMegaLents, false,true);
      deplace.sPincesPresqueHaute();
      deplace.goTo(0.25,xInterrupteur,parametres, false,Angle(0),true);
      deplace.attendre(0.2);
      if(cubesMilieu)
      {
        deplace.goTo(0.4,xInterrupteur, parametres, true);
      }
      else
      {
        deplace.goTo(0.3,xInterrupteur, parametres, true);
      }
      deplace.sPincesHaute();
      deplace.attendre(0.2);

      //Ouverture tube
      if(deplace.getCoteVert())
      {
        deplace.goTo(0.3, 0.610, parametres,true);
        deplace.goTo(1.2, 0.610, parametres, true);
        deplace.goTo(0.74, 0.08, parametresMegaLents, false);
      }
      else
      {    
        if(cubesMilieu)
        {
          deplace.goTo(0.4, 0.3, parametresLents, true);
        }
        else
        {
          deplace.goTo(0.3, 0.3, parametresLents, true);
        }
        deplace.sPincesPresqueHaute(); 
        deplace.attendre(0.7);
        deplace.sGripMid();
        deplace.goTo(0.72, 0.15, parametresMegaLents, false);
        deplace.goTo(0.72, 0.15, parametresMegaLents, false, Angle(-0.593), true);
      }
      
      deplace.sPincesInterrupteur();
      deplace.commandeScore(10);
      deplace.attendre(2);
      deplace.sPincesHaute();

      Serial.println("Robot secondaire");
      break;
    case ROBOT_TEST :
      deplace.goTo(0.5,0,parametres, false, Angle(PI/2),true);
      //deplace.pathfinder.findGoTo(0,0,1.85, 0.27, parametres, false, deplace, true);
      //deplace.goTo(0.5,0,parametres, false);
      //deplace.deplacerAX12(Servo::Servo1,Angle(0));
      //deplace.deplacerAX12(Servo::Servo2,Angle(0));
      //deplace.attendre(2.1f);
      //deplace.pathfinder.findGoTo(1.899,0,1.899,2.699,parametres, false,deplace);
      //deplace.deplacerAX12(Servo::Servo1,Angle(PI/2));
      //deplace.deplacerAX12(Servo::Servo2,Angle(PI/2));
      //deplace.goTo(0.5,0.5,Angle(PI/2), false, false);
      Serial.println("Robot test");
      break;
    default :
      Serial.println("Grosse merde");
  }
}
