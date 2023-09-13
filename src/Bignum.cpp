#include <cstdint>
#include <iostream>
#include <iomanip>

#include "Bignum.hpp"

using namespace std;

Bignum::Bignum(int x) : top (0), size (1) {
  tab = new unsigned [1];
  if (x >= 0) {
    tab[0] = x;
    sign = 0;
  }
  else {
    tab[0] = -x;
    sign = 1;
  }
}


Bignum::Bignum(unsigned x) : top (0), size (1), sign (0) {
  tab = new unsigned [1];
  tab[0] = x;
}

Bignum::Bignum(Bignum const & x) : top (x.top), size (x.size), sign (x.sign) {
  tab = new unsigned [size];
  for (unsigned i = 0; i < top; ++i) tab[i] = x.tab[i];
}

Bignum::Bignum(Bignum && x) : tab (x.tab), top (x.top), size (x.size), sign (x.sign) {
  x.tab = nullptr;
}

Bignum::Bignum(unsigned *xtab, unsigned xtop, unsigned xsize, unsigned xsign) : tab (xtab), top (xtop), size (xsize), sign (xsign) {}

Bignum::~Bignum() {
  delete [] tab;
}


Bignum & Bignum::operator=(Bignum const & x) {
  if (x.size > size) {
    delete [] tab;
    tab = new unsigned [x.size];
    size = x.size;
  }
  top = x.top;
  sign = x.sign;
  for (unsigned i = 0; i < size+1; ++i) tab[i] = x.tab[i];
  return *this;
}


Bignum & Bignum::operator=(Bignum && x) {
  delete [] tab;
  tab = x.tab;
  x.tab = nullptr;
  top = x.top;
  size = x.size;
  sign = x.sign;
  return *this;
}


Bignum addSameSign(Bignum const & x, Bignum const & y) {
  if (x.top < y.top) return addSameSign(y,x);
  //cout << x.top <<','<<y.top<<endl;
  unsigned size = (x.top+1 == x.size) ? x.size+1 : x.size;
  unsigned * tab = new unsigned [size];
  uint64_t carry = 0;
  for (unsigned i = 0; i < y.top+1; ++i) {
    uint64_t tmp = carry + x.tab[i] + y.tab[i];
    carry = tmp >> 32;
    tab[i] = tmp;
  }
  for (unsigned i = y.top+1; i < x.top+1; ++i) {
    uint64_t tmp = carry + x.tab[i];
    carry = tmp >> 32;
    tab[i] = tmp;
  }
  tab[x.top+1] = carry;
  return Bignum(tab, x.top +1 + carry, size, x.sign);
}
/*ca maaaaaaarche*/

//renvoie true si x>=y false sinon
bool isBigest(Bignum const & x, Bignum const & y){
  int i = x.top;
  while(i>0 && x.tab[i]==0) i-- ;
  int j=y.top;
  while(j>0 && y.tab[j]==0) j-- ;
  if (i>j) return true;
  if (i<j) return false;
  while(i > -1){
    if (x.tab[i]<y.tab[i]) return false;
    if (x.tab[i]>y.tab[i]) return true;
    i--;
  }
  return true;
}


Bignum addDiffSign(Bignum const & x, Bignum const & y) {
  if(isBigest(x,y)==false) return addDiffSign(y,x);
  unsigned top=x.top;
  unsigned size=x.size;
  unsigned * tab=new unsigned[size];
  int carry = 0;
  for(unsigned i = 0; i<y.top+1; i++){
    if (x.tab[i]<y.tab[i]){
      uint64_t tmp =1<<32 + x.tab[i] - y.tab[i] + carry;
      tab[i]= tmp;
      carry = 1;
    }
    else{
      tab[i]=x.tab[i] - y.tab[i]- carry;
      carry = 0;
    }
  }
  for (unsigned i=y.top+1; i<x.top+1; i++){
    tab[i]=x.tab[i]-carry;
    carry = 0;
  }
  return Bignum( tab, top, size , x.sign);
}


Bignum operator+(Bignum const & x, Bignum const & y){
  if (x.sign==y.sign){
    return addSameSign(x,y);
  }
  else{
    return addDiffSign(x,y);
  }
}

Bignum operator-(Bignum const & x, Bignum const & y){
  Bignum c;
  int tmp;
  if (x.sign!=y.sign){
    tmp=x.sign;
    c=addSameSign(x,y);
    c.sign=tmp;
    return c;
  }
  else{
    if (isBigest(x,y)==true) tmp=x.sign;
    else {
      if (y.sign==0) tmp=1;
      else tmp=0;
    }
    c=addDiffSign(x,y);
    c.sign=tmp;
    return c;
  }
}

Bignum & Bignum::operator+=(Bignum const & x){
  if (top+1<size && x.top+1<size){
    if (sign==x.sign){
      int carry = 0;
      for (unsigned i = 0; i < x.top+1; ++i) {
        uint64_t tmp = carry + tab[i] + x.tab[i];
        carry = tmp >> 32;
        tab[i] = tmp;
      }
      for (unsigned i = x.top+1; i < top+1; ++i) {
        uint64_t tmp = carry + tab[i];
        carry = tmp >> 32;
        tab[i] = tmp;
      }
      tab[x.top+1]=carry;
    }
    else{
      if(isBigest(*this,x)==false){
        sign=x.sign;
      }
      int carry = 0;
      for(unsigned i = 0; i<x.top+1; i++){
        if (tab[i]<x.tab[i]){
          uint64_t tmp =1<<32 + tab[i] - x.tab[i] + carry;
          tab[i]= tmp;
          carry = 1;
        }
        else{
          tab[i]=tab[i] - x.tab[i]- carry;
          carry = 0;
        }
      }
      for (unsigned i=x.top+1; i<top+1; i++){
        tab[i]=tab[i]-carry;
        carry = 0;
      }
    }
    return *this;
  }
  else{
    *this = *this + x;
    return *this;
  }
}

Bignum & Bignum::operator-=(Bignum const & x){
  if (sign ==x.sign){
    if (isBigest(*this, x)){
      uint64_t carry = 0;
      for (int i=0; i<top+1; i++){
        if (tab[i]>=x.tab[i]+carry){
          tab[i]=tab[i]-x.tab[i]-carry;
          carry=0;
        }
        else{
          tab[i]=1<<32 +tab[i]-x.tab[i]-carry;
          carry = 1;
        }
      }
    }
    else{
      uint64_t carry = 0;
      for (int i=0; i<x.top+1; i++){
        if (x.tab[i]>=tab[i]+carry){
          tab[i]=x.tab[i]-tab[i]-carry;
          carry=0;
        }
        else{
          tab[i]=1<<32 +x.tab[i]-tab[i]-carry;
          carry = 1;
        }
      }
      top=x.top;
      size=x.size;
      sign=(sign==1)?0:1;
    }
  }
  else{
    *this+=x;
  }
  return *this;
}

bool operator==(Bignum const & x, Bignum const & y){
  if (x.sign!=y.sign) return false;
  int pointeurx=x.top;
  while (x.tab[pointeurx]==0) pointeurx--;
  int pointeury=y.top;
  while (y.tab[pointeury]==0) pointeury--;
  if (pointeurx!=pointeury) return false;
  for (int i=0; i<pointeurx+1; i++){
    if (x.tab[i]!=y.tab[i]) return false;
  }
  return true;
}

bool operator!=(Bignum const & x, Bignum const & y){
  if (x.sign!=y.sign) return true;
  int pointeurx=x.top;
  while (x.tab[pointeurx]==0) pointeurx--;
  int pointeury=y.top;
  while (y.tab[pointeury]==0) pointeury--;
  if (pointeurx!=pointeury) return true;
  for (int i=0; i<pointeurx+1; i++){
    if (x.tab[i]!=y.tab[i]) return true;
  }
  return false;
}

ostream & operator<<(ostream& os, Bignum const & x){
  os<< "sign = "<<x.sign<<", top = "<<x.top<<", size = "<<x.size;
  // for (int i=x.size-1; i!=-1; i--){
  //   os<<", tab["<<i<<"] = "<<x.tab[i];
  // }
  // os<<"\n\n";
  // for (int i=x.size-1; i!=0; i--){
  //   os<<"(2^32)^"<<i<<"x"<<x.tab[i]<<"+";
  // }
  // os<<x.tab[0];
  // os << "\n"<<endl;
  for (int i=x.size-1; i!=-1; i--){
    uint64_t tmp=x.tab[i];
    os << setfill ('0') << setw(8)<<hex<< tmp;
  }
  return os;
}

Bignum operator*(Bignum const & x, Bignum const & y){
  if (isBigest(y, x)){
    return operator*(y, x);
  }
  unsigned sign= (x.size==y.size) ? 0 : 1;
  int pointeurx=x.top;
  while (x.tab[pointeurx]==0) pointeurx--;
  int pointeury=y.top;
  while (y.tab[pointeury]==0) pointeury--;
  unsigned size=pointeurx*pointeury;
  unsigned top=size-1;
  unsigned *tab=new unsigned[size];
  Bignum resultat=(tab, top, size, sign);
  for (int i=0; i<pointeury; i++){
    unsigned *tmptab=new unsigned[size];
    Bignum tmp=(tmptab, top, size, sign);
    for (int j=0; j<pointeurx; j++){
      
    }
  }
  return resultat;
}
