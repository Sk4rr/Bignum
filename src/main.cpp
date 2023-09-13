#include "Bignum.hpp"

#include <iostream>

using namespace std;

int main(){
    unsigned *tab_a=new unsigned[3];
    *tab_a =13;
    *(tab_a+1)=12;
    *(tab_a+2)=1154665;
    unsigned *tab_b=new unsigned[3];
    *tab_b=10;
    *(tab_b+1)=9;
    *(tab_b+2)=8;

    Bignum  a(tab_a, 2, 3, 0);
    Bignum  b(tab_b, 4, 5, 0);
    Bignum  c; 
    //a=c-b;
    bool alpha=a==b;
    cout << alpha << endl;
    cout << a;

    c=a*b;
    cout << "\n\n\n"<<endl;
    //cout <<c;

    int beta=12;
    cout << dec;
    cout<<beta<<endl;
    // a-=b;
    // cout << a.size<<endl;
    // cout<<a.top<<endl;
    // cout<<a.sign<<endl;
    // cout<<a.tab[2]<<","<<a.tab[1]<<","<<a.tab[0]<<endl;

    // unsigned tab_d[1]={4};
    // unsigned tab_e[1]={7};
    // Bignum d(tab_d, 0, 1, 0);
    // Bignum e(tab_e, 0, 1, 0);
    // Bignum f;
    // f=mult2BNsize1(d,e);
    // cout << f.size<<endl;
    // cout<<f.top<<endl;
    // cout<<f.sign<<endl;
    // cout<<f.tab[1]<<","<<c.tab[0]<<endl;
    return 0;
}