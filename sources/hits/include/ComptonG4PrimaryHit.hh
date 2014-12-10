/*
 * ComptonG4PrimaryHit.hh
 *
 *  Created on: Dec 09, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PRIMARYHIT_HH
#define COMPTONG4PRIMARYHIT_HH

#include "ComptonG4EDepHit.hh"
#include "ComptonG4PrimaryData.hh"
class G4Step;
class G4Track;

/*
 *
 */
class ComptonG4PrimaryHit : public ComptonG4EDepHit {
public:
  ComptonG4PrimaryHit();
  ComptonG4PrimaryHit(ComptonG4PrimaryData *data);
  ComptonG4PrimaryHit(const ComptonG4PrimaryHit &right);
  virtual ~ComptonG4PrimaryHit();
  virtual void ClearHit();
  ComptonG4PrimaryData GetData() { return data(); }

  virtual void ProcessStep(G4Step* step);
  virtual void ProcessTrack(G4Track* track);

  virtual ComptonG4PrimaryData& data() { return
    dynamic_cast<ComptonG4PrimaryData&>(ComptonG4EDepHit::data()); }

  /* Getters */
  std::string GetVolume() { return data().volume; }
  /*  Setters */
  void SetVolume(std::string vol) { data().volume = vol; }
};

#endif /* COMPTONG4HIT_HH */
