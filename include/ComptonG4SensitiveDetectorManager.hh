/*
 * ComptonG4SensitiveDetectorManager.hh
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTORMANAGER_HH_
#define COMPTONG4SENSITIVEDETECTORMANAGER_HH_

#include <vector>
#include <G4String.hh>

class ComptonG4Analysis;
class ComptonG4SensitiveDetector;
class G4VSensitiveDetector;

class ComptonG4SensitiveDetectorManager {
public:
  ComptonG4SensitiveDetectorManager(ComptonG4Analysis*);
  virtual ~ComptonG4SensitiveDetectorManager();

  // Register new detector
  G4VSensitiveDetector* RegisterDetector(G4String name);

private:
  ComptonG4Analysis *fAnalysis;
  std::vector<ComptonG4SensitiveDetector*> fDetectors;
};

#endif /* COMPTONG4SENSITIVEDETECTORMANAGER_HH_ */

