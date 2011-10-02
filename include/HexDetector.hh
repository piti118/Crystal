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
      int order = id - idoffset;
      return order >=0 && order < posmap.size();
    }
    void initPosMap();
    
    virtual int calorL(int id){return posmap[id-idoffset].l;}
    virtual int calorK(int id){return posmap[id-idoffset].k;}
    virtual double calorX(int id){return posmap[id-idoffset].toXY().first;}
    virtual double calorY(int id){
      return posmap[id-idoffset].toXY().second;}
    virtual int ringno(int id){return posmap[id-idoffset].ringno;}
    virtual int segmentno(int id){return posmap[id-idoffset].segmentno;}
    
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

