/*
 * ComptonG4SensitiveDetector.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>

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

//  fAnalysis->AddEDep(track->GetVolume()->GetName(),
//      step->GetTotalEnergyDeposit()/MeV);

  G4String vol = track->GetVolume()->GetName();
  int volIndex  = 0;
  if(fVolumeIndices.find(vol) == fVolumeIndices.end()) {
    // New physical volume in our list
    fVolumeIndices[vol] = volIndex = fOpticalHits.size();
    fVolumeNames.push_back(vol);
    fOpticalHits.push_back(0);
    fEDeps.push_back(0.0);
  } else {
    volIndex = fVolumeIndices[vol];
  }

  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
      fOpticalHits[volIndex] += 1.0;
  } else {
    fEDeps[fVolumeIndices[volIndex]] += step->GetTotalEnergyDeposit()/MeV;
  }
  return true;
}

void ComptonG4SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  //G4cout << "Events in " << GetName() << ":" << G4endl;
  for(unsigned int i = 0; i < fVolumeNames.size(); i++ ) {
//    G4cout << "\tVol: " << fVolumeNames[i] << " has " << fOpticalHits[i]
//      << " hits." << G4endl;
    fAnalysis->AddEDep(fVolumeNames[i],fEDeps[i]/MeV);
    fAnalysis->AddOpticalHits(fVolumeNames[i],fOpticalHits[i]);
  }
  CleanEvent();
}

void ComptonG4SensitiveDetector::CleanEvent()
{
  fOpticalHits.clear();
  fEDeps.clear();
  fVolumeNames.clear();
  fVolumeIndices.clear();
}
