#ifndef _COMPTONG4ANALYSIS_H
#define _COMPTONG4ANALYSIS_H


/*!
 * @class ComptonG4Analysis
 *
 * @brief Default class to process the simulation data
 *
 * This class processes the simulation data, including writing to a ROOT file
 * when the event is finished.
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4Analysis {
public:
  ComptonG4Analysis();
  ~ComptonG4Analysis();

  // Public G4 Analyzer functions
  void Initialize();
  void StartOfEvent();
  void EndOfEvent();
};

#endif
