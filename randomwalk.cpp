#include "randomwalk.h"
#include <thread>

RandomWalk::RandomWalk(const double & sigma, const double & dt,const unsigned int &nthreads, const size_t & n, const size_t & nsteps, const uint32_t &seed) : sigma{sigma},dt{dt}, data{nullptr}, n{n}, nsteps{nsteps}, nthreads{nthreads}
{
    for (unsigned int i=0;i<nthreads;++i) {
        rnd.push_back(Rng(seed+i));
    }
    data=new double[n*nsteps];
}
RandomWalk::~RandomWalk(){
    delete [] data;
}


void randomWalk(double *d, const double & sigma, const size_t & nsteps, const double & dt, Rng & r) noexcept{
    double x=0.0;
    for (unsigned int i=0;i<nsteps;i++) {
        x+=r.normal_gauss()*sigma*dt;
        d[i]=x;
    }
}
void pbc_(double *d, const size_t & nsteps, const double & l) noexcept {
    for (unsigned int i=0;i<nsteps;i++) {
        d[i]=d[i]-l*std::round(d[i]/l);
    }
}

void RandomWalk::pbc(const double & l) noexcept {
    std::vector<std::thread> threads;
    size_t sizeth=n/nthreads;
    for (unsigned int ith=0;ith<nthreads;++ith){
        threads.push_back(std::thread([&,ith](){
            size_t start = sizeth*ith;
            size_t stop = sizeth*(ith+1);
            if (ith==nthreads-1) stop=n;
            for (size_t istep=start;istep<stop;++istep){
                pbc_(&data[nsteps*istep],nsteps,l);
            }
        }));
    }
    for (unsigned int ith=0;ith<nthreads;ith++)
        threads[ith].join();

    threads.clear();
}

void RandomWalk::calcola()  noexcept{

    std::vector<std::thread> threads;
    size_t sizeth=n/nthreads;
    for (unsigned int ith=0;ith<nthreads;++ith){
        threads.push_back(std::thread([&,ith](){
            size_t start = sizeth*ith;
            size_t stop = sizeth*(ith+1);
            if (ith==nthreads-1) stop=n;
            for (size_t istep=start;istep<stop;++istep){
                randomWalk(&data[nsteps*istep],sigma,nsteps,dt,rnd[ith]);
            }
        }));
    }
    for (unsigned int ith=0;ith<nthreads;ith++)
        threads[ith].join();

    threads.clear();

}
