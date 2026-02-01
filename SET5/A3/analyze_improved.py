import pandas as pd
import numpy as np

df = pd.read_csv('results_improved.csv')

B = 12
m = 2**B

# Память
memory_std = m
memory_improved = int(m * 5 / 8)  # 5 бит на регистр
savings = memory_std - memory_improved
savings_percent = 100 * savings / memory_std

print(f"Стандартный HyperLogLog: {memory_std} байт ({memory_std/1024:.2f} КБ)")
print(f"Компактный HyperLogLog: {memory_improved} байт ({memory_improved / 1024:.2f} КБ)")
print(f"Экономия: {savings} байт ({savings_percent:.2f}%)")
print()

df['error_std'] = np.abs(df['mean_std'] - df['exact']) / df['exact'] * 100
df['error_compact'] = np.abs(df['mean_compact'] - df['exact']) / df['exact'] * 100

mean_error_std = df['error_std'].mean()
mean_error_cmp = df['error_compact'].mean()

print(f"Средняя ошибка стандартного: {mean_error_std:.4f}%")
print(f"Средняя ошибка компактного: {mean_error_cmp:.4f}%")
print(f"Разница: {abs(mean_error_std - mean_error_cmp):.4f}%")
print()

df['cv_std'] = df['std_dev_std'] / df['mean_std'] * 100
df['cv_compact'] = df['std_dev_compact'] / df['mean_compact'] * 100

mean_cv_std = df['cv_std'].mean()
mean_cv_cmp = df['cv_compact'].mean()

print(f"Средний коэф. вариации стандартного: {mean_cv_std:.4f}%")
print(f"Средний коэф. вариации компактного: {mean_cv_cmp:.4f}%")
print()

print(f"Экономия памяти: {savings_percent:.1f}% ({savings} байт)")
print(f"Потеря точности: {abs(mean_error_std - mean_error_cmp):.4f}%")
print(f"Изменение стабильности: {abs(mean_cv_std - mean_cv_cmp):.4f}%")
