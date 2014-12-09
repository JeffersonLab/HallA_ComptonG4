/*
 * VComptonG4Data.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef VCOMPTONG4DATA_HH
#define VCOMPTONG4DATA_HH
#include <Rtypes.h>
#include <vector>
#include <TObject.h>

/*
 * The data that can be stored in a rootfile
 */
class VComptonG4Data : public TObject {
public:
  // ROOT stuff
  VComptonG4Data(){}
  virtual ~VComptonG4Data(){}
  ClassDef(VComptonG4Data,1);
};

#endif /* VCOMPTONG4DATA_HH */
