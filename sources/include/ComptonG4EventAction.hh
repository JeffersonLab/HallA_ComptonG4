#ifndef COMPTONG4EVENTACTION_H
#define COMPTONG4EVENTACTION_H

// GEANT4 includes
#include <G4UserEventAction.hh>
#include <G4Types.hh>

// Pre-defined classes
class ComptonG4Analysis;
class ComptonG4EventMessenger;
class G4Timer;

/*!
 * @class ComptonG4EventAction
 *
 * @brief Manages each event
 *
 * This manages each event in the simulation
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4EventAction : public G4UserEventAction {
public:
  ComptonG4EventAction(ComptonG4Analysis *analysis);
  ~ComptonG4EventAction();

  // G4UserEventAction related classes
  void BeginOfEventAction(const G4Event* evt);
  void EndOfEventAction(const G4Event* evt);
  void SetEventPrintNumber(const unsigned int evt) {
    fEventPrintNumber = evt;
  }


private:
  ComptonG4Analysis   *fAnalysis;    // Pointer to the Data IO instance
  unsigned int fEventCounter;
  G4Timer *fEventTimer;
  G4double fRealElapsedTime;
  G4double fSystemElapsedTime;
  G4double fUserElapsedTime;
  unsigned int fEventPrintNumber;
  ComptonG4EventMessenger *fEventMessenger;
};

#endif
