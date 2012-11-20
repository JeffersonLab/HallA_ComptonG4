#ifndef COMPTONG4RUNTACTION_H
#define COMPTONGRUNTACTION_H

// GEANT4 includes
#include <G4UserRunAction.hh>

// Pre-defined classes
class ComptonG4Analysis;

class ComptonG4RunAction : public G4UserRunAction {
public:
  ComptonG4RunAction(ComptonG4Analysis *analysis);
  ~ComptonG4RunAction();

  // G4UserRunAction related classes


private:
  ComptonG4Analysis   *fAnalysis;    // Pointer to the Data IO instance
};

#endif
