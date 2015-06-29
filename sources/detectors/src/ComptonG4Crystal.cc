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

#include "ComptonG4Utils.hh"

ComptonG4Crystal::ComptonG4Crystal(
    G4String name) :
  VComptonG4SensitiveDetector(name), fStoreEDepHits(false),
  fStoreOpticalHits(false)
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

      // Only store optical hits if asked to
      if(fStoreOpticalHits) {
        ComptonG4OpticalHit hit;
        hit.ProcessStep(step);
        fOpticalHits[volIndex].push_back(hit);
        fOpticalData[volIndex].push_back(hit.GetData());
      }
      fAnalysis->OpticalHit();
      ToOpticalTracker(track,false);
    }
    fAnalysis->ProcessOpticalTrackID(track->GetTrackID());
    CheckUniqueTrack(track);
  }
  if(fStoreEDepHits) {
    ComptonG4EDepHit hit;
    hit.ProcessStep(step);
    fEDepHits[volIndex].push_back(hit);
    fEDepData[volIndex].push_back(hit.GetData());
  }
  fTotalEnergyDeposited[volIndex] += step->GetTotalEnergyDeposit()/CLHEP::MeV;
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

  // Summary variables
  fTotalOpticalCerenkov = 0;
  fTotalOpticalScintillation = 0;
  fTotalOpticalOther = 0;
  fOpticalProducedTrackID.clear();
  fOpticalProducedProcess.clear();
}


void ComptonG4Crystal::CreateTreeBranch(TTree* tree)
{
  // Create detector branches
  for(size_t i = 0; i < fVolumes.size(); i++ ) {
    tree->Branch(Form("%s_eDep",fVolumes[i]->GetName().c_str()),
        &(fTotalEnergyDeposited[i]));

    tree->Branch(Form("%s_num_photons",fVolumes[i]->GetName().c_str()),
        &(fTotalOpticalPhotons[i]));

    if(fStoreEDepHits)
      tree->Branch(Form("%s_eDep_hits",fVolumes[i]->GetName().c_str()),
          &(fEDepData[i]));

    if(fStoreOpticalHits)
      tree->Branch(Form("%s_optical_hits",fVolumes[i]->GetName().c_str()),
          &(fOpticalDataPtr[i]));

    tree->Branch("Crystals_produced_optical_cerenkov",
        &fTotalOpticalCerenkov );
    tree->Branch("Crystals_produced_optical_scintillation",
        &fTotalOpticalScintillation);
    tree->Branch("Crystals_produced_optical_other",
        &fTotalOpticalOther);
  }
}

void ComptonG4Crystal::SetOptions(std::map<G4String,G4String> options,
    bool ignore_unknown)
{
  std::map<G4String,G4String>::iterator it;
  for(it = options.begin(); it != options.end(); it++ ) {
    if( ProcessOptionBool(it->first,
          it->second,"store_edep_hits",fStoreEDepHits) ) {
    } else if ( ProcessOptionBool(it->first,
          it->second,"store_optical_hits",fStoreOpticalHits) ) {
    } else if (ignore_unknown) { // Okay, fine, ignore it
    } else { // Uknown option passed, complain!
      UnknownOption(it->first,it->second);
    }
  }
}


void ComptonG4Crystal::CheckUniqueTrack(G4Track *track)
{
  if(!track->GetCreatorProcess())
    return;

  G4String process = track->GetCreatorProcess()->GetProcessName();

  G4int id = track->GetTrackID();
  G4int processInt = -1;
  if( ComptonG4Utils::SameIgnore("Cerenkov",process) ) {
    processInt = 0;
  } else if ( ComptonG4Utils::SameIgnore("Scintillation",process) ) {
    processInt = 1;
  }
  for(size_t i = 0; i < fOpticalProducedTrackID.size(); i++ ) {
    if( id == fOpticalProducedTrackID[i]
        && fOpticalProducedProcess[i] == processInt )
      return;
  }

  // Alright, new track it seems
  fOpticalProducedTrackID.push_back(id);
  fOpticalProducedProcess.push_back(processInt);
  switch(processInt) {
    case 0:
      fTotalOpticalCerenkov++;
      break;
    case 1:
      fTotalOpticalScintillation++;
      break;
    default:
      fTotalOpticalOther++;
      break;
  }
}

// Finally register the class
COMPTONG4_SD_REGISTER(ComptonG4Crystal)
