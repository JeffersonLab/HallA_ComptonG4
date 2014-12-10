/*
 * ComptonG4Hit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4HIT_HH
#define COMPTONG4HIT_HH

#include "VComptonG4Hit.hh"
#include <G4ThreeVector.hh>
#include <TObject.h>
#include <ComptonG4Data.hh>

// Forward declarations
class G4Track;
class G4Step;

/*
 *
 */
class ComptonG4Hit : public VComptonG4Hit {
public:
  ComptonG4Hit();
  ComptonG4Hit(ComptonG4Data *data);

  /*
   * Copy Constructors
   */
  ComptonG4Hit(const ComptonG4Hit &right);

  virtual ~ComptonG4Hit();
  virtual void ClearHit();
  ComptonG4Data GetData() { return data(); }
  virtual ComptonG4Data& data() { return dynamic_cast<ComptonG4Data&>(
      VComptonG4Hit::data()); }

  virtual void ProcessStep(G4Step *step);
  virtual void ProcessTrack(G4Track *track);

  /*  Getters */
  G4ThreeVector GetPosition() {
    return G4ThreeVector(data().position[0],data().position[1],data().position[2]);
  }
  G4double GetGlobalTime() { return G4double(data().global_time*CLHEP::ns); }
  G4double GetLotalTime()  { return G4double(data().local_time*CLHEP::ns); }
  G4ThreeVector GetDirection() {
    return G4ThreeVector(data().direction[0],data().direction[1],
        data().direction[2]);
  }
  G4ThreeVector GetMomentum() {
    return G4ThreeVector(data().momentum[0],data().momentum[1],data().momentum[2]);
  }
  G4double GetKineticEnergy() { return G4double(
      data().kinetic_energy*CLHEP::MeV); }
  G4double GetTotalLength() { return G4double(data().total_length*CLHEP::mm); }
  G4int GetParticleID() { return data().particle_id; }
  G4int GetTrackID() { return data().track_id; }
  G4int GetParentID() { return data().parent_id; }
  G4int GetStepNumber() { return data().step_number; }
  G4int GetTrackStatus() { return data().track_status; }
  G4ThreeVector GetVertexPosition() {
    return G4ThreeVector(data().vertex_position[0],data().vertex_position[1],
        data().vertex_position[2]);
  }
  G4ThreeVector GetVertexDirection() {
    return G4ThreeVector(data().vertex_direction[0],data().vertex_direction[1],
        data().vertex_direction[2]);
  }
  G4double GetVertexKineticEnergy() { return G4double(
      data().vertex_kinetic_energy*CLHEP::MeV); }
  std::string GetCreationProcess() { return data().creation_process; }

  /* Setters  */
  void SetPosition(G4ThreeVector pos) {
    data().position[0] = pos.x();
    data().position[1] = pos.y();
    data().position[2] = pos.z();
  }
  void SetGlobalTime(G4double time ) { data().global_time = time/CLHEP::ns; }
  void SetLocalTime(G4double time ) { data().local_time = time/CLHEP::ns; }
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
  void SetKineticEnergy(G4double energy) {
    data().kinetic_energy = energy/CLHEP::MeV; }
  void SetTotalLength(G4double length) {
    data().total_length = length/CLHEP::mm; }
  void SetParticleID(G4int id) { data().particle_id = id; }
  void SetTrackID(G4int id) { data().track_id = id; }
  void SetParentID(G4int id) { data().parent_id = id; }
  void SetStepNumber(G4int number) { data().step_number = number; }
  void SetTrackStatus(G4int status) { data().track_status = status; }
  void SetVertexPosition(G4ThreeVector pos) {
    data().vertex_position[0] = pos.x();
    data().vertex_position[1] = pos.y();
    data().vertex_position[2] = pos.z();
  }
  void SetVertexDirection(G4ThreeVector dir) {
    data().vertex_direction[0] = dir.x();
    data().vertex_direction[1] = dir.y();
    data().vertex_direction[2] = dir.z();
  }
  void SetVertexKineticEnergy(G4double energy) {
    data().vertex_kinetic_energy = energy/CLHEP::MeV; }
  void SetCreationProcess(std::string process) {
    data().creation_process = process; }
};

#endif /* COMPTONG4HIT_HH */
