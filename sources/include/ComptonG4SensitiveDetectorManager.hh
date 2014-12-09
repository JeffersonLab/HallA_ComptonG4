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
class VComptonG4SensitiveDetector;
class G4VSensitiveDetector;
class G4LogicalVolume;
class G4VPhysicalVolume;

class ComptonG4SensitiveDetectorManager {
public:
  ComptonG4SensitiveDetectorManager(ComptonG4Analysis*);
  virtual ~ComptonG4SensitiveDetectorManager();

  /*
   * Register new detector
   */
  void RegisterDetector(G4LogicalVolume *volume, G4String type);

  /*
   * Add Physical volume to sensitive detector
   */
  void AddVolume(G4String detName, G4VPhysicalVolume *vol);

  /*
   * Get detector by name
   */
  VComptonG4SensitiveDetector* GetDetector(G4String name);

  /*
   * Get list of Sensitive Detectors
   */
  std::vector<VComptonG4SensitiveDetector*> GetSensitiveDetectors() {
    return fDetectors;
  }

private:
  ComptonG4Analysis *fAnalysis;
  std::vector<VComptonG4SensitiveDetector*> fDetectors;
};

#endif /* COMPTONG4SENSITIVEDETECTORMANAGER_HH_ */

