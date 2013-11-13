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
  fAnalysis(analysis),fHits(0)
{

}

ComptonG4SensitiveDetector::~ComptonG4SensitiveDetector()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4SensitiveDetector::Initialize(G4HCofThisEvent *)
{
  fHits = 0;
}

G4bool ComptonG4SensitiveDetector::ProcessHits(G4Step* aStep,
    G4TouchableHistory*)
{
  G4Track *aTrack = aStep->GetTrack();
  if( !aTrack ) return false;

  G4String str = aTrack->GetDefinition()->GetParticleName();

  if( aTrack->GetDefinition() == G4OpticalPhoton::Definition() ) {
    fHits++;
  }
  return true;
}

void ComptonG4SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
/*  G4cout << "End of event, found " << fHits << " hits in "
   << GetName() << G4endl;
   */
}


