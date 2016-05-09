#include "ComptonG4DipoleField.hh"
#include <CLHEP/Units/SystemOfUnits.h>

#include <fstream>

const G4double ComptonG4DipoleField::kGausToTeslaConversion = 1e-4*CLHEP::tesla;

ComptonG4DipoleField::ComptonG4DipoleField()
{

}

ComptonG4DipoleField::~ComptonG4DipoleField()
{

}

void ComptonG4DipoleField::GetFieldValue(const G4double location[4],
    G4double *fieldValue) const
{
  size_t numy = fCoordinate[1].size();
  size_t numz = fCoordinate[2].size();
  fieldValue[0] = 0.0;
  fieldValue[1] = 0.0;
  fieldValue[2] = 0.0;

  if( numy == 0 || numz  == 0) {
    return;
  }

  // First, let's find out if they even fall within this field's definition.
  // To do so, we have to first convert gloabl coordinate to the local
  // coordinate of the field.
  G4double y = 0.0*CLHEP::cm;
  G4double z = 0.0*CLHEP::cm;
  G4int rotationIndex = -1;

  G4bool insideField = false;
  for( size_t i = 0; i < fOffsets.size() && !insideField; i++ ) {
    G4ThreeVector v = fRotations[i].inverse()(G4ThreeVector(location[0],
          location[1],location[2])-fOffsets[i]);
    if ( v.x() >= fMinCoordinate[0] && v.x() <= fMaxCoordinate[0] &&
        v.y() >= fMinCoordinate[1] && v.y() <= fMaxCoordinate[1] &&
        std::abs(v.z()) >= fMinCoordinate[2] &&
          std::abs(v.z()) <= fMaxCoordinate[2] ) {
      insideField = true;
      rotationIndex = i;
      y = v.y();
      z = std::abs(v.z());
    }
  }

  if( !insideField ) {
    return;
  }

  G4double tmpdelta;

  // Find the index that is closest in y
  size_t indexy = 0;
  G4double deltay = 1e5;
  for(size_t i = 0; i < numy; i++ ) {
    tmpdelta = (fCoordinate[1][i] - y);
    if( std::abs(tmpdelta) < std::abs(deltay) ) { // Found smallest delta so far
      deltay = tmpdelta;
      indexy = i;
    }
  }

  // Find the index that is closest in z
  size_t indexz = 0;
  G4double deltaz = 1e5;
  for(size_t i = 0; i < numz; i++ ) {
    tmpdelta = (fCoordinate[2][i] - z);
    if( std::abs(tmpdelta) < std::abs(deltaz) ) { // Found smallest delta so far
      deltaz = tmpdelta;
      indexz = i;
    }
  }

  // Since at the moment we only want the fastest way of doing it...not the best
  // extrapolation, then just return the field at those minimim deltas.
  // However, we still have to rotate the field to the volume coordinates
  G4ThreeVector field(fMagneticField[0][indexy][indexz],0.0,0.0);
  G4ThreeVector fieldRotated = fRotations[rotationIndex](field);
  fieldValue[0] = fieldRotated.x();
  fieldValue[1] = fieldRotated.y();
  fieldValue[2] = fieldRotated.z();
}

void ComptonG4DipoleField::InitFromFile(G4String file)
{
  std::fstream in;
  in.open(file,std::ios::in);

  if( !in.is_open() ) {
    return;
  }

  std::vector<G4double> tmpYs;

  // For now, we are ignoring the x's in the Geant4 coordinate system
  // So we resize the magnetic Bx vector to 1
  fCoordinate[0].push_back(0.0);
  fMinCoordinate[0] = -3*CLHEP::cm;
  fMaxCoordinate[0] = +3*CLHEP::cm;
  fMagneticField.resize(fCoordinate[0].size());

  // First read the number of columns present
  int cols;
  in >> cols;

  // Now that we know the number of columns, let's resize the By vector
  fMagneticField[0].resize(cols);

  G4double tmpCord;
  // Now read in all the x coordinates
  for(int i = 0; i < cols && !in.eof(); i++ ) {
    in >> tmpCord;
    fCoordinate[1].push_back(tmpCord*CLHEP::cm);
    if(i == 0) {
      fMinCoordinate[1] = fMaxCoordinate[1] = tmpCord*CLHEP::cm;
    } else {
      if(tmpCord*CLHEP::cm > fMaxCoordinate[1])
        fMaxCoordinate[1] = tmpCord*CLHEP::cm;
      if(tmpCord*CLHEP::cm < fMinCoordinate[1])
        fMinCoordinate[1] = tmpCord*CLHEP::cm;
    }
  }

  G4bool first = true;
  G4double z, b;
  // Now read in the magnetic fields
  while( in >> z && !in.eof() ) {
    z = std::abs(z);
    fCoordinate[2].push_back(std::abs(z*CLHEP::cm)); // symmetry in z
    if(first) {
      fMinCoordinate[2] = fMaxCoordinate[2] = z*CLHEP::cm;
      first = false;
    } else {
      if(z*CLHEP::cm > fMaxCoordinate[2])
        fMaxCoordinate[2] = z*CLHEP::cm;
      if(z*CLHEP::cm < fMinCoordinate[2])
        fMinCoordinate[2] = z*CLHEP::cm;
    }
    for(int i = 0; i < cols && !in.eof() && in >> b; i++ ) {
      // The multiplier is to account for the fact that we ran it at 103 A
      // but the tests where for 110 A
      fMagneticField[0][i].push_back(b*kGausToTeslaConversion*0.936363636364);
    }
  }
}


void ComptonG4DipoleField::AddNewActiveRegion( G4ThreeVector trans,
    G4RotationMatrix *rot)
{
  fOffsets.push_back(trans);
  if( rot == 0 ) {
    fRotations.push_back(G4RotationMatrix());
  } else {
    fRotations.push_back(*rot); // Store the inverse matrix
  }
}
