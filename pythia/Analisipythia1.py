## Analisi di tutti i file prodotti da simulazione!
#importo librerie e moduli
import numpy as np               # libreria molto nota per calcolo scientifico
import matplotlib.pyplot as plt  # libreria per la parte grafica
import os                        # modulo interazione con il sistema operativo


#creo una funzione che crea tre grafici in un unico pdf ma siccome voglio che vada bene per qualsiasi set di dati che simulo impongo un indice
def genera_istogrammi(indice, bins=500, salva_come=None, titolo=None):
    # Costruzione dei file
    pT_file = f'pT{indice}.txt'
    theta_file = f'theta{indice}.txt'
    phi_file = f'phi{indice}.txt'

    # Carico i dati
    dati_pT = np.loadtxt(pT_file)
    dati_theta = np.loadtxt(theta_file)
    dati_phi = np.loadtxt(phi_file)

    # Crea una figura con 3 sottoplot verticali condividendo l'asse x
    fig, axs = plt.subplots(3, 1, figsize=(10, 10))

    # Istogramma pT
    axs[0].hist(dati_pT, bins=bins, color='skyblue')
    axs[0].set_xlim(-0.5, 4)
    axs[0].set_xlabel(r'pT [GeV]')
    axs[0].set_ylabel('# Eventi')
    axs[0].grid(True)

    # Istogramma theta
    axs[1].hist(dati_theta, bins=bins, color='salmon')
    axs[1].set_xlim(-0.5, np.pi + 0.5)
    axs[1].set_xlabel(r'$\theta$ [rad]')
    axs[1].set_ylabel('# Eventi')
    axs[1].grid(True)

    # Istogramma phi
    axs[2].hist(dati_phi, bins=bins, color='lightgreen')
    axs[2].set_xlim(-np.pi - 0.5, np.pi + 0.5)
    axs[2].set_xlabel(r'$\phi$ [rad]')
    axs[2].set_ylabel('# Eventi')
    axs[2].grid(True)

    #Titolo generale della figura
    if titolo:
        fig.suptitle(titolo, fontsize=16)
    #salvataggio grafici (bisogna scrivere sempre anche l'estensio del file che si salva, io salvo sempre in pdf)
    if salva_come:
        plt.savefig(salva_come)
    # Ottimizza lo spazio
    plt.tight_layout()

#faccio la lista dei titoli da apporre ad ognuno dei grafici
titoli = [
    "Grafici della Simulazione Fotone-Fotone (200 GeV)",
    "Grafici della Simulazione Fotone su oro (200 GeV)",
    "Grafici della Simulazione Fotone su oro (1 GeV)",
    "Grafici della Simulazione Fotone su oro (10 GeV)"
]
# Esegui la funzione per i tre set di dati, imponendo 4 set di dati, apparendomi anomalo il terzo set ho effettuato una simulazione in più
for i in range(1, 5):
    genera_istogrammi(i, salva_come=f'plot{i}.pdf', titolo=titoli[i-1])

plt.show()


