////////////////////////////////////////////////////////////////////////////////
//
//
///
////////////////////////////////////////////////////////////////////////////////

// Standard G4 includes
#include <G4RunManager.hh>
#include <QGSP_BERT.hh>
#include <G4UImanager.hh>

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

// ... IT BEGIIINNNNSSSS!!!!
int main( int argc, char **argv)
{
  // Choose the random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Seed the random number generator with a constant seed...cause, you know,
  // confusion is great!!!
  CLHEP::HepRandom::setTheSeed(20091010.);

  // Run Manager
  G4RunManager *runManager = new G4RunManager();

  // Auxiliary DataIO class
  ComptonG4Analysis *analysis = new ComptonG4Analysis();

  // Mandatory Detector Constructor
  ComptonG4DetectorConstruction *detector = new ComptonG4DetectorConstruction();
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization( new QGSP_BERT() );
  runManager->SetUserInitialization( new ComptonG4PhysicsList() );

  // UI Session pointer
  G4UIsession *session = 0;

  // Are we in interactive mode (GUI) or batch-mode?
#ifndef COMPTONG4_BATCH_MODE // Interactive mode

#ifdef G4UI_USE_QT
  session = new G4UIQt(argc,argv);
#else
  session = new G4UIterminal();
#endif

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

  // Set user action classes
  runManager->SetUserAction(new ComptonG4PrimaryGeneratorAction(analysis));

  // Initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

#ifndef COMPTONG4_BATCH_MODE // Interactive mode
  // Apply commands
  UI->ApplyCommand("/control/execute gui.mac");

#if defined(G4UI_USE_XM) || defined(G4UI_USE_WIN32) || defined(G4UI_USE_QT)
  // Customize the G4UIXm,Win32 menubar with a macro file :
  UI->ApplyCommand("/control/execute gui.mac");
#endif

  session->SessionStart();

  delete session;
#else // Batch-mode

#ifdef G4VIS_USE
  visManager->SetVerboseLevel("quiet");
#endif

  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command+fileName);
#endif

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  return 0;
}
