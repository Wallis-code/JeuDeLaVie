import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("Simu_wator.csv")

plt.plot(df["generation"], df["fish"],  label="Poissons", color="green")
plt.plot(df["generation"], df["shark"], label="Requins",  color="red")

plt.xlabel("Génération")
plt.ylabel("Population")
plt.title("Simulation Wa-Tor — Lotka-Volterra")
plt.legend()
plt.savefig("wator_courbes.png", dpi=150)
plt.show()
