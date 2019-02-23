#ifndef UTILITY_H
#define UTILITY_H

template<class T1, class T2>
struct pair{
  T1 first;
  T2 second;

pair(const T1& __a, const T2& __b)
: first(__a), second(__b) { }

inline bool operator==(const pair<T1, T2>& b){
  return first==b.first && second == b.second;
}
inline pair<T1, T2> operator-(const pair<T1, T2>& b){
  return pair<T1, T2>(first-b.first, second - b.second);
}
inline pair<T1, T2> operator+(const pair<T1, T2>& b){
  return pair<T1, T2>(first + b.first, second + b.second);
}
};

template<class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
    return pair<T1, T2>(x, y);
}

template<class T>
class stack{
private:
    struct Element{
        T element;
        Element* precedent = nullptr;

        Element(const T& element, Element* precedent ):element(element),precedent(precedent)
        {}

        ~Element(){}
    };

    Element* tete = nullptr;

public:

    stack(){}
    inline bool empty(){
        return tete==nullptr;
    }
    void push(const T& elem){
        tete = new Element(elem, tete);
    }
    T& top(){
        return tete->element;
    }
    void pop(){
      if(empty())return;
      Element* tmp = tete->precedent;
      delete tete;
      tete = tmp;
    }

};

template<typename T>                // For lvalues (T is T&),
T&& forward(T&& param)         // take/return lvalue refs.
{                                   // For rvalues (T is T),
    return static_cast<T&&>(param); // take/return rvalue refs.
}


#endif
