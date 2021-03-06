#ifndef _COMPTONDETECTORCONSTRUCTION_H
#define _COMPTONDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>
#include <map>

// Predefine classes
class ComptonG4DetectorConstructionMessenger;
class ComptonG4SensitiveDetectorManager;
class ComptonG4Analysis;
class ComptonG4DipoleField;
class G4LogicalVolume;
class G4VPhysicalVolume;

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
  ComptonG4DetectorConstruction(G4String geometry_file,
      ComptonG4SensitiveDetectorManager* manager,ComptonG4Analysis *analysis);
  ~ComptonG4DetectorConstruction();

  G4VPhysicalVolume* Construct();
  void ActivateDetector(G4String det);
  void RegisterMagneticVolume(G4LogicalVolume *v);
  void RegisterMagneticPhysical(G4VPhysicalVolume *p, G4LogicalVolume *v);
  void ProcessMagneticVolumeOptions(G4String name,
      std::map<G4String, G4String> options);
  G4LogicalVolume* GetMagneticVolume(G4String name);
private:
  G4VPhysicalVolume* fPhysicsWorld;

  ComptonG4DetectorConstructionMessenger *fMessenger;
  ComptonG4Analysis *fAnalysis;
  std::vector<G4LogicalVolume*> fMagneticVolumes;
  std::vector<ComptonG4DipoleField *> fMagneticFields;
};

#endif
