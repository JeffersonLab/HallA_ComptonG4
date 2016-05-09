/*
 * ComptonG4BeamPolarizer.hh
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4BEAMPOLARIZER_HH_
#define COMPTONG4BEAMPOLARIZER_HH_

#include <VComptonG4SensitiveDetector.hh>
#include <ComptonG4BeamHit.hh>
#include <map>
#include <vector>

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4Track;
class G4TouchableHistory;

class ComptonG4BeamPolarizer: public VComptonG4SensitiveDetector {
public:
  ComptonG4BeamPolarizer(G4String name);
  virtual ~ComptonG4BeamPolarizer();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void CleanEvent();

  /*
   * Set sensitive detector options
   *
   * \param options The options to parse, separated by a semicolon
   */
  virtual void SetOptions(std::map<G4String,G4String> options,
      bool ignore_unknown);


  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch);

  /*
   * Add a managed volume to this SD
   */
  virtual void AddVolume(G4VPhysicalVolume* vol) {
    VComptonG4SensitiveDetector::AddVolume(vol);
    fBeamPolarizers.push_back(new ComptonG4BeamHitsCollection(vol->GetName()));
  }

private:
  std::vector<ComptonG4BeamHitsCollection*> fBeamPolarizers;
};

#endif /* COMPTONG4BEAMPOLARIZER_HH_ */
