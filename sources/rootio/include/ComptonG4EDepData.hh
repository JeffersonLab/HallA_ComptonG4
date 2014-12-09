/*
 * ComptonG4EDepData.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4EDEPDATA_HH
#define COMPTONG4EDEPDATA_HH

#include "ComptonG4Data.hh"
#include <vector>

/*
 *
 */
class ComptonG4EDepData : public ComptonG4Data {
public:
  Double_t energy_deposited;  // [MeV] Energy deposited by this step

  // ROOT stuff
  ComptonG4EDepData(){}
  virtual ~ComptonG4EDepData(){}
  ClassDef(ComptonG4EDepData,1);
};

#endif /* COMPTONG4DATA_HH */
