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
  virtual VComptonG4Data* GetDataRef() { return fData; };
  void SetDataRef(ComptonG4OpticalData *data) { fData = data; }
  void CopyData(ComptonG4OpticalData &data) { *fData = data; }
  ComptonG4OpticalData GetData() { return *fData; }

  virtual void ProcessStep(G4Step* step);

  // Increase photon count as a new photon came
  void NewPhoton() { fData->num_photons++; };

  /* Getters */
  G4int GetNumPhotons() { return fData->num_photons; }
  /*  Setters */
  void SetNumPhotons(G4int num) { fData->num_photons = num; }
private:
  ComptonG4OpticalData *fData;
};

#endif /* COMPTONG4HIT_HH */
