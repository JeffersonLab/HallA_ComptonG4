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
#include <TTree.h>

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
      fTotalOpticalPhotons[volIndex]++;
      //G4cout << "Creator process: " << track->GetCreatorProcess()->GetProcessName() << G4endl;
      ComptonG4OpticalHit hit;
      hit.ProcessStep(step);
      fOpticalHits[volIndex].push_back(hit);
      fOpticalData[volIndex].push_back(hit.GetData());
      fAnalysis->OpticalHit();
    }
    fAnalysis->ProcessOpticalTrackID(track->GetTrackID());
  }
  ComptonG4EDepHit hit;
  hit.ProcessStep(step);
  fEDepHits[volIndex].push_back(hit);
  fEDepData[volIndex].push_back(hit.GetData());
  fTotalEnergyDeposited[volIndex] += step->GetTotalEnergyDeposit()/MeV;
  return true;
}

void ComptonG4Crystal::EndOfEvent(G4HCofThisEvent*)
{
  // Ensure the pointers point to the correct data vectors
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fEDepDataPtr[i] = &fEDepData[i];
    fOpticalDataPtr[i] = &fOpticalData[i];
  }
}

void ComptonG4Crystal::CleanEvent()
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fTotalEnergyDeposited[i] = 0.0;
    fTotalOpticalPhotons[i] = 0.0;
    fEDepHits[i].clear();
    fOpticalHits[i].clear();
    fEDepData[i].clear();
    fOpticalData[i].clear();
  }
}


void ComptonG4Crystal::CreateTreeBranch(TTree* tree)
{
  // Create detector branches
  for(size_t i = 0; i < fVolumes.size(); i++ ) {
    tree->Branch(Form("%s_eDep",fVolumes[i]->GetName().c_str()),
        &(fTotalEnergyDeposited[i]));

    tree->Branch(Form("%s_num_photons",fVolumes[i]->GetName().c_str()),
        &(fTotalOpticalPhotons[i]));

    tree->Branch(Form("%s_eDep_hits",fVolumes[i]->GetName().c_str()),
          &(fEDepData[i]));

    tree->Branch(Form("%s_optical_hits",fVolumes[i]->GetName().c_str()),
        &(fOpticalDataPtr[i]));
  }
}

// Finally register the class
COMPTONG4_SD_REGISTER(ComptonG4Crystal)
