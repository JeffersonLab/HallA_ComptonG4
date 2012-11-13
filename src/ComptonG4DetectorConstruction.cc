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
  parser.Read("ComptonGeometry.xml");
  fPhysicsWorld = parser.GetWorldVolume();
}

ComptonG4DetectorConstruction::~ComptonG4DetectorConstruction()
{
}


G4VPhysicalVolume*  ComptonG4DetectorConstruction::Construct()
{
  return fPhysicsWorld;
}
