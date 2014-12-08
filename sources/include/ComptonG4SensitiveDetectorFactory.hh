/*
 * ComptonG4SensitiveDetectorFactory.hh
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTORFACTORY_HH_
#define COMPTONG4SENSITIVEDETECTORFACTORY_HH_

#include <map>
#include <string>

/*
 * Forward class declarations
 */
class VComptonG4SensitiveDetector;

/*
 * Sub-classes of VComptonG4SensitiveDetector will register themselves by
 * templating this function and returning a new instance of themselves.
 */
template <typename T>
VComptonG4SensitiveDetector* CreateVComptonG4SensitiveDetector(std::string name)
{
  return new T(name);
}

/*
 * A factory of sensitive detectors. Users may now specify the type
 * in the GDML file, and this will map it to a concrete class
 */
class ComptonG4SensitiveDetectorFactory {
public:
  typedef std::map<std::string,VComptonG4SensitiveDetector*(*)(std::string)>
    _ComptonG4_SD_Map;
  ComptonG4SensitiveDetectorFactory() {}
  virtual ~ComptonG4SensitiveDetectorFactory() {}

  // Register this object to the factory
  template <typename T>
  static bool Register(const std::string name) {
    map()->insert(std::make_pair(name,
          &CreateVComptonG4SensitiveDetector<T>));
    return true;
  }

  // Create a new instance of this type (if it exists)
  static VComptonG4SensitiveDetector* Create(std::string name,
      std::string volName) {
    _ComptonG4_SD_Map::iterator it =
      map()->find(name);
    if( it != map()->end() ) {
      return it->second(volName);
    }

    // Not found, return a null
    return 0;
  }

private:
  static _ComptonG4_SD_Map *fFactoryObjects;
  static _ComptonG4_SD_Map *map() {
    if(!fFactoryObjects) {
      fFactoryObjects = new _ComptonG4_SD_Map();
    }
    return fFactoryObjects;
  }
};


/*
 * Define a macro to simplify registration
 */
#define COMPTONG4_SD_REGISTER(NAME) \
  const bool void_NAME = ComptonG4SensitiveDetectorFactory::Register<NAME>(#NAME);

#endif /* COMPTONG4SENSITIVEDETECTORFACTORY_HH_ */
