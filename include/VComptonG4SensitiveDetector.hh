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

#include "ComptonG4SensitiveDetectorFactory.hh"

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class VComptonG4SensitiveDetector: public G4VSensitiveDetector {
public:
  VComptonG4SensitiveDetector(G4String name);
  virtual ~VComptonG4SensitiveDetector();

  void Initialize(G4HCofThisEvent *) = 0;
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) = 0;
  void EndOfEvent(G4HCofThisEvent*) = 0;

  void AddVolume(G4VPhysicalVolume* vol) {
    fVolumes.push_back(vol);
  }

  // Provide a way to set the Analysis pointer
  static void SetAnalysis(ComptonG4Analysis *ptr) {
    fAnalysis = ptr;
  }

  static ComptonG4Analysis* Analysis() { return fAnalysis; }

private:
  static ComptonG4Analysis *fAnalysis;
  std::vector<G4VPhysicalVolume*> fVolumes;

  void CleanEvent();
  int GetIndex(G4VPhysicalVolume* vol);
};

#endif /* COMPTONG4SENSITIVEDETECTOR_HH_ */
