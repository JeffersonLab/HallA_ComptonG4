/*
 * ComptonG4Data.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4DATA_HH
#define COMPTONG4DATA_HH

#include "VComptonG4Data.hh"
#include <vector>
#include <string>

/*
 * The data that can be stored in a rootfile
 */
class ComptonG4Data : public VComptonG4Data {
public:
  Double_t  position[3];      // [mm] End position in mm (x,y,z)
  Double_t  global_time;      // [ns] Time since start of event/entry
  Double_t  local_time;       // [ns] Time since start of track
  //Double_t ProperTime;        // [ns] Time in particle's frame or reference
  Double_t  direction[3];     // [unitless] Momentum direction
  Double_t  momentum[3];      // [MeV/c] Momentum
  Double_t  kinetic_energy;   // [MeV] Kinetic Energy
  Double_t  total_length;     // [mm] Total length traveled up to this point
  Int_t     particle_id;      // [unitless] Particle ID
  //std::string particle_name;  // [string] Full particle name
  Int_t     track_id;         // [unitless] ID of this track
  Int_t     parent_id;        // [unitelss] ID of parent track
  Int_t     step_number;      // [unitless] Step number
  Int_t     track_status;     // [unitless] Track status (G4TrackStatus)
  Double_t  vertex_position[3];   // [mm] Start position in mm (x,y,z)
  Double_t  vertex_direction[3];  // [unitless] Start Momentum direction
  Double_t  vertex_kinetic_energy;// [MeV] Start Kinetic Energy
  std::string creation_process;   // [string] Name of process that created track

  // ROOT stuff
  ComptonG4Data(){}
  virtual ~ComptonG4Data(){}
  ClassDef(ComptonG4Data,1);
};

#endif /* COMPTONG4DATA_HH */
