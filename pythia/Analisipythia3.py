#Il seguente programma serve a mettere a confronto le tre distribuzione di pT, nel primo si utilizza semplicemente density= True di plt.hist mentre nel secondo metodo si utilizza la normalizzazione, fatta in maniera manuale, analoga a
#quella che si fa da manuale di Root, nella stessa cartella si trova un analogo scritto in C++ che fa la stessa cosa in modo da poterla confrontare, 
#nelle istruzioni ho fatto una spiegazione più accurata per capire la differenza tra i due 

import numpy as np
import matplotlib.pyplot as plt

#Faccio una funzione per caricare e filtrare i dati questo per togliere quel picco a 0 dallo spetto 3#
#(se uno stampa i len dei dati ripuliti nota che diminuisce praticament solo quello della terza simulazione e di poco quello della prima come ci si aspetta)
def carica_e_filtra(files):
    dati = np.loadtxt(files)
    return dati[dati > 1e-8]

# Faccio anche una funzione che normalizza in modo analogo al metodo usato con Root, che metto a confronto con quella fatta mettendo density=True su plt.hist
def normalizza(counts):
    return counts / counts.sum()

#Faccio una funzione anche per il plot con density True (così evito di doverlo riscrivere tre volte sotto forma di plot, ma uso una lista per i tre .txt)
def plot_hist_density(data_list, labels, colors, bins, x_range, xlim, title, salva_come=None):
    plt.figure(figsize=(10, 6))
    for data, label, color in zip(data_list, labels, colors): # zip di file .txt, legenda  e colori
        plt.hist(data, bins=bins, range=x_range, histtype='step',
                 density=True, label=label, color=color) # classico plot  per hist, metto a step così si vedono meglio i tre istogrammi
    plt.xlabel("pT [GeV]")
    plt.ylabel(r"$\frac{dN}{N\cdot dp_t} [GeV^]{-1}$")
    plt.xlim(xlim)
    plt.legend()
    plt.grid(True)
    plt.title(title)
    if salva_come:
        plt.savefig(salva_come)
#Faccio una funzione per gli istogrammi usando però plt.stairs
#(che è un tipo di grafico a linea a gradini. stile Root, mi sembrava più appropriato
# per poterlo anche confrontare con quello che ho graficato con Root stesso)
#inoltre molto utile se si hanno già conteggi e bin normalizzati cosa che nella funzione si ottengono dopo np.histogram
def plot_hist_stairs(data_list, labels, colors, bins, x_range, xlim, ylim, title, log=False, salva_come=None): #imposto già log False così va in automatico
    plt.figure(figsize=(10, 6))
    for data, label, color in zip(data_list, labels, colors):
        counts, bin_edges = np.histogram(data, bins=bins, range=x_range)
        counts_norm = normalizza(counts) # utilizzo della funzione che normalizza
        plt.stairs(counts_norm, bin_edges, label=label, color=color, fill=False)
    plt.xlabel("pT [GeV]")
    plt.ylabel(r"$\frac{dN}{cdot dp_t}$")
    plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)
    if log:
        plt.yscale('log')
    plt.legend()
    plt.grid(True)
    plt.title(title)
    if salva_come:
        plt.savefig(salva_come)
    

# Inizio dal caricare e filtrare i dati
files = ['pT1.txt', 'pT2.txt', 'pT3.txt']
data_list = [carica_e_filtra(fp) for fp in files]
labels = [ "Istogramma di pT della Simulazione Fotone-Fotone (200 GeV)",
    "Istogramma di pT della Simulazione Fotone su oro (200 GeV)",
    "Istogramma di pT Simulazione Fotone su oro (1 GeV)",]
colors = ['skyblue', 'salmon', 'purple']

# Calcolo range e bin comuni
all_values = np.concatenate(data_list)
x_min, x_max = np.min(all_values), np.max(all_values)
x_range = (x_min, x_max)
bins = 600

# 1. Plot con density=True
plot_hist_density(data_list, labels, colors, bins, x_range, xlim=(-0.5, 4),
                  title="Normalizzazione da Matplotlib (density=True)", salva_come = "confronto_density.pdf")

# 2. Normalizzazione manuale (stile ROOT)
plot_hist_stairs(data_list, labels, colors, bins, x_range,
                 xlim=(-0.5, 4), ylim=(0, 0.08),
                 title="Normalizzazione Manuale, con lo stesso metodo di Root", salva_come = "confronto_simRoot.pdf")

# 3. Scala logaritmica
plot_hist_stairs(data_list, labels, colors, bins, x_range,
                 xlim=(-0.5, 15), ylim=None, log=True,
                 title="Normalizzazione Manuale - Scala Logaritmica", salva_come = "confronto_simRoot_log.pdf")
             
plt.show()
