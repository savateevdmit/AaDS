import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results_improved.csv')

plt.figure(figsize=(14, 7))
plt.plot(df['percent'], df['exact'], 'b-',
         label='Точное значение Ft0', linewidth=2.5, zorder=3)

plt.plot(df['percent'], df['mean_std'], 'r--',
         label='Стандартный HyperLogLog', linewidth=2.5, alpha=0.8, zorder=2)

plt.plot(df['percent'], df['mean_compact'], 'g:',
         label='Улучшенный HyperLogLog', linewidth=2.5, alpha=0.8, zorder=1)

plt.xlabel('Часть потока в %', fontsize=13)
plt.ylabel('Количество уникальных элементов', fontsize=13)
plt.title('Сравнение стандартного, улучшенного HyperLogLog и точного значения', fontsize=15)
plt.legend(fontsize=12, loc='upper left')
plt.grid(True, alpha=0.3, linestyle='--')
plt.tight_layout()
plt.savefig('graph3_improved_comparison.png', dpi=300)
plt.close()

plt.figure(figsize=(14, 7))

plt.plot(df['percent'], df['mean_compact'], 'g-',
         label='E(N_t) - улучшенная версия', linewidth=2.5, zorder=3)

lower_compact = df['mean_compact'] - df['std_dev_compact']
upper_compact = df['mean_compact'] + df['std_dev_compact']
plt.fill_between(df['percent'], lower_compact, upper_compact,
                 alpha=0.3, color='green',
                 label='Область неопределённости (±σ)', zorder=1)

plt.plot(df['percent'], df['exact'], 'b--',
         label='Точное значение Ft0', linewidth=2, alpha=0.7, zorder=2)


plt.xlabel('Часть потока в %', fontsize=13)
plt.ylabel('Количество уникальных элементов', fontsize=13)
plt.title('Статистика улучшенного HyperLogLog с неопределённостью', fontsize=15)
plt.legend(fontsize=12, loc='upper left')
plt.grid(True, alpha=0.3, linestyle='--')
plt.tight_layout()
plt.savefig('graph4_improved_statistics.png', dpi=300)
plt.close()

import numpy as np

df['error_std'] = np.abs(df['mean_std'] - df['exact']) / df['exact'] * 100
df['error_compact'] = np.abs(df['mean_compact'] - df['exact']) / df['exact'] * 100

plt.figure(figsize=(14, 7))

plt.plot(df['percent'], df['error_std'], 'r-',
         label='Ошибка стандартного', linewidth=2.5, marker='o', markersize=5)
plt.plot(df['percent'], df['error_compact'], 'g-',
         label='Ошибка улучшенного', linewidth=2.5, marker='s', markersize=5)

plt.xlabel('Часть потока в %', fontsize=13)
plt.ylabel('Относительная ошибка (%)', fontsize=13)
plt.title('Сравнение относительных ошибок двух версий', fontsize=15, fontweight='bold')
plt.legend(fontsize=12)
plt.grid(True, alpha=0.3, linestyle='--')
plt.tight_layout()
plt.savefig('graph5_error_comparison.png', dpi=300)
plt.close()

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

memory_std = 4096
memory_compact = 2560
savings = memory_std - memory_compact

categories = ['Стандартный\n4096 байт', 'Улучшенный\n2560 байт']
values = [memory_std, memory_compact]
colors = ['#ff6b6b', '#51cf66']

bars = ax1.bar(categories, values, color=colors, edgecolor='black', linewidth=1.5)
ax1.set_ylabel('Память (байты)', fontsize=12)
ax1.set_title('Потребление памяти', fontsize=14, fontweight='bold')
ax1.set_ylim(0, 5000)
ax1.grid(True, alpha=0.3, axis='y')

for bar, val in zip(bars, values):
    height = bar.get_height()
    ax1.text(bar.get_x() + bar.get_width()/2., height + 100,
             f'{val} байт\n({val/1024:.2f} КБ)',
             ha='center', va='bottom', fontsize=11, fontweight='bold')

savings_percent = 100 * savings / memory_std
labels = [f'Используется\n{memory_compact} байт\n({100-savings_percent:.1f}%)',
          f'Сэкономлено\n{savings} байт\n({savings_percent:.1f}%)']
sizes = [memory_compact, savings]
colors_pie = ['#51cf66', '#e0e0e0']
explode = (0.05, 0)

wedges, texts, autotexts = ax2.pie(sizes, labels=labels, autopct='',
                                   colors=colors_pie, explode=explode,
                                   startangle=90, textprops={'fontsize': 11})
ax2.set_title('Экономия памяти (37.5%)', fontsize=14, fontweight='bold')

plt.tight_layout()
plt.savefig('graph6_memory_savings.png', dpi=300)
plt.close()