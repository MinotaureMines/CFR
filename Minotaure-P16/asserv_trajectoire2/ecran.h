#ifndef ECRAN
#define ECRAN

#include <LiquidCrystal.h>

class Ecran
{
  public :
    ~Ecran();
    void init(int rs, int en, int d4, int d5, int d6, int d7);
    void afficherTexte(String texte);
    void test();
    static Ecran& getInstance();

  private :
    LiquidCrystal lcd;
    explicit Ecran();
};

#endif
