/*
 * ComptonG4SensitiveDetectorManager.hh
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTORMANAGER_HH_
#define COMPTONG4SENSITIVEDETECTORMANAGER_HH_

#include <vector>
#include <map>
#include <G4String.hh>

class ComptonG4Analysis;
class ComptonG4OpticalTracker;
class VComptonG4SensitiveDetector;
class G4VSensitiveDetector;
class G4LogicalVolume;
class G4VPhysicalVolume;
class ComptonG4SensitiveDetectorMessenger;

class ComptonG4SensitiveDetectorManager {
public:
  ComptonG4SensitiveDetectorManager(ComptonG4Analysis *analysis,
      ComptonG4OpticalTracker *optical_tracker);
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

  /*
   * Process Options for named detector
   *
   * \param name : Name of detector
   * \param options : Options to pass to detector
   */
  void SetDetectorOptions(G4String name, std::map<G4String,G4String> options);


private:
  ComptonG4SensitiveDetectorMessenger *fMessenger;
  ComptonG4Analysis *fAnalysis;
  ComptonG4OpticalTracker *fOpticalTracker;
  std::vector<VComptonG4SensitiveDetector*> fDetectors;

  // Strings of special option specifiers
  static const G4String fOptionsAll; // Apply to all (reject bad options)
  static const G4String fOptionsAllIgnore; // Apply to all (ignore uknown)
};

#endif /* COMPTONG4SENSITIVEDETECTORMANAGER_HH_ */

