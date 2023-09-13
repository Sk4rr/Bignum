#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <iostream>
using namespace std;

class Bignum {
public:
  Bignum(int = 0);  //
  Bignum(unsigned);  //
  Bignum(Bignum const &);  //
  Bignum(Bignum &&);  //
  Bignum(unsigned *tab, unsigned top, unsigned size, unsigned sign);

  ~Bignum();  //

  Bignum & operator=(Bignum const &);  //copie
  Bignum & operator=(Bignum &&);  // deplacement



   //
  Bignum & operator-=(Bignum const &);
  Bignum & operator+=(Bignum const &);

  friend bool operator!=(Bignum const &, Bignum const &);
  friend bool operator==(Bignum const &, Bignum const &);
  friend Bignum operator-(Bignum const &, Bignum const &); //fait
  friend Bignum operator+(Bignum const &, Bignum const &); //fait

  friend Bignum operator*(Bignum const &, Bignum const &);
  friend Bignum operator/(Bignum const &, Bignum const &);

  friend ostream & operator<< (ostream&, Bignum const &);


  friend Bignum addSameSign(Bignum const &, Bignum const &); //fait
  friend Bignum addDiffSign(Bignum const &, Bignum const &); //fait
  friend bool isBigest(Bignum const &, Bignum const &); //fait

//private:
  unsigned *tab;
  unsigned top;
  unsigned size;
  unsigned sign;
};


#endif
