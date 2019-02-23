#ifndef CAPTEURS_INC
#define CAPTEURS_INC

//Pins des capteurs incrementaux
#define INC_1_D 19
#define INC_2_D 18
#define INC_1_G 2
#define INC_2_G 3
 
class CapteursInc
{
  private:
    static void inc1D();
    static void inc2D();
    static void inc1G();
    static void inc2G();
    
  public:
    static void init();
    static volatile int d, g;
};

#endif


