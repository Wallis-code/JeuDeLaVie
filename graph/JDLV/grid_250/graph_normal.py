import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

df = pd.read_csv("simulation.csv")
densities = sorted(df["density"].unique())
premiers = densities[:10]
derniers = densities[10:]

def plot_group_norm(group, title, filename):
    colors = cm.rainbow(np.linspace(0, 1, len(group)))
    fig, ax = plt.subplots(figsize=(12, 6))
    for density, color in zip(group, colors):
        subset = df[(df["density"] == density) & (df["generation"] <= 100)].set_index("generation")
        val_init = subset["population"].iloc[0]
        if val_init > 0:
            y = subset["population"] / val_init
            ax.plot(subset.index, y, label=f"{int(density*100)}%", color=color)
    ax.set_xlabel("Génération", fontsize=16)
    ax.set_ylabel("Population normalisée (base 1)", fontsize=16)
    ax.set_title(title, fontsize=18)
    ax.tick_params(axis='both', labelsize=14)
    ax.legend(title="Densité", bbox_to_anchor=(1.05, 1), loc="upper left",
              fontsize=13, title_fontsize=14)
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    plt.show()

plot_group_norm(premiers, "Densités faibles — normalisé (5% à 50%)",  "courbes_faibles_norm.png")
plot_group_norm(derniers, "Densités élevées — normalisé (55% à 95%)", "courbes_elevees_norm.png")
