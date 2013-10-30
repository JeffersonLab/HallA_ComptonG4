#ifndef _COMPTONDETECTORCONSTRUCTION_H
#define _COMPTONDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

// Predefine classes
class ComptonG4DetectorConstructionMessenger;
class ComptonG4SensitiveDetectorManager;

/*!
 * @class ComptonG4DetectorConstruction
 *
 * @brief Default Generator of Primaries for the Compton Scattering process
 *
 * Construct the detector from a specified GDML geometry class. To specify
 * the file to use, use the /Compton/geometry/file command
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4DetectorConstruction : public G4VUserDetectorConstruction {
public:
  ComptonG4DetectorConstruction(G4String geometry_file, ComptonG4SensitiveDetectorManager*);
  ~ComptonG4DetectorConstruction();

  G4VPhysicalVolume* Construct();
  void ActivateDetector(G4String det);
private:
  G4VPhysicalVolume* fPhysicsWorld;

  ComptonG4DetectorConstructionMessenger *fMessenger;
};

#endif
