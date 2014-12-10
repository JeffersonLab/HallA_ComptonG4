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

  // ROOT stuff
  ComptonG4PrimaryData() : volume("") {}
  virtual ~ComptonG4PrimaryData(){}
  ClassDef(ComptonG4PrimaryData,1);
};

typedef std::vector<ComptonG4PrimaryData> VecComptonG4PrimaryData;
#endif /* COMPTONG4PRIMARYDATA_HH */
