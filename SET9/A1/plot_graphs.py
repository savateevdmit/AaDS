import json
import os
import plotly.graph_objects as go
import plotly.io as pio

pio.templates.default = "plotly_white"

with open("data/measurements.json", "r", encoding="utf-8") as f:
    data = json.load(f)
measurements = data["measurements"]

os.makedirs("graphics", exist_ok=True)

array_types = ['random', 'reverse', 'almost_sorted', 'common_prefix']
array_type_names = {
    'random': 'Случайный',
    'reverse': 'Обратно отсортированный',
    'almost_sorted': 'Почти отсортированный',
    'common_prefix': 'Массив с общим префиксом'
}

ALG_ORDER = [
    'QUICKSORT (Стандартный)',
    'MERGESORT (Стандартный)',
    'STRING QUICKSORT (Тернарный)',
    'STRING MERGESORT',
    'MSD RADIX SORT (Без переключения)',
    'MSD RADIX SORT (С переключением)',
]

colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#e377c2']
dash_styles = ['solid', 'dash', 'dot', 'dashdot', 'longdash', 'longdashdot']


def get_data(array_type, algorithm, metric):
    pts = [(m['size'], m[metric]) for m in measurements
           if m['array_type'] == array_type and m['algorithm'] == algorithm]
    pts.sort()
    return [p[0] for p in pts], [p[1] for p in pts]


for metric, ylabel, filename_suffix, title_suffix in [
    ('avg_char_comparisons', 'Число символьных сравнений', 'comparisons', 'Количество посимвольных сравнений'),
    ('avg_time_us', 'Время выполнения, мкс', 'time', 'Время выполнения'),
]:
    for arr_type in array_types:
        fig = go.Figure()
        for i, alg in enumerate(ALG_ORDER):
            xs, ys = get_data(arr_type, alg, metric)
            if xs:
                fig.add_trace(go.Scatter(
                    x=xs, y=ys,
                    mode='lines+markers',
                    name=alg,
                    line=dict(color=colors[i], width=2.5, dash=dash_styles[i % len(dash_styles)]),
                    marker=dict(size=6, color=colors[i], symbol='circle'),
                ))

        type_name = array_type_names[arr_type]
        title_text = f"{title_suffix} - {type_name} массив"

        fig.update_layout(
            title=dict(
                text=title_text,
                font=dict(size=17, color='#222'),
                x=0.5,
                xanchor='center',
            ),
            width=1100,
            height=560,
            margin=dict(l=80, r=40, t=130, b=80),
            legend=dict(
                orientation='h',
                yanchor='bottom',
                y=1.02,
                xanchor='center',
                x=0.5,
                font=dict(size=11.5, color='#222'),
                bgcolor='rgba(255,255,255,0.85)',
                bordercolor='#ccc',
                borderwidth=1,
            ),
            plot_bgcolor='white',
            paper_bgcolor='white',
            xaxis=dict(
                title=dict(text='Размер массива (количество строк)', font=dict(size=13, color='#333')),
                tickfont=dict(size=11, color='#333'),
                gridcolor='#e8e8e8',
                gridwidth=1,
            ),
            yaxis=dict(
                title=dict(text=ylabel, font=dict(size=13, color='#333')),
                tickfont=dict(size=11, color='#333'),
                gridcolor='#e8e8e8',
                gridwidth=1,
            ),
        )

        fname = f"graphics/{filename_suffix}_{arr_type}.png"
        fig.write_image(fname, scale=2)
        print(f"Saved: {fname}")
