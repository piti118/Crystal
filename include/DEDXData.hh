#ifndef DEDXDATA
#define DEDXDATA 1
#include "mongo/client/dbclient.h"
#include <map>
#include <G4UnitsTable.hh>
#include <iostream>
#include <ostream>
#include "DetectorConstruction.hh"
#include "BSONInterface.hh"


class DEDXData: public BSONInterface{
public:
  int eventno;
  int runno;
  double angle;
  std::map<int, double> dedx;//map from calorid to dedx 
  virtual ~DEDXData(){}
  void setup(int runno, int eventno, double angle){
    this->runno = runno;
    this->eventno = eventno;
    this->angle = angle;
    dedx.clear();
  }
  
  void accumulate(int calorId,double dedxval){
    if(dedx.find(calorId)==dedx.end()){
      dedx[calorId]=0.;
    }
    dedx[calorId]+=dedxval;
    //std::cout << "accumulate" << " " << calorId << std::endl;
  }
  
  void reset(int runno, int eventno){
    using std::map;
    for(map<int,double>::iterator it = dedx.begin(); it!=dedx.end();++it){
      it->second = 0;
    }
  }
  
  double sumE() const{
    using std::map;
    double sum = 0;
    for(map<int,double>::const_iterator it = dedx.begin();it!=dedx.end();++it){
      sum += it->second;
    }
    return sum;
  }
  
  mongo::BSONObj toBSON();
  
};

class DEDXDatabase{
public:
  static std::vector<DEDXData> data;
  
  static void save(const std::string& host="localhost",const std::string& db="crystal",const std::string& coll="raw");
};
#endif
