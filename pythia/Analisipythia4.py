#Il seguente programma serve a mettere a confronto le tre distribuzione di pT che escono dal programma pythia simulazione02.cc 
import matplotlib.pyplot as plt

def hist(filename):
    bins = []
    valori = []
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 2:
                continue
            bin_center = float(parts[0])
            valore = float(parts[1])
            bins.append(bin_center)
            valori.append(valore)
    return bins, valori

# Nomi esportati dalla seconda simulazione effettuata con pythia
files = [
    "gamma_gamma.txt",
    "gamma_au_200.txt",
    "gamma_au_1.txt"
]

# Colori per ogni curva
colors = [
     "blue",
     "green",
     "red"
]

labels = [
    r"$\gamma \gamma$ a 200 GeV",
    r"$\gamma$-Au a 200 GeV",
    r"$\gamma$-Au a 1 GeV"
]

plt.figure(figsize=(10, 6))

for filename, color, label in zip(files, colors, labels):
    bins, valori = hist(filename)
    bin_width = bins[1] - bins[0] if len(bins) > 1 else 0.2
    plt.plot(bins, valori, drawstyle='steps-mid', label=label, color=color)

plt.xlabel(" pT [GeV]")
plt.ylabel(r" $\frac{dN}{N} $ ")
plt.xlim((0, 4))
plt.title("Confronto istogrammi normalizzati diretttamente da Pythia")
plt.legend()
plt.grid(True)
plt.savefig("Prova_finaleNormPythia.pdf")
plt.tight_layout()
plt.show()

