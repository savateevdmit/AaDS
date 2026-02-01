import pandas as pd
import numpy as np

df = pd.read_csv('results.csv')

B = 12
m = 2**B

theoretical_error_1 = 1.04 / np.sqrt(m)
theoretical_error_2 = 1.3 / np.sqrt(m)

df['relative_error_percent'] = np.abs(df['mean_estimate'] - df['exact']) / df['exact'] * 100

mean_error = df['relative_error_percent'].mean()
max_error = df['relative_error_percent'].max()

print(f"B = {B}, m = {m}")
print(f"Средняя ошибка: {mean_error:.4f}%")
print(f"Максимальная ошибка: {max_error:.4f}%")
print(f"Граница 1.04/sqrt(m): {theoretical_error_1*100:.4f}%")
print(f"Граница 1.3/sqrt(m): {theoretical_error_2*100:.4f}%")
print()

df['coef_variation_percent'] = df['std_dev'] / df['mean_estimate'] * 100
mean_cv = df['coef_variation_percent'].mean()

print(f"Средний коэффициент вариации: {mean_cv:.4f}%")
print(f"Средняя ширина интервала ±σ: {(df['std_dev'].mean()/df['mean_estimate'].mean()*100*2):.4f}%")
print(f"Min std_dev: {df['std_dev'].min():.2f}")
print(f"Max std_dev: {df['std_dev'].max():.2f}")
print()

alpha = 0.7213 / (1.0 + 1.079 / m)
print(f"Параметр B = {B}")
print(f"Память: {m} байт ({m/1024:.1f} КБ)")
print(f"Константа α: {alpha:.6f}")
print(f"Фактическая ошибка {mean_error:.4f}% < теоретической {theoretical_error_1*100:.4f}%")
