#include "DetectorConstruction.hh"

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

const G4double DetectorConstruction::crystal_x = 3*cm;
const G4double DetectorConstruction::crystal_y = 3*cm;
const G4double DetectorConstruction::crystal_z = 13*cm;

DetectorConstruction::DetectorConstruction():
  LYSO(0),Air(0),world_box(0),world_log(0),world_pv(0),
  calor_box(),calor_log(),calor_pv()
{
  DefineMaterials();
  //and do nothing...
}

DetectorConstruction::~DetectorConstruction(){ 
    //do nothing
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  
  return ConstructCalorimeter();
}

void DetectorConstruction::DefineMaterials()
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

G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{
  using std::string;
  using std::vector;
  using std::cout;
  using std::endl;
  //all the dimensions
  
  G4double offset_x = 0*mm;
  G4double offset_y = 0*mm;
  G4double offset_z = 0*mm;
  
  G4double gap_x = 0.1*mm;
  G4double gap_y = 0.1*mm;
  
  G4double padding_x = 0.1*mm;
  G4double padding_y = 0.1*mm;
  G4double padding_z = 0.1*mm;
  
  G4double total_crystal_x = (numrow*crystal_x + (numrow-1)*gap_x);
  G4double total_crystal_y = (numcol*crystal_y + (numcol-1)*gap_y);
  
  G4double crystal_origin_x = -total_crystal_x/2 + crystal_x/2 + offset_x;
  G4double crystal_origin_y = -total_crystal_y/2 + crystal_y/2 + offset_y;
  G4double crystal_origin_z = crystal_z/2 + offset_z;
  
  G4double worldsize_x = total_crystal_x+2*padding_x;
  G4double worldsize_y = total_crystal_y+2*padding_y;
  G4double worldsize_z = 2*crystal_z + 2*padding_z;
  
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
    
  //row run from x
    for(unsigned int irow=0;irow<numrow;irow++){
      for(unsigned int icol=0;icol<numcol;icol++){
        char temp[100];
        sprintf(temp,"crystal_%d_%d",irow,icol);
        string thisname(temp);
        G4double thisx = crystal_origin_x+ crystal_x*irow + gap_x*irow;
        G4double thisy = crystal_origin_y+ crystal_y*icol + gap_y*icol;
        G4double thisz = crystal_origin_z;
        cout << irow << "," << icol << "--" << thisx << thisy << endl;
        G4Box* thisbox = new G4Box(thisname.c_str(), crystal_x/2,crystal_y/2,crystal_z/2);
        G4LogicalVolume* thislog = new G4LogicalVolume(thisbox, LYSO,(thisname+"_log").c_str());
        G4PVPlacement* thispv = new G4PVPlacement(
            0,
            G4ThreeVector(thisx,thisy,thisz),
            thislog,
            (thisname+"_pv").c_str(),
            world_log,
            false,
            idoffset+calorIndex(irow,icol)
          );
          calor_box.push_back(thisbox);
          calor_log.push_back(thislog);
          calor_pv.push_back(thispv);
          G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0,1.0));
          simpleBoxVisAtt->SetVisibility(true);
          thislog->SetVisAttributes(simpleBoxVisAtt);
      }
    }
  
  //                                        
  // Visualization attributes
  //
  // logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  // 


  return world_pv;
}
