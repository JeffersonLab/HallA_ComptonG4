/*
 * ComptonG4EDepHit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4EDEPHIT_HH
#define COMPTONG4EDEPHIT_HH

#include "ComptonG4Hit.hh"
#include "ComptonG4EDepData.hh"
class G4Step;

/*
 *
 */
class ComptonG4EDepHit : public ComptonG4Hit {
public:
  ComptonG4EDepHit();
  ComptonG4EDepHit(ComptonG4EDepData *data);
  ComptonG4EDepHit(const ComptonG4EDepHit &right);
  virtual ~ComptonG4EDepHit();
  virtual void ClearHit();
  virtual VComptonG4Data* GetDataRef() { return fData; };
  void CopyData(ComptonG4EDepData &data) { *fData = data; }
  ComptonG4EDepData GetData() { return *fData; }

  virtual void ProcessStep(G4Step* step);

  /*  Setters */
  void SetEnergyDeposited(G4double edep) {
    fData->energy_deposited = edep/CLHEP::MeV; }
  /* Getters */
  G4double GetEnergyDeposited() { return G4double(
      fData->energy_deposited*CLHEP::MeV); }

private:
  ComptonG4EDepData *fData;
};

#endif /* COMPTONG4HIT_HH */
