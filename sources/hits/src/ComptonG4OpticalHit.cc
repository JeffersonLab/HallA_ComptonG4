#include "ComptonG4OpticalHit.hh"
#include <G4Step.hh>

/*
 *
 */
ComptonG4OpticalHit::ComptonG4OpticalHit() : ComptonG4Hit(0), fData(0)
{
  fData = new ComptonG4OpticalData();
  ComptonG4Hit::SetDataRef(fData);
  ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4OpticalHit::ComptonG4OpticalHit(const ComptonG4OpticalHit &right) :
  ComptonG4Hit(0), fData(0)
{
  fData = new ComptonG4OpticalData();
  ComptonG4Hit::SetDataRef(fData);
  CopyData(*right.fData);
}


/*
 *
 */
ComptonG4OpticalHit::ComptonG4OpticalHit(ComptonG4OpticalData *data) :
  ComptonG4Hit(data), fData(data)
{
  ClearHit();
}


/*
 *
 */
ComptonG4OpticalHit::~ComptonG4OpticalHit()
{
}

/*
 *
 */
void ComptonG4OpticalHit::ClearHit()
{
  ComptonG4Hit::ClearHit();
  fData->num_photons = 0;
}


/*
 * Process step information
 */
void ComptonG4OpticalHit::ProcessStep(G4Step *step)
{
  ComptonG4Hit::ProcessStep(step);
}
