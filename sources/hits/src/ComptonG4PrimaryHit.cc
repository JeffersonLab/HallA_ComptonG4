#include "ComptonG4PrimaryHit.hh"
#include <G4Step.hh>

/*
 * Default constructor
 */
ComptonG4PrimaryHit::ComptonG4PrimaryHit() :
  ComptonG4EDepHit(new ComptonG4PrimaryData())
{
  ClearHit();
}

/*
 * Constructor: Initialize from already defined data
 */
ComptonG4PrimaryHit::ComptonG4PrimaryHit(ComptonG4PrimaryData *ptr) :
  ComptonG4EDepHit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4PrimaryHit::ComptonG4PrimaryHit(const ComptonG4PrimaryHit &right) :
  ComptonG4EDepHit(new ComptonG4PrimaryData())
{
  Clone(&right);
}


/*
 *
 */
ComptonG4PrimaryHit::~ComptonG4PrimaryHit()
{
}

/*
 *
 */
void ComptonG4PrimaryHit::ClearHit()
{
  ComptonG4EDepHit::ClearHit();
  data().volume = "";
}


/*
 * Process step information
 */
void ComptonG4PrimaryHit::ProcessStep(G4Step *step)
{
  G4Track *track = step->GetTrack();
  if(track)
    ProcessTrack(track);
}

/*
 * Process track information
 */
void ComptonG4PrimaryHit::ProcessTrack(G4Track *track)
{
  ComptonG4EDepHit::ProcessTrack(track);
  data().volume = track->GetVolume()->GetName();
}
