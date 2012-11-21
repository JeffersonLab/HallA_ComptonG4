///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4EventAction.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4Event.hh>

ComptonG4EventAction::ComptonG4EventAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis)
{

}

ComptonG4EventAction::~ComptonG4EventAction()
{
}

void ComptonG4EventAction::BeginOfEventAction(const G4Event* evt)
{
}

void ComptonG4EventAction::EndOfEventAction(const G4Event* evt)
{
}
