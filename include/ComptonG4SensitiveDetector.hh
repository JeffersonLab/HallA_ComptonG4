/*
 * ComptonG4SensitiveDetector.hh
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTOR_HH_
#define COMPTONG4SENSITIVEDETECTOR_HH_

#include <G4VSensitiveDetector.hh>
#include <map>
#include <vector>

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class ComptonG4SensitiveDetector: public G4VSensitiveDetector {
public:
  ComptonG4SensitiveDetector(G4String name);
  virtual ~ComptonG4SensitiveDetector();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  // Provide a way to set the Analysis pointer
  static void SetAnalysis(ComptonG4Analysis *ptr) {
    fAnalysis = ptr;
  }

  static ComptonG4Analysis* Analysis() { return fAnalysis; }

private:
  static ComptonG4Analysis *fAnalysis;
  std::vector<G4int> fOpticalHits;
  std::vector<G4double> fEDeps;
  std::vector<G4String> fVolumeNames;
  std::map<G4String,G4int> fVolumeIndices;

  void CleanEvent();
};

#endif /* COMPTONG4SENSITIVEDETECTOR_HH_ */
