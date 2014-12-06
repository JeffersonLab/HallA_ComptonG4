/*
 * ComptonG4PMTCathode.hh
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PMTCATHODE_HH_
#define COMPTONG4PMTCATHODE_HH_

#include <VComptonG4SensitiveDetector.hh>
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

private:
  std::vector<G4int> fOpticalHits;
  std::vector<std::vector<G4double> > fGlobalTimes;
  std::vector<G4double> fEDeps;
};

#endif /* COMPTONG4PMTCATHODE_HH_ */
