#include "ComptonG4OpticalHit.hh"
#include <G4Step.hh>

/*
 * Default constructor
 */
ComptonG4OpticalHit::ComptonG4OpticalHit() :
  ComptonG4Hit(new ComptonG4OpticalData())
{
  ClearHit();
}

/*
 * Constructor: initialize from already defined data
 */
ComptonG4OpticalHit::ComptonG4OpticalHit(ComptonG4OpticalData *ptr) :
  ComptonG4Hit(ptr)
{
  if(GetDataRef())
    ClearHit();
}

/*
 * Copy constructor
 */
ComptonG4OpticalHit::ComptonG4OpticalHit(const ComptonG4OpticalHit &right) :
  ComptonG4Hit(new ComptonG4OpticalData())
{
  Clone(&right);
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
  data().num_photons = 0;
}


/*
 * Process step information
 */
void ComptonG4OpticalHit::ProcessStep(G4Step *step)
{
  ComptonG4Hit::ProcessStep(step);
  NewPhoton();
}
