import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
import ast, time

path = r'FSS_DBMS.txt'
status = [{}]


def open_DBMS():
    file = open(path, 'r')
    file_content = file.readlines()
    file.close()
    for item in file_content:
        if item != "":
            status.append(ast.literal_eval(item))

    status.remove({})


def create_chart(values):
    fig = go.Figure()

# trace Stadel
    fig.add_trace(go.Scatter(x=values[0], y=values[3], mode='lines + markers', name='Temperatur Heu', line=dict(color='#FF3131', width=4, dash='dot')))
    fig.add_trace(go.Scatter(x=values[0], y=values[4], mode='lines + markers', name='Temperatur Gruemet', line=dict(color='#E63636', width=4, dash='dot')))
    fig.add_trace(go.Scatter(x=values[0], y=values[5], mode='lines + markers', name='Temperatur Stadel', line=dict(color='#C32A2A', width=4, dash='dot')))
    fig.add_trace(go.Scatter(x=values[0], y=values[6], mode='lines + markers', name='Feuchtigketi Stadel', line=dict(color='#1B750E', width=4, dash='dot')))

# trace Wasser
    fig.add_trace(go.Scatter(x=values[1], y=values[7], mode='lines + markers', name='Temperatur Wasser', line=dict(color='#2F77FF', width=4, dash='dot')))

# trace Stall
    fig.add_trace(go.Scatter(x=values[2], y=values[8], mode='lines + markers', name='Temperatur Stall', line=dict(color='#D8B1B1', width=4, dash='dot')))
    fig.add_trace(go.Scatter(x=values[2], y=values[9], mode='lines + markers', name='Feuchtigkeit Stall', line=dict(color='#6FA882', width=4, dash='dot')))

    fig.update_layout(title='Temperaturverlauf', xaxis_title='Zeit', yaxis_title='Temperatur (Grad Celsius)')
    return fig


def create_chart_data(time_span):

    time_stadel = []
    temp_heu = []
    temp_gruemet = []
    temp_stadel = []
    hum_stadel = []

    time_wasser = []
    temp_wasser = []

    time_stall = []
    temp_stall = []
    hum_stall = []

# saving times to lists
    for item in status:
        time_stadel.append(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(item[1]['time'])))

    for item in status:
        time_wasser.append(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(item[2]['time'])))

    for item in status:
        time_stall.append(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(item[3]['time'])))

# saving values to lists
    for item in status:
        temp_heu.append(item[1][2][0])

    for item in status:
        temp_gruemet.append(item[1][2][1])

    for item in status:
        temp_stadel.append(item[1][3]['temp'])

    for item in status:
        hum_stadel.append(item[1][3]['hum'])

    for item in status:
        temp_wasser.append(item[2][2])

    for item in status:
        temp_stall.append(item[3][3]['temp'])

    for item in status:
        hum_stall.append(item[3][3]['hum'])

    value = [time_stadel, time_wasser, time_stall, temp_heu, temp_gruemet, temp_stadel, hum_stadel, temp_wasser, temp_stall, hum_stall]
    return value


open_DBMS()

values = create_chart_data(0)
chart = create_chart(values)
chart.show()
