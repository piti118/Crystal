#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include <iostream>
#include "HexPosition.hh"
int main(int argc,char** argv)
{

  //HexPositionGenerator hexgen;
  //hexgen.generate(10);
  //return 0;
  G4RunManager * runManager = new G4RunManager();

  // Set mandatory initialization classes
  //
  DetectorConstruction* detector = new DetectorConstruction();
  Simulation::getInstance()->detector = detector;
  runManager->SetUserInitialization(detector);
  // Physics list
  PhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);
    
  // Set user action classes
  //
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  Simulation::getInstance()->pgaction = gen_action;
  runManager->SetUserAction(gen_action);
  
  //
  RunAction* run_action = new RunAction;  
  Simulation::getInstance()->runaction = run_action;
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction();
  Simulation::getInstance()->eventaction = event_action;
  runManager->SetUserAction(event_action);
  //
  SteppingAction* stepping_action =
                    new SteppingAction();
  runManager->SetUserAction(stepping_action);
  
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  
  if (argc == 1)   // batch mode
    {
      for(int deg=0;deg<90;deg++){
        gen_action->SetAngle(deg);
        runManager->BeamOn(10);
      }
      DEDXDatabase::save();
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute " __TOP_DIR__ "/macro/vis.mac"); 
#endif
      if (ui->IsGUI())
	UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
