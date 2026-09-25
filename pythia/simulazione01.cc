//il seguente programma serve a simulare con Pythia tre diversi eventi:
//target 1: fotone-fotone a 200 GeV, target 2: fotone a 200 GeV 
//su targhetta d'Oro e target 3: fotone a 1 GeV su targhetta
//d'Oro. si registrano su tre file di testo i valori registrati rispettivamente di impulso trasverso e delle coordinate polari


// richiamo le librerie di Pythia e aggiugo le librerie classiche di C++ per scrivere i valori raccolti su file
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/ProgressLog.h"
#include <fstream>
#include <iostream>

using namespace Pythia8;

int main() {
  // Per prima cosa interfaccio con l'utente che utilizza il programma, chiedendo il target che vuole simulare, il numero di eventi che 
  //vuole simulare (per esempio nel nostro caso 100000) e in più chiedo quali nomi dei file vuole usare per la simulazione, ricordo che va apportata anche l'estensione del file: .txt (nel mio caso i file sono salvati pT1.txt, theta1.txt e phi1.txt, e per gli altri terget cambio il numero, sottolineo che per avere la corrispondenza con il programma di analisi fatto con python i file si debbono chiamare così o bisogna cambiare nel programmastesso)
  int target;
  std::cout << "Scegliere con 1, 2, 3 il processo di fotone-fotone, fotone(200GeV)-oro o fotone(1GeV)-oro:" << std::endl;
  std::cin >> target;

  int nEvents;
  std::cout << "Scrivere quanti eventi si vogliono simulare:" << std::endl;
  std::cin >> nEvents;

  std::string fileName_pT, fileName_Theta, fileName_Phi;
  std::cout << "Inserire il nome del file per pT, theta e phi: " << std::endl;
  std::cin >> fileName_pT >> fileName_Theta >> fileName_Phi;
  

  // Verifica di aver impostato uno dei numeri presenti nella simulazione
  if (target != 1 && target != 2 && target != 3) {
      std::cout << "Errore selezione processo, di default scelto processo 1." << std::endl;
      target = 1;
  }

  // Setup Pythia (creiamo gli eventi, impostando per ogni targetle rispettive caratteristiche)
  Pythia pythia;
  Event& event = pythia.event;

  if (target == 1) { 
    pythia.readString("Beams:frameType = 1"); // è impostato a 1 perchè è una simulazione fotone fotone, impostiamo quindi un'energia a centro di massa
    pythia.readString("Beams:idA = 22"); //fotone
    pythia.readString("Beams:idB = 22"); // fotone
    pythia.readString("Beams:eCM = 200."); // energia al centro di massa
    pythia.readString("SoftQCD:all = on"); //imposto SoftQDC
    
  }
  if (target == 2) {
    pythia.readString("Beams:frameType = 2"); // è impostato a 2 perchè è una simulazione fascio-targhetta
    pythia.readString("Beams:idA = 22"); //fotone
    pythia.readString("Beams:idB = 1000791970"); // oro (imposto id dell'elemento)
    pythia.readString("Beams:eA = 200."); // energia fotone
    pythia.readString("Beams:eB = 183.5"); // la targhetta deve essere ferma, quindi metto un'energia inferiore alla massa a riposo dell'oro cioè 197
    pythia.readString("SoftQCD:all = on"); // imposto SoftQDC
    
  }
  if (target == 3) {  
    pythia.readString("Beams:frameType = 2"); // è impostato a 2 perchè è una simulazione fascio-targhetta
    pythia.readString("Beams:idA = 22"); //fotone
    pythia.readString("Beams:idB = 1000791970"); // oro (imposto id dell'elemento)
    pythia.readString("Beams:eA = 10."); // energia fotone
    pythia.readString("Beams:eB = 183.5"); // la targhetta deve essere ferma, quindi metto un'energia inferiore alla massa a riposo dell'oro cioè 197
    pythia.readString("SoftQCD:all = on"); // imposto SoftQDC
   
  }

  pythia.init();

  // Creiamo i file per le tre quantità che si vogliono raccolgliere 
  std::ofstream file_pT(fileName_pT);
  std::ofstream file_Theta(fileName_Theta);
  std::ofstream file_Phi(fileName_Phi);

  if (!file_pT || !file_Theta || !file_Phi) {
    std::cerr << "Errore apertura file di output!" << std::endl;
    return 1;
  }

  // Log di progresso (si mette per simulazioni lunghe, stampa un log dell'avanzamento della simulazione)
  ProgressLog logger(nEvents);

  // il ciclo per gli eventi
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()) continue;

    for (int i = 0; i < event.size(); ++i) {
      if (event[i].isFinal() && event[i].isCharged()) {
        // Salvo i valori grezzi riga per riga
        file_pT << event[i].pT() << "\n";
        file_Theta << event[i].theta() << "\n";
        file_Phi << event[i].phi() << "\n";
      }
    }

    logger(iEvent);
  }

  pythia.stat();

  // Chiudo i file
  file_pT.close();
  file_Theta.close();
  file_Phi.close();

  std::cout << "Simulazione completata e dati salvati." << std::endl;
  return 0;
}

