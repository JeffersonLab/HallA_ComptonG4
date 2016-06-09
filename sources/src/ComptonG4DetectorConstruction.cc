////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////

#include "ComptonG4DetectorConstruction.hh"
#include "ComptonG4DetectorConstructionMessenger.hh"
#include "ComptonG4SensitiveDetectorManager.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4Utils.hh"
#include "ComptonG4DipoleField.hh"

#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include <G4Colour.hh>
#include <G4VisAttributes.hh>
#include <G4SolidStore.hh>
#include <G4VSolid.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4SurfaceProperty.hh>
#include <G4OpticalSurface.hh>
#include <G4SDManager.hh>
#include <G4Version.hh>

// Magnetic fields stuff
#include <G4FieldManager.hh>
#include <G4UniformMagField.hh>
#include <G4Mag_SpinEqRhs.hh>
#include <G4MagIntegratorStepper.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>
#include <G4SimpleRunge.hh>
#include <G4ExplicitEuler.hh>
#include <G4ClassicalRK4.hh>

// GEANT4 Geometry related includes
#include <G4GDMLParser.hh>


// Include the Boost stuff we need
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/regex.hpp>

ComptonG4DetectorConstruction::ComptonG4DetectorConstruction(
    G4String geometry_file, ComptonG4SensitiveDetectorManager* senseManager,
    ComptonG4Analysis *analysis) :
  fPhysicsWorld(0),fAnalysis(analysis)
{
  G4GDMLParser parser;
  parser.Read(geometry_file);
  fPhysicsWorld = parser.GetWorldVolume();

  // Create an instance of the messenger class
  fMessenger = new ComptonG4DetectorConstructionMessenger(this);

  // Material table
  const G4MaterialTable* materials =  G4Material::GetMaterialTable();
  for(size_t i = 0; i < materials->size(); i++ ) {
    G4cout << (*materials)[i] << G4endl;
    if((*materials)[i]->GetMaterialPropertiesTable()) {
      (*materials)[i]->GetMaterialPropertiesTable()->DumpTable();
    }
  }

  // For visualization purposes, load and respect the color attributes
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  const G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  const G4SolidStore* solids = G4SolidStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvciter;
  std::vector<G4VPhysicalVolume*>::const_iterator pvciter;
  std::vector<G4VSolid*>::const_iterator soliter;
  G4Colour color;
  G4cout << "Parsing through solids" << G4endl;
  for( soliter = solids->begin(); soliter != solids->end(); soliter++ ) {
    G4cout << (*soliter)->GetName() << G4endl;
  }

  G4cout << "Parsing through logical volumes" << G4endl;
  for( lvciter = lvs->begin(); lvciter != lvs->end(); lvciter++ )
  {
    G4cout << (*lvciter)->GetName() << G4endl;
    G4GDMLAuxListType auxInfo = parser.GetVolumeAuxiliaryInformation(*lvciter);
#if G4VERSION_NUMBER >= 1020
    std::vector<G4GDMLAuxStructType>::const_iterator ipair = auxInfo.begin();
#else
    std::vector<G4GDMLAuxPairType>::const_iterator ipair = auxInfo.begin();
#endif
    for( ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++ )
    {

      G4String str=ipair->type;
      G4String val=ipair->value;
      if( str == "SensDet" ) {
        // Register this new sensitive detector
        //(*lvciter)->SetSensitiveDetector(senseManager->RegisterDetector(val));
        senseManager->RegisterDetector(*lvciter,val);
      } else if ( str == "Color" ) {
        double red = 1.0;
        double green = 1.0;
        double blue = 1.0;
        double alpha = 1.0;
        std::stringstream ss;
        unsigned int temp;
        /*
           G4cout << " Auxiliary Information is found for Logical Volume :  "
           << (*lvciter)->GetName() << G4endl;
           G4cout << " Name of Auxiliary type is     :  " << str << G4endl;
           G4cout << " Associated Auxiliary value is :  " << val << G4endl;
           */
        // Check to see if it's not a know color
        if(!G4Colour::GetColour(val,color)) {
          // Sigh...okay, let's figure out the color. There are four possible
          // combinations. They can either be numbers or hex values, and either
          // one of these can include an alpha value or not.
          // For now, assume only HEX values in the form of #AABBCC or if they
          // have the alpha value #AABBCCDD
          boost::to_lower(val);
          boost::replace_all(val,"#","");
          /*std::vector<std::string> results;
            boost::split_regex(results,val,
            boost::regex("[a-f]"));*/
          if(val.size() >= 6 ) {
            ss << std::hex << val.substr(0,2);
            ss >> temp;
            red = temp/256.;
            ss.str("");
            ss.clear();
            ss << std::hex << val.substr(2,2);
            ss >> temp;
            green = temp/256.;
            ss.str("");
            ss.clear();
            ss << std::hex << val.substr(4,2);
            ss >> temp;
            blue = temp/256.;
            ss.str("");
            ss.clear();
            if(val.size() >= 8 ) {
              ss << std::hex << val.substr(6,2);
              ss >> temp;
              alpha = temp/256.;
              ss.str("");
              ss.clear();
            }
          }
          color = G4Colour(red,green,blue,alpha);
        }
        (*lvciter)->SetVisAttributes(new G4VisAttributes(color));
      } else if ( str == "Magnetic" ) { // For now only support the standard
        // Compton Dipole xwidth = 3 cm is fixed
        RegisterMagneticVolume(*lvciter);
      }
    }
  }

  G4cout << "Parsing through physical volumes" << G4endl;
  for( pvciter = pvs->begin(); pvciter != pvs->end(); pvciter++ ) {
    G4cout << "Physical Volume: " << (*pvciter)->GetName() << " \tHash ID: "
      << (*pvciter)->GetName().hash(G4String::exact) << G4endl;
    G4GDMLAuxListType auxInfo = parser.GetVolumeAuxiliaryInformation(
      (*pvciter)->GetLogicalVolume());
#if G4VERSION_NUMBER >= 1020
    std::vector<G4GDMLAuxStructType>::const_iterator ipair = auxInfo.begin();
#else
    std::vector<G4GDMLAuxPairType>::const_iterator ipair = auxInfo.begin();
#endif
    for( ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++ ) {
      G4String str=ipair->type;
      G4String val=ipair->value;
      if( str == "SensDet" ) {
        senseManager->AddVolume((*pvciter)->GetLogicalVolume()->GetName(),
            *pvciter);
        //fAnalysis->AddDetector((*pvciter)->GetName());
        //((ComptonG4SensitiveDetector*)
        //    (*pvciter)->GetLogicalVolume()->GetSensitiveDetector())
        //  ->AddVolume(*pvciter);
      } else if( str == "Magnetic" ) {
        RegisterMagneticPhysical(*pvciter,(*pvciter)->GetLogicalVolume());
      }
    }
  }

  G4cout << "Logical Skin surface info: " << G4endl;
  const G4LogicalSkinSurfaceTable *surfaces =
    G4LogicalSkinSurface::GetSurfaceTable ();
  for(size_t i = 0; i < surfaces->size(); i++ ) {
    G4LogicalSkinSurface *surf = 0;
    surf = surfaces->at(i);
    G4OpticalSurface *op =
      dynamic_cast<G4OpticalSurface*>(surf->GetSurfaceProperty());
    if(op) { // Looks like an optical surface. Will need to update property
      // tables
      op->SetMaterialPropertiesTable(
          surf->GetLogicalVolume()->GetMaterial()->GetMaterialPropertiesTable());
      G4cout << "Set surface to logical volume: " << surf->GetLogicalVolume()->GetName() << G4endl;
    }
    //surf->GetLogicalVolume()->GetMaterial()->

  }
  G4LogicalSkinSurface::DumpInfo();
  G4cout << "Logical border surface info: " << G4endl;
  G4LogicalBorderSurface::DumpInfo();
  G4cout << "Surface Properties: " << G4endl;
  G4SurfaceProperty::DumpTableInfo();
  const G4SurfacePropertyTable* table = G4SurfaceProperty::GetSurfacePropertyTable();
  for(size_t i = 0; i < table->size(); i++ ) {
    // Check for optical photon properties
    const G4OpticalSurface* surf =
      dynamic_cast<const G4OpticalSurface*>((*table)[i]);
    if(surf) {
      surf->DumpInfo();
      G4MaterialPropertiesTable *tab = surf->GetMaterialPropertiesTable();
      if(tab) {
        tab->DumpTable();
      }
    }
  }
}

ComptonG4DetectorConstruction::~ComptonG4DetectorConstruction()
{
  if(fMessenger)
    delete fMessenger;
}


G4VPhysicalVolume*  ComptonG4DetectorConstruction::Construct()
{
  return fPhysicsWorld;
}

void ComptonG4DetectorConstruction::ActivateDetector(G4String)
{
}

void ComptonG4DetectorConstruction::RegisterMagneticVolume(G4LogicalVolume *v)
{
  fMagneticFields.push_back(new ComptonG4DipoleField());
  fMagneticVolumes.push_back(v);
}

void ComptonG4DetectorConstruction::RegisterMagneticPhysical(
    G4VPhysicalVolume *p, G4LogicalVolume *v)
{
  G4int index = -1;
  for(size_t i = 0; i < fMagneticVolumes.size(); i++ ) {
    if( fMagneticVolumes[i] == v ) {
      index = i;
    }
  }

  // Seems like a new logical volume...how did it slip through the cracks?
  if( index < 0 )
    return;

  fMagneticFields[index]->AddNewActiveRegion( p->GetTranslation(),
      p->GetRotation() );
}



void ComptonG4DetectorConstruction::ProcessMagneticVolumeOptions(G4String name,
    std::map<G4String, G4String> options)
{
  G4int volIndex = -1;
  for(size_t i = 0; i < fMagneticVolumes.size() && volIndex == -1; i++ ) {
    if( fMagneticVolumes[i]->GetName() == name) {
      volIndex = i;
    }
  }

  if( volIndex < 0 ) { // Ignore unknown volumes!
  G4cerr << "NOT FOUND: " << name << G4endl;
    return;
  }
  G4cerr << "FOUND: " << name << G4endl;

  G4LogicalVolume *vol = fMagneticVolumes[volIndex];
  ComptonG4DipoleField *magField = fMagneticFields[volIndex];

  // Need just the file name
  G4String file_name;

  std::map<G4String,G4String>::iterator it;
  for(it = options.begin(); it != options.end(); it++ ) {
    if( ComptonG4Utils::SameIgnore(it->first,"file_name") ) {
      file_name = it->second;
    } else { // Uknown option passed, complain!
      //UnknownOption(it->first,it->second);
    }
  }

  magField->InitFromFile(file_name);
  G4FieldManager *fieldMan =
    new G4FieldManager(magField);
  G4Mag_EqRhs *eqn = new G4Mag_SpinEqRhs(magField);
  //G4MagIntegratorStepper *stepper = new G4ClassicalRK4(eqn,12);
  //G4MagIntegratorStepper *stepper = new G4SimpleRunge(eqn, 12);
  G4MagIntegratorStepper *stepper = new G4ExplicitEuler(eqn, 12);
  G4MagInt_Driver *int_driver = new G4MagInt_Driver(0.01*CLHEP::mm,
      stepper,stepper->GetNumberOfVariables());
  G4ChordFinder* finder = new G4ChordFinder(int_driver);
  fieldMan->SetChordFinder(finder);

  vol->SetFieldManager(fieldMan,true);
}

G4LogicalVolume* ComptonG4DetectorConstruction::GetMagneticVolume(G4String name)
{
  std::vector<G4LogicalVolume*>::iterator it;
  for( it = fMagneticVolumes.begin(); it != fMagneticVolumes.end(); it++) {
    if((*it)->GetName() == name)
      return *it;
  }

  // Not found? Boo!
  return 0;
}
