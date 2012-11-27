////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////

#include "ComptonG4DetectorConstruction.hh"
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>

// GEANT4 Geometry related includes
#include <G4GDMLParser.hh>

ComptonG4DetectorConstruction::ComptonG4DetectorConstruction()
{
  G4GDMLParser parser;

  // Be flexible in this. If the user has provided a CG4_GEOMETRY_FILE
  // environmental variable, then use that
  char *geometryFile = getenv("CG4_GEOMETRY_FILE");
  if(geometryFile!=NULL) {
    parser.Read(geometryFile);
  } else {
    parser.Read("ComptonG4Geometry.xml");
  }
  fPhysicsWorld = parser.GetWorldVolume();
}

ComptonG4DetectorConstruction::~ComptonG4DetectorConstruction()
{
}


G4VPhysicalVolume*  ComptonG4DetectorConstruction::Construct()
{
  return fPhysicsWorld;
}
