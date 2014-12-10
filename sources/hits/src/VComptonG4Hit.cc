#include "VComptonG4Hit.hh"

/*
 *
 */
VComptonG4Hit::VComptonG4Hit(VComptonG4Data *ptr) : fData(ptr)
{
}

/*
 *
 */
VComptonG4Hit::~VComptonG4Hit()
{
}

void VComptonG4Hit::Clone(const VComptonG4Hit* other)
{
  if(!fData)
    fData = new VComptonG4Data(); // Create generic one

  *fData = *other->fData;
}
