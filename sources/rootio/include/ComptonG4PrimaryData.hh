/*
 * ComptonG4PrimaryData.hh
 *
 *  Created on: Dec 09, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4PRIMARYDATA_HH
#define COMPTONG4PRIMARYDATA_HH

#include "ComptonG4EDepData.hh"
#include <vector>

/*
 *
 */
class ComptonG4PrimaryData : public ComptonG4EDepData {
public:
  std::string volume;  // [string] Volume name of place of interaction
  Double_t  pre_position[3];   // [mm] Start position in mm (x,y,z)
  Double_t  pre_direction[3];  // [unitless] Start Momentum direction
  Double_t  pre_kinetic_energy;// [MeV] Start Kinetic Energy
  Int_t     pre_status;     // [unitless] Start status (G4StepStatus)
  Double_t  post_position[3];   // [mm] End position in mm (x,y,z)
  Double_t  post_direction[3];  // [unitless] End Momentum direction
  Double_t  post_kinetic_energy;// [MeV] End Kinetic Energy
  Int_t     post_status;     // [unitless] End status (G4StepStatus)

  // ROOT stuff
  ComptonG4PrimaryData() : volume("") {}
  virtual ~ComptonG4PrimaryData(){}
  ClassDef(ComptonG4PrimaryData,2);
};

typedef std::vector<ComptonG4PrimaryData> VecComptonG4PrimaryData;
#endif /* COMPTONG4PRIMARYDATA_HH */
