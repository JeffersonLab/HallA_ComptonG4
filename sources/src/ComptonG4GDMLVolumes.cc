////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////

#include "ComptonG4GDMLVolumes.hh"

// GEANT4 Geometry related includes
#include <G4GDMLParser.hh>
#include <G4PVPlacement.hh>

ComptonG4GDMLVolumes::ComptonG4GDMLVolumes() : fWorldParser(0),
  fCheckOverlap(false),fCurrentParserIndex(-1),fCurrentVolumeIndex(-1),
  fCurrentVolumeCopyIndex(-1)
{
}


ComptonG4GDMLVolumes::~ComptonG4GDMLVolumes()
{
  if(fWorldParser)
    delete fWorldParser;
}

G4VPhysicalVolume* ComptonG4GDMLVolumes::GetWorldFromFile(G4String fileName)
{
  fWorldParser = new G4GDMLParser();
  fWorldParser->Read(fileName);
  return fWorldParser->GetWorldVolume();
}

G4GDMLAuxListType ComptonG4GDMLVolumes::GetVolumeAuxiliaryInformation(
    G4LogicalVolume *vol)
{
  G4GDMLAuxListType auxInfo;
  // First, search the world
  if(fWorldParser) {
    auxInfo = fWorldParser->GetVolumeAuxiliaryInformation(vol);

    // If  this parser knows about this logical volume, then we are done!
    if(auxInfo.size() >0)
      return auxInfo;
  }

  // Parse through all the volumes and see if they know about this volume
  for(size_t i = 0; i < fParsers.size(); i++) {
    auxInfo = fParsers[i].parser->GetVolumeAuxiliaryInformation(vol);

    // If  this parser knows about this logical volume, then we are done!
    if(auxInfo.size() > 0)
      return auxInfo;
  }

  // If we ge to here, it's very very likely that we didn't find it.
  // It's likely that no auxiliary information was set for this volume,
  // so this is an acceptable outcome.
  return auxInfo;
}

void ComptonG4GDMLVolumes::SetReadGDMLFile(G4String fileName)
{
  // Reset current parser and volume indices
  fCurrentParserIndex = -1;
  fCurrentVolumeIndex = -1;

  // Find out if this file name is already known, and if we already
  // have a parser for it.
  for(size_t i = 0; i < fParsers.size(); i++) {
    if( fileName == fParsers[i].fileName ) { // match found!
      fCurrentParserIndex = int(i);
      return;
    }
  }

  // If we reach this point, then this file has not been read before and
  // we should create a new parser.
  fCurrentParserIndex = int(fParsers.size());
  fParsers.push_back(ComptonG4GDMLParser());
  fParsers[fCurrentParserIndex].parser = new G4GDMLParser();
  fParsers[fCurrentParserIndex].fileName = fileName;
}

bool ComptonG4GDMLVolumes::CheckValidParser()
{
  if(fCurrentParserIndex==-1) {
    G4cerr << "No GDML specified before trying to add volume."
      << G4endl;
    return false;
  }

  return true;
}

bool ComptonG4GDMLVolumes::CheckValidVolume()
{
  if(fCurrentVolumeIndex==-1) {
    G4cerr << "No volume specified before trying to manipulate volume."
      << G4endl;
    return false;
  }
  return true;
}


void ComptonG4GDMLVolumes::AddVolume(G4String volName)
{
  if(!CheckValidParser())
    return;
  fCurrentVolumeIndex = -1;

  // Search to see if this volume already exists
  for(size_t j = 0; j < fParsers[fCurrentParserIndex].volumes.size()
      && fCurrentVolumeIndex == -1; j++) {
    if(fParsers[fCurrentParserIndex].volumes[j].name == volName) {
      fCurrentVolumeIndex = int(j);
    }
  }

  if(fCurrentVolumeIndex == -1) {
    fCurrentVolumeIndex = int(fParsers[fCurrentParserIndex].volumes.size());
    fParsers[fCurrentParserIndex].volumes.push_back(ComptonG4GDMLVolumeCopy());
    fParsers[fCurrentParserIndex].volumes[fCurrentVolumeIndex].name = volName;
  }
  fCurrentVolumeCopyIndex = fParsers[fCurrentParserIndex].
    volumes[fCurrentVolumeIndex].positions.size();

  fParsers[fCurrentParserIndex].volumes[fCurrentVolumeIndex].positions.
    push_back(G4ThreeVector(0.0,0.0,0.0));
  fParsers[fCurrentParserIndex].volumes[fCurrentVolumeIndex].rotations.
    push_back(0);
}

void ComptonG4GDMLVolumes::SetVolumePosition(G4ThreeVector pos)
{
  if(!CheckValidVolume())
    return;

  fParsers[fCurrentParserIndex].volumes[fCurrentVolumeIndex].
    positions[fCurrentVolumeCopyIndex] = pos;
}

void ComptonG4GDMLVolumes::SetVolumeRotation(G4RotationMatrix *rot)
{
  if(!CheckValidVolume())
    return;

  fParsers[fCurrentParserIndex].volumes[fCurrentVolumeIndex].
    rotations[fCurrentVolumeCopyIndex] = rot;
}

void ComptonG4GDMLVolumes::Construct(G4LogicalVolume *world)
{
  // Loop over GDLM files
  for(size_t i = 0; i < fParsers.size(); i++) {
    G4GDMLParser *parser = fParsers[i].parser;
    parser->Read(fParsers[i].fileName);

    // Loop over volume names
    for(size_t j = 0; j < fParsers[i].volumes.size(); j++ ) {
      G4String volName = fParsers[i].volumes[j].name;
      G4LogicalVolume *vol = parser->GetVolume(volName);
      // Loop over positions for given volume names (copies)
      for(size_t k = 0; k < fParsers[i].volumes[j].positions.size(); k++) {
        new G4PVPlacement(fParsers[i].volumes[j].rotations[k],
            fParsers[i].volumes[j].positions[k],
            vol,volName,world,false,k,fCheckOverlap);
      }
    }
  }
}
