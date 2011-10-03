#ifndef DEDXDATA
#define DEDXDATA 1
#include "mongo/client/dbclient.h"
#include <map>
#include <G4UnitsTable.hh>
#include <iostream>
#include <ostream>
#include "BSONInterface.hh"
#include <vector>

class DEDXData: public BSONInterface{
public:
  int eventno;
  int runno;
  double angle;
  std::map<int, double> dedx;//map from calorid to dedx 
  virtual ~DEDXData(){}
  void setup(int runno, int eventno, double angle);
  
  void accumulate(int calorId,double dedxval);
  
  void reset();
  
  double sumE() const;
  
  mongo::BSONObj toBSON();
  
};

class DEDXDatabase{
public:
  static std::vector<DEDXData> data;
  
  static void save(const std::string& host="localhost",const std::string& db="crystal",const std::string& coll="raw");
};
#endif
