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
  ComptonG4Hit(const ComptonG4Hit &right);
  virtual ~ComptonG4Hit();
  virtual void ClearHit();
  virtual VComptonG4Data* GetDataRef() { return fData; };
  void SetDataRef(ComptonG4Data *data) { fData = data; }
  void CopyData(ComptonG4Data &data) { *fData = data; }
  ComptonG4Data GetData() { return *fData; }

  virtual void ProcessStep(G4Step *step);
  virtual void ProcessTrack(G4Track *track);

  /*  Getters */
  G4ThreeVector GetPosition() {
    return G4ThreeVector(fData->position[0],fData->position[1],fData->position[2]);
  }
  G4double GetGlobalTime() { return G4double(fData->global_time*CLHEP::ns); }
  G4double GetLotalTime()  { return G4double(fData->local_time*CLHEP::ns); }
  G4ThreeVector GetDirection() {
    return G4ThreeVector(fData->direction[0],fData->direction[1],
        fData->direction[2]);
  }
  G4ThreeVector GetMomentum() {
    return G4ThreeVector(fData->momentum[0],fData->momentum[1],fData->momentum[2]);
  }
  G4double GetKineticEnergy() { return G4double(
      fData->kinetic_energy*CLHEP::MeV); }
  G4double GetTotalLength() { return G4double(fData->total_length*CLHEP::mm); }
  G4int GetParticleID() { return fData->particle_id; }
  G4int GetTrackID() { return fData->track_id; }
  G4int GetParentID() { return fData->parent_id; }
  G4int GetStepNumber() { return fData->step_number; }
  G4int GetTrackStatus() { return fData->track_status; }
  G4ThreeVector GetVertexPosition() {
    return G4ThreeVector(fData->vertex_position[0],fData->vertex_position[1],
        fData->vertex_position[2]);
  }
  G4ThreeVector GetVertexDirection() {
    return G4ThreeVector(fData->vertex_direction[0],fData->vertex_direction[1],
        fData->vertex_direction[2]);
  }
  G4double GetVertexKineticEnergy() { return G4double(
      fData->vertex_kinetic_energy*CLHEP::MeV); }
  std::string GetCreationProcess() { return fData->creation_process; }

  /* Setters  */
  void SetPosition(G4ThreeVector pos) {
    fData->position[0] = pos.x();
    fData->position[1] = pos.y();
    fData->position[2] = pos.z();
  }
  void SetGlobalTime(G4double time ) { fData->global_time = time/CLHEP::ns; }
  void SetLocalTime(G4double time ) { fData->local_time = time/CLHEP::ns; }
  void SetDirection(G4ThreeVector dir) {
    fData->direction[0] = dir.x();
    fData->direction[1] = dir.y();
    fData->direction[2] = dir.z();
  }
  void SetMomentum(G4ThreeVector mom) {
    fData->momentum[0] = mom.x();
    fData->momentum[1] = mom.y();
    fData->momentum[2] = mom.z();
  }
  void SetKineticEnergy(G4double energy) {
    fData->kinetic_energy = energy/CLHEP::MeV; }
  void SetTotalLength(G4double length) {
    fData->total_length = length/CLHEP::mm; }
  void SetParticleID(G4int id) { fData->particle_id = id; }
  void SetTrackID(G4int id) { fData->track_id = id; }
  void SetParentID(G4int id) { fData->parent_id = id; }
  void SetStepNumber(G4int number) { fData->step_number = number; }
  void SetTrackStatus(G4int status) { fData->track_status = status; }
  void SetVertexPosition(G4ThreeVector pos) {
    fData->vertex_position[0] = pos.x();
    fData->vertex_position[1] = pos.y();
    fData->vertex_position[2] = pos.z();
  }
  void SetVertexDirection(G4ThreeVector dir) {
    fData->vertex_direction[0] = dir.x();
    fData->vertex_direction[1] = dir.y();
    fData->vertex_direction[2] = dir.z();
  }
  void SetVertexKineticEnergy(G4double energy) {
    fData->vertex_kinetic_energy = energy/CLHEP::MeV; }
  void SetCreationProcess(std::string process) {
    fData->creation_process = process; }
private:
  ComptonG4Data *fData;
};

#endif /* COMPTONG4HIT_HH */
