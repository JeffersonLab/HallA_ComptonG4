/*
 * VComptonG4SensitiveDetectorIO.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef VCOMPTONG4SENSITIVEDETECTORIO_HH_
#define VCOMPTONG4SENSITIVEDETECTORIO_HH_

#include <map>
#include <vector>

// Forward declarations
class TTree;

/*
 *
 */
class VComptonG4SensitiveDetectorIO {
public:
  VComptonG4SensitiveDetectorIO();
  virtual ~VComptonG4SensitiveDetectorIO();

  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *branch) = 0;
};

#endif /* VCOMPTONG4SENSITIVEDETECTORIO_HH_ */
