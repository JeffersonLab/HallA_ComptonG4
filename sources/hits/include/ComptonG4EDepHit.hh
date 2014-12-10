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
class G4Track;

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
  ComptonG4EDepData GetData() { return data(); }
  virtual ComptonG4EDepData& data() { return dynamic_cast<ComptonG4EDepData&>(
      ComptonG4Hit::data()); }

  virtual void ProcessStep(G4Step* step);
  virtual void ProcessTrack(G4Track* track);

  /*  Setters */
  void SetEnergyDeposited(G4double edep) {
    data().energy_deposited = edep/CLHEP::MeV; }
  /* Getters */
  G4double GetEnergyDeposited() { return G4double(
      data().energy_deposited*CLHEP::MeV); }
};

#endif /* COMPTONG4HIT_HH */
