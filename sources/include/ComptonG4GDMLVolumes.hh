#ifndef _COMPTONG4GDMLVOLUMES_H
#define _COMPTONG4GDMLVOLUMES_H

#include <G4ThreeVector.hh>
#include <G4GDMLReadStructure.hh>

class G4GDMLParser;
class G4VPhysicalVolume;
class G4LogicalVolume;

typedef struct {
  G4String name;
  std::vector<G4ThreeVector> positions;
  std::vector<G4RotationMatrix*> rotations;
} ComptonG4GDMLVolumeCopy;

typedef struct {
  G4GDMLParser *parser;
  G4String fileName;
  std::vector<ComptonG4GDMLVolumeCopy> volumes;
} ComptonG4GDMLParser;

/*!
 * @class ComptonG4GDMLVolumes
 *
 * @brief Manages volumes read in from a GDML file (in addition to the
 *  world volume)
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4GDMLVolumes {
public:
  ComptonG4GDMLVolumes();
  ~ComptonG4GDMLVolumes();
  void SetReadGDMLFile(G4String fileName);
  void AddVolume(G4String volName);
  void SetVolumePosition(G4ThreeVector pos);
  void SetVolumeRotation(G4RotationMatrix *rot);
  G4GDMLAuxListType GetVolumeAuxiliaryInformation(G4LogicalVolume *vol);
  G4VPhysicalVolume* GetWorldFromFile(G4String fileName);
  void Construct(G4LogicalVolume *world);
private:
  std::vector<ComptonG4GDMLParser> fParsers;
  G4GDMLParser *fWorldParser;
  bool fCheckOverlap;
  int fCurrentParserIndex;
  int fCurrentVolumeIndex;
  int fCurrentVolumeCopyIndex;
  bool CheckValidParser();
  bool CheckValidVolume();
};

#endif
