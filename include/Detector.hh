#ifndef DETECTOR_H
#define DETECTOR_H value
#include "mongo/client/dbclient.h"
#include "BSONInterface.hh"
#include "G4TouchableHandle.hh"
#include "G4VUserDetectorConstruction.hh"
class Detector: public BSONInterface, public G4VUserDetectorConstruction{
public:
  virtual ~Detector(){}

  //determine whether detector part with this id is in this detector
  virtual bool inDetector(int id){return false;}
  
  //determine wheter touchablehandle with is in this detector
  virtual bool touchableInDetector(const G4TouchableHandle& t){
    return inDetector(t->GetCopyNumber());
  }
  virtual G4ThreeVector randPos(){
    G4ThreeVector toReturn(0,0,0);
    return toReturn;
  }
  //dump detector description to BSONObject
  virtual mongo::BSONObj toBSON();
};
#endif
