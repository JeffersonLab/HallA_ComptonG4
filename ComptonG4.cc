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
//#include <QGSP_BERT.hh>
#include <FTFP_BERT.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4String.hh>
#include <G4OpticalPhysics.hh>

// Standard Lib includes
#include <fstream>

#ifdef G4UI_USE_QT
    #include "G4UIQt.hh"
#endif

#ifdef G4VIS_USE
    #include "G4VisExecutive.hh"
#endif

// Includes for this project
#include "ComptonG4DetectorConstruction.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4EventAction.hh"
#include "ComptonG4RunAction.hh"
#include "ComptonG4SteppingAction.hh"

// Include BOOST headers and setup boost namespace
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// ... IT BEGIIINNNNSSSS!!!!
int main( int argc, char **argv)
{

  G4String geometry_file;
  G4String config_file;
  G4String batch_file;

  // Prepare the command line options
  // Generic command line options
  po::options_description generic("Generic options");
  generic.add_options()
    ("help,h","print this help message") // help
    ("config,c",po::value<std::string>(&geometry_file)->default_value("./")
        ,"config file") // config file
    ;

  // Shared with configuration file
  po::options_description config("Configuration");
  config.add_options()
    ("geometry-file",po::value<std::string>(),"Geometry filename")
    ("batch-file",po::value<std::string>(),"Batch filename")
    ("output-dir",po::value<std::string>(),"path output directory")
    ;

  // Finally, add them to boost
  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config);

  po::options_description config_file_options;
  config_file_options.add(config);

  po::options_description visible("Allowed options");
  visible.add(generic).add(config);

  // Now process them generic options
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,cmdline_options),vm);
  po::notify(vm);
  // First, check for the config file before parsing
  if(vm.count("config")) { // was specified!
    config_file = vm["config"].as<std::string>();
    // Open the file as a stream for boost
    std::ifstream ifs(config_file);
    if(ifs) {
      // Process config
      po::store(parse_config_file(ifs,config_file_options),vm);
    }
  }

  // Process help
  if(vm.count("help")) {
    std::cout << generic << "\n";
    return 1;
  }

  // Process the mandatory geometry file
  if(!vm.count("geometry-file")){
    G4cerr << "Did not specify a geometry file!" << G4endl;
    return -1;
  } else {
    geometry_file = vm["geometry-file"].as<std::string>();
  }

#ifdef COMPTONG4_BATCH_MODE // We are in batch mode
  // Process the batch file
  if(!vm.count("geometry-file")){
    G4cerr << "Did not specify a batch file!" << G4endl;
    return -1;
  } else {
    batch_file = vm["batch-file"].as<std::string>();
  }
#endif


  // Seed the random number generator with a constant seed...cause, you know,
  // confusion is great!!!
  CLHEP::HepRandom::setTheSeed(20091010.);
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Run Manager
  G4RunManager *runManager = new G4RunManager();

  // Auxiliary DataIO class
  ComptonG4Analysis *analysis = new ComptonG4Analysis();

  // Create the default /Compton directory for all messengers
  G4UIdirectory *compDir = new G4UIdirectory("/Compton/");
  compDir->SetGuidance("UI commands for the ComptonG4 simulation");

  // Mandatory Detector Constructor

  runManager->SetUserInitialization(new ComptonG4DetectorConstruction(geometry_file));
  //runManager->SetUserInitialization( new QGSP_BERT() );
  FTFP_BERT *physicsList = new FTFP_BERT();
  physicsList->RegisterPhysics(new G4OpticalPhysics() );
  runManager->SetUserInitialization( physicsList );
  if(physicsList->GetPhysics("Optical") ){
    G4cout << "***Optical Processes ON***" << G4endl;
  } else {
    G4cout << "***Optical Processes OFF***" << G4endl;
  }

  // Are we in interactive mode (GUI) or batch-mode?
#ifndef COMPTONG4_BATCH_MODE // Interactive mode

  // UI Session pointer
  G4UIsession *session = 0;

#ifdef G4UI_USE_QT
  session = new G4UIQt(argc,argv);
#else
  session = new G4UIterminal(argc,argv);
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
  UI->ApplyCommand(command+batch_file);
#endif

  delete compDir;
  delete runManager;

  return 0;
}
