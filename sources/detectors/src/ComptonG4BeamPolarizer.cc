/*
 * ComptonG4BeamPolarizer.cc
 *
 *  Created on: Jun 14, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4BeamPolarizer.hh"
#include "ComptonG4Analysis.hh"
#include <G4Electron.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>
#include <TTree.h>

#include "ComptonG4Utils.hh"

ComptonG4BeamPolarizer::ComptonG4BeamPolarizer(
    G4String name) :
  VComptonG4SensitiveDetector(name)
{
}


ComptonG4BeamPolarizer::~ComptonG4BeamPolarizer()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4BeamPolarizer::Initialize(G4HCofThisEvent *)
{
  CleanEvent();
}

G4bool ComptonG4BeamPolarizer::ProcessHits(G4Step* step,
    G4TouchableHistory*)
{
  G4Track *track = step->GetTrack();
  if( !track ) return false;

  if(track->GetParentID() != 0) // Not a primary particle (i.e. beam, so exit)
    return false;

  // If it is not an electron then exit too
  if(track->GetDefinition() != G4Electron::Definition())
    return false;

  G4VPhysicalVolume *vol = track->GetVolume();
  int volIndex  = GetIndex(vol);

  // Sanity check, ensure volumes were properly registered
  if(volIndex < 0 ) {
    return false;
  }

  // Finally just process the electron (beam)
  ComptonG4BeamHit *hit = fBeamPolarizers[volIndex]->NewHit();
  hit->ProcessTrack(track);

  return true;
}

void ComptonG4BeamPolarizer::EndOfEvent(G4HCofThisEvent*)
{
  for(size_t i = 0; i < fBeamPolarizers.size(); i++ ) {
    fBeamPolarizers[i]->FillTree();
  }
}

void ComptonG4BeamPolarizer::CleanEvent()
{
  for(size_t i = 0; i < fBeamPolarizers.size(); i++ ) {
    fBeamPolarizers[i]->ClearAndInit();
  }
}


void ComptonG4BeamPolarizer::CreateTreeBranch(TTree* tree)
{
  for(size_t i = 0; i < fBeamPolarizers.size(); i++ ) {
    fBeamPolarizers[i]->CreateTreeBranch(tree);
  }
}

void ComptonG4BeamPolarizer::SetOptions(std::map<G4String,G4String>,
    bool)
{
}

// Finally register the class
COMPTONG4_SD_REGISTER(ComptonG4BeamPolarizer)
