#include "ComptonG4UIcmdWithStringOptions.hh"

ComptonG4UIcmdWithStringOptions::ComptonG4UIcmdWithStringOptions(
    const char *command_path, G4UImessenger *mssgr, bool with_receiver ) :
  G4UIcommand(command_path,mssgr),
  fWithReceiver(with_receiver)
{
  SetOptionsSplitDelimiter(); // Specify default delimiter
  SetOptionsAssignmentDelimiter(); // Specify default delimiter
  // Create a new string parameter
  SetParameter(new G4UIparameter('s'));
  SetName("receiver");
}


/*
 * Set whether a receiver is required (default is yes)
 *
 * \parama with_receiver [bool] require receiver? Default is true
 */
void ComptonG4UIcmdWithStringOptions::RequireReceiver(bool with_receiver)
{
  fWithReceiver = with_receiver;
  GetParameter(0)->SetOmittable(fWithReceiver);
}

/*
 * Set receiver name (calls "::"RequireReceiver(true))
 *
 * \parama name Set the required receivers name
 */
void ComptonG4UIcmdWithStringOptions::SetReceiverName(G4String name)
{
  RequireReceiver(true);
  SetName(name);
}


/*
 * Specify a candidate list for receiver parameter (calls "::"RequireReceiver(true))
 *
 * \param candidate_list List of candidates separated by spaces
 */
void ComptonG4UIcmdWithStringOptions::SetReceiverCandidates(
    const char *candidate_list)
{
  RequireReceiver(true);
  GetParameter(0)->SetParameterCandidates(candidate_list);
}

/*
 * Sets the name of the default parameter
 *
 * \param name Parameter name
 */
void ComptonG4UIcmdWithStringOptions::SetName(G4String name)
{
  GetParameter(0)->SetParameterName(name);
}

/*
 * Parse the parameter list
 *
 * \parameter parameter_list Parameter list
 */
G4int ComptonG4UIcmdWithStringOptions::DoIt(G4String parameter_list)
{
  G4String params(parameter_list);
  if(!ParseReceiver(params))
    return 300;

  if(!ParseOptions(params))
    return 301;

  return G4UIcommand::DoIt(parameter_list);
}

/*
 * Parse Receiver
 *
 * \param parameter_list List of user parameters
 */
bool ComptonG4UIcmdWithStringOptions::ParseReceiver(G4String &parameter_list)
{
  if(parameter_list.empty())
    return false; // Parameter list is empty, bad!

  // Ensure that if a receiver is required it is actually specified
  if(fWithReceiver) {
    if(parameter_list.empty())
      return false; // Parameter is bad (no receiver specified!)

    size_t pos = parameter_list.find(" ");
    G4String val = parameter_list.substr(0,pos);
    if(val.empty())
      return false; // Parameter is bad (no receiver specified!)

    if(pos>parameter_list.size())
      return false; // Either no options or no receiver specified (hard to tell)

    fReceiver = val;
    parameter_list = parameter_list.substr(pos+1,parameter_list.size());
  }

  return true;
}

/*
 * Parse Receiver
 *
 * \param parameter_list List of user parameters
 */
bool ComptonG4UIcmdWithStringOptions::ParseOptions(G4String &parameter_list)
{
  fOptions.clear();
  while( !parameter_list.empty() ) {
    size_t pos = parameter_list.find(fSplit);
    G4String val = parameter_list.substr(0,pos);
    if(!val.empty()) {
      size_t pos2 = val.find(fAssignment);
      if(pos2>val.size()) { // Option without assignment
        fOptions[val] = "";
      } else {
        G4String val2 = val.substr(0,pos2);
        fOptions[val2] = val.substr(pos2+1,val.size());
      }
    }

    if(pos>parameter_list.size()) { // Looks like we reached the end!
      parameter_list = "";
    } else {
      parameter_list = parameter_list.substr(pos+1,parameter_list.size());
    }
  }

  return true;
}


/*
 * Set delimiter that specifies split options
 *
 * \parama delimiter Delimiter for splitting options
 */
void ComptonG4UIcmdWithStringOptions::SetOptionsSplitDelimiter(
    G4String delimiter)
{
  if( !delimiter.contains(" ") && !delimiter.empty() ) {
    fSplit = delimiter;
  }
}

/*
 * Set delimiter that specifies split options
 *
 * \parama delimiter Delimiter for splitting options
 */
void ComptonG4UIcmdWithStringOptions::SetOptionsAssignmentDelimiter(
    G4String delimiter)
{
  if( !delimiter.contains(" ") && !delimiter.empty() ) {
    fAssignment = delimiter;
  }
}
