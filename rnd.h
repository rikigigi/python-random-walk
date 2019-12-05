#ifndef RND_H
#define RND_H

#include <stdint.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>

class Rng {

public:
    Rng( uint32_t seed=123)  noexcept: jsr{seed} {
        //warm up generator
        for (unsigned int i=0;i<1000;++i) SHR3();
        init_cmwc4096();
        //warm up second generator
        for (unsigned int i=0;i<10*4096;++i) cmwc4096();
    }
    void set_SHR3_jsr(const uint32_t & j) noexcept {jsr=j;}
    uint32_t SHR3() noexcept { return (jz=jsr, jsr^=(jsr<<13), jsr^=(jsr>>17), jsr^=(jsr<<5),jz+jsr) ;}
    void init_cmwc4096() noexcept{
        for (int i=0;i<4096;i++){
            Q_[i]=SHR3();
        }
    }
    // questa funzione genera un numero intero pseudo-casuale a 32 bit (compreso fra 0 e 2^32-1) -- algoritmo di Marsaglia
    uint32_t cmwc4096() noexcept{
    uint64_t t, a=18782LL;
    static uint32_t i=4095,c_=362436;
    uint32_t x,r=0xfffffffe;
        i=(i+1)&4095;
        t=a*Q_[i]+c_;
        c_=(t>>32);
        x=t+c_;
        if(x<c_){
            x++;c_++;
        }
        return(Q_[i]=r-x);
    }
    // questa funzione combina due numeri interi a 32 bit in un numero intero a 64 bit, usando le operazioni fra bit.
    uint64_t rnd64() noexcept{
    uint64_t t,r=0;
    r=( ( (uint64_t) cmwc4096() << 32 )& 0xFFFFFFFF00000000ull) | ( (uint64_t) cmwc4096() & 0x00000000FFFFFFFFull); // << sposta i bit a sinistra della quantita' intera specificata. | esegue un "or" bit per bit, & "and"
    return r;
    }

    // prende direttamente i bit da un numero intero casuale e li mette dentro la mantissa del numero float, impostando anche l'esponente. Il numero generato e' compreso fra 0.5 e 1 (il numero e' scritto in base 2).
    double rnd_double() noexcept {
    double t;
    uint64_t r;
    uint32_t rr;
        if (sizeof(double)==8) { // float a 64 bit
            r=rnd64();
    //                            001111111110=0x3FE
            r= (r>>12)& 0x000FFFFFFFFFFFFFull  | 0x3FE0000000000000ull;
            t= * (double*) &r;
        }else if (sizeof(double)==4) { // float a 32 bit
            rr=cmwc4096();
    //                            001111110000 = 0x3F000000
            rr=(rr>>9)& 0b00000000011111111111111111111111 | 0x3F000000;
            t=*(double*)&rr;
        } else {
            fprintf(stderr,"Errore: tipo double non riconosciuto.\n");
            abort();
        }
    return t;
    }

    // stessa cosa con il tipo a 32 bit
    float rnd_single() noexcept {
    uint32_t r;
    float t;
        if (sizeof(float)==4){
            r=cmwc4096();
    //                          001111110000
            r=(r>>9)& 0b00000000011111111111111111111111 | 0x3F000000;
            t=*(float*)&r;
        } else {
            fprintf(stderr,"Errore: tipo float non riconosciuto.\n");
            abort();
        }
    return t;
    }

    double UNI() noexcept{return (rnd_double()-0.5)*2;}

    /* implementazione avanzata di un generatore di numeri pseudo-casuali con distribuzione gaussiana (http://www.jstatsoft.org/v05/i08/paper (George Marsaglia; Wai Wan Tsang (2000). "The Ziggurat Method for Generating Random Variables". Journal of Statistical Software 5 (8).)
    */

    double normal_gauss() noexcept{
     int i,indice;
    double x,y;

    i=cmwc4096();
    indice=i & 127;
    x=wtab[indice]*i;
        if (std::abs(i)<ktab[indice]){ // sono completamente dentro la gaussiana, accetto subito!
            return i*wtab[indice];
        }else if (indice==127){ // aiai sono finito sulla coda
                do {
                    x=-std::log(UNI())/PARAM_R;
                    y=-std::log(UNI());
                } while (y+y<x*x);
                return (i>0)? PARAM_R+x : -PARAM_R-x;
        } else if (ytab[indice]+UNI()*(ytab[indice+1]-ytab[indice]) < exp(-.5*x*x) ) { // sono sul bordo esterno del rettangolino, devo vedere dove cade la y per decidere se sono dentro la gaussiana o no!
            return x;
        }else {
            return normal_gauss();
        }

    }


private:
    uint32_t Q_[4096], jz,jsr;
    const static double PARAM_R,wtab[128],ytab[128];
    const static unsigned long ktab[128];
};

#endif
