import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams["figure.figsize"] = (10, 6)
plt.rcParams["font.size"] = 12

def plot_single(df, x_col, y_col, title, fname):
    plt.figure()

    plt.plot(
        df[x_col], df[y_col],
        linestyle='-',
        linewidth=0.9
    )

    plt.xlabel("Размер массива")
    plt.ylabel("Время (мс), медиана 5 запусков")
    plt.title(title)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(fname, dpi=200)
    plt.close()

std_random = pd.read_csv("standard_random.csv")
std_reversed = pd.read_csv("standard_reversed.csv")
std_almost = pd.read_csv("standard_almost_sorted.csv")

plot_single(std_random, "size", "ms", "Merge Sort - случайные данные", "std_random.png")
plot_single(std_reversed, "size", "ms", "Merge Sort - обратно отсортированные данные", "std_reversed.png")
plot_single(std_almost, "size", "ms", "Merge Sort - почти отсортированные данные", "std_almost_sorted.png")

hyb_random = pd.read_csv("hybrid_random.csv")
hyb_reversed = pd.read_csv("hybrid_reversed.csv")
hyb_almost = pd.read_csv("hybrid_almost_sorted.csv")

def plot_hybrid(df, title, fname):
    plt.figure()
    thresholds = [c for c in df.columns if c != "size"]
    for t in thresholds:
        plt.plot(
            df["size"], df[t],
            linewidth=0.9,
            linestyle='-',
            label=f"Порог {t}"
        )
    plt.xlabel("Размер массива")
    plt.ylabel("Время (мс), медиана 5 запусков")
    plt.title(title)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(fname, dpi=200)
    plt.close()

window = 5
std_random["ms_smooth"] = std_random["ms"].rolling(window, center=True, min_periods=1).mean()
std_reversed["ms_smooth"] = std_reversed["ms"].rolling(window, center=True, min_periods=1).mean()
std_almost["ms_smooth"] = std_almost["ms"].rolling(window, center=True, min_periods=1).mean()

plot_hybrid(hyb_random, "Merge+Insertion Sort - случайные данные", "hyb_random.png")
plot_hybrid(hyb_reversed, "Merge+Insertion Sort - обратно отсортированные данные", "hyb_reversed.png")
plot_hybrid(hyb_almost, "Merge+Insertion Sort - почти отсортированные данные", "hyb_almost.png")

plt.figure()
plt.plot(std_random["size"], std_random["ms_smooth"], linestyle='-', linewidth=0.9, label="Случайные данные")
plt.plot(std_reversed["size"], std_reversed["ms_smooth"], linestyle='-', linewidth=0.9, label="Обратно отсортированные")
plt.plot(std_almost["size"], std_almost["ms_smooth"], linestyle='-', linewidth=0.9, label="Почти отсортированные")

plt.xlabel("Размер массива")
plt.ylabel("Время (мс), медиана запусков")
plt.title("Standard Merge Sort — Сравнение типов входных данных")
plt.legend()
plt.grid(True, linestyle='--', linewidth=0.4, alpha=0.8)
plt.tight_layout()

# Сохранить картинку с хорошим DPI
plt.savefig("standard_comparison.png", dpi=200)
plt.close()
