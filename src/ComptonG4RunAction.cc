///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4RunAction.hh"
#include "ComptonG4Analysis.hh"

ComptonG4RunAction::ComptonG4RunAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis)
{
}

ComptonG4RunAction::~ComptonG4RunAction()
{
  delete fAnalysis;
}

void ComptonG4RunAction::BeginOfRunAction(const G4Run* run)
{
  fAnalysis->Initialize();
}

void ComptonG4RunAction::EndOfRunAction(const G4Run* run)
{
  fAnalysis->Finished();
}
