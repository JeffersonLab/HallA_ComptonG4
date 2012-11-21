/*!
* @brief A Compton Polarimeter GEANT4 based Simulation Package
*
* This is a Compton Polarimeter GEANT4 based Simulation Package. It can load
* specified Geometry files and output a ROOT file with the results.
*
* @author Juan Carlos Cornejo <cornejo@jlab.org>
*
* $Header $
!*/

// Standard G4 includes
#include <G4RunManager.hh>
#include <QGSP_BERT_HP.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>

#ifdef G4UI_USE_QT
    #include "G4UIQt.hh"
#endif

#ifdef G4VIS_USE
    #include "G4VisExecutive.hh"
#endif

// Includes for this project
#include "ComptonG4DetectorConstruction.hh"
#include "ComptonG4PhysicsList.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4EventAction.hh"
#include "ComptonG4RunAction.hh"
#include "ComptonG4SteppingAction.hh"

// ... IT BEGIIINNNNSSSS!!!!
int main( int argc, char **argv)
{
  // Seed the random number generator with a constant seed...cause, you know,
  // confusion is great!!!
  CLHEP::HepRandom::setTheSeed(20091010.);

  // Run Manager
  G4RunManager *runManager = new G4RunManager();

  // Auxiliary DataIO class
  ComptonG4Analysis *analysis = new ComptonG4Analysis();

  // Create the default /Compton directory for all messengers
  G4UIdirectory *compDir = new G4UIdirectory("/Compton/");
  compDir->SetGuidance("UI commands for the ComptonG4 simulation");

  // Mandatory Detector Constructor

  runManager->SetUserInitialization(new ComptonG4DetectorConstruction());
  //runManager->SetUserInitialization( new QGSP_BERT() );
  runManager->SetUserInitialization( new ComptonG4PhysicsList() );

  // Are we in interactive mode (GUI) or batch-mode?
#ifndef COMPTONG4_BATCH_MODE // Interactive mode

  // UI Session pointer
  G4UIsession *session = 0;

#ifdef G4UI_USE_QT
  session = new G4UIQt(argc,argv);
#else
  session = new G4UIterminal();
#endif

#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  //
  // Simple graded message scheme - give first letter or a digit:
  //  0) quiet,         // Nothing is printed.
  //  1) startup,       // Startup and endup messages are printed...
  //  2) errors,        // ...and errors...
  //  3) warnings,      // ...and warnings...
  //  4) confirmations, // ...and confirming messages...
  //  5) parameters,    // ...and parameters of scenes and views...
  //  6) all            // ...and everything available.

  G4VisManager* visManager = new G4VisExecutive;
  //visManager -> SetVerboseLevel (1);
  visManager ->Initialize();
#endif

#endif

  // Set user action classes
  runManager->SetUserAction(new ComptonG4PrimaryGeneratorAction(analysis));
  runManager->SetUserAction(new ComptonG4SteppingAction(analysis));
  runManager->SetUserAction(new ComptonG4EventAction(analysis));
  runManager->SetUserAction(new ComptonG4RunAction(analysis));

  // Initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

#ifndef COMPTONG4_BATCH_MODE // Interactive mode

#if defined(G4UI_USE_XM) || defined(G4UI_USE_WIN32) || defined(G4UI_USE_QT)
  // Read default GUI mac file
  char *guiFile= getenv("COMPTONG4_GUI_MAC_FILE");
  if(guiFile!=NULL) {
    G4String cmd("/control/execute");
    cmd += guiFile;
    UI->ApplyCommand(cmd );
  } else {
    UI->ApplyCommand("/control/execute ComptonG4_GUI.mac");
  }
#endif

  session->SessionStart();

  delete session;

#ifdef G4VIS_USE
  delete visManager;
#endif

#else // Batch-mode

  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command+fileName);
#endif

  delete compDir;
  delete runManager;

  return 0;
}
