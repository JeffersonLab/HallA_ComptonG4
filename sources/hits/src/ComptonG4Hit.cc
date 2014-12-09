#include "ComptonG4Hit.hh"
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>

/*
 *
 */
ComptonG4Hit::ComptonG4Hit() : fData(0)
{
  ComptonG4Hit(new ComptonG4Data());
}

/*
 * Copy constructor
 */
ComptonG4Hit::ComptonG4Hit(const ComptonG4Hit &right) : VComptonG4Hit(right),
  fData(0)
{
  fData = new ComptonG4Data();
  CopyData(*right.fData);
}


/*
 *
 */
ComptonG4Hit::ComptonG4Hit(ComptonG4Data *data) : VComptonG4Hit(),
  fData(data)
{
  ClearHit();
}

/*
 * ClearHit
 */
void ComptonG4Hit::ClearHit()
{
  if(!fData)
    return;

  for(UInt_t i = 0; i < 3; i++ ) {
    fData->position[i] = 0.0;
    fData->direction[i] = 0.0;
    fData->momentum[i] = 0.0;
    fData->vertex_position[i] = 0.0;
    fData->vertex_direction[i] = 0.0;
  }
  fData->global_time = 0.0*CLHEP::ns;
  fData->local_time = 0.0*CLHEP::ns;
  fData->kinetic_energy = 0.0*CLHEP::MeV;
  fData->total_length = 0.0;
  fData->particle_id = -1;
  fData->track_id = -1;
  fData->parent_id = -1;
  fData->vertex_kinetic_energy = 0.0;
  fData->creation_process = "";
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
