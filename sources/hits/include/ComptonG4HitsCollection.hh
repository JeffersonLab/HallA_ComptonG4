/*
 * ComptonG4HitsCollection.hh
 *
 *  Created on: May 27, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4HITSCOLLECTION_HH_
#define COMPTONG4HITSCOLLECTION_HH_

#include <VComptonG4Hit.hh>
#include <vector>

#include <TTree.h>

template <class H, class D>
class ComptonG4HitsCollection {
public:
  ComptonG4HitsCollection(G4String name = "") : fData(0), fName(name) {
    ClearAndInit();
  };

  virtual ~ComptonG4HitsCollection() {
    Clear();
  };

  /*
   * Set the tree name of this collection.
   */
  void SetName(G4String name) {
    fName = name;
  };

  /*
   * Create and initialize the Output Branch
   */
  virtual void CreateTreeBranch(TTree *tree) {
    // Create hit branch
    tree->Branch(fName.c_str(),&fData);
  }

  /*
   * Create a new hit and return a pointer to it
   */
  H* NewHit() {
    fHits.push_back(new H());
    return fHits.back();
  }

  /*
   * Initialize things for next event
   */
  void ClearAndInit() {
    Clear();
    fData = new std::vector<D>;
  };

  /*
   * Fill the event information
   */
  void FillTree() {
    for(size_t i = 0; i < fHits.size(); i++ ) {
      fData->push_back(fHits[i]->GetData());
    }
  }

  /*
   * Clear all data
   */
  void Clear() {
    // We own all the data in the hits, so it's safe to delete them
    for(size_t i = 0; i < fHits.size(); i++ ) {
      if(fHits[i]) {
        delete fHits[i];
      }
    }
    fHits.clear();
    if(fData) {
      delete fData;
    }
  };

  /*
   * Retrieve the number of hits in this collection
   */
  size_t size() { return fHits.size(); }

  /*
   * Operator to retreive a given hit
   */
  H* operator[](size_t i) { return Get(i); }

  /*
   * Get the i'th hit (doesn't check that it's valid)
   */
  H* Get(size_t i) { return fHits[i]; }

private:
  std::vector<H*> fHits;
  std::vector<D> *fData;
  G4String fName;
};

#endif /* COMPTONG4HITSCOLLECTION_HH_ */
