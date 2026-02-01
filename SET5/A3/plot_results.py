import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')

# График 1: Сравнение оценки и точного значения
plt.figure(figsize=(12, 6))
plt.plot(df['percent'], df['exact'], 'b-', label='Точное значение Ft0', linewidth=2.5)
plt.plot(df['percent'], df['mean_estimate'], 'r--', label='Оценка HyperLogLog N_t', linewidth=2.5)
plt.xlabel('Часть потока в %', fontsize=12)
plt.ylabel('Количество уникальных элементов', fontsize=12)
plt.title('Сравнение оценки HyperLogLog и точного значения', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('graph1_comparison.png', dpi=300)
plt.close()

# График 2: Статистики с областью неопределённости
plt.figure(figsize=(12, 6))
plt.plot(df['percent'], df['mean_estimate'], 'g-', label='E(N_t) - средняя оценка', linewidth=2.5)
plt.fill_between(df['percent'], df['lower_bound'], df['upper_bound'],
                 alpha=0.3, color='green', label='Область неопределённости (±σ)')
plt.plot(df['percent'], df['exact'], 'b--', label='Точное значение Ft0', linewidth=1.5, alpha=0.7)
plt.xlabel('Часть потока в %', fontsize=12)
plt.ylabel('Количество уникальных элементов', fontsize=12)
plt.title('Статистика оценок HyperLogLog с неопределённостью', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('graph2_statistics.png', dpi=300)
plt.show()
