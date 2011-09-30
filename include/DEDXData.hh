#ifndef DEDXDATA
#define DEDXDATA 1
#include <map>

#include <G4UnitsTable.hh>
#include <iostream>
#include <ostream>
#include "DetectorConstruction.hh"

//#include "yaml.h"
#include "mongo/client/dbclient.h"
using namespace std;

class DEDXData{
public:
  int eventno;
  int runno;
  double angle;
  std::map<int, double> dedx;//map from calorid to dedx 
  
  void setup(int runno, int eventno, double angle, int minCalorId,int maxCalorId){
    this->runno = runno;
    this->eventno = eventno;
    this->angle = angle;
    dedx.clear();
    for(int i=minCalorId; i<=maxCalorId; ++i){
      dedx[i] = 0.;
    }
  }
  
  void accumulate(int calorId,double dedxval){
    dedx[calorId]+=dedxval;
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
  
//  void writeYAML(YAML::Emitter& out) const{
//    using std::cout;using std::endl;
//    cout << "Warning YAML output is deprecated" << endl;
//    using std::map;
//    out << YAML::BeginMap;
//    out << YAML::Key << "runno" << YAML::Value << runno;
//    out << YAML::Key << "eventno" << YAML::Value << eventno;
//    out << YAML::Key << "angle" << YAML::Value << angle;
//    out << YAML::Key << "dedx" << YAML::Value;
//    out << YAML::BeginSeq;
//    for(map<int,double>::const_iterator it = dedx.begin(); it!=dedx.end(); ++it){
//      int calorId = it->first;
//      double thisDEDx = it->second;
//      out << YAML::BeginMap;
//      out << YAML::Key << "calorId" << YAML::Value << calorId;
//      out << YAML::Key << "row" << YAML::Value << DetectorConstruction::calorRow(calorId);
//      out << YAML::Key << "col" << YAML::Value << DetectorConstruction::calorCol(calorId);  
//      out << YAML::Key << "dedx" << YAML::Value << thisDEDx/MeV << YAML::Comment("MeV");
//      out << YAML::EndMap;
//    }
//    out << YAML::EndSeq;
//    out << YAML::Key << "Total DEDX" <<  YAML::Value << sumE()/MeV << YAML::Comment("MeV");
//    out << YAML::EndMap;
//  }
  
  mongo::BSONObj toBSON() const{
    using mongo::BSONObjBuilder;
    using mongo::BSONArrayBuilder;
    using mongo::BSONObj;
    using std::map;
    BSONObjBuilder b;
    b << "runno" << runno;
    b << "eventno" << eventno;
    b << "angle" << angle;
   
    BSONArrayBuilder dedxlist;
    for(map<int,double>::const_iterator it = dedx.begin(); it!=dedx.end(); ++it){
      int calorId = it->first;
      double thisDEDx = it->second;
      BSONObjBuilder calor;
      calor << "calorId" << calorId;
      calor << "row" << DetectorConstruction::calorRow(calorId);
      calor << "col" << DetectorConstruction::calorCol(calorId);
      calor << "dedx" << thisDEDx/MeV;
      dedxlist << calor.obj();
    }
    BSONObj arr = dedxlist.done();
    b << "dedx" << arr;
    b << "total" << sumE()/MeV;
    return b.obj(); 
  }
  
};

class DEDXDatabase{
public:
  static std::vector<DEDXData> data;
//  static void writeYAML(std::ostream& stream){
//    for(unsigned int i=0;i<data.size();++i){
//      YAML::Emitter out;
//      data[i].writeYAML(out);
//      //assert(out.good());
//      stream << out.c_str() << std::endl;
//
//      stream << "---" << std::endl;
//    }
//  }
  
  static void save(const std::string& host="localhost",const std::string& db="crystal",const std::string& coll="raw"){
    mongo::DBClientConnection c;
    c.connect(host);
    std::string ns = db+"."+coll;
    c.dropCollection(ns);
    for(unsigned int i=0;i<data.size();++i){
      mongo::BSONObj obj = data[i].toBSON();
      if(i%10000==0){cout << "Inserting "<< i << "/" << data.size() <<endl;}
      c.insert(ns,obj);
    }
  }
};
#endif
