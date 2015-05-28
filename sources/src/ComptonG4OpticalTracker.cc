/*
 * ComptonG4OpticalTracker.cc
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4OpticalTracker.hh"
#include "ComptonG4HitsCollection.hh"
#include "ComptonG4Utils.hh"

// Geant4 includes
#include <G4OpticalPhoton.hh>
#include <G4Track.hh>

// ROOT includes
#include <TTree.h>

ComptonG4OpticalTracker::ComptonG4OpticalTracker(G4bool enable) :
  fEnabled(enable), fOpticalTracks("optical_tracker")
{
}


ComptonG4OpticalTracker::~ComptonG4OpticalTracker()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4OpticalTracker::ProcessNewTrack(const G4Track *track)
{
  if(!fEnabled)
    return;

  // Ensure this is an optical photon
  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    ComptonG4OpticalTrackerHit *hit = fOpticalTracks.NewHit();
    hit->ProcessTrack(track,true);
  }
}


void ComptonG4OpticalTracker::CreateTreeBranch(TTree* tree)
{
  // Create output tree branch if enabled
  if(fEnabled)
    fOpticalTracks.CreateTreeBranch(tree);
}

void ComptonG4OpticalTracker::ProcessTrack(const G4Track *track,
    G4bool detected)
{
  if(!fEnabled)
    return;

  // Ensure this is an optical photon
  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    // Alright, now find the old track to store things in
    G4int track_id = track->GetTrackID();
    for(size_t i = 0; i < fOpticalTracks.size(); i++ ) {
      if(track_id == fOpticalTracks[i]->GetTrackID()) {
        fOpticalTracks[i]->ProcessTrack(track,false,detected);
        return;
      }
    }
  }
}
