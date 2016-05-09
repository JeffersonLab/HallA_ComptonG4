#include "ComptonG4BeamHit.hh"
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>


/*
 * Default constructor
 */
ComptonG4BeamHit::ComptonG4BeamHit() :
  VComptonG4Hit(new ComptonG4BeamData())
{
  ClearHit();
}

/*
 * Constructor where we initialize from already defined
 */
ComptonG4BeamHit::ComptonG4BeamHit(ComptonG4BeamData *ptr) :
  VComptonG4Hit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4BeamHit::ComptonG4BeamHit(
    const ComptonG4BeamHit &right) :
  VComptonG4Hit(new ComptonG4BeamData())
{
  Clone(&right);
}

/*
 * ClearHit
 */
void ComptonG4BeamHit::ClearHit()
{
  if(!GetDataRef())
    return;

  SetPosition(G4ThreeVector(0.0,0.0,0.0));
  SetDirection(G4ThreeVector(0.0,0.0,0.0));
  SetMomentum(G4ThreeVector(0.0,0.0,0.0));
  SetPolarization(G4ThreeVector(0.0,0.0,0.0));
}

/*
 *
 */
ComptonG4BeamHit::~ComptonG4BeamHit()
{
  ClearHit();
}

/*
 * Process step information
 */
void ComptonG4BeamHit::ProcessStep(G4Step *step)
{

  ProcessTrack(step->GetTrack());
}

/*
 * Process track information
 */
void ComptonG4BeamHit::ProcessTrack(const G4Track *track)
{
  SetPosition(track->GetPosition());
  SetDirection(track->GetMomentumDirection());
  SetMomentum(track->GetMomentum());
  SetPolarization(track->GetPolarization());
}
