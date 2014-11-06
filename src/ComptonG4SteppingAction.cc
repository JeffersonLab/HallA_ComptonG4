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

void ComptonG4SteppingAction::UserSteppingAction(const G4Step*)
{
}
