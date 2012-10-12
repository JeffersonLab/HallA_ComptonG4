////////////////////////////////////////////////////////////////////////////////
//
//
///
////////////////////////////////////////////////////////////////////////////////

// Standard G4 includes
#include <G4RunManager.hh>

// Includes for this project
#include "ComptonDetectorConstruction.hh"
#include "ComptonPhysicsList.hh"

// ... IT BEGIIINNNNSSSS!!!!
int main( int argc, char **argv)
{
  // Choose the random enegine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Seed the random number generator with a constant seed...cause, you know,
  // confusion is great!!!
  CLHEP::HepRandom::setTheSeed(20091010.);

  // Run Manager
  G4RunManager *runManager = new G4RunManager();

  // Mandatory Detector Constructor
  ComptonDetectorConstruction *detector = new ComptonDetectorConstruction();
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new ComptonPhysicsList() );

  // Initialize G4 kernel
  runManager->Initialize();
}
