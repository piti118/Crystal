#include "DEDXData.hh"
#include <vector>
std::vector< DEDXData > DEDXDatabase::data;

mongo::BSONObj DEDXData::toBSON() {
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

void DEDXDatabase::save(const std::string& host,const std::string& db,const std::string& coll){
  mongo::DBClientConnection c;
  c.connect(host);
  std::string ns = db+"."+coll;
  c.dropCollection(ns);
  for(unsigned int i=0;i<data.size();++i){
    mongo::BSONObj obj = data[i].toBSON();
    if(i%10000==0){std::cout << "Inserting "<< i << "/" << data.size() <<std::endl;}
    c.insert(ns,obj);
  }
}
