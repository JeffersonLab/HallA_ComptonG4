#ifndef _COMPTONG4ANALYSIS_H
#define _COMPTONG4ANALYSIS_H

// GEANT4 Includes
#include <G4UIcommand.hh>

// Pre-defined classes
class TTree;
class TFile;

/*!
 * @class ComptonG4Analysis
 *
 * @brief Default class to process the simulation data
 *
 * This class processes the simulation data, including writing to a ROOT file
 * when the event is finished.
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4Analysis {
public:
  ComptonG4Analysis();
  ~ComptonG4Analysis();

  // Public G4 Analyzer functions
  void Initialize(G4int runnum = 0001);
  void StartOfEvent();
  void EndOfEvent();

private:
  TTree               *fTree;         // Generated TTree
  TFile               *fFile;         // Output TFile
  G4String            fOutputPath;    // Path to output rootfiles

  // Tree variables per event
  G4double            fAsym;          // Theoretical asymmetry
  G4double            fRho;           // Normalized Gamma scattering energy
  G4double            fGammaE;        // Absolute Gamma energy
  G4double            fEDep;          // Energy deposited in the crystal
};

#endif
