#ifndef _COMPTONG4ANALYSIS_H
#define _COMPTONG4ANALYSIS_H

// GEANT4 Includes
#include <G4UIcommand.hh>

// ROOT Includes
#include <Rtypes.h>

// General includes
#include <vector>
#include <string>

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

  // Detector structure
  typedef struct {
    Double_t eDep;
    Double_t nOpticalPhotons;
  } DetectorStruct;

  // Public G4 Analyzer functions
  void Initialize(G4int runnum = 0001);
  void StartOfEvent();
  void EndOfEvent();
  void Finished();
  void CleanEvent();
  void SetAsym(Double_t asym) { fAsym = asym; }
  void SetGammaE(Double_t e) { fGammaE = e; }
  void SetRho(Double_t rho) { fRho = rho; }
  void SetTheta(Double_t theta) { fTheta = theta; }
  void SetPhi(Double_t phi) { fPhi = phi; }
  void AddDetector(std::string name);
  void SetOutputPath(std::string path);

  void AddEDep(std::string name,Double_t e);
  void AddOpticalHits(std::string name,Double_t hits);

  /*
  void AddStepSize(std::string name,Double_t size) {
    fStepSize[fDetectorIndices[name]] += size;
  }
  void AddEDepAllCharges(std::string name,Double_t e) {
    fEDepAll[fDetectorIndices[name]] += e;
  }
  void AddStepSizeAllCharges(std::string name,Double_t size) {
    fStepSizeAll[fDetectorIndices[name]] += size;
  }*/

private:
  TTree               *fTree;         // Generated TTree
  TFile               *fFile;         // Output TFile
  std::string            fOutputPath;    // Path to output rootfiles
  std::vector<std::string> fDetectorNames; // Names of detectors
  std::vector<DetectorStruct> fDetectors;  // Detectors

  // Global tree variables per event
  Double_t            fAsym;          // Theoretical asymmetry
  Double_t            fRho;           // Normalized Gamma scattering energy
  Double_t            fGammaE;        // Absolute Gamma energy
  Double_t            fTheta;         // The scattering angle in Degrees
  Double_t            fPhi;           // The azimuthal angle

  Int_t               fNumberOfEvents;

  G4int DetectorIndex(std::string name);
};

#endif
