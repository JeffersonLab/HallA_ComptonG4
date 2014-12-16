/*
 * ComptonG4PMTCathode.hh
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PMTCATHODE_HH_
#define COMPTONG4PMTCATHODE_HH_

#include <VComptonG4SensitiveDetector.hh>
#include <ComptonG4OpticalHit.hh>
#include <map>
#include <vector>

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class ComptonG4PMTCathode: public VComptonG4SensitiveDetector {
public:
  ComptonG4PMTCathode(G4String name);
  virtual ~ComptonG4PMTCathode();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void CleanEvent();

  /*
   * Set sensitive detector options
   *
   * \param options The options to parse, separated by a semicolon
   */
  virtual void SetOptions(std::map<G4String,
      G4String> options, bool ignore_unknown);


  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch);

  /*
   * Add a managed volume to this SD
   */
  virtual void AddVolume(G4VPhysicalVolume* vol) {
    VComptonG4SensitiveDetector::AddVolume(vol);
    std::vector<ComptonG4OpticalHit> hit_optical;
    fOpticalHits.push_back(hit_optical);
    std::vector<ComptonG4OpticalData> data_optical;
    fOpticalData.push_back(data_optical);
    fOpticalDataPtr.push_back(&fOpticalData.back());
    fTotalOpticalPhotons.push_back(0);
  }

private:
  std::vector<std::vector<ComptonG4OpticalHit> > fOpticalHits;
  std::vector<std::vector<ComptonG4OpticalData> > fOpticalData;
  std::vector<std::vector<ComptonG4OpticalData>* > fOpticalDataPtr;
  std::vector<G4int> fTotalOpticalPhotons;
};

#endif /* COMPTONG4PMTCATHODE_HH_ */
