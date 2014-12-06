/*
 * ComptonG4Crystal.cc
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4Crystal.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>

ComptonG4Crystal::ComptonG4Crystal(
    G4String name) :
  VComptonG4SensitiveDetector(name)
{
}


ComptonG4Crystal::~ComptonG4Crystal()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4Crystal::Initialize(G4HCofThisEvent *)
{
  for(UInt_t i = 0; i < fVolumes.size(); i++ ) {
    fOpticalHits.push_back(0.0);
    fEDeps.push_back(0.0);
  }
  fGlobalTimes.resize(fVolumes.size());
  CleanEvent();
}

G4bool ComptonG4Crystal::ProcessHits(G4Step* step,
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

    fAnalysis->ProcessOpticalTrackID(track->GetTrackID());
  }

  fEDeps[volIndex] += step->GetTotalEnergyDeposit()/MeV;
  return true;
}

void ComptonG4Crystal::EndOfEvent(G4HCofThisEvent*)
{
  // Send info to Analysis instance
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fAnalysis->SetEDep(fVolumes[i]->GetName(),fEDeps[i]);
    fAnalysis->SetOpticalHits(fVolumes[i]->GetName(),fOpticalHits[i]);
    fAnalysis->SetGlobalTimes(fVolumes[i]->GetName(),fGlobalTimes[i]);
  }
  CleanEvent();
}

void ComptonG4Crystal::CleanEvent()
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fOpticalHits[i] = 0.0;
    fEDeps[i] = 0.0;
    fGlobalTimes[i].clear();
  }
}

// Finally register the class
COMPTONG4_SD_REGISTER(ComptonG4Crystal)
