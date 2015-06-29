/*
 * ComptonG4Crystal.hh
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4CRYSTAL_HH_
#define COMPTONG4CRYSTAL_HH_

#include <VComptonG4SensitiveDetector.hh>
#include <ComptonG4EDepHit.hh>
#include <ComptonG4OpticalHit.hh>
#include <map>
#include <vector>

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4Track;
class G4TouchableHistory;

class ComptonG4Crystal: public VComptonG4SensitiveDetector {
public:
  ComptonG4Crystal(G4String name);
  virtual ~ComptonG4Crystal();

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
    std::vector<ComptonG4EDepHit> hit_edep;
    fEDepHits.push_back(hit_edep);
    std::vector<ComptonG4OpticalHit> hit_optical;
    fOpticalHits.push_back(hit_optical);
    std::vector<ComptonG4EDepData> data_edep;
    fEDepData.push_back(data_edep);
    fEDepDataPtr.push_back(&fEDepData.back());
    std::vector<ComptonG4OpticalData> data_optical;
    fOpticalData.push_back(data_optical);
    fOpticalDataPtr.push_back(&fOpticalData.back());
    fTotalEnergyDeposited.push_back(0.0);
    fTotalOpticalPhotons.push_back(0);
  }

  virtual void CheckUniqueTrack(G4Track *track);

private:
  std::vector<std::vector<ComptonG4EDepHit> > fEDepHits;
  std::vector<std::vector<ComptonG4OpticalHit> > fOpticalHits;
  std::vector<std::vector<ComptonG4EDepData> > fEDepData;
  std::vector<std::vector<ComptonG4OpticalData> > fOpticalData;
  std::vector<std::vector<ComptonG4EDepData>* > fEDepDataPtr;
  std::vector<std::vector<ComptonG4OpticalData>* > fOpticalDataPtr;
  std::vector<G4double> fTotalEnergyDeposited;
  std::vector<G4int> fTotalOpticalPhotons;
  G4int fTotalOpticalCerenkov;
  G4int fTotalOpticalScintillation;
  G4int fTotalOpticalOther;
  std::vector<G4int> fOpticalProducedTrackID;
  std::vector<G4int> fOpticalProducedProcess;
  bool fStoreEDepHits;
  bool fStoreOpticalHits;
};

#endif /* COMPTONG4CRYSTAL_HH_ */
