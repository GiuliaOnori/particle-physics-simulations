// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>


namespace B1
{

RunAction::RunAction()
{
  // Definizione di nuove unità per la dose
  const G4double milligray = 1.e-3 * gray;
  const G4double microgray = 1.e-6 * gray;
  const G4double nanogray  = 1.e-9 * gray;
  const G4double picogray  = 1.e-12 * gray;

  new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
  new G4UnitDefinition("nanogray",  "nanoGy",  "Dose", nanogray);
  new G4UnitDefinition("picogray",  "picoGy",  "Dose", picogray);

  // Registro delle quantità accumulabili
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // Disattiva il salvataggio dei semi casuali
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Reset degli accumulabili
  G4AccumulableManager::Instance()->Reset();
  
  // Recupera l'energia di PrimaryGeneratorAction
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
  G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  
  G4String energyStr = "unknown";
  if (generatorAction) {
  const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
  if (particleGun) {
    G4double energyMeV = particleGun->GetParticleEnergy() / MeV;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << energyMeV << "MeV";
    energyStr = oss.str();
  }
}

  // Crea i file con nomi univoci
  std::map<std::string, std::string> baseNames = {
    {"Seno", "Seno_" + energyStr + ".txt"},
    {"Tumore", "Tumore_" + energyStr + ".txt"},
    {"HT", "HT_" + energyStr + ".txt"},
    {"Sistemati", "Sistemati_" + energyStr + ".txt"}
  };
  
  // Apri gli stream dei file e salvali nella mappa
  for (const auto& [key, filename] : baseNames) {
    std::ofstream* ofs = new std::ofstream(filename);
    if (!ofs->is_open()) {
      G4cerr << "Errore nelll'apertura del file: " << filename << G4endl;
    }
    fOutputStreams[key] = ofs;
  }   
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nEvents = run->GetNumberOfEvent();
  //metto il risultato in un file di testo per l'analisi
  std::ofstream Eventi_data("nEvents.txt");
  Eventi_data<<nEvents<<std::endl;
  if (nEvents == 0) return;

  // Unisci i valori degli accumulabili
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Recupera i valori accumulati
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double rms = edep2 - edep * edep / nEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  // ottengo i volumi dal detectorconstruction
  const auto detConstruction = static_cast<const DetectorConstruction*>(
  G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
  //ottengo i volumi
  G4double SenoVol = detConstruction->GetScoringSenoVolume()->GetSolid()->GetCubicVolume();
  G4double TumoreVol = detConstruction->GetScoringTumoreVolume()->GetSolid()->GetCubicVolume();
  G4double HTVol = detConstruction->GetScoringHTVolume()->GetSolid()->GetCubicVolume();
  
  //calcolo la massa tramite comando GetMass
  G4double Seno_mass = detConstruction->GetScoringSenoVolume()->GetMass();
  G4double Tumore_mass = detConstruction->GetScoringTumoreVolume()->GetMass();
  G4double HT_mass = detConstruction->GetScoringHTVolume()->GetMass();
  
  //calcolo della dose per il seno  
  G4double doseSeno = edep / Seno_mass;
  G4double rmsDoseSeno = rms / Seno_mass;
  
  //creo un file .txt per salvare le masse calcolate
  std::ofstream mass_data("mass.txt");
  mass_data << G4BestUnit(Seno_mass, "Mass")<< " " << G4BestUnit(Tumore_mass, "Mass") << " " << G4BestUnit(HT_mass, "Mass") << std::endl;
  
  //creo un file .txt per salvare volumi calcolati
  std::ofstream volume_data("volume.txt");
  volume_data << G4BestUnit(SenoVol, "Volume")<< " " << G4BestUnit(TumoreVol, "Volume") << " " << G4BestUnit(HTVol, "Volume") << std::endl;


  // Recupera informazioni dal generatore primario

  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  
  G4String runCondition;  
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    //nomino l'energia utilizzata e la sottoscrivo in un file (dovrebbe essere 10 MeV)
    G4double particleEnergy = particleGun->GetParticleEnergy();
    std::ofstream energy_data("energy.txt");
    energy_data<<G4BestUnit(particleEnergy,"Energy")<<std::endl;
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Stampa il resoconto
  if (IsMaster()) {
    G4cout << "\n-------------------- End of Global Run -----------------------";
  } else {
    G4cout << "\n-------------------- End of Local Run ------------------------";
  }

  G4cout
     << "\n il numero di eventi sono: " << nEvents << " " << runCondition
     << "\n si è usato come primo target (Seno): "
     << detConstruction->GetScoringSenoVolume()->GetName()  << " che ha massa: " <<  G4BestUnit(Seno_mass, "Mass")
     <<G4endl
     << "\n si è usato come secondo target (Tumore): "
     << detConstruction->GetScoringTumoreVolume()->GetName()  << " che ha massa: " <<  G4BestUnit(Tumore_mass, "Mass")
      <<G4endl
     << "\n si è usato come terzo target (Tessuto Sano): "
     << detConstruction->GetScoringHTVolume()->GetName()  << " che ha massa: " <<  G4BestUnit(HT_mass, "Mass")
     <<G4endl
     << "\n------------------------------------------------------------\n" << G4endl;
     
  //Chiudere i file aperti all'inizio
  CloseOutputStreams();
     
 }

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep * edep;
}

void RunAction::CloseOutputStreams()
{
  for (auto& [key, streamPtr] : fOutputStreams) {
    if (streamPtr) {
      streamPtr->close();
      delete streamPtr;
      streamPtr = nullptr;
    }
  }
  fOutputStreams.clear();
  
}

std::ofstream* RunAction::GetOutputStream(const std::string& name) const
{
  auto it = fOutputStreams.find(name);
  return (it != fOutputStreams.end()) ? it->second : nullptr;
}

}
