void compareHistograms() {
    // Funzione che prende dai file i dati raccolti, ne fa un istogramma che viene normalizzato!
    auto Histo_da_file = [](const char* filename, const char* histName, int nBins, double xMin, double xMax) -> TH1D* {
        //metto per prima cosa l'apertura del file e un if per un possibile errore
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "Errore: impossibile aprire il file " << filename << std::endl;
            return nullptr;
        }
        // creo un vettore vuoto che conterrà tutti i numeri letti dal file
        std::vector<double> valori;
        double x; // variabile temporanea che viene letta
        while (infile >> x) {
            if (x>=1e-8) valori.push_back(x); // se tutto va come deve andare si aggiungono al vettore solo i valori più grandi del parametro, per evitare i valori intorno allo 0 problema sopratutto della terza simulazione
        }
        infile.close();
        
        // infine creo l'istogramma e lo normalizzo con integral
        TH1D* hist = new TH1D(histName, histName, nBins, xMin, xMax);
        for (double val : valori) hist->Fill(val);

        double integral = hist->Integral();
        if (integral != 0) hist->Scale(1.0 / integral);  // Normalizza

        return hist;
    };

    // Impostazioni comuni per tutti gli istogrammi
    //numero di bin
    int nBins = 600;

    // Faccio un'altra funzione per avere range comune, leggiamo prima tutti i dati per ricavare min e max globali (volendo lo stesso binning e range, per comparare le distribuzioni)
    std::vector<double> allValori;
    auto readValori= [](const char* filename) {
        std::vector<double> vals;
        std::ifstream f(filename);
        double x;
        while (f >> x) vals.push_back(x);
        return vals;
    };
    
    // prendo i tre vettori di valori da plottare
      
    std::vector<double> vals1 = readValori("pT1.txt");
    std::vector<double> vals2 = readValori("pT2.txt");
    std::vector<double> vals3 = readValori("pT3.txt");
    
    // richiamo la funzione per metterli tutti insieme e def min e max comuni

    allValori.insert(allValori.end(), vals1.begin(), vals1.end());
    allValori.insert(allValori.end(), vals2.begin(), vals2.end());
    allValori.insert(allValori.end(), vals3.begin(), vals3.end());

    double xMin = *std::min_element(allValori.begin(), allValori.end());
    double xMax = *std::max_element(allValori.begin(), allValori.end());
    

    // Crea gli istogrammi tramite funzione fatta prima, metto sempre un if per possibile errore
    
    TH1D* hist1 = Histo_da_file("pT1.txt", "pT1", nBins, xMin, xMax);
    TH1D* hist2 = Histo_da_file("pT2.txt", "pT2", nBins, xMin, xMax);
    TH1D* hist3 = Histo_da_file("pT3.txt", "pT3", nBins, xMin, xMax);
     

    if (!hist1 || !hist2 || !hist3) {
        std::cerr << "Errore nella creazione degli istogrammi" << std::endl;
        return;
    }

    // Colori e spessore delle linee per distinguere i tre plot e metto anche i limiti per gli assi
    hist1->SetLineColor(kMagenta-3);
    hist2->SetLineColor(kBlue+3);
    hist3->SetLineColor(kGreen-3);
    hist1->SetLineWidth(2);
    hist2->SetLineWidth(2);
    hist3->SetLineWidth(2);

    // Disegna sul primo canva in maniera lineare
    TCanvas* c = new TCanvas("c", "Confronto istogrammi pT", 800, 600);
    
    // faccio il primo "clone" degli istogrammi per la parte lineare!
    TH1* h1_lin = (TH1*)hist1->Clone("h1_lin");
    TH1* h2_lin = (TH1*)hist2->Clone("h2_lin");
    TH1* h3_lin = (TH1*)hist3->Clone("h3_lin");
    
    h1_lin->SetTitle("Confronto istogrammi normalizzati; pT[GeV]; dN/N  ");
    h1_lin->GetXaxis()->SetRangeUser(-0.5,4);
    h1_lin->GetYaxis()->SetRangeUser(0,0.08);
    h1_lin->Draw("HIST");      // Primo istogramma
    h2_lin->Draw("HIST SAME"); // Sovrapposto
    h3_lin->Draw("HIST SAME"); // Sovrapposto
    c->Update();
    c->SaveAs("Norm_Root.pdf");
    
    // Disegna sul primo canva in maniera log
    TCanvas* c2 = new TCanvas("c2", "Confronto istogrammi pT, scala log", 800, 600);
    c2->SetLogy();
    
    //faccio il secondo "clone" per la parte log
    TH1* h1_log = (TH1*)hist1->Clone("h1_log");
    TH1* h2_log = (TH1*)hist2->Clone("h2_log");
    TH1* h3_log = (TH1*)hist3->Clone("h3_log");
    h1_log->SetTitle("Confronto istogrammi normalizzati; pT[GeV]; dN/N ");
    h1_log->GetXaxis()->SetRangeUser(0,19);
    h1_log->Draw("HIST");      // Primo istogramma
    h2_log->Draw("HIST SAME"); // Sovrapposto
    h3_log->Draw("HIST SAME"); // Sovrapposto
    c2->Update();
    c2->SaveAs("Norm_Root_log.pdf");
}

