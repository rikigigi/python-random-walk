#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include <vector>
#include <cstddef>
#include "rnd.h"



class RandomWalk
{
public:
    RandomWalk(const double & sigma, const double & dt, const unsigned int &nthreads, const size_t & n, const size_t & nsteps, const uint32_t &seed);
    ~RandomWalk();
    void calcola() noexcept;
    void pbc(const double & l) noexcept;
    std::vector<size_t> get_shape() const noexcept { return {n,nsteps} ; }
    std::vector<size_t> get_stride() const noexcept { return { nsteps*sizeof (double), sizeof(double)};}
    double * get_data() noexcept {return data;}

private:
    double * data,sigma,dt;
    size_t n,nsteps,nthreads;
    std::vector<Rng> rnd;

};

#endif // RANDOMWALK_H
