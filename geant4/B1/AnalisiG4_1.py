#importo le librerie e moduli che  mi servono

from matplotlib import pyplot as plt
import os

#definisco il nome del file testo in cui salvo le informazioni che vengono anche stampate sul pront 
output_path = "Risultati.txt"
with open (output_path, 'w') as out:
    out.write("RISULTATI PER LE VARIE SIMULAZIONI")

#definisco bin
n_bin = 100
min_bin = 0
max_bin = 100
dx = (max_bin - min_bin) / n_bin
bins = [min_bin + i * dx for i in range(n_bin + 1)]
bins2 = [(bins[i] + bins[i + 1]) / 2 for i in range(n_bin)]

qe = 1.60217663e-19  # mi scrivo carica elementare per conversione che farò dopo da MeV a juole per la definizione di dose
start = os.getcwd()

#liste per grafici
energie_plot = []
dose_seno_plot = []
dose_tumore_plot = []
dose_HT_plot = []


# ho riportato un file .txt con le energie utilizzate nel nostro caso, se ipoteticamente si facessero macro diverse andrebbe aggiornato questo file
with open(start + "/build/energie_usate.txt", 'r') as f:
    energy_line = f.readline().split()
    #creo la lista delle nergie per il ciclo successivo
    energy_list = [energy_line[0], energy_line[2], energy_line[4]]  



# Masse, queste vengono lette dal file creato dalla stessa simulazione
with open(start + "/build/mass.txt", 'r') as f:
    mass_line = f.readline().split()
    #vado a definire le tre masse (sono le colonne pari del file perchè ho fatto stampare anche le unità di misura)
    Seno_mass = float(mass_line[0])
    Tumore_mass = float(mass_line[2]) * 1e-3 # nota la massa del tumore è data in mg e la voglio in grammi
    HT_mass = float(mass_line[4])

# Funzione per calcolare la dose, dal mio programma g4 si salvano energie e lunghezze io voglio la dose quindi converto
def Calcolo_dose_lunghezza(file_path, mass, nome):
    lunghezze, doses = [], []
    wl, ge, j = 0, 0, 0 # questi valori si incrementano ogni volta che c'è un problema con la lettura del file per avere un controllo sulla lettura  
    with open(file_path, 'r') as file:
        for line in file:
            if len(line.split()) == 2:
                try:
                    lunghezza = abs(float(line.split()[0])-50) # sottraggo un fattore 50 che è la distanza dal centro ddel cono (in teoria è poco di meno ma va bene lo stesso)
                    dose = (float(line.split()[1]) * 1e6 * qe) / (mass * 1e-3) ## colverto in dose, Grey
                    lunghezze.append(lunghezza)
                    doses.append(dose)
                    if len(lunghezze) != len(doses): ge += 1
                except:
                    j += 1 ## errore nel try esempio se ho numeri troppo grandi o problemi con le righe
            else:
                wl += 1 ## errore sul if ipo se ho più di due righe nel file che richiamo!
                j += 1
    # qui stampo l'informativa sugli errori riscontrati
    print("-------------------------------------------------------------------------------")
    print(f"{j} righe errate in {nome}: {wl} con lunghezza sbagliata; {ge} errori generici")
    return lunghezze, doses

# adesso faccio un'altra funzione per riempire l'istogramma, suddivide le dosi e somma l'energia per ogni intervallo di bin
def riempi_bins(lunghezze, doses, bins, n_bin):
    bins_out = [0.0] * n_bin
    for i in range(n_bin):
        for d in range(len(lunghezze)):
            if bins[i] < lunghezze[d] <= bins[i + 1]:
                bins_out[i] += doses[d]
    return bins_out

# questa funzione fa il plot, uso step perchè ho costruito sia le x sia le y a mo di istogramma
def plot_dose(x, y, label, color, filename):
    plt.figure()
    plt.step(x, y, where='mid', color=color, label=label)
    plt.xlabel("Lunghezza [mm]")
    plt.ylabel("Dose [Gy]")
    plt.title(label)
    plt.grid(True)
    plt.legend(loc='upper right', fontsize='small')
    plt.savefig(filename)
    

# Ciclo sulle 3 energie faccio un ciclo for per ogni energia simulata, così ottimizzo il più possibile il programma e
# soprattutto se dovessi simulare altre energie dovrei sono aggiungere nel file le energie aggiungere un valore nel programma di lettura e avere l'accortezza di chiamare
# allo stesso modo i file .txt mma questo  lo fa già il programma scritto per geant4!
for i, energy in enumerate(energy_list):
    suffix = f"_{energy}MeV"

    # Nomi file, in totale ne prende tre per simulazione
    Seno_file = f"build/Seno{suffix}.txt"
    Tumore_file = f"build/Tumore{suffix}.txt"
    HT_file = f"build/HT{suffix}.txt"
    
    

    # Calcolo dose e lunghezza, usando la funzione numero uno
    lunghezzaS_dati, DoseS_dati = Calcolo_dose_lunghezza(Seno_file, Seno_mass, f"file del Seno {energy}")
    lunghezzaT_dati, DoseT_dati = Calcolo_dose_lunghezza(Tumore_file, Tumore_mass, f"file del Tumore {energy}")
    lunghezzaHT_dati, DoseHT_dati = Calcolo_dose_lunghezza(HT_file, HT_mass, f"file del HT {energy}")

     
    # Salva per grafico
    energie_plot.append(float(energy))
    dose_seno_plot.append(sum(DoseS_dati))
    dose_tumore_plot.append(sum(DoseT_dati))
    dose_HT_plot.append(sum(DoseHT_dati))
    
    # creo i bins usando la funzione numero due
    S_bins = riempi_bins(lunghezzaS_dati, DoseS_dati, bins, n_bin)
    T_bins = riempi_bins(lunghezzaT_dati, DoseT_dati, bins, n_bin)
    HT_bins = riempi_bins(lunghezzaHT_dati, DoseHT_dati, bins, n_bin)
    
    # mi calcolo dose totale
    dosetot = sum(DoseS_dati) + sum(DoseT_dati) + sum(DoseHT_dati)
    
    # mi salvo sul file testo le info importati
    with open(output_path, 'a') as out:
         out.write(f"\n--- Risultati per energia {energy} MeV ---\n")
         out.write(f'Dose totale: {dosetot:.2e} Gy\n')
         out.write(f'Dose al Seno: {sum(DoseS_dati):.2e} Gy ({100*sum(DoseS_dati)/dosetot:.2f}%)\n')
         out.write(f'Dose al Tumore: {sum(DoseT_dati):.2e} Gy ({100*sum(DoseT_dati)/dosetot:.2f}%)\n')
         out.write(f'Dose al Tessuto Sano: {sum(DoseHT_dati):.2e} Gy ({100*sum(DoseHT_dati)/dosetot:.2f}%)\n')
   
    # Faccio i plot tramite funzione numero tre
    plot_dose(bins2, S_bins, f'Dose al Seno a {energy} MeV', 'purple', f'Dose_seno_{energy}.pdf')
    plot_dose(bins2, T_bins, f'Dose al Tumore a {energy} MeV', 'green', f'Dose_tumore_{energy}.pdf')
    plot_dose(bins2, HT_bins, f'Dose al Tessuto limitrofo a {energy} MeV', 'cyan', f'Dose_HT_{energy}.pdf')
    
    # Faccio la comparazione del grafico del tessuto limitrofo al tumore e il tumore
    plt.figure(figsize=(8, 6))
    plt.hist(bins[:-1], bins=bins, weights=T_bins, edgecolor='orange', histtype='stepfilled', density=False, alpha=0.3, label=f'Dose al tumore a {energy} Mev')
    plt.hist(bins[:-1], bins=bins, weights=HT_bins, edgecolor='green', histtype='stepfilled', density=False, alpha=0.3, label=f'Dose al tessuto sano a {energy} MeV')
    plt.xlabel("Lunghezza [mm]")
    plt.ylabel("Dose [Gy]")
    plt.title(f'Dose vs Lunghezza, confronto tumore e tessuto sano (a {energy} MeV)')
    plt.grid(True)
    plt.legend(loc='upper right', fontsize='small')
    plt.savefig(f'Comparazione_Dose2_{energy}.pdf')
 


# Grafico comparativo finale: Dose vs lunghezza nel seno per tutte le energie 
colors = ['red', 'blue', 'black']
plt.figure(figsize=(8, 6))

for i, energy in enumerate(energy_list):
    suffix = f"_{energy}MeV"
    Seno_file = f"build/Seno{suffix}.txt"
    lunghezze, edeps = Calcolo_dose_lunghezza(Seno_file, Seno_mass, f"Seno {energy} MeV")
    Seno_bins = riempi_bins(lunghezze, edeps, bins, n_bin)
    
    
    plt.hist(bins[:-1], bins=bins, weights=Seno_bins, color=colors[i],
             histtype='stepfilled', alpha=0.5, label=f"{energy} MeV")

plt.xlabel("Lunghezza [mm]")
plt.ylabel("Dose [Gy]")
plt.title("Confronto dose nel seno a diverse energie")
plt.grid(True)
plt.legend(loc='upper right', fontsize='small')
plt.savefig("Comparazione_Dose_Seno_TutteEnergie.pdf")

# Grafico finale: dose totale al seno vs energia
plt.figure(figsize=(8, 6))
plt.plot(energie_plot, dose_seno_plot, 'o-', color='purple', label="Dose nel seno")
plt.plot(energie_plot, dose_tumore_plot, 'o-', color='red', label="Dose nel tumore")
plt.plot(energie_plot, dose_HT_plot, 'o-', color='yellow', label="Dose nel tessuto limitrofo")
plt.xlabel("Energia del fascio gamma [MeV]")
plt.ylabel("Dose assorbita [Gy]")
plt.title("Dose assorbita ai vari casi vs Energia del fascio")
plt.grid(True)
plt.legend()
plt.savefig("Dose_seno_vs_energia.pdf")

   
plt.show()    
    

    
