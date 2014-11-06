///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4RunAction.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4RunMessenger.hh"

ComptonG4RunAction::ComptonG4RunAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis), fAutoSaveEntry(0), fRunNumber(0)
{
  fMessenger = new ComptonG4RunMessenger(this);
}

ComptonG4RunAction::~ComptonG4RunAction()
{
  delete fAnalysis;
}

void ComptonG4RunAction::BeginOfRunAction(const G4Run*)
{
  fAnalysis->Initialize(fRunNumber,fAutoSaveEntry);
}

void ComptonG4RunAction::EndOfRunAction(const G4Run*)
{
  fAnalysis->Finished();
}
