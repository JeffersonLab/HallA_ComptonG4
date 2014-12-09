#include "ComptonG4EDepHit.hh"
#include <G4Step.hh>

/*
 *
 */
ComptonG4EDepHit::ComptonG4EDepHit() : ComptonG4Hit(0), fData(0)
{
  fData = new ComptonG4EDepData();
  ComptonG4Hit::SetDataRef(fData);
  ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4EDepHit::ComptonG4EDepHit(const ComptonG4EDepHit &right) :
  ComptonG4Hit(right), fData(0)
{
  fData = new ComptonG4EDepData();
  ComptonG4Hit::SetDataRef(fData);
  CopyData(*right.fData);
}


/*
 *
 */
ComptonG4EDepHit::ComptonG4EDepHit(ComptonG4EDepData *data) :
  ComptonG4Hit(data), fData(data)
{
  ClearHit();
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
  fData->energy_deposited = 0.0;
}

/*
 * Process step information
 */
void ComptonG4EDepHit::ProcessStep(G4Step *step)
{
  ComptonG4Hit::ProcessStep(step);
  SetEnergyDeposited(step->GetTotalEnergyDeposit());
}
