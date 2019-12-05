# python random walk
small library to do many random walks in python using C++ code (a lot faster). Used to do numerical experiments. Depends on pybind11 (included). Multithreaded.

## build
Python must be configured correctly.
```
mkdir build
cd build
cmake ../
make
```

## usage
You will find `pydif.cpython-37m-x86_64-linux-gnu.so`, or a similar file depending on your python version, in the build directory. This is the full library. To use it, put in the python path or in the same directory of the python script, and do, for example:
```
import pydif
import numpy as np

rndWalk = pydif.RandomWalk(sigma, dt**.5, nthreads, n_different_random_walks, nsteps, seed)
rndWalk.calc()
rndWalk.pbc(lenght_of_box) # if you want to apply pbc to the random walk

#data can be directly read by numpy. First index is realization index, second is time index
data = np.array(rndWalk, copy=False)

#do whatever you want

```
