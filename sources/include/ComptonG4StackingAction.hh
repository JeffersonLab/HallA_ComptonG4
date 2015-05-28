#ifndef _COMPTONG4StackingAction_H
#define _COMPTONG4StackingAction_H

#include "G4UserStackingAction.hh"
#include "globals.hh"

class ComptonG4OpticalTracker;

/*!
 * @class ComptonG4StackingAction
 *
 * @brief Default class to process the simulation data
 *
 * This class processes the simulation data, including writing to a ROOT file
 * when the event is finished.
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4StackingAction : public G4UserStackingAction {
public:
  ComptonG4StackingAction(ComptonG4OpticalTracker *optical_tracker = 0);
  virtual ~ComptonG4StackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);
  void SetAnalysis(ComptonG4OpticalTracker *optical_tracker) {
    fOpticalTracker = optical_tracker;
  }
private:
  G4double fOpticalPhotonMinEnergy;
  G4double fOpticalPhotonMaxEnergy;
  ComptonG4OpticalTracker *fOpticalTracker;
};

#endif
