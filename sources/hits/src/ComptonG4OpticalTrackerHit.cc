#include "ComptonG4OpticalTrackerHit.hh"
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>


/*
 * Default constructor
 */
ComptonG4OpticalTrackerHit::ComptonG4OpticalTrackerHit() :
  VComptonG4Hit(new ComptonG4OpticalTrackerData())
{
  ClearHit();
}

/*
 * Constructor where we initialize from already defined
 */
ComptonG4OpticalTrackerHit::ComptonG4OpticalTrackerHit(ComptonG4OpticalTrackerData *ptr) :
  VComptonG4Hit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4OpticalTrackerHit::ComptonG4OpticalTrackerHit(
    const ComptonG4OpticalTrackerHit &right) :
  VComptonG4Hit(new ComptonG4OpticalTrackerData())
{
  Clone(&right);
}

/*
 * ClearHit
 */
void ComptonG4OpticalTrackerHit::ClearHit()
{
  if(!GetDataRef())
    return;

  SetStartPosition(G4ThreeVector(0.0,0.0,0.0));
  SetStartDirection(G4ThreeVector(0.0,0.0,0.0));
  SetStartMomentum(G4ThreeVector(0.0,0.0,0.0));
  SetStartVolumeID(0);

  SetEndPosition(G4ThreeVector(0.0,0.0,0.0));
  SetEndDirection(G4ThreeVector(0.0,0.0,0.0));
  SetEndMomentum(G4ThreeVector(0.0,0.0,0.0));
  SetEndVolumeID(0);

  SetKineticEnergy(0.0*CLHEP::MeV);
  SetTotalLength(0.0*CLHEP::nm);
  SetTrackID(-1);
  SetParentID(-1);
  SetCreationProcess(ComptonG4GenericOptical);
  SetDetected(false);
  SetRecordedEnd(false);
}

/*
 *
 */
ComptonG4OpticalTrackerHit::~ComptonG4OpticalTrackerHit()
{
  ClearHit();
}

/*
 * Process step information
 */
void ComptonG4OpticalTrackerHit::ProcessStep(G4Step *step)
{

  ProcessTrack(step->GetTrack());
}

/*
 * Process track information
 */
void ComptonG4OpticalTrackerHit::ProcessTrack(const G4Track *track,
    G4bool start, G4bool detected)
{
  if(start) {
    SetStartPosition(track->GetPosition());
    SetStartDirection(track->GetMomentumDirection());
    SetStartMomentum(track->GetMomentum());
    SetStartVolumeID(track->GetVolume()->GetName().hash(G4String::exact));
    SetTotalLength(track->GetTrackLength());
    SetKineticEnergy(track->GetKineticEnergy());
    SetTrackID(track->GetTrackID());
    SetParentID(track->GetParentID());
    SetStepNumber(track->GetCurrentStepNumber());
    // Primary tracks have no registered creation process, so check that it
    // exists before trying to set it.
    const G4VProcess *process = track->GetCreatorProcess();
    ComptonG4OpticalProcess p = ComptonG4GenericOptical;
    if(process) {
      if(process->GetProcessName().compareTo(
            "Scintillation",G4String::ignoreCase) == 0 ) {
        p = ComptonG4Scintillation;
      } else if(process->GetProcessName().compareTo(
            "Cerenkov",G4String::ignoreCase) == 0 ) {
        p = ComptonG4Cerenkov;
      }
      SetCreationProcess(p);
    } else {
      p = ComptonG4NoOpticalCreationProcess;
    }
    SetCreationProcess(p);
  } else {
    SetEndPosition(track->GetPosition());
    SetEndDirection(track->GetMomentumDirection());
    SetEndMomentum(track->GetMomentum());
    SetEndVolumeID(track->GetVolume()->GetName().hash(G4String::exact));
    SetRecordedEnd(true);
    SetDetected(detected);
  }
}
