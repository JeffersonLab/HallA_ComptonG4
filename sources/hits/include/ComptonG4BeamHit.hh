/*
 * ComptonG4BeamHit.hh
 *
 *  Created on: Jun 14, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4BEAMHIT_HH
#define COMPTONG4BEAMHIT_HH

#include "VComptonG4Hit.hh"
#include <G4ThreeVector.hh>
#include <TObject.h>
#include <ComptonG4BeamData.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include "ComptonG4HitsCollection.hh"

// Forward declarations
class G4Track;
class G4Step;

/*
 *
 */
class ComptonG4BeamHit : public VComptonG4Hit {
public:
  ComptonG4BeamHit();
  ComptonG4BeamHit(ComptonG4BeamData *data);

  /*
   * Copy Constructors
   */
  ComptonG4BeamHit(const ComptonG4BeamHit &right);

  virtual ~ComptonG4BeamHit();
  virtual void ClearHit();
  ComptonG4BeamData GetData() { return data(); }
  virtual ComptonG4BeamData& data() {
    return dynamic_cast<ComptonG4BeamData&>(VComptonG4Hit::data()); }

  virtual void ProcessStep(G4Step *step);
  virtual void ProcessTrack(const G4Track *track);

  /*  Getters */
  G4ThreeVector GetPosition() {
    return G4ThreeVector(data().position[0],
        data().position[1],data().position[2]);
  }
  G4ThreeVector GetDirection() {
    return G4ThreeVector(data().direction[0],data().direction[1],
        data().direction[2]);
  }
  G4ThreeVector GetMomentum() {
    return G4ThreeVector(data().momentum[0],
        data().momentum[1],data().momentum[2]);
  }
  G4ThreeVector GetPolarization() {
    return G4ThreeVector(data().polarization[0],
        data().polarization[1],data().polarization[2]);
  }

  /* Setters  */
  void SetPosition(G4ThreeVector pos) {
    data().position[0] = pos.x();
    data().position[1] = pos.y();
    data().position[2] = pos.z();
  }
  void SetDirection(G4ThreeVector dir) {
    data().direction[0] = dir.x();
    data().direction[1] = dir.y();
    data().direction[2] = dir.z();
  }
  void SetMomentum(G4ThreeVector mom) {
    data().momentum[0] = mom.x();
    data().momentum[1] = mom.y();
    data().momentum[2] = mom.z();
  }
  void SetPolarization(G4ThreeVector pol) {
    data().polarization[0] = pol.x();
    data().polarization[1] = pol.y();
    data().polarization[2] = pol.z();
  }
};

typedef ComptonG4HitsCollection<ComptonG4BeamHit,
        ComptonG4BeamData> ComptonG4BeamHitsCollection;

#endif /* COMPTONG4BEAMHIT_HH */
