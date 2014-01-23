/*
 * ComptonG4SensitiveDetector.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>

ComptonG4SensitiveDetector::ComptonG4SensitiveDetector(
    G4String name, ComptonG4Analysis *analysis) :
  G4VSensitiveDetector(name),
  fAnalysis(analysis)
{

}

ComptonG4SensitiveDetector::~ComptonG4SensitiveDetector()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4SensitiveDetector::Initialize(G4HCofThisEvent *)
{
  fHits.clear();
  fVolumeNames.clear();
  fVolumeIndices.clear();
}

G4bool ComptonG4SensitiveDetector::ProcessHits(G4Step* aStep,
    G4TouchableHistory*)
{
  G4Track *aTrack = aStep->GetTrack();
  if( !aTrack ) return false;

  G4String str = aTrack->GetDefinition()->GetParticleName();

  if( aTrack->GetDefinition() == G4OpticalPhoton::Definition() ) {
    G4String vol = aTrack->GetVolume()->GetName();
    if(fVolumeIndices.find(vol) == fVolumeIndices.end()) {
      // New physical volume in our list
      fVolumeIndices[vol] = fHits.size();
      fHits.push_back(1);
      fVolumeNames.push_back(vol);
    } else {
      fHits[fVolumeIndices[vol]]++;
    }
  }
  return true;
}

void ComptonG4SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  G4cout << "Events in " << GetName() << ":" << G4endl;
  for(int i = 0; i < fVolumeNames.size(); i++ ) {
    G4cout << "\tVol: " << fVolumeNames[i] << " has " << fHits[i]
      << " hits." << G4endl;
  }
}


