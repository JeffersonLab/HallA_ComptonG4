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
#include "ComptonG4SensitiveDetectorManager.hh"
#include "ComptonG4StackingAction.hh"

// Include BOOST headers and setup boost namespace
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// ... IT BEGIIINNNNSSSS!!!!
int main( int argc, char **argv)
{

  G4String geometry_file;
  G4String config_file;
  G4String batch_file;
  G4bool use_optical;
  G4double random_seed;
  G4String output_dir;
  G4String rootfile_prefix;
  std::vector<std::string> ui_cmds;
  G4int run;
  G4int run_min_digits;

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
    ("output-dir",po::value<std::string>()->default_value("./")
     ,"path output directory")
    ("rootfile-prefix",po::value<std::string>()->default_value("ComptonG4_")
     ,"Output ROOT File Prefix, ex: ComptonG4_")
    ("enable-optical", po::value<G4bool>()->default_value(true)
     ->implicit_value(true),"Enable/Disable optical photons")
    ("random-seed",po::value<G4double>()->default_value(17760704.),
     "Set the random seed")
    ("command",po::value<std::vector<std::string> >(),
     "ordered list of commands to process")
    ("run,r",po::value<G4int>()->default_value(0),
     "Set the run number (can also be set in a macro)")
    ("run-min-digits",po::value<G4int>()->default_value(5),
     "Minimum number of digits for run number (pad with zeros for smaller runs numbers)")
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
  // This commented out method does not allow unrecognized options to be
  // passed onto other programs
  //po::store(po::parse_command_line(argc,argv,cmdline_options),vm);
  // This line allows unrecognized options to be parsed by other programs
  // i.e. GEANT
  po::store(po::command_line_parser(argc,argv)
    .options(cmdline_options).allow_unregistered().run(),vm);
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
    std::cout << config << "\n";
    return 1;
  }

  // Process the mandatory geometry file
  if(!vm.count("geometry-file")){
    G4cerr << "Did not specify a geometry file!" << G4endl;
    return -1;
  } else {
    geometry_file = vm["geometry-file"].as<std::string>();
  }

  // Process optional parameters
  run = vm["run"].as<G4int>();
  run_min_digits = vm["run-min-digits"].as<G4int>();
  use_optical = vm["enable-optical"].as<G4bool>();
  random_seed = vm["random-seed"].as<G4double>();
  output_dir = vm["output-dir"].as<std::string>();
  rootfile_prefix = vm["rootfile-prefix"].as<std::string>();
  if(vm.count("command")){
    ui_cmds = vm["command"].as<std::vector<std::string> >();
  }

#ifdef COMPTONG4_BATCH_MODE // We are in batch mode
  // Process the batch file
  if(!vm.count("batch-file")){
    G4cerr << "Did not specify a batch file!" << G4endl;
    return -1;
  } else {
    batch_file = vm["batch-file"].as<std::string>();
  }
#endif


  // Seed the random number generator with the command line option
  G4cout << "Initializing random seed to " << std::fixed << random_seed << G4endl;
  CLHEP::HepRandom::setTheSeed(random_seed);
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Run Manager
  G4RunManager *runManager = new G4RunManager();

  // Auxiliary DataIO class
  ComptonG4Analysis *analysis = new ComptonG4Analysis();
  analysis->SetOutputPath(output_dir);
  analysis->SetRootfilePrefix(rootfile_prefix);

  // Sensitive Detector Manager
  ComptonG4SensitiveDetectorManager* sensManager =
    new ComptonG4SensitiveDetectorManager(analysis);
  analysis->SetSDManager(sensManager);

  // Create the default /Compton directory for all messengers
  G4UIdirectory *compDir = new G4UIdirectory("/Compton/");
  compDir->SetGuidance("UI commands for the ComptonG4 simulation");

  //runManager->SetUserInitialization( new QGSP_BERT() );
  FTFP_BERT *physicsList = new FTFP_BERT();
  // Optical photons controlled by command line flag
  if( use_optical ) {
    G4OpticalPhysics *optical = new G4OpticalPhysics(0);
    optical->SetFiniteRiseTime(true);
    physicsList->RegisterPhysics(optical);
  }
  runManager->SetUserInitialization( physicsList );
  if(physicsList->GetPhysics("Optical") ){
    G4cout << "***Optical Processes ON***" << G4endl;
  } else {
    G4cout << "***Optical Processes OFF***" << G4endl;
  }

  // Mandatory Detector Constructor
  runManager->SetUserInitialization(new
      ComptonG4DetectorConstruction(geometry_file,sensManager,analysis));


  // Ensure that the random number status is properly stored on each event
  runManager->StoreRandomNumberStatusToG4Event(1);

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
  ComptonG4RunAction *runAction = new ComptonG4RunAction(analysis);
  runAction->SetRunNumber(run);
  runAction->SetRunMinDigits(run_min_digits);
  runManager->SetUserAction(runAction);
  runManager->SetUserAction(new ComptonG4StackingAction());

  // Initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Execute any commands the user passed as parameter
  for(size_t i = 0; i < ui_cmds.size(); i++ ) {
    UI->ApplyCommand(ui_cmds[i]);
  }

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
