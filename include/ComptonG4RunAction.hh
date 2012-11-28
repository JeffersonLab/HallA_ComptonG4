#ifndef COMPTONG4RUNTACTION_H
#define COMPTONGRUNTACTION_H

// GEANT4 includes
#include <G4UserRunAction.hh>

// Pre-defined classes
class ComptonG4Analysis;
class ComptonG4RunMessenger;

/*!
 * @class ComptonG4RunAction
 *
 * @brief Compton simulator run action
 *
 * Class that takes care of a compton "run"
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4RunAction : public G4UserRunAction {
public:
  ComptonG4RunAction(ComptonG4Analysis *analysis);
  ~ComptonG4RunAction();

  void SetRunNumber(int run) { fRunNumber = run; }

  // G4UserRunAction related classes
  void BeginOfRunAction(const G4Run* run);
  void EndOfRunAction(const G4Run* run);

private:
  ComptonG4Analysis     *fAnalysis;   // Pointer to the Data IO instance
  ComptonG4RunMessenger *fMessenger;  // Pointer to the run messenger

  int fRunNumber;    // The run number specified by user
};

#endif
