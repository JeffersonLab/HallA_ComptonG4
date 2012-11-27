#ifndef _ComptonG4PhysicsLIST_H
#define _ComptonG4PhysicsLIST_H


#include "G4VUserPhysicsList.hh"

/*!
 * @class ComptonG4PhysicsList
 *
 * @brief Standard Compton Polarimeter physics list
 *
 * Defines the physics particles and processes that will be used in the
 * simulation.
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4PhysicsList : public G4VUserPhysicsList {
public:
  ComptonG4PhysicsList();
  ~ComptonG4PhysicsList();
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

private:
  void ConstructEM();
  void ConstructDecay();
};

#endif
