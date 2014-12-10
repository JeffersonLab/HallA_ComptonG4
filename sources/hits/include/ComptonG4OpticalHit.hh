/*
 * ComptonG4OpticalHit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4OPTICALHIT_HH
#define COMPTONG4OPTICALHIT_HH

#include "ComptonG4Hit.hh"
#include "ComptonG4OpticalData.hh"
class G4Step;

/*
 *
 */
class ComptonG4OpticalHit : public ComptonG4Hit {
public:
  ComptonG4OpticalHit();
  ComptonG4OpticalHit(ComptonG4OpticalData *data);
  ComptonG4OpticalHit(const ComptonG4OpticalHit &right);
  virtual ~ComptonG4OpticalHit();
  virtual void ClearHit();
  ComptonG4OpticalData GetData() { return data(); }

  virtual void ProcessStep(G4Step* step);
  virtual ComptonG4OpticalData& data() { return
    dynamic_cast<ComptonG4OpticalData&>(ComptonG4Hit::data()); }

  // Increase photon count as a new photon came
  void NewPhoton() { data().num_photons++; };

  /* Getters */
  G4int GetNumPhotons() { return data().num_photons; }
  /*  Setters */
  void SetNumPhotons(G4int num) { data().num_photons = num; }
};

#endif /* COMPTONG4HIT_HH */
