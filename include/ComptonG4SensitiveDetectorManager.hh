/*
 * ComptonG4SensitiveDetectorManager.hh
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTORMANAGER_HH_
#define COMPTONG4SENSITIVEDETECTORMANAGER_HH_

#include <vector>

class ComptonG4Analysis;
class ComptonG4SensitiveDetector;

class ComptonG4SensitiveDetectorManager {
public:
  ComptonG4SensitiveDetectorManager(ComptonG4Analysis*);
  virtual ~ComptonG4SensitiveDetectorManager();

private:
  ComptonG4Analysis *fAnalysis;
  std::vector<ComptonG4SensitiveDetector*> fDetector;
};

#endif /* COMPTONG4SENSITIVEDETECTORMANAGER_HH_ */

