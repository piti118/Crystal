//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: HexDetector.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HexDetector::HexDetector():
  LYSO(0),Air(0),world_box(0),world_log(0),world_pv(0),
  calor_box(),calor_log(),calor_pv()
{
  DefineMaterials();
  //and do nothing...
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HexDetector::~HexDetector(){ 
    //do nothing
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
   
  //numbering system for hexagonal placement is 
  //ring number and crystal number
  //ring number start from 0 at the central ring
  //crystal starts from 0 at the on with lowest theta
  //measured from x axis counter clockwise from the center of (0,0)hexagonal to the 
  //center hexagon in question
  
  for(unsigned int iring=0;iring<numring;iring++){
    numsegment = 6*iring;
  

    for(unsigned int iseg=0;iseg<numsegment;iseg++){
      char temp[100];
      sprintf(temp,"crystal_%d_%d",iring,iseg);
      string thisname(temp):
      G4Polyhedra* thishex = new Polyhedra(
          thisname.c_str(),
          0,2*Pi, // phistart, totalphi
          6,//numside
          2,//numzplane
          {0*cm,13*cm},//zplanes
          {0*cm,0*cm},//rinner
          {3*cm,3*cm}//router
        );
        
        G4LogicalVolume* thislog = new G4LogicalVoluem(thishex,LYSO,(thisname+"_log").c_str())
        G4VPlacement* thispv = new G4PVPlacement(
          0,
          G4ThreeVector(thisx,thisy,thisz),
          thislog,
          (thisname+"_pv").c_str(),
          world_log,
          false,
          calorIndex(iring,iseg)
        )
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
