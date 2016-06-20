# GEANT4 Simulation for The Hall A Compton Polarimeter

Simulation package used to extract an electron beam polarization via detection
of Compton Scattered photons and/or electrons.

## Getting Started
These instructions are meant to provide a very quick guide to compiling and
running a sample simulation using the PbWO4 detector used during the DVCS 2016
running period.

### Requirements
* Geant4 > 10.0
  * With GDML, Qt and CLHEP support build int.
  * For Geant4 > 10.2, a compiler with C++11 support is also needed.
* BOOST libraries
* cmake > 2.8
* git (for contributing to the development process)

### Setup your build environment
For simplicity, these instructions will use a directory structure such that
the code will be inside one directory and the compilation will happen on
another. For example:
```shell
cd $HOME
mkdir HallACompton
cd HallACompton
mkdir build
```

### Downloading and Compiling
You can get the latest copy of the software form Github at
https://github.com/JeffersonLab/HallA_ComptonG4. The recommended method of
getting a copy is to use git to clone a fresh copy of the master branch. The
path that you will use depends on whether you have a github account and
intend on contributing to the development process, **or** you do not have
a github account.

If you have a github account, you can use SSH authetnication with any of
SSH keys you have added to your account. Then to get a copy of the HallA_ComptonG4
code, you use:
```shell
cd $HOME/HallACompton
git clone git@github.com:JeffersonLab/HallA_ComptonG4.git
```
Alternatively, if you do not have an account, or, you cannot use SSH due
to restrictions on your network, you can get the code from HTTPS like so:
a copy of the code, like so:
```shell
cd $HOME/HallACompton
git clone https://github.com/JeffersonLab/HallA_ComptonG4.git
```
#### Building the software
After you have used **one** of the two methods above to get a copy of the code,
you should have two directories in `$HOME/HallACompton`: **build** and
**HallA_ComptonG4**.

We use `cmake` to build the software, which requires the use of a different
(and clean) directory than the source code. The *recommended* method is
to use a directory outside the HallA_ComptonG4 directory containing the
github code. In our example directory structure, we will build the code in
`$HOME/HallACompton/build` and the actual source code will live in
`$HOME/HallACompton/HallA_ComptonG4`.

Then to build the code we use:
```
## Change into the build directory
cd $HOME/HallACompton/build
## Use cmake to configure the build (needs to be re-run after adding
## any files to the source tree)
cmake -D CMAKE_BUILD_TYPE=RelWithDebInfo ../HallA_ComptonG4
## Build the actual code
make
## Optionally, you can install the software in a common location
## (you can specify the install location by adding
##      -D CMAKE_INSTALL_PREFIX=/path/to/install/location
## right before the ../HallA_ComptonG4 part.
make install ## Optional
```


### Status:
So far, only the photon detector is enabled. But electron detector will be
added soon....
