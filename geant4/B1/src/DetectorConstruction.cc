
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"


namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.0 *m;
  G4double world_sizeZ  = 1.0 *m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World",                           // its name
     0.5*world_sizeXY, 0.5*world_sizeXY,  0.5*world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking


 // visualizzazione per colori
 G4VisAttributes * env_cono = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
 env_cono -> SetVisibility(true);
 env_cono -> SetForceSolid(true);
 G4VisAttributes * Shape1_cono = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.5));
 Shape1_cono -> SetVisibility(true);
 Shape1_cono -> SetForceSolid(true);
 
 G4VisAttributes * Shape2_sfera = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
 Shape2_sfera -> SetVisibility(true);
 Shape2_sfera -> SetForceSolid(true);
 
 G4VisAttributes * Shape3_sfera = new G4VisAttributes(G4Colour(1.0, 1.0, 0.5, 0.5));
 Shape3_sfera -> SetVisibility(true);
 Shape3_sfera -> SetForceSolid(true);
 
  //
  // imposto questo come collimatore, imposto come materiale il tugsteno conico che contiene l'area della sorgente
  //
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_W");
  G4ThreeVector pose = G4ThreeVector(0*cm, 10*cm, -2*cm);
  G4RotationMatrix*  rot = new G4RotationMatrix();
  rot->rotateX(-90*deg);
  rot->rotateY(0*deg);
  rot->rotateZ(0*deg);
  // Envelope parametri conici
  //
  
  G4double env_rmina =  1.*cm, env_rmaxa = 1.5*cm;
  G4double env_rminb =  0.3*cm, env_rmaxb = 0.7*cm;
  G4double env_hz = 4.*cm;
  G4double env_phimin = 0.*deg, env_phimax = 360.*deg;
  auto solidEnv = new G4Cons("Envelope", env_rmina, env_rmaxa, env_rminb, env_rmaxb,
    env_hz, env_phimin, env_phimax);
    
  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
    env_mat,                                        // its material
    "Envelope");  

  

 new G4PVPlacement(rot,  // no rotation
    pose,          // position
    logicEnv,                 // its logical volume
   "Envelope",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  //
  // Shape 1 sarebbe il cono cioè il seno
  //
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
  G4ThreeVector pos1 = G4ThreeVector(0*cm, 0*cm, 0*cm);

  // Conical section shape
  G4double shape1_rmina =  0.*cm, shape1_rmaxa = 5.*cm;
  G4double shape1_rminb =  0.*cm, shape1_rmaxb = 0.*cm;
  G4double shape1_hz = 5.*cm;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
    shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
    shape1_mat,                                        // its material
    "Shape1");                                         // its name



  new G4PVPlacement(nullptr,  // no rotation
    pos1,                     // at position
    logicShape1,              // its logical volume
    "Shape1",                 // its name
    logicWorld,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking
    
  logicShape1 -> SetVisAttributes(Shape1_cono);

  //
  // Shape 2 TUMORE --> SFERA, VOLUME GENITORE È IL CONO
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos2 = G4ThreeVector(0*cm, 1*cm, -2*cm);
  // uso orb che automaticamete è una sfera piena
  
  auto solidShape2 = new G4Orb("Shape2", 0.5*cm);

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
    shape2_mat,                                        // its material
    "Shape2");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    pos2,                     // at position
    logicShape2,              // its logical volume
    "Shape2",                 // its name
    logicShape1,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking
  
  
  logicShape2 -> SetVisAttributes(Shape2_sfera);
  
  // Shape 3 Tessuto_limitrofo --> SFERA, VOLUME GENITORE È IL CONO
  //
  G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
  G4ThreeVector pos3 = G4ThreeVector(0*cm, 1*cm, -2*cm);
  //uso sphere in modo da avere un guscio di sfera
  G4double shape3_rmin =  0.5*cm, shape3_rmax =  0.8*cm;
  G4double phi3_min = 0 *degree, phi3_max = 360 *degree; 
  G4double theta3_min = 0 *degree, theta3_max = 360 *degree; ;
  
  auto solidShape3 = new G4Sphere("Shape3", shape3_rmin,  shape3_rmax, phi3_min, phi3_max, theta3_min, theta3_max);

  auto logicShape3 = new G4LogicalVolume(solidShape3,  // its solid
    shape3_mat,                                        // its material
    "Shape3");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    pos3,                     // at position
    logicShape3,              // its logical volume
    "Shape3",                 // its name
    logicShape1,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking
  
  
  logicShape3 -> SetVisAttributes(Shape3_sfera);
  
// creo le targhette di selezione per (scoring volume) per i step 
 fScoringSenoVolume = logicShape1;  //seno
 fScoringTumoreVolume = logicShape2;  //tumore
 fScoringHTVolume = logicShape3;  //tessuto sano intorno al seno
    

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
