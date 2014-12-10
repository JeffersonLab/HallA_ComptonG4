/*
 * VComptonG4Hit.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef VCOMPTONG4HIT_HH
#define VCOMPTONG4HIT_HH

// Include ROOT default types (so it can be written to a ROOT file by default)
#include <Rtypes.h>

// Parent Hit class
#include <G4VHit.hh>
#include <VComptonG4Data.hh>

/*
 *
 */
class VComptonG4Hit : public G4VHit {
public:
  VComptonG4Hit(VComptonG4Data *data = 0);

  virtual ~VComptonG4Hit();
  virtual void ClearHit() = 0;
  VComptonG4Data GetData() const { return *fData; }

  virtual VComptonG4Data& data() { return *fData; }
  virtual VComptonG4Data* GetDataRef() { return fData; }
  void SetData(VComptonG4Data* data) { fData = data; }
protected:
  const VComptonG4Data* ConstPointer() const { return fData; }
  void Clone(const VComptonG4Hit* other);
private:
  VComptonG4Data *fData;
};

#endif /* VCOMPTONG4HIT_HH */
