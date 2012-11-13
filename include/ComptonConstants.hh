#ifndef COMPTONCONSTANTS_HH
#define COMPTONCONSTANTS_HH


#include <globals.hh>

namespace ComptonConstants {

  // Typical constants
  G4double kC         = 2.99792e+8 * m/s;     // Speed of light
  G4double kPlanck    = 4.135669e-21 * s*MeV; // Planck's Constant

  // Typical Masses
  G4double kMassElectron = 0.511*MeV/(kC*kC);   // Electron mass
}

#endif
