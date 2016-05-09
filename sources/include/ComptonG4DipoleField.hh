/*
 * ComptonG4DipoleField.hh
 *
 *  Created on: Jun 14, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4DIPOLEFIELD_HH
#define COMPTONG4DIPOLEFIELD_HH

#include <G4MagneticField.hh>
#include <G4RotationMatrix.hh>

#include <vector>

class ComptonG4DipoleField : public G4MagneticField {
public:
  ComptonG4DipoleField();
  ComptonG4DipoleField(G4String file_name, G4double xoffset, G4double yoffset,
      G4double zoffset, G4double phi, G4double theta);
  virtual ~ComptonG4DipoleField();
  virtual void GetFieldValue(const G4double location[4],
      G4double *fieldValue) const;
  void InitFromFile(G4String file_name);
  void AddNewActiveRegion( G4ThreeVector trans, G4RotationMatrix *rot);

  static const G4double kGausToTeslaConversion;
private:
  std::vector<G4double> fCoordinate[3];
  std::vector<std::vector<std::vector<G4double> > > fMagneticField;
  std::vector<G4ThreeVector> fOffsets;
  std::vector<G4RotationMatrix> fRotations;
  G4double fMaxCoordinate[3];
  G4double fMinCoordinate[3];
};

#endif // COMPTONG4DIPOLEFIELD_HH
