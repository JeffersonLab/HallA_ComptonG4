#ifndef COMPTONG4STEPPINGACTION_H
#define COMPTONG4STEPPINGACTION_H

#include <G4UserSteppingAction.hh>
#include <G4Types.hh>
#include "ComptonG4PrimaryHit.hh"

class ComptonG4Analysis;
class ComptonG4SteppingMessenger;
class TTree;

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
  void SetOpticalMaxStepTime(G4double time) {
    fOpticalMaxStepTime = time;
  }
  void SetVerbose(G4int verbose) {
    fVerbose = verbose;
  }

  void ClearEvent();

  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch);


private:
  ComptonG4Analysis *fAnalysis;
  ComptonG4SteppingMessenger *fStepMessenger;
  G4double fOpticalMaxStepTime;
  G4int fVerbose;
  std::vector<ComptonG4PrimaryHit> fPrimaryHits;
  std::vector<ComptonG4PrimaryData>* fPrimaryDataPtr;
  std::vector<ComptonG4Hit> fStoppedOpticalHits;
  std::vector<ComptonG4Data>* fStoppedOpticalDataPtr;

  bool fStoreFullOpticalStopped;
  G4int fStoppedOpticalCerenkov;
  G4int fStoppedOpticalScintillation;
  G4int fStoppedOpticalOther;
  G4int fStoppedOpticalTotal;
};

#endif
