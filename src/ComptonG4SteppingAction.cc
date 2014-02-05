///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4SteppingAction.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4VPhysicalVolume.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

ComptonG4SteppingAction::ComptonG4SteppingAction(ComptonG4Analysis *analysis) :
fAnalysis(analysis)
{

}

ComptonG4SteppingAction::~ComptonG4SteppingAction()
{
}

void ComptonG4SteppingAction::UserSteppingAction(const G4Step* step)
{
  const G4Track* track = step->GetTrack();
  G4VPhysicalVolume* volume = track->GetVolume();
  G4ThreeVector pos(track->GetPosition());
  /* G4cout << "@" << volume->GetName() << " \tcharge: " << track->GetDefinition()->GetPDGCharge()
      << " \tParentID: " << track->GetParentID() << " \tThisID: " << track->GetTrackID() << " \tEnergy: "
      << step->GetTotalEnergyDeposit()/MeV
      << " \tPre Step Status: " << step->GetPreStepPoint()->GetStepStatus()
      << " \tPre Step Energy: " << step->GetPreStepPoint()->GetTotalEnergy()/MeV
      << " \tPost Step Status: " << step->GetPostStepPoint()->GetStepStatus()
      << " \tPost Step Energy: " << step->GetPostStepPoint()->GetTotalEnergy()/MeV
      << G4endl;*/
  // TODO: The detector should not be hard coded!! Use the Active Detectors
  // command to specify which detector to use
  /*if (//track->GetDefinition()->GetPDGCharge() != 0. &&
      !strcmp(volume->GetName(),"LeadTungstateCrystal_Logical_PV")) {
    if(track->GetDefinition()->GetPDGCharge() != 0 ) {
      fAnalysis->AddEDep(step->GetTotalEnergyDeposit()/MeV);
      fAnalysis->AddStepSize(step->GetStepLength()/mm);
    }
    fAnalysis->AddEDepAllCharges(step->GetTotalEnergyDeposit()/MeV);
    fAnalysis->AddStepSizeAllCharges(step->GetStepLength()/mm);

  }// else {
  //  std::cout << "At volume: " << volume->GetName() << " \tEDep: "
  //      << step->GetTotalEnergyDeposit()/MeV << "\n";
  //}*/
}
