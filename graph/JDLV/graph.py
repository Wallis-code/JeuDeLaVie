import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

df = pd.read_csv("simulation.csv")
densities = sorted(df["density"].unique())

# 10 premières densités (0.05 à 0.50)
premiers = densities[:10]
# 9 dernières densités (0.55 à 0.95)
derniers = densities[10:]

def plot_group(group, title, filename):
    colors = cm.rainbow(np.linspace(0, 1, len(group)))
    fig, ax = plt.subplots(figsize=(12, 6))
    for density, color in zip(group, colors):
        subset = df[df["density"] == density].set_index("generation")
        ax.plot(subset.index, subset["population"],
                label=f"{int(density*100)}%", color=color)
    ax.set_xlabel("Génération")
    ax.set_ylabel("Population vivante")
    ax.set_title(title)
    ax.legend(title="Densité", bbox_to_anchor=(1.05, 1), loc="upper left")
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    plt.show()

plot_group(premiers, "Densités faibles (5% à 50%)", "courbes_faibles.png")
plot_group(derniers, "Densités élevées (55% à 95%)", "courbes_elevees.png")
