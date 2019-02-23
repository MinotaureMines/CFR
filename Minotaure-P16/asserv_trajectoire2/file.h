#ifndef FILE_H
#define FILE_H

/**
*Classe de file de pointeurs. AUCUN pointeur passé dans cette file ne sera supprimé : il faut le faire ailleur dans le code
*/
template <typename T>
class File
{

private:
    struct Element{
        T* element;
        Element* suivant = nullptr;

        Element(T* element, Element* precedent ):element(element)
        {
            if(precedent)
                precedent->suivant = this;
        }

        ~Element(){}//ne pas supprimer le pointeur ici !!!}
    };
    Element* tete = nullptr;
    Element* queue = nullptr;

public:
    File(){};
    ~File();
    void ajouter(T *i);
    void ajouterPriorite(T *i);
    T* retirer();
    bool estVide();
    void concatener(File<T>* file2);// vide la file2 dans la file courante

};


template<typename T>
File<T>::~File()
{
    T *tmp;
    while (tmp = retirer()){
      delete tmp;
    }
}

template<typename T>
void File<T>::ajouter(T *i)
{
    tete = new Element(i, tete);
    if(!queue){
      queue = tete; // si la pile etait vide
    }
}

template<typename T>
void File<T>::ajouterPriorite(T *i)
{
    if(!queue){
      ajouter(i); // si la pile etait vide
    }
    Element* nqueue = new Element(i, nullptr);
    queue->suivant = nqueue;
    queue = nqueue;
}

template<typename T>
T* File<T>::retirer()
{
    if(estVide())return nullptr;
    T* res = queue->element;
    Element* newQueue = queue->suivant;
    delete queue;
    queue = newQueue;
    if(queue==nullptr)tete=nullptr;
    return res;
}

template<typename T>
bool File<T>::estVide()
{
  return queue == nullptr;
}

template<typename T>
void File<T>::concatener(File<T>* file2)
{
    if(!file2->estVide()){
        tete->suivant = file2->queue;
        tete = file2->tete;
        file2->queue = nullptr;//on vide la file2 pour eviter des effets de bord incontrolles
        file2->tete = nullptr;
    }
}

#endif
