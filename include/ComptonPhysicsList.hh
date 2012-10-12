#ifndef _COMPTONPHYSICSLIST_H
#define _COMPTONPHYSICSLIST_H


#include "G4VUserPhysicsList.hh"

class ComptonPhysicsList : public G4VUserPhysicsList {
public:
  ComptonPhysicsList();
  ~ComptonPhysicsList();
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();
};

#endif
