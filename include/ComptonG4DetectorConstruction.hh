#ifndef _COMPTONDETECTORCONSTRUCTION_H
#define _COMPTONDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

// Predefine classes
class G4VPhysicalVolume;

class ComptonG4DetectorConstruction : public G4VUserDetectorConstruction {

public:
  ComptonG4DetectorConstruction();
  ~ComptonG4DetectorConstruction();

  G4VPhysicalVolume* Construct();
};

#endif
