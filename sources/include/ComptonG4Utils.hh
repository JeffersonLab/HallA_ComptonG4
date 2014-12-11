
/*
 *
 */
#ifndef COMPTONG4UTILS_H
#define COMPTONG4UTILS_H

#include <G4Types.hh>
#include <G4String.hh>

namespace ComptonG4Utils {

  /*
   * Determine if string is a boolean, and if so, parse it
   */
  G4int GetBool(G4String string, G4bool &result);

  /*
   * Determine if two strings are the same (while ignoring case)
   */
  G4bool SameIgnore(G4String left, G4String right);
}


#endif // COMPTONG4UTILS_H
