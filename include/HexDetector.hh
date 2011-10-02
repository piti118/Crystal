#ifndef HexDetector_h
#define HexDetector_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <vector>
#include "Detector.hh"
#include "HexPosition.hh"
#include "G4Polyhedra.hh"
#include <cmath>
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

class HexDetector : public Detector
{
  public:
    HexDetector(int nring);
    ~HexDetector();
    
    int nring;
    std::vector<HexPosition> posmap;
    static const unsigned int idoffset = 10000;
    
    G4VPhysicalVolume* Construct();

    inline unsigned int calorRing(int id){return posmap[id-idoffset].ringno;}
    inline unsigned int calorSeg(int id){return posmap[id-idoffset].segmentno;}
    
    bool inDetector(int id){
      int order = id = idoffset;
      return id >=0 && id < posmap.size();
    }
    void initPosMap();
  private:
    
    G4Material* LYSO;
    G4Material* Air;
    
    G4Box* world_box;
    G4LogicalVolume* world_log;
    G4VPhysicalVolume* world_pv;
    
    std::vector<G4Polyhedra*> calor_box;
    std::vector<G4LogicalVolume*> calor_log;
    std::vector<G4VPhysicalVolume*> calor_pv; 
    inline double hexsize2r(double hexsize) const {double pi =  std::atan(1)*4;return hexsize/std::cos(pi/6);}
  private:
    
     void DefineMaterials();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

#endif

