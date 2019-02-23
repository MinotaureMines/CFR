#ifndef MOTEURS
#define MOTEURS

//Pins des moteurs
#define PWG 4
#define GA 5
#define GR 6
#define PWD 9
#define DA 8
#define DR 7

class Moteurs
{
  public:
    static void init();
    static void avancent(int pwmD, int pwmG);
    static void stopper();
};

#endif


