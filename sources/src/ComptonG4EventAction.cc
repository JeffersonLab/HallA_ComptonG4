///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4EventAction.hh"
#include "ComptonG4EventMessenger.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4Event.hh>
#include <G4Timer.hh>
#include <Randomize.hh>

ComptonG4EventAction::ComptonG4EventAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis), fEventCounter(0),fEventTimer(new G4Timer),
  fRealElapsedTime(0),fSystemElapsedTime(0),fUserElapsedTime(0),
  fEventPrintNumber(1000)
{
  // Messenger class
  fEventMessenger = new ComptonG4EventMessenger(this);

}

ComptonG4EventAction::~ComptonG4EventAction()
{
  if(fEventMessenger)
    delete fEventMessenger;
}

void ComptonG4EventAction::BeginOfEventAction(const G4Event*)
{
  fAnalysis->StartOfEvent();
  if(fEventCounter%fEventPrintNumber==0) {
    if(fEventCounter==0) {
      fEventTimer->Start();
    } else {
      fEventTimer->Stop();
      G4double real = fEventTimer->GetRealElapsed();
      G4double system = fEventTimer->GetSystemElapsed();
      G4double user = fEventTimer->GetUserElapsed();
      fRealElapsedTime += real;
      fSystemElapsedTime += system;
      fUserElapsedTime += user;
      G4cout << "Processing event: " << fEventCounter
        << " . Times for " << fEventPrintNumber << "  events "
        << "(real,system,user): (" << real << ", " << system << ", "
        << user << ")  Times per single event: (" << real/double(fEventPrintNumber)<< ", "
        << system/double(fEventPrintNumber) << ", "
        << user/double(fEventPrintNumber) << ")"
        << " total: (" << fRealElapsedTime << ", " << fSystemElapsedTime
        << ", " << fUserElapsedTime << ")"
        << G4endl;
      fEventTimer->Start();
    }
  }
  fEventCounter++;
}

void ComptonG4EventAction::EndOfEventAction(const G4Event* evt)
{
  // Store the random seed
  fAnalysis->StoreRandomSeed(evt->GetRandomNumberStatus());
  fAnalysis->EndOfEvent();
}
