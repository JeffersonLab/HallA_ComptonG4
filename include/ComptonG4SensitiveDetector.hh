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
  ComptonG4SensitiveDetector(G4String name, ComptonG4Analysis* analysis = 0);
  virtual ~ComptonG4SensitiveDetector();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

private:
  ComptonG4Analysis *fAnalysis;
  std::vector<G4int> fHits;
  std::vector<G4String> fVolumeNames;
  std::map<G4String,G4int> fVolumeIndices;
};

#endif /* COMPTONG4SENSITIVEDETECTOR_HH_ */
