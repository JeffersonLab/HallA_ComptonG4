/*
 * VComptonG4Hit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef VCOMPTONG4HIT_HH
#define VCOMPTONG4HIT_HH

// Include ROOT default types (so it can be written to a ROOT file by default)
#include <Rtypes.h>

// Parent Hit class
#include <G4VHit.hh>
#include <VComptonG4Data.hh>

/*
 *
 */
class VComptonG4Hit : public G4VHit {
public:
  VComptonG4Hit();
  VComptonG4Hit(const VComptonG4Hit &){};
  virtual ~VComptonG4Hit();
  virtual void ClearHit() = 0;

  virtual VComptonG4Data* GetDataRef() = 0;
};

#endif /* VCOMPTONG4HIT_HH */
