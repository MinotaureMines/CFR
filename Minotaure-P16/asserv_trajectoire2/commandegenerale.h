#ifndef COMMANDE_GENERALE_H
#define COMMANDE_GENERALE_H
//Commande pouvant executer n'importe quel lambda. A manier avec precaution!

#include "utility.h"

class Commande
{
  public:
    Commande();
    virtual ~Commande();
    virtual void executer();
};

template<class Lambda> 
class CommandeGenerale : public Commande
{
  private :
    Lambda _aExecuter;
  public:
    CommandeGenerale(Lambda &&aExecuter):_aExecuter(forward<Lambda>(aExecuter)){
    }
    ~CommandeGenerale(){

    }
    void executer(){
        _aExecuter();
    }

};

template<class Lambda>
CommandeGenerale<Lambda>* make_CommandeGenerale_p(Lambda&& t) {
    return new CommandeGenerale<Lambda>(forward<Lambda>(t));
}

#endif //COMMANDE_GENERALE_H
