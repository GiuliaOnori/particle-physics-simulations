#il programma mette a confronto i tre istogrammi di pT, senza normalizzazione, ho pensato potesse
#essere utile per paragonare quale fisica produce maggiori eventi e ne faccio un grafico normale
#e uno in scala log per enfatizzare le code non apprezzabili nel  primo grafico

#importo librerie e moduli
import numpy as np               # libreria molto nota per calcolo scientifico
import matplotlib.pyplot as plt  # libreria per la parte grafica
import os                        # modulo interazione con il sistema operativo



# Lista dei parametri che si usano per la creazione dei grafici  
# la prima mette i file testo prodotti dalle tre simulazioni, label, il colore, e alpha
file_info = [
    ('pT1.txt', r'Simulazione 1 $\gamma\gamma$ [200GeV]', 'red', 0.8),
    ('pT2.txt', r'Simulazione 2 $\gamma$-Au [200GeV]', 'orange', 0.5),
    ('pT3.txt', r'Simulazione 3 $\gamma$-Au [1GeV]', 'yellow', 0.8)
]



# Caricamento dei dati
data_list = [] # lista vuota dove mettere i dati letti da ciascun file
for filename, _, _, _ in file_info: # prendo il primo elemento (file di testo) nel file_info
    if not os.path.exists(filename): # controllo dell'esistenza del file se no dà Errore
        raise FileNotFoundError(f"File non trovato: {filename}")
    # se non ci sono errori si crea la lista che contiene tre array, uno per ognuno file    
    data = np.loadtxt(filename)
    data_list.append(data)

# prendo i parametri  e li riorganizzo 
filenames, labels, colors, alphas = zip(*file_info)

# Funzione per la creazione dei tre istogrammi sovrapposti
def plot_histos(data_list, labels, colors, alphas, bins, log=None, xlim=None, titolo=None, salva_come=None, fig_num=None):
    fig = plt.figure(num=fig_num, figsize=(10, 6)) #assegno un nome alla figura così posso crearne diverse, per esempio io volgio farne una normale e una a scala log!
    for data, label, color, alpha in zip(data_list, labels, colors, alphas):
        plt.hist(data, bins=bins, alpha=alpha, label=label, color=color, density=False, log=log) #impostazione per "estetica" grafico
    plt.xlabel('pT [GeV]')
    plt.ylabel('# Eventi')
    plt.title(titolo)
    if xlim:
        plt.xlim(*xlim)
    plt.legend()
    plt.grid(True)
    if salva_come: # se imposto il voler salvare le immagini qui imposto il nome del file che si crea
        plt.savefig(salva_come)
    plt.tight_layout() # adatta gli elementi nell'immagine
    return fig # ritorna la figura 

# Primo plot (lineare) richiamo come figura 1  che appare 
fig1 = plot_histos(
     data_list, labels, colors, alphas,
     bins=600,
     log=False,
     xlim=(-0.5, 5),
     titolo='Confronto tra numero eventi delle tre simulazioni',
     salva_come= 'ConforontoEventi.pdf',
     fig_num=1
)

# Secondo plot (logaritmico) richiamo come figura 2 che appare

fig2 = plot_histos(
     data_list, labels, colors, alphas,
     bins=600,
     log=True,
     xlim=(-0.5, 19),
     titolo='Confronto tra istogrammi delle tre simulazioni, con scala log',
     salva_come='ConfrontoEventilog.pdf',
     fig_num=2
)

# si mostrano entrambe le immagini a schermo, in modo che l'utente può anche fare degli zoom (che può anche salvare separatamente), comunque nella cartella si salvano le due immagini in pdf (con le impostazioni impostate da me)

plt.show()

