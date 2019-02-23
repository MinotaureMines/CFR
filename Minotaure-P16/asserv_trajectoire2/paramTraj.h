#ifndef PARAM_TRAJ
#define PARAM_TRAJ

class ParamTraj
{
  public :
    double vitesseAvancer = 0.3;
    double vitesseRotation = 2;
    double accAvancer = 2;
    double accRotation = 2;

    ParamTraj(double vA, double vR, double aA, double aC)
    {
      vitesseAvancer = vA;
      vitesseRotation = vR;
      accAvancer = aA;
      accRotation = aC;
    };
    ParamTraj(){};
    
    ~ParamTraj()
    {
      
    };
};
const ParamTraj parametresLents(0.5,1.5,0.15,1.5);

#endif
