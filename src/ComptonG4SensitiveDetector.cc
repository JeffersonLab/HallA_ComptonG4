/*
 * ComptonG4SensitiveDetector.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"

ComptonG4SensitiveDetector::ComptonG4SensitiveDetector(
    G4String name, ComptonG4Analysis *analysis) :
  G4VSensitiveDetector(name),
  fAnalysis(analysis)
{

}

ComptonG4SensitiveDetector::~ComptonG4SensitiveDetector()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4SensitiveDetector::Initialize(G4HCofThisEvent *)
{
}

G4bool ComptonG4SensitiveDetector::ProcessHits(G4Step*, G4TouchableHistory*)
{
  return false;
}

void ComptonG4SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}


