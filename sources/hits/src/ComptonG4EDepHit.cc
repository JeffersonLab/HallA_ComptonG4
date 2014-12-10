#include "ComptonG4EDepHit.hh"
#include <G4Step.hh>


/*
 * Default constructor
 */
ComptonG4EDepHit::ComptonG4EDepHit() : ComptonG4Hit(new ComptonG4EDepData())
{

}

/*
 * Constructor: Instatiate from already existing data
 */
ComptonG4EDepHit::ComptonG4EDepHit(ComptonG4EDepData *ptr) :
  ComptonG4Hit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4EDepHit::ComptonG4EDepHit(const ComptonG4EDepHit &right) :
  ComptonG4Hit(new ComptonG4EDepData())
{
  Clone(&right);
}



/*
 *
 */
ComptonG4EDepHit::~ComptonG4EDepHit()
{
}

/*
 * 
 */
void ComptonG4EDepHit::ClearHit()
{
  ComptonG4Hit::ClearHit();
  data().energy_deposited = 0.0;
}

/*
 * Process step information
 */
void ComptonG4EDepHit::ProcessStep(G4Step *step)
{
  ComptonG4Hit::ProcessStep(step);
  SetEnergyDeposited(step->GetTotalEnergyDeposit());
}


/*
 * Process track information
 */
void ComptonG4EDepHit::ProcessTrack(G4Track *track)
{
  ComptonG4Hit::ProcessTrack(track);
}
