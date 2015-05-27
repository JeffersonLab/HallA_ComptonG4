/*
 * ComptonG4OpticalTrackerHit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4HIT_HH
#define COMPTONG4HIT_HH

#include "VComptonG4Hit.hh"
#include <G4ThreeVector.hh>
#include <TObject.h>
#include <ComptonG4OpticalTrackerData.hh>
#include <CLHEP/Units/SystemOfUnits.h>

// Forward declarations
class G4Track;
class G4Step;

/*
 *
 */
class ComptonG4OpticalTrackerHit : public VComptonG4Hit {
public:
  ComptonG4OpticalTrackerHit();
  ComptonG4OpticalTrackerHit(ComptonG4OpticalTrackerData *data);

  /*
   * Copy Constructors
   */
  ComptonG4OpticalTrackerHit(const ComptonG4OpticalTrackerHit &right);

  virtual ~ComptonG4OpticalTrackerHit();
  virtual void ClearHit();
  ComptonG4OpticalTrackerData GetData() { return data(); }
  virtual ComptonG4OpticalTrackerData& data() {
    return dynamic_cast<ComptonG4OpticalTrackerData&>(VComptonG4Hit::data()); }

  virtual void ProcessStep(G4Step *step);
  virtual void ProcessTrack(G4Track *track, G4bool start = false);

  /*  Getters */
  G4ThreeVector GetStartPosition() {
    return G4ThreeVector(data().start_position[0],
        data().start_position[1],data().start_position[2]);
  }
  G4ThreeVector GetStartDirection() {
    return G4ThreeVector(data().start_direction[0],data().start_direction[1],
        data().start_direction[2]);
  }
  G4ThreeVector GetStartMomentum() {
    return G4ThreeVector(data().start_momentum[0],
        data().start_momentum[1],data().start_momentum[2]);
  }
  G4ThreeVector GetEndPosition() {
    return G4ThreeVector(data().end_position[0],
        data().end_position[1],data().end_position[2]);
  }
  G4ThreeVector GetEndDirection() {
    return G4ThreeVector(data().end_direction[0],data().end_direction[1],
        data().end_direction[2]);
  }
  G4ThreeVector GetEndMomentum() {
    return G4ThreeVector(data().end_momentum[0],
        data().end_momentum[1],data().end_momentum[2]);
  }
  G4bool GetDetected() {
    return data().detected;
  }
  G4double GetKineticEnergy() { return G4double(
      data().kinetic_energy*CLHEP::MeV); }
  G4double GetTotalLength() { return G4double(data().total_length*CLHEP::mm); }
  G4int GetTrackID() { return data().track_id; }
  G4int GetParentID() { return data().parent_id; }
  G4int GetStepNumber() { return data().step_number; }
  ComptonG4OpticalProcess GetCreationProcess() {
    return data().creation_process;
  }

  /* Setters  */
  void SetStartPosition(G4ThreeVector pos) {
    data().start_position[0] = pos.x();
    data().start_position[1] = pos.y();
    data().start_position[2] = pos.z();
  }
  void SetStartDirection(G4ThreeVector dir) {
    data().start_direction[0] = dir.x();
    data().start_direction[1] = dir.y();
    data().start_direction[2] = dir.z();
  }
  void SetStartMomentum(G4ThreeVector mom) {
    data().start_momentum[0] = mom.x();
    data().start_momentum[1] = mom.y();
    data().start_momentum[2] = mom.z();
  }
  void SetEndPosition(G4ThreeVector pos) {
    data().end_position[0] = pos.x();
    data().end_position[1] = pos.y();
    data().end_position[2] = pos.z();
  }
  void SetEndDirection(G4ThreeVector dir) {
    data().end_direction[0] = dir.x();
    data().end_direction[1] = dir.y();
    data().end_direction[2] = dir.z();
  }
  void SetEndMomentum(G4ThreeVector mom) {
    data().end_momentum[0] = mom.x();
    data().end_momentum[1] = mom.y();
    data().end_momentum[2] = mom.z();
  }
  void SetDetected(G4bool detected) { data().detected = detected; }
  void SetKineticEnergy(G4double energy) {
    data().kinetic_energy = energy/CLHEP::MeV; }
  void SetTotalLength(G4double length) {
    data().total_length = length/CLHEP::mm; }
  void SetTrackID(G4int id) { data().track_id = id; }
  void SetParentID(G4int id) { data().parent_id = id; }
  void SetStepNumber(G4int number) { data().step_number = number; }
  void SetCreationProcess(ComptonG4OpticalProcess process) {
    data().creation_process = process;
  }
};

#endif /* COMPTONG4HIT_HH */
