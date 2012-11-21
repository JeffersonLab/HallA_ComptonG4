#ifndef COMPTONG4STEPPINGACTION_H
#define COMPTONG4STEPPINGACTION_H

#include <G4UserSteppingAction.hh>

class ComptonG4Analysis;

/*!
 * @class ComptonG4SteppingAction
 *
 * @brief Compton Stepping Action Class
 *
 * Handles the simulation on a step-by-step way
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4SteppingAction : public G4UserSteppingAction {
public:
  ComptonG4SteppingAction(ComptonG4Analysis *analysis);
  ~ComptonG4SteppingAction();

  // G4UserSteppingAction related functions
  void UserSteppingAction(const G4Step* step);

private:
  ComptonG4Analysis *fAnalysis;
};

#endif
