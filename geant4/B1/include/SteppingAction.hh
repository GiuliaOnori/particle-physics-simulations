/// \file B1/include/SteppingAction.hh
/// \brief Definition of the B1::SteppingAction class

#ifndef B1SteppingAction_h
#define B1SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"



class G4LogicalVolume;
class G4Step;

/// Stepping action class
///

namespace B1
{

class EventAction;
class RunAction;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction, RunAction* runAction);
    ~SteppingAction() override = default;

    // metodo chiamato a ogni step
    void UserSteppingAction(const G4Step*) override;
    
    

  private:
    EventAction* fEventAction = nullptr;
    RunAction* fRunAction = nullptr;
    G4LogicalVolume* fScoringSenoVolume = nullptr;
    G4LogicalVolume* fScoringTumoreVolume = nullptr;
    G4LogicalVolume* fScoringHTVolume = nullptr;
   
  
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
