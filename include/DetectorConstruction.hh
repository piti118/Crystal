#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "Detector.hh"
#include "globals.hh"
#include <vector>
#include <cstdlib>
#include "G4ThreeVector.hh"
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

/**
* This class represents array of square 3x3x13 cm crystal
**/

class DetectorConstruction : public Detector
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
    
    G4ThreeVector randPos(){
      G4double x=((double)std::rand()/(double)RAND_MAX)*crystal_x-crystal_x/2;//shift back to center
      G4double y=((double)std::rand()/(double)RAND_MAX)*crystal_y-crystal_y/2;
      G4double z=0.;
      G4ThreeVector toReturn(x,y,z);
      return toReturn;
    }
    
    virtual bool inDetector(int id){
      return calorMinId() <= id && id <= calorMaxId();
    }
    
    static inline unsigned int calorIndex(unsigned int irow, unsigned icol){return irow*numcol+icol;}
    //return inclusive limit
    static inline unsigned int calorMinId() {return idoffset+calorIndex(0,0);}
    static inline unsigned int calorMaxId(){return idoffset+calorIndex(numrow-1,numcol-1);}
    static inline unsigned int calorRow(int id){return (id-idoffset)/numcol;}
    static inline unsigned int calorCol(int id){return (id-idoffset)%numcol;}
  private:
    
    static const unsigned int numrow = 9;//make them both odd so one crystal is at the center
    static const unsigned int numcol = 9;
    static const unsigned int idoffset = 1000;
    
    static const G4double crystal_x;
    static const G4double crystal_y;
    static const G4double crystal_z;
    
    
    G4Material* LYSO;
    G4Material* Air;
    
    G4Box* world_box;
    G4LogicalVolume* world_log;
    G4VPhysicalVolume* world_pv;
    
    std::vector<G4Box*> calor_box;
    std::vector<G4LogicalVolume*> calor_log;
    std::vector<G4VPhysicalVolume*> calor_pv; 
      
  private:
    
     void DefineMaterials();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

#endif

