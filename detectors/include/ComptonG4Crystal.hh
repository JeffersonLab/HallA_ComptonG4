/*
 * ComptonG4Crystal.hh
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4CRYSTAL_HH_
#define COMPTONG4CRYSTAL_HH_

#include <VComptonG4SensitiveDetector.hh>
#include <map>
#include <vector>

class ComptonG4Analysis;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class ComptonG4Crystal: public VComptonG4SensitiveDetector {
public:
  ComptonG4Crystal(G4String name);
  virtual ~ComptonG4Crystal();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void CleanEvent();

private:
  std::vector<G4int> fOpticalHits;
  std::vector<std::vector<G4double> > fGlobalTimes;
  std::vector<G4double> fEDeps;
};

#endif /* COMPTONG4CRYSTAL_HH_ */
