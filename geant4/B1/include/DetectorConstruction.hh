/// \file B1/include/DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction la classe che definisce materiale e geometrie

namespace B1
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringSenoVolume() const { return fScoringSenoVolume; }
    G4LogicalVolume* GetScoringTumoreVolume() const { return fScoringTumoreVolume; }
    G4LogicalVolume* GetScoringHTVolume() const { return fScoringHTVolume; }

  protected:
    G4LogicalVolume* fScoringSenoVolume = nullptr;
     G4LogicalVolume* fScoringTumoreVolume = nullptr;
     G4LogicalVolume* fScoringHTVolume = nullptr;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
