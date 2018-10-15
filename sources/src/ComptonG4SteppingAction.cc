///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4SteppingAction.hh"
#include "ComptonG4SteppingMessenger.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4Utils.hh"

// GEANT4 Includes
#include <G4VPhysicalVolume.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>
#include <G4OpticalPhoton.hh>

// ROOT includes
#include <TTree.h>

ComptonG4SteppingAction::ComptonG4SteppingAction(ComptonG4Analysis *analysis) :
fAnalysis(analysis),fOpticalMaxStepTime(1.0e-6*CLHEP::s),fVerbose(0),
  fStoreFullOpticalStopped(false),fStoppedOpticalCerenkov(0),
  fStoppedOpticalScintillation(0),fStoppedOpticalOther(0),
  fStoppedOpticalTotal(0)
{
  // Initialize the stored data pointer
  fPrimaryDataPtr = new std::vector<ComptonG4PrimaryData>;
  fStoppedOpticalDataPtr = new std::vector<ComptonG4Data>;

  // Tell the analyzer about us
  analysis->SetSteppingAction(this);

  // Messenger class
  fStepMessenger = new ComptonG4SteppingMessenger(this);
}

ComptonG4SteppingAction::~ComptonG4SteppingAction()
{
}

void ComptonG4SteppingAction::UserSteppingAction(const G4Step* step)
{

  // G4cout<<"stp "<<step->GetStepLength()<<"\t"<<G4endl;
  // G4cout<<"\t"<<step->GetPreStepPoint()->GetPosition()<<"\t>>>\t"<<step->GetPostStepPoint()->GetPosition()<<G4endl;
  // G4cout<<"\t"<<step->GetPreStepPoint()->GetTouchableHandle()->GetVolume(0)->GetName()<<"\t>>>\t"<<step->GetPostStepPoint()->GetTouchableHandle()->GetVolume(0)->GetName()<<G4endl;
  // G4cout<<"stp "<<step->GetPreStepPoint()->GetMomentum()<<"\t>>>\t"<<step->GetPostStepPoint()->GetMomentum()<<G4endl;
  G4Track *track = step->GetTrack();
  if( !track )
    return;

  // G4cout<<__PRETTY_FUNCTION__<<G4endl
  // 	<<"\t"<<track->GetPosition()<<"\t"<<track->GetTrackID()<<"\t"<<track->GetMomentum()<<G4endl;
  // std::cin.ignore();
  // We want to track the primary particle all the way through
  if( track->GetParentID() == 0 ) {
    ComptonG4PrimaryHit hit;
    hit.ProcessStep(step);
    hit.SetEnergyDeposited(step->GetTotalEnergyDeposit());
    fPrimaryHits.push_back(hit);
    fPrimaryDataPtr->push_back(hit.GetData());
  }

  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    if ( track->GetGlobalTime() > fOpticalMaxStepTime ) {
      if(fStoreFullOpticalStopped) {
        ComptonG4Hit hit;
        hit.ProcessTrack(track);
        fStoppedOpticalHits.push_back(hit);
        fStoppedOpticalDataPtr->push_back(hit.GetData());
      }
      const G4VProcess *process = track->GetCreatorProcess();
      if(process) {
        if(ComptonG4Utils::SameIgnore("Cerenkov",process->GetProcessName())) {
          fStoppedOpticalCerenkov++;
        } else {
          fStoppedOpticalScintillation++;
        }
      } else {
      fStoppedOpticalOther++;
      }
      fStoppedOpticalTotal++;
      track->SetTrackStatus(fStopAndKill);
      //fAnalysis->StoppedOpticalPhoton();
    }
  }
}

/*
 *
 */
void ComptonG4SteppingAction::CreateTreeBranch(TTree* tree)
{
  // Create detector branches
  tree->Branch("Primary",&(fPrimaryDataPtr));
  if(fStoreFullOpticalStopped)
    tree->Branch("OpticalPhotons_stopped",&(fStoppedOpticalDataPtr));

  tree->Branch("nOpticalStopped_cerenkov",&fStoppedOpticalCerenkov);
  tree->Branch("nOpticalStopped_scintillation",&fStoppedOpticalScintillation);
  tree->Branch("nOpticalStopped_other",&fStoppedOpticalOther);
  tree->Branch("nOpticalStopped_total",&fStoppedOpticalTotal);
}


void ComptonG4SteppingAction::ClearEvent()
{
  fPrimaryHits.clear();
  fPrimaryDataPtr->clear();
  fStoppedOpticalHits.clear();
  fStoppedOpticalDataPtr->clear();

  // Also clear the stopped optical photon count
  fStoppedOpticalCerenkov = 0;
  fStoppedOpticalScintillation = 0;
  fStoppedOpticalOther = 0;
  fStoppedOpticalTotal = 0;
}
