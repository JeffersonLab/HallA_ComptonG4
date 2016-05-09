#include "ComptonG4Utils.hh"
#include <stdlib.h>

namespace ComptonG4Utils {

  G4int GetBool(G4String string, bool &result)
  {
    G4int rc = -1;
    // Check for true
    if( SameIgnore(string,"yes") ||
        SameIgnore(string,"true") ) {
      rc = 0;
      result = true;
    } else if ( SameIgnore(string,"no") ||
        SameIgnore(string, "false") ) {
      rc = 0;
      result = false;
    }

    return rc;
  }

  G4double GetDouble(G4String string)
  {
    return strtod(string.data(),NULL);
  }

  G4bool SameIgnore(G4String left, G4String right)
  {
    return left.compareTo(right,G4String::ignoreCase) == 0;
  }

}
