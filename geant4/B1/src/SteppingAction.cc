/// \file B1/src/SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include <iostream>

namespace B1
{



// Costruttore: riceve un puntatore a EventAction
SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction)
: fEventAction(eventAction), fRunAction(runAction)
{
 
}


// Metodo chiamato a ogni step
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Inizializzazione dei volumi di scoring
  if (!fScoringSenoVolume || !fScoringTumoreVolume || !fScoringHTVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringSenoVolume   = detConstruction->GetScoringSenoVolume();
    fScoringTumoreVolume = detConstruction->GetScoringTumoreVolume();
    fScoringHTVolume = detConstruction->GetScoringHTVolume();
    
  }

 

  // Energia depositata nello step
  G4double edepStep = step->GetTotalEnergyDeposit();
  if (edepStep <= 0.) return;

  // Volume in cui avviene lo step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // Coordinate iniziali e finali
  G4ThreeVector position_i = step->GetPreStepPoint()->GetPosition(); // pos iniziale dello step
  G4ThreeVector position_f = step->GetPostStepPoint()->GetPosition(); // pos finale dello step
  G4double ystepi = position_i.y(); // coordinata iniziale y direzione del fascio
  G4double ystepf = position_f.y();  // coordinata finale y 
  G4double step_length = step->GetStepLength(); // lunghezza totale dello step
  
  std::ofstream* senoStream = fRunAction->GetOutputStream("Seno");
  std::ofstream* tumoreStream = fRunAction->GetOutputStream("Tumore");
  std::ofstream* htStream = fRunAction->GetOutputStream("HT");
  std::ofstream* sistematiStream = fRunAction->GetOutputStream("Sistemati");
 

  // Scrittura dei dati nei 3 file Seno Tumore e Tessuto sano
  if (volume == fScoringSenoVolume && senoStream && *senoStream) {
    (*senoStream) << ystepf << "  " << edepStep << std::endl;
    if (sistematiStream && *sistematiStream) {
    (*sistematiStream) << G4BestUnit(ystepf, "Length") << " " << G4BestUnit(edepStep, "Energy") << std::endl;
    }  
  }

  if (volume == fScoringTumoreVolume && tumoreStream && *tumoreStream) {
    (*tumoreStream) << ystepf << "  " << edepStep << std::endl;
  }
  
   if (volume == fScoringHTVolume && htStream && *htStream) {
    (*htStream) << ystepf << "  " << edepStep << std::endl;
  }

  

  // controllo debug
  static const bool debug = false;
  if (debug && step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetCurrentStepNumber() < 20) {
  //Stampare posizione iniziale dello step lungo y
  G4cout << "Y_step posizione iniziale: " << G4BestUnit(ystepi, "Length") << G4endl;
  //Stampare posizione finale dello step lungo y
  G4cout << "Y_step posizione finale: " << G4BestUnit(ystepf, "Length") << G4endl;
  //Stampere sto considerando che l'energia depositiata in questo step è localizzata al centro
  G4cout << "Energia depositata: " << G4BestUnit((ystepi + ystepf) / 2., "Length")
         << " salvata al:  " << (ystepi + ystepf) / 2. << G4endl;
  // Stampa la lunghezza dello step       
  G4cout << "step lunghezza " << G4BestUnit(step_length, "Length") << G4endl;
  // stampa quanta energia è stata depositata in quello step
  G4cout << "Energia depositata nello step:" << G4BestUnit(edepStep, "Energy") << G4endl;
  }
  // Se lo step non è nei volumi di interesse, esce
 // if (volume != fScoringSenoVolume && volume != fScoringTumoreVolume && volume != fScoringHTVolume) return;

  // Accumula energia per l’evento corrente
  fEventAction->AddEdep(edepStep);
  }

} // namespace B1
