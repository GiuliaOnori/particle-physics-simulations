// questo programma serve a fare le simulazioni richieste, ma fa solo l'isotgramma normalizzato dell'impuso trasverso è per fare una comparazione con quello fatto in root e in python 

// richiamo le librerie di Pythia e aggiugo le librerie classiche di C++ per scrivere i valori raccolti su file
#include "Pythia8/Pythia.h"
#include <iostream>
#include <fstream>
#include "Pythia8Plugins/ProgressLog.h"
using namespace Pythia8;

int main() {
    Pythia pythia;
    Hist pT_hist("pT distribuzione", 600, 0.0, 20.0);

     int target;
     std::cout << "Scegliere con 1, 2, 3 il processo di fotone-fotone, fotone(200GeV)-oro o fotone(1GeV)-oro:" << std::endl;
     std::cin >> target;
     
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
    pythia.readString("Beams:eA = 1."); // energia fotone
    pythia.readString("Beams:eB = 183.5"); // la targhetta deve essere ferma, quindi metto un'energia inferiore alla massa a riposo dell'oro cioè 197
    pythia.readString("SoftQCD:all = on"); // imposto SoftQDC
   
  }     

    

    pythia.init();

    const int nEventi = 100000;
    ProgressLog logger(nEventi);


    for (int iEvent = 0; iEvent < nEventi; ++iEvent) {
        if (!pythia.next()) continue;

        for (int i = 0; i < pythia.event.size(); ++i) {
            if (!pythia.event[i].isFinal()) continue;
            if (!pythia.event[i].isCharged()) continue;
            pT_hist.fill(pythia.event[i].pT());
        }
    logger(iEvent);
    }

    pythia.stat();

    // Normalizza e salva su file
    pT_hist.normalize();

    std::string filename;
    if (target == 1)
        filename = "gamma_gamma.txt";
    else if (target == 2)
        filename = "gamma_au_200.txt";
    else
        filename = "gamma_au_1.txt";

    std::ofstream outFile(filename);
    pT_hist.table(outFile);
    outFile.close();

    std::cout << "Istogramma salvato in: " << filename << "\n";

    return 0;
}

