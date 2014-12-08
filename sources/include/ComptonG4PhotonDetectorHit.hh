/*
 * ComptonG4PhotonDetectorHit.h
 *
 *  Created on: May 23, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PHOTONDETECTORHIT_H_
#define COMPTONG4PHOTONDETECTORHIT_H_

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

class ComptonG4PhotonDetectorHit: public G4VHit {
public:
  ComptonG4PhotonDetectorHit();
  virtual ~ComptonG4PhotonDetectorHit();
  ComptonG4PhotonDetectorHit(const ComptonG4PhotonDetectorHit &right);
  const ComptonG4PhotonDetectorHit& operator=(const ComptonG4PhotonDetectorHit &right);
  int operator==(const ComptonG4PhotonDetectorHit &right) const;

  inline void * operator new(size_t);
  inline void operator delete(void *aHit);

  void Draw();
  void Print();

  // Setters
  void SetEnergyDeposited(double e) { fEDep = e; }
  void SetPosition(G4ThreeVector p) { fPosition = p; }

  // Getters
  G4double GetEnergyDeposited() { return fEDep; }
  G4ThreeVector GetPosition() { return fPosition; }

private:
  G4double fEDep;
  G4ThreeVector fPosition;
};

typedef G4THitsCollection<ComptonG4PhotonDetectorHit> ComptonG4PhotonDetectorHitsCollection;

extern G4Allocator<ComptonG4PhotonDetectorHit> ComptonG4PhotonDetectorHitAllocator;

inline void* ComptonG4PhotonDetectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) ComptonG4PhotonDetectorHitAllocator.MallocSingle();
  return aHit;
}

inline void ComptonG4PhotonDetectorHit::operator delete(void *aHit)
{
  ComptonG4PhotonDetectorHitAllocator.FreeSingle((ComptonG4PhotonDetectorHit*) aHit);
}

#endif /* COMPTONG4PHOTONDETECTORHIT_H_ */
