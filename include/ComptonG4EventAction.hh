#ifndef COMPTONG4EVENTACTION_H
#define COMPTONG4EVENTACTION_H

// GEANT4 includes
#include <G4UserEventAction.hh>

// Pre-defined classes
class ComptonG4Analysis;

class ComptonG4EventAction : public G4UserEventAction {
public:
  ComptonG4EventAction(ComptonG4Analysis *analysis);
  ~ComptonG4EventAction();

  // G4UserEventAction related classes
  void BeginOfEventAction(const G4Event* evt);
  void EndOfEventAction(const G4Event* evt);


private:
  ComptonG4Analysis   *fAnalysis;    // Pointer to the Data IO instance
};

#endif
