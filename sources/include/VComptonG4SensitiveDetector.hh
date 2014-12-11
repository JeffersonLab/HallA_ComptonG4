/*
 * VComptonG4SensitiveDetector.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef VCOMPTONG4SENSITIVEDETECTOR_HH_
#define VCOMPTONG4SENSITIVEDETECTOR_HH_

#include <G4VSensitiveDetector.hh>
#include <map>
#include <vector>
#include <Rtypes.h>

#include "ComptonG4SensitiveDetectorFactory.hh"

// Forward declarations
class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class TTree;

/*
 *
 */
class VComptonG4SensitiveDetector: public G4VSensitiveDetector {
public:
  VComptonG4SensitiveDetector(G4String name);
  virtual ~VComptonG4SensitiveDetector();

  void Initialize(G4HCofThisEvent *) = 0;
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) = 0;
  void EndOfEvent(G4HCofThisEvent*) = 0;

  virtual void AddVolume(G4VPhysicalVolume* vol) {
    fVolumes.push_back(vol);
  }

  /*
   * Specify analyzer
   */
  void SetAnalysis(ComptonG4Analysis *ptr) {
    fAnalysis = ptr;
  }

  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch) = 0;

  /*
   * Set sensitive detector options
   *
   * \param options The options to parse, separated by a semicolon
   */
  virtual void SetOptions(std::map<G4String, G4String> options,
      bool ignore_unknown) = 0;

protected:
  void CleanEvent();
  int GetIndex(G4VPhysicalVolume* vol);
  ComptonG4Analysis *fAnalysis;
  std::vector<G4VPhysicalVolume*> fVolumes;

  G4bool ProcessOptionBool(G4String param, G4String value, G4String option_name,
      G4bool &result);

  void UnknownOption(G4String param, G4String value);
};

#endif /* COMPTONG4SENSITIVEDETECTOR_HH_ */
