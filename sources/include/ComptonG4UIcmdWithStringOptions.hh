#ifndef COMPTONG4UICMDWITHSTRINGOPTIONS_H
#define COMPTONG4UICMDWITHSTRINGOPTIONS_H

#include <G4UIcommand.hh>
#include <map>

/*
 *
 */
class ComptonG4UIcmdWithStringOptions : public G4UIcommand {
public:
  /*
   * Constructor of G4UI command with string option(s)
   *
   * This string options can take the form of
   * 1) <options> (withReceiver == false)
   * 2) <receiver_name> <options> (withReceiver == true)
   *
   * \param command_path Full path of command
   * \param messenger Pointer to reffering messenger class
   * \param with_receiver Require receiver before options? (Default is no)
   */
  ComptonG4UIcmdWithStringOptions(const char *command_path,
      G4UImessenger *messenger, bool with_receiver = false);

  /*
   * Set whether a receiver is required (default is yes)
   *
   * \parama with_receiver [bool] require receiver? Default is true
   */
  void RequireReceiver(bool with_receiver = true);

  /*
   * Set delimiter that specifies split options
   *
   * \parama delimiter Delimiter for splitting options
   */
  void SetOptionsSplitDelimiter(G4String delimiter = ";");

  /*
   * Set delimiter that specifies asignment operator in options
   *
   * \parama delimiter Delimiter used in assignment operator in options
   */
  void SetOptionsAssignmentDelimiter(G4String delimiter = "=");



  /*
   * Set receiver name (calls "::"RequireReceiver(true))
   *
   * \parama name Set the required receivers name
   */
  void SetReceiverName(G4String name);


  /*
   * Specify a candidate list for receiver parameter (calls "::"RequireReceiver(true))
   *
   * \param candidate_list List of candidates separated by spaces
   */
  void SetReceiverCandidates(const char *candidate_list);

  /*
   * Set candiate list for options
   *
   * \param candiate_list Optional list of candidates
   *
   *
  void SetAvailableOptions(const char *option_list = 0);
  */

  /*
   * Get the receiver value
   */
  G4String GetReceiver() { return fReceiver; }

  /*
   * Get the options list
   */
  std::map<G4String,G4String> GetOptions() { return fOptions; }

  /*
   * Parse the parameter list
   *
   * \parameter parameter_list Parameter list
   */
  virtual G4int DoIt(G4String parameter_list);


private:
  bool fWithReceiver;       // Require a receiver before the options list?
  G4String fReceiver;       // Value of the receiver
  std::map<G4String,G4String> fOptions; // Vector of Options parsed
  G4String fSplit;          // Delimiter for splitting options
  G4String fAssignment;     // Delimiter for option assignment operator

  /*
   * Sets the name of the default parameter
   *
   * \param name Parameter name
   */
  void SetName(G4String name);

  /*
   * Parse Receiver
   *
   * \param parameter_list List of user parameters
   */
 bool ParseReceiver(G4String &parameter_list);

  /*
   * Parse Receiver
   *
   * \param parameter_list List of user parameters
   */
  bool ParseOptions(G4String &parameter_list);
};

#endif /* COMPTONG4UICMDWITHSTRINGOPTIONS_H */
