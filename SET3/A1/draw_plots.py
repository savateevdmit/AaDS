import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math
from matplotlib import rc


def calculate_exact_area():
    pi = math.pi
    arcsin_08 = math.asin(0.8)
    return 0.25 * pi + 1.25 * arcsin_08 - 1

exact_area = calculate_exact_area()
df = pd.read_csv('results.csv')

df['error1_percent'] = np.abs(df['res1'] - exact_area) / exact_area * 100
df['error2_percent'] = np.abs(df['res2'] - exact_area) / exact_area * 100

max_error = max(df['error1_percent'].max(), df['error2_percent'].max())
upper_limit = max_error * 1.2

# приближенные значения площади
plt.figure(figsize=(10, 6))
plt.plot(df['N'], df['res1'], 'b-', linewidth=2.5, label='Способ 1')
plt.plot(df['N'], df['res2'], 'r-', linewidth=2.5, label='Способ 2')
plt.axhline(y=exact_area, color='g', linestyle='--', linewidth=2.5,
            label=f'Точное значение: {exact_area:.5f}')
plt.xscale('log')
plt.xlabel('Количество точек N', fontsize=13)
plt.ylabel('Приближенное значение площади', fontsize=13)
plt.title('Зависимость приближенного значения площади\nот количества случайных точек', fontsize=15, fontweight='bold')
plt.legend(fontsize=12, loc='best')
plt.grid(True, which="both", ls="--", alpha=0.6)
plt.tight_layout()
plt.savefig('plot1.png', dpi=300)
plt.close()

# относительное отклонение в процентах (линейная шкала от 0)
plt.figure(figsize=(10, 6))
plt.plot(df['N'], df['error1_percent'], 'b-', linewidth=2.5, label='Способ 1')
plt.plot(df['N'], df['error2_percent'], 'r-', linewidth=2.5, label='Способ 2')
plt.xscale('log')
plt.ylim(0, upper_limit)  # Шкала начинается с 0
plt.xlabel('Количество точек N', fontsize=13)
plt.ylabel('Относительное отклонение (%)', fontsize=13)
plt.title('Зависимость относительного отклонения\nот количества случайных точек', fontsize=15, fontweight='bold')
plt.legend(fontsize=12, loc='best')
plt.grid(True, which="both", ls="--", alpha=0.6)
plt.tight_layout()
plt.savefig('plot2.png', dpi=300)
plt.close()

last_row = df.iloc[-1]