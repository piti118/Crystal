#include "HexDetector.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4NistManager.hh"
#include <string>
#include <vector>
#include <iostream>
#include "G4Polyhedra.hh"
#include <cmath>
HexDetector::HexDetector():
  LYSO(0),Air(0),world_box(0),world_log(0),world_pv(0),
  calor_box(),calor_log(),calor_pv()
{
  DefineMaterials();
  //and do nothing...
}

HexDetector::~HexDetector(){ 
    //do nothing
}

G4VPhysicalVolume* HexDetector::Construct()
{
  
  return ConstructCalorimeter();
}

void HexDetector::DefineMaterials()
{ 
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Element* Lu = nistManager->FindOrBuildElement("Lu");
  G4Element* Y = nistManager->FindOrBuildElement("Y");
  G4Element* Si = nistManager->FindOrBuildElement("Si");
  G4Element* O = nistManager->FindOrBuildElement("O");
  G4Element* Ce = nistManager->FindOrBuildElement("Ce");
  
  
  LYSO = new G4Material("LYSO", 7.1*g/cm3, 5, kStateSolid);
  LYSO->AddElement(Lu, 71.43*perCent);
  LYSO->AddElement(Y, 4.03*perCent);
  LYSO->AddElement(Si, 6.37*perCent);
  LYSO->AddElement(O, 18.14*perCent);
  LYSO->AddElement(Ce, 0.02*perCent);
  
  Air = nistManager->FindOrBuildMaterial("G4_AIR");
  
}

G4VPhysicalVolume* HexDetector::ConstructCalorimeter()
{
  using std::string;
  using std::vector;
  using std::cout;
  using std::endl;
  //all the dimensions
  cout << "aaaaaa" << endl;
  G4double crystal_x = 3*cm;
  G4double crystal_y = 3*cm;
  G4double crystal_z = 13*cm;
  
  G4double offset_x = 0*mm;
  G4double offset_y = 0*mm;
  G4double offset_z = 0*mm;
  
  G4double gap_x = 0.1*mm;
  G4double gap_y = 0.1*mm;
  
  G4double padding_x = 0.1*mm;
  G4double padding_y = 0.1*mm;
  G4double padding_z = 0.1*mm;
  G4double worldsize_x = 1000*mm;
  G4double worldsize_y = 1000*mm;
  G4double worldsize_z = 1000*mm;
  double pi =  std::atan(1)*4;
  world_box = new G4Box("world_box",worldsize_x/2,worldsize_y/2,worldsize_z/2);
  world_log = new G4LogicalVolume(world_box,Air,"world_log");
  //world_log->SetVisAttributes (G4VisAttributes::Invisible);
  world_pv = new G4PVPlacement(
      0, //no rotation
      G4ThreeVector(), //at origin
      world_log,
      "world_pv",
      0,
      false, //no boolean
      0); //copy number
   
  //numbering system for hexagonal placement is 
  //identify by ring number and crystal number
  //the crystal number start at 0 from (ringnumber,0)
  //in lk coordinate (arrange so that the tip of the hex point upward)
  //l is vector to the next top right
  //k is vector to the next lower right 
  
  for(unsigned int iring=0;iring<numring;iring++){
    unsigned int numsegment = 6*iring;
  
    G4double zplanes[2];
    zplanes[0]=0*cm;zplanes[1]=13*cm;
    G4double rinner[2];
    rinner[0]=0*cm;rinner[1]=0*cm;
    G4double router[2];
    router[0]=3*cm;router[1]=3*cm;
    G4double thisx=0;
    G4double thisy=0;
    G4double thisz=0;
    for(unsigned int iseg=0;iseg<numsegment;iseg++){
      char temp[100];
      sprintf(temp,"crystal_%d_%d",iring,iseg);
      string thisname(temp);
      G4Polyhedra* thishex = new G4Polyhedra(
          thisname.c_str(),
          0,2*pi, // phistart, totalphi
          6,//numside
          2,//numzplane
          zplanes,
          rinner,
          router
        );
        
      G4LogicalVolume* thislog = new G4LogicalVolume(thishex,LYSO,(thisname+"_log").c_str());
      G4PVPlacement* thispv = new G4PVPlacement(
          0,
          G4ThreeVector(thisx,thisy,thisz),
          thislog,
          (thisname+"_pv").c_str(),
          world_log,
          false,
          calorIndex(iring,iseg)
          );
    }
  }
   
   
   
   
   
   
   
    
  //row run from x
    // for(unsigned int irow=0;irow<numrow;irow++){
    //   for(unsigned int icol=0;icol<numcol;icol++){
    //     char temp[100];
    //     sprintf(temp,"crystal_%d_%d",irow,icol);
    //     string thisname(temp);
    //     G4double thisx = crystal_origin_x+ crystal_x*irow + gap_x*irow;
    //     G4double thisy = crystal_origin_y+ crystal_y*icol + gap_y*icol;
    //     G4double thisz = crystal_origin_z;
    //     cout << irow << "," << icol << "--" << thisx << thisy << endl;
    //     G4Box* thisbox = new G4Box(thisname.c_str(), crystal_x/2,crystal_y/2,crystal_z/2);
    //     G4LogicalVolume* thislog = new G4LogicalVolume(thisbox, LYSO,(thisname+"_log").c_str());
    //     G4PVPlacement* thispv = new G4PVPlacement(
    //         0,
    //         G4ThreeVector(thisx,thisy,thisz),
    //         thislog,
    //         (thisname+"_pv").c_str(),
    //         world_log,
    //         false,
    //         idoffset+calorIndex(irow,icol)
    //       );
    //       calor_box.push_back(thisbox);
    //       calor_log.push_back(thislog);
    //       calor_pv.push_back(thispv);
    //       G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0,1.0));
    //       simpleBoxVisAtt->SetVisibility(true);
    //       thislog->SetVisAttributes(simpleBoxVisAtt);
    //   }
    // }
  
  //                                        
  // Visualization attributes
  //
  // logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  // 


  return world_pv;
}
