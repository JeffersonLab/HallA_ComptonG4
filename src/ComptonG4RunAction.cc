///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4RunAction.hh"
#include "ComptonG4Analysis.hh"

ComptonG4RunAction::ComptonG4RunAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis)
{
  fAnalysis->Initialize();
}

ComptonG4RunAction::~ComptonG4RunAction()
{
  delete fAnalysis;
}
