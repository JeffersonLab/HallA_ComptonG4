/*
 * ComptonG4OpticalData.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4OPTICALDATA_HH
#define COMPTONG4OPTICALDATA_HH

#include "ComptonG4Data.hh"
#include <vector>

/*
 *
 */
class ComptonG4OpticalData : public ComptonG4Data {
public:
  Int_t num_photons;  // [unitless] Number of photons absorbed

  // ROOT stuff
  ComptonG4OpticalData(){}
  virtual ~ComptonG4OpticalData(){}
  ClassDef(ComptonG4OpticalData,1);
};

typedef std::vector<ComptonG4OpticalData> VecComptonG4OpticalData;
#endif /* COMPTONG4DATA_HH */
