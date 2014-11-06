/*
 * ComptonG4SensitiveDetector.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>

// Set it to nothing for now
ComptonG4Analysis *ComptonG4SensitiveDetector::fAnalysis = 0;

ComptonG4SensitiveDetector::ComptonG4SensitiveDetector(
    G4String name) :
  G4VSensitiveDetector(name)
{
}


ComptonG4SensitiveDetector::~ComptonG4SensitiveDetector()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4SensitiveDetector::Initialize(G4HCofThisEvent *)
{
  CleanEvent();
}

G4bool ComptonG4SensitiveDetector::ProcessHits(G4Step* step,
    G4TouchableHistory*)
{
  G4Track *track = step->GetTrack();
  if( !track ) return false;

  G4VPhysicalVolume *vol = track->GetVolume();
  int volIndex  = GetIndex(vol);

  // Sanity check, ensure volumes were properly registered
  if(volIndex < 0 ) {
    return false;
  }

  // Treat special for optical photons
  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    // Only increment photon count if the particle is stopped here
    // (meaning, it likely got absorbed)
    G4TrackStatus tStatus = track->GetTrackStatus();
    if(tStatus == fStopButAlive ||
        tStatus == fStopAndKill ) {
      fOpticalHits[volIndex] += 1.0;
      //G4cout << "Creator process: " << track->GetCreatorProcess()->GetProcessName() << G4endl;
      fGlobalTimes[volIndex].push_back(track->GetGlobalTime());
      fAnalysis->OpticalHit();
    }
    fEDeps[volIndex] += step->GetTotalEnergyDeposit()/MeV;

    G4int id = track->GetTrackID();
    Bool_t found = false;
    for(UInt_t i = 0; i < fTrackIDs.size() && !found; i++ ) {
      if(id==fTrackIDs[i]) {
        found = true;
      }
    }

    if(!found) {
      fAnalysis->NewOpticalPhoton();
      fTrackIDs.push_back(id);
    }

    // Kill photons that travel too far
    // TODO: Make this be UI command
    if ( track->GetGlobalTime() > 1e-6*CLHEP::s) {
      track->SetTrackStatus(fStopAndKill);
      fAnalysis->StoppedOpticalPhoton();
    }

  }

  fEDeps[volIndex] += step->GetTotalEnergyDeposit()/MeV;
  return true;
}

void ComptonG4SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  // Send info to Analysis instance
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fAnalysis->SetEDep(fVolumes[i]->GetName(),fEDeps[i]);
    fAnalysis->SetOpticalHits(fVolumes[i]->GetName(),fOpticalHits[i]);
    fAnalysis->SetGlobalTimes(fVolumes[i]->GetName(),fGlobalTimes[i]);
  }
  CleanEvent();
}

void ComptonG4SensitiveDetector::CleanEvent()
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fOpticalHits[i] = 0.0;
    fEDeps[i] = 0.0;
    fGlobalTimes[i].clear();
  }
  fTrackIDs.clear();
}

int ComptonG4SensitiveDetector::GetIndex(G4VPhysicalVolume *vol)
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    if(vol==fVolumes[i])
      return i;
  }

  return -1;
}
