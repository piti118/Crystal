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
// $Id: PrimaryGeneratorAction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(
                                             DetectorConstruction* DC)
:Detector(DC),rndmFlag("off")
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="e-");
  particleGun->SetParticleDefinition(particle);
  angle = 0;
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(105.*MeV);
  //G4double position = -0.5*(Detector->GetWorldSizeX());
  particleGun->SetParticlePosition(G4ThreeVector(0*cm,0.*cm,0.*cm));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  // G4double x0 = -0.5*(Detector->GetWorldSizeX());
  // G4double y0 = 0.*cm, z0 = 0.*cm;
  // if (rndmFlag == "on")
  //    {y0 = (Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
  //     z0 = (Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
  //    } 
  // particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

