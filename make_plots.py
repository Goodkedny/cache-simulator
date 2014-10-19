#!/usr/bin/python2
"""
Make plots for simulation results

Assumes simulations were run using ./run_simulations.sh
"""

import numpy as np
import matplotlib.pyplot as plt
import os

def plot_graph(title, ylabel, configs, traces, out_line):
    data = []
    for conf in configs:
        conf_data = []
        for trace in traces:
            conf_data.append(float(open("simulation_results/%s/%s.out" % (conf, trace)).readlines()[out_line]))
        data.append(conf_data)

    fig, ax = plt.subplots()
    graph_width = len(traces)
    indeces = np.arange(graph_width)
    width = 0.8/len(configs)
    colors = ['orange', 'b', 'm', 'g', 'r', 'k', 'c', 'y']
    for i,conf in enumerate(configs):
        ax.bar(indeces + width*i, data[i], width, color=colors[i%len(colors)])
    
    confnames = [c.replace(".conf", "") for c in configs]
    tracenames = [t.replace(".trace", "") for t in traces]

    ax.set_ylabel(ylabel)
    ax.set_xlabel("Trace")
    ax.set_title(title)

    ax.set_xticks(indeces + 0.4)
    ax.set_xticklabels(tracenames)

    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    ax.legend(confnames, fontsize='small', loc='center left', bbox_to_anchor=(1, 0.5), title="Config")

    plt.savefig(title.replace(' ', '') + ".png")

configs = os.listdir("lab2_confs/confs/")
traces = os.listdir("traces/")

hit_rate_line = 0
memory_latency_line = 4

plot_graph('Total Hit Rate', 'Hit Rate', configs, traces, hit_rate_line)
plot_graph('Average Memory Access Latency', 'Memory Latency', configs, traces, memory_latency_line)
