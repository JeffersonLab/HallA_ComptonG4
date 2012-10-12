#ifndef _COMPTONDETECTORCONSTRUCTION_H
#define _COMPTONDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

// Prefedine classes
class G4VPhysicalVolume;

class ComptonDetectorConstruction : public G4VUserDetectorConstruction {

public:
  ComptonDetectorConstruction();
  ~ComptonDetectorConstruction();

  G4VPhysicalVolume* Construct();
};

#endif
