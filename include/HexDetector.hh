#ifndef HexDetector_h
#define HexDetector_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <vector>
#include "Detector.hh"
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HexDetector : public Detector
{
  public:
  
    HexDetector();
   ~HexDetector();
     
    G4VPhysicalVolume* Construct();
    static inline unsigned int calorIndex(unsigned int iring, unsigned iseg){return 1000;}
    static inline std::vector<int> calorIdList(){
      std::vector<int> toReturn;
      return toReturn;
    }
    static inline unsigned int calorRing(int id){return 0;}
    static inline unsigned int calorSeg(int id){return 0;}
  private:
    
    static const unsigned int numring = 6;
    static const unsigned int idoffset = 10000;
    
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

