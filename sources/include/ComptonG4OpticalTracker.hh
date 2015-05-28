/*
 * ComptonG4OpticalTracker.hh
 *
 *  Created on: May 27, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4OPTICALTRACKER_HH_
#define COMPTONG4OPTICALTRACKER_HH_

#include <ComptonG4OpticalTrackerHit.hh>
#include <map>
#include <vector>

class TTree;
class G4Track;

class ComptonG4OpticalTracker {
public:
  ComptonG4OpticalTracker( G4bool enable = false);
  virtual ~ComptonG4OpticalTracker();

  /*
   * Process a new track
   */
  void ProcessNewTrack(const G4Track *track);

  /*
   * Process stopped track
   */
  void ProcessTrack(const G4Track *track, G4bool detected);

  /*
   * Initialize tracker for new event
   */
  void StartOfEvent() {
    fOpticalTracks.ClearAndInit();
  }

  /*
   * Fill event info into tree
   */
  void EndOfEvent() {
    fOpticalTracks.FillTree();
  }

  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch);

private:
  G4bool fEnabled;  // Should we track and store this info?
  ComptonG4OpticalTrackerHitsCollection fOpticalTracks;
};

#endif /* COMPTONG4OPTICALTRACKER_HH_ */
