# GEANT4 Simulation for The Hall A Compton Polarimeter

Simulation package used to extract an electron beam polarization via detection
of Compton Scattered photons and/or electrons.

## Compiling

In a separate build directory type:
```
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX /path/to/install /path/to/source/code
make
```


### Status:
So far, only the photon detector is enabled. But electron detector will be
added soon....
