#include "ComptonG4DipoleField.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <iostream>
#include <fstream>

const G4double ComptonG4DipoleField::kGausToTeslaConversion = 1e-4*CLHEP::tesla;
// Set whether you're using a TOSCA map or not.
G4bool TOSCAmap = true;


ComptonG4DipoleField::ComptonG4DipoleField()
{
    
}

ComptonG4DipoleField::~ComptonG4DipoleField()
{
    
}

void ComptonG4DipoleField::GetFieldValue(const G4double location[4],
    G4double *fieldValue) const
{
   

 G4bool readfrommap = true;
    
    
  
  size_t numx = fCoordinate[0].size();
  size_t numy = fCoordinate[1].size();
  size_t numz = fCoordinate[2].size();
  fieldValue[0] = 0.0;
  fieldValue[1] = 0.0;
  fieldValue[2] = 0.0;


//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------Volume test--------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
    
if(!readfrommap){
    if(location[2] < 495.5*CLHEP::cm && location[2]  > 465.5*CLHEP::cm){
        fieldValue[0] = 0.0685;
        fieldValue[1] = 0;
        fieldValue[2] = 0;


    }
    
    else if(location[2] < 585.0*CLHEP::cm && location[2] > 555.0*CLHEP::cm){
        fieldValue[0] = -0.0685;
        fieldValue[1] = 0;
        fieldValue[2] = 0;
        
        
    }
    if(location[2] < 725.0*CLHEP::cm && location[2] > 695.0*CLHEP::cm){
        fieldValue[0] = -0.0685;
        fieldValue[1] = 0;
        fieldValue[2] = 0;
        
        
    }
    if(location[2] < 814.5*CLHEP::cm && location[2] > 784.5*CLHEP::cm){
        fieldValue[0] = 0.0685;
        fieldValue[1] = 0;
        fieldValue[2] = 0;
        
        
    }
    
   //G4cout << " Z position= "<<location[2]/CLHEP::cm << " B field = "<< fieldValue[0] << G4endl;
    
    }
    
//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------end----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------



    

else if(readfrommap){
   
//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------Update-------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
 
    if(TOSCAmap){
        
        
  
        
        if(numx == 0 || numz  == 0) {
            return;
        }
        // First, let's find out if they even fall within this field's definition.
        // To do so, we have to first convert gloabl coordinate to the local
        // coordinate of the field.
        
        G4double z = 0.0*CLHEP::cm;
        G4int rotationIndex = -1;
        
        G4bool insideField = false;
        for( size_t i = 0; i < fOffsets.size() && !insideField; i++ ) {
            G4ThreeVector v = fRotations[i].inverse()(G4ThreeVector(location[0],
            location[1],location[2]-6600*CLHEP::mm)-fOffsets[i]); // for the chicane we have an offset (along z) of 6.6 m from the origin.
                                         //------ ^ ------
            
            if ( v.x() >= fMinCoordinate[0] && v.x() <= fMaxCoordinate[0] &&
                v.y() >= fMinCoordinate[1] && v.y() <= fMaxCoordinate[1] &&
                std::abs(v.z()) >= fMinCoordinate[2] &&
                std::abs(v.z()) <= fMaxCoordinate[2]) {
                insideField = true;
                rotationIndex = i;
                z = std::abs(v.z());
            }
        }
        if( !insideField ) {
            return;
        }
        
        
        
         G4double tmpdelta;
        
        
        
        // Find the index that is closest in z
        size_t indexz = 0;
        G4double deltaz = 1e5;
        for(size_t i = 0; i < numz; i++ ) {
            tmpdelta = (fCoordinate[2][i] - z);
            if( std::abs(tmpdelta) < std::abs(deltaz) ) { // Found smallest delta so far
                deltaz = tmpdelta;
                indexz = i;
                
                
            }
        }
        // fudgeFactor to help increase the strength of the magnetic field, if it's not strong enough.
        G4double fudgeFactor = 53;
       
        G4ThreeVector field(fudgeFactor*fMagneticField[0][indexz][0],fMagneticField[1][indexz][0],fMagneticField[2][indexz][0]);
        G4ThreeVector fieldRotated = fRotations[rotationIndex](field);
        fieldValue[0] = fieldRotated.x();
        fieldValue[1] = fieldRotated.y();
        fieldValue[2] = fieldRotated.z();
        
        //G4cout  << " Z position= "<<location[2]/CLHEP::cm  <<  " B field = "<< fieldValue[0] << G4endl;
        
        
    }
    
    
//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------end----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    
    
    
    
    
else if(!TOSCAmap)  {
    
    
     if(numy == 0 || numz  == 0) {
    return;
  }
  // First, let's find out if they even fall within this field's definition.
  // To do so, we have to first convert gloabl coordinate to the local
  // coordinate of the field.
  
  G4double y = 0.0*CLHEP::cm;
  G4double z = 0.0*CLHEP::cm;
  G4int rotationIndex = -1;

  G4bool insideField = false;
  for( size_t i = 0; i < fOffsets.size() && !insideField; i++ ) {
    G4ThreeVector v = fRotations[i].inverse()(G4ThreeVector(location[0],
                    location[1],location[2])-fOffsets[i]);
    
     
      
    if ( v.x() >= fMinCoordinate[0] && v.x() <= fMaxCoordinate[0] &&
        v.y() >= fMinCoordinate[1] && v.y() <= fMaxCoordinate[1] &&
        std::abs(v.z()) >= fMinCoordinate[2] &&
          std::abs(v.z()) <= fMaxCoordinate[2]) {
      insideField = true;
      rotationIndex = i;
      y = v.y();
      z = std::abs(v.z());
    }
  }
  if( !insideField ) {
    return;
  }

  G4double tmpdelta;

  // Find the index that is closest in y
    
    
    
  size_t indexy = 0;
  G4double deltay = 1e5;
  for(size_t i = 0; i < numy; i++ ) {
    tmpdelta = (fCoordinate[1][i] - y);
    if( std::abs(tmpdelta) < std::abs(deltay) ) { // Found smallest delta so far
      deltay = tmpdelta;
      indexy = i;
        
    }
  }

  // Find the index that is closest in z
  size_t indexz = 0;
  G4double deltaz = 1e5;
  for(size_t i = 0; i < numz; i++ ) {
    tmpdelta = (fCoordinate[2][i] - z);
    if( std::abs(tmpdelta) < std::abs(deltaz) ) { // Found smallest delta so far
      deltaz = tmpdelta;
      indexz = i;
        
        
    }
  }
    
    

  // Since at the moment we only want the fastest way of doing it...not the best
  // extrapolation, then just return the field at those minimim deltas.
  // However, we still have to rotate the field to the volume coordinates
  G4ThreeVector field(fMagneticField[0][indexy][indexz],0.0,0.0);
  G4ThreeVector fieldRotated = fRotations[rotationIndex](field);
  fieldValue[0] = fieldRotated.x();
  fieldValue[1] = fieldRotated.y();
  fieldValue[2] = fieldRotated.z();
  


}
   
    
//G4cout  << " Z position= "<<location[2]/CLHEP::cm  <<  " B field = "<< fieldValue[0] << G4endl;
    
 
    }
   
 
}




void ComptonG4DipoleField::InitFromFile(G4String file)
{

   
 
  std::fstream in;
  in.open(file,std::ios::in);
    if( !in.is_open() ) {
    return;
  }
    
//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------Update------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
if(TOSCAmap){
    
   int rows = 37966;
    
    
    // Read in the columns of the file
    G4double xCord, yCord, zCord, Bx, By, Bz;
    
    
    
    // resize the Magnetic field vector
    fMagneticField.resize(3);
    fMagneticField[0].resize(rows);
    fMagneticField[1].resize(rows);
    fMagneticField[2].resize(rows);
    
    
    // preset the min and max for y, for the chicane y has a max of 567 mm.
    
    fMinCoordinate[1] = -57*CLHEP::cm;
    fMaxCoordinate[1] = +57*CLHEP::cm;
    
    
    
    
    
    for(int i = 0 ; i < rows && !in.eof(); i++){
        
        
        in >> xCord >> yCord >> zCord >> Bx >> By >> Bz ;
        
        // The field map used contains x and z coordinates, whith y set to zero.
        
        
        
        // read in the x coordinate
        // find the x min and max
        
        fCoordinate[0].push_back(xCord*CLHEP::cm);
        if(i == 0) {
            fMinCoordinate[0] = fMaxCoordinate[0] = xCord*CLHEP::cm;
        } else {
            if(xCord*CLHEP::cm > fMaxCoordinate[0])
                fMaxCoordinate[0] = xCord*CLHEP::cm;
            if(xCord*CLHEP::cm < fMinCoordinate[0])
                fMinCoordinate[0] = xCord*CLHEP::cm;
        }
        
        
        //read in the y coordinate
        //note that the min and max for y is preset
        
        fCoordinate[1].push_back(yCord*CLHEP::cm);

        
        
        // read in the z coordinate
        // find the z min and max
        
        
        fCoordinate[2].push_back(zCord*CLHEP::cm);
        if(i == 0) {
            fMinCoordinate[2] = fMaxCoordinate[2] = zCord*CLHEP::cm;
            
        } else {
            if(zCord*CLHEP::cm > fMaxCoordinate[2])
                fMaxCoordinate[2] = zCord*CLHEP::cm;
            if(zCord*CLHEP::cm < fMinCoordinate[2])
                fMinCoordinate[2] = zCord*CLHEP::cm;
        }
        
        // The field map used had negative values, hence take the abs of all the values
        
        By=std::abs(By);
        
 // read the B field values. Note that for the chicane we want a non-zero Bx field, however the file has a non-zero By field hence we swap them.
        
        fMagneticField[0][i].push_back(By*kGausToTeslaConversion*0.936363636364);
        fMagneticField[1][i].push_back(Bx*kGausToTeslaConversion*0.936363636364);
        fMagneticField[2][i].push_back(Bz*kGausToTeslaConversion*0.936363636364);
        
        
        
        
        
    }
   
    
    
    sort(fCoordinate[0].begin() , fCoordinate[0].end());
    fCoordinate[0].erase( unique( fCoordinate[0].begin(), fCoordinate[0].end()), fCoordinate[0].end());
    
    sort(fCoordinate[2].begin() , fCoordinate[2].end());
    fCoordinate[2].erase( unique( fCoordinate[2].begin(), fCoordinate[2].end()), fCoordinate[2].end());
    
/*
 // read out the elements of fMagneticField[0]
    for(size_t i = 0 ; i < fMagneticField[0].size() ; i++ ){
        for (size_t j = 0 ; j < fMagneticField[0][i].size() ; j++){
            G4cerr << "[" << i << "][" << j << "] " << fMagneticField[0][i][j] << G4endl;
        }
    }
   */
   
}
    
//---------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------end----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
    
    
else if(!TOSCAmap){
 
std::vector<G4double> tmpYs;

  // For now, we are ignoring the x's in the Geant4 coordinate system
  // So we resize the magnetic Bx vector to 1
  fCoordinate[0].push_back(0.0);
  fMinCoordinate[0] = -3*CLHEP::cm;
  fMaxCoordinate[0] = +3*CLHEP::cm;
  fMagneticField.resize(fCoordinate[0].size());

  // First read the number of columns present
  int cols;
 
  in >> cols ;
 
  // Now that we know the number of columns, let's resize the By vector
    fMagneticField[0].resize(cols);


  G4double tmpCord;
  // Now read in all the x coordinates
  for(int i = 0; i < cols && !in.eof(); i++ ) {
    in >> tmpCord;
      
    fCoordinate[1].push_back(tmpCord*CLHEP::cm);
    if(i == 0) {
      fMinCoordinate[1] = fMaxCoordinate[1] = tmpCord*CLHEP::cm;
    } else {
      if(tmpCord*CLHEP::cm > fMaxCoordinate[1])
        fMaxCoordinate[1] = tmpCord*CLHEP::cm;
      if(tmpCord*CLHEP::cm < fMinCoordinate[1])
        fMinCoordinate[1] = tmpCord*CLHEP::cm;
    }
  }
   

  G4bool first = true;
  G4double z, b;
  // Now read in the magnetic fields
  while( in >> z && !in.eof() ) {
    
           
    z = std::abs(z);
      
      
      
    fCoordinate[2].push_back(std::abs(z*CLHEP::cm)); // symmetry in z
    if(first) {
      fMinCoordinate[2] = fMaxCoordinate[2] = z*CLHEP::cm;
      first = false;
    } else {
      if(z*CLHEP::cm > fMaxCoordinate[2])
        fMaxCoordinate[2] = z*CLHEP::cm;
      if(z*CLHEP::cm < fMinCoordinate[2])
        fMinCoordinate[2] = z*CLHEP::cm;
    }
    for(int i = 0; i < cols && !in.eof() && in >> b; i++ ) {
        
      // The multiplier is to account for the fact that we ran it at 103 A
      // but the tests where for 110 A
      fMagneticField[0][i].push_back(b*kGausToTeslaConversion*0.936363636364);
        
   }
      
  
  }
}
   
}



void ComptonG4DipoleField::AddNewActiveRegion( G4ThreeVector trans,
    G4RotationMatrix *rot)
{   fOffsets.push_back(trans);
  if( rot == 0 ) {
    fRotations.push_back(G4RotationMatrix());
  } else {
    fRotations.push_back(*rot); // Store the inverse matrix
  }
}
