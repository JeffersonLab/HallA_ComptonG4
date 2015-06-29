/*
 * ComptonG4BeamData.hh
 *
 *  Created on: May 24, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4BEAMDATA_HH
#define COMPTONG4BEAMDATA_HH

#include "VComptonG4Data.hh"
#include <vector>
#include <string>


/*
 * The data that can be stored in a rootfile
 */
class ComptonG4BeamData : public VComptonG4Data {
public:
  // Starting Information
  Double_t  position[3];      // [mm] End position in mm (x,y,z)
  Double_t  direction[3];     // [unitless] Momentum direction
  Double_t  momentum[3];      // [MeV/c] Momentum
  Double_t  polarization[3];  // [unitless] Polarization

  /*
  Double_t  kinetic_energy;   // [MeV] Kinetic Energy
  Double_t  total_length;     // [mm] Total length traveled up to this point
  Int_t     track_id;         // [unitless] ID of this track
  Int_t     parent_id;        // [unitelss] ID of parent track
  Int_t     step_number;      // [unitless] Step number
  ComptonG4OpticalProcess  creation_process;// [enum] Process that created track

  Bool_t    recorded_end;     // Whether we cleanly recorded the end of this
                              // track (as opposed to just loosing it)
  */

  // ROOT stuff
  ComptonG4BeamData(){}
  virtual ~ComptonG4BeamData(){}
  ClassDef(ComptonG4BeamData,1);
};

#endif /* COMPTONG4BEAMDATA_HH */
