#include "ComptonG4Hit.hh"
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>


/*
 * Default constructor
 */
ComptonG4Hit::ComptonG4Hit() : VComptonG4Hit(new ComptonG4Data())
{
  ClearHit();
}

/*
 * Constructor where we initialize from already defined
 */
ComptonG4Hit::ComptonG4Hit(ComptonG4Data *ptr) : VComptonG4Hit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4Hit::ComptonG4Hit(const ComptonG4Hit &right) :
  VComptonG4Hit(new ComptonG4Data())
{
  Clone(&right);
}




/*
 * ClearHit
 */
void ComptonG4Hit::ClearHit()
{
  if(!GetDataRef())
    return;

  for(UInt_t i = 0; i < 3; i++ ) {
    data().position[i] = 0.0;
    data().direction[i] = 0.0;
    data().momentum[i] = 0.0;
    data().vertex_position[i] = 0.0;
    data().vertex_direction[i] = 0.0;
  }
  data().global_time = 0.0*CLHEP::ns;
  data().local_time = 0.0*CLHEP::ns;
  data().kinetic_energy = 0.0*CLHEP::MeV;
  data().total_length = 0.0;
  data().particle_id = -1;
  data().track_id = -1;
  data().parent_id = -1;
  data().vertex_kinetic_energy = 0.0;
  data().creation_process = "";
}

/*
 *
 */
ComptonG4Hit::~ComptonG4Hit()
{
  ClearHit();
}

/*
 * Process step information
 */
void ComptonG4Hit::ProcessStep(G4Step *step)
{

  ProcessTrack(step->GetTrack());
}

/*
 * Process track information
 */
void ComptonG4Hit::ProcessTrack(G4Track *track)
{
  SetPosition(track->GetPosition());
  SetGlobalTime(track->GetGlobalTime());
  SetLocalTime(track->GetLocalTime());
  SetDirection(track->GetMomentumDirection());
  SetMomentum(track->GetMomentum());
  SetKineticEnergy(track->GetKineticEnergy());
  SetTotalLength(track->GetTrackLength());
  SetParticleID(track->GetDynamicParticle()->GetPDGcode());
  SetTrackID(track->GetTrackID());
  SetParentID(track->GetParentID());
  SetStepNumber(track->GetCurrentStepNumber());
  SetTrackStatus(track->GetTrackStatus());
  SetVertexPosition(track->GetVertexPosition());
  SetVertexDirection(track->GetVertexMomentumDirection());
  SetVertexKineticEnergy(track->GetVertexKineticEnergy());
  // Primary tracks have no registered creation process, so check that it
  // exists before trying to set it.
  const G4VProcess *process = track->GetCreatorProcess();
  if(process)
    SetCreationProcess(process->GetProcessName());
}
