/*
 * ComptonG4PrimaryHit.hh
 *
 *  Created on: Dec 09, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PRIMARYHIT_HH
#define COMPTONG4PRIMARYHIT_HH

#include "ComptonG4EDepHit.hh"
#include "ComptonG4PrimaryData.hh"
class G4Step;
class G4Track;

/*
 *
 */
class ComptonG4PrimaryHit : public ComptonG4EDepHit {
public:
  ComptonG4PrimaryHit();
  ComptonG4PrimaryHit(ComptonG4PrimaryData *data);
  ComptonG4PrimaryHit(const ComptonG4PrimaryHit &right);
  virtual ~ComptonG4PrimaryHit();
  virtual void ClearHit();
  ComptonG4PrimaryData GetData() { return data(); }

  virtual void ProcessStep(G4Step* step);
  virtual void ProcessStep(const G4Step* step);
  virtual void ProcessTrack(G4Track* track);

  void SetPrePosition(G4ThreeVector pos) {
    data().pre_position[0] = pos.x();
    data().pre_position[1] = pos.y();
    data().pre_position[2] = pos.z();
  }
  void SetPreDirection(G4ThreeVector dir) {
    data().pre_direction[0] = dir.x();
    data().pre_direction[1] = dir.y();
    data().pre_direction[2] = dir.z();
  }
  void SetPreKineticEnergy(G4double energy) {
    data().pre_kinetic_energy = energy/CLHEP::MeV; }
  void SetPreStatus(G4int status) {
    data().pre_status = status;
  }

  void SetPostPosition(G4ThreeVector pos) {
    data().post_position[0] = pos.x();
    data().post_position[1] = pos.y();
    data().post_position[2] = pos.z();
  }
  void SetPostDirection(G4ThreeVector dir) {
    data().post_direction[0] = dir.x();
    data().post_direction[1] = dir.y();
    data().post_direction[2] = dir.z();
  }
  void SetPostKineticEnergy(G4double energy) {
    data().post_kinetic_energy = energy/CLHEP::MeV; }
  void SetPostStatus(G4int status) {
    data().post_status = status;
  }

  virtual ComptonG4PrimaryData& data() { return
    dynamic_cast<ComptonG4PrimaryData&>(ComptonG4EDepHit::data()); }

  /* Getters */
  std::string GetVolume() { return data().volume; }
  /*  Setters */
  void SetVolume(std::string vol) { data().volume = vol; }
};

#endif /* COMPTONG4HIT_HH */
