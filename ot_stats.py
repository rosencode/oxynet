import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


if __name__ == "__main__":
    print("Module: ot_stats.")


def load_spikes(filename):

    # Read spike data into dataframe    
    df = pd.read_csv(filename)
    print(f'Loaded datafile {filename}.\n')

    # Change spike times units (ms -> s)
    df['Spike_time'] = 0.001 * df['Spike_time']
      
    return df


def freqs(ts, t_1, t_2, delta_t):

    n_bins = 1 + int((t_2 - t_1) / delta_t)
    bin_edges = [(t_1 + i*delta_t) for i in range(n_bins)]

    return np.histogram(ts, bin_edges)


def firing_rates(df_grouped, cells, t_1, t_2, binsize):

    nplots = len(cells)
    
    # Create subplots
    (fig, axes) = plt.subplots(nplots, figsize = (14, 8))

    for iplot, cell in enumerate(cells):

        # Extract spikes for selected cell
        ts = df_grouped.get_group(cell)['Spike_time'] 
   
        # Compute firing rates
        (f, edges) = freqs(ts, t_1, t_2, binsize)

        axes[iplot].plot(edges[:-1],f, '.-', label = f'Neuron #{cell}')
#         axes[i].bar(edges[:-1],f)     

        axes[iplot].set_ylabel('Frequency (spikes/s)')
        axes[iplot].legend(loc = 'upper left', fontsize = 12)
        
    # set x-axis label on bottom plot
    axes[nplots-1].set_xlabel('Time (s)')
    
    return fig


def raster_plot(df_grouped, cells, t_1, t_2):

    fig = plt.figure(num = 2, figsize=(8, 8))

    for i, cell in enumerate(cells):
        ts = df_grouped.get_group(cell)['Spike_time']
        ts = ts[(ts>t_1) & (ts<t_2)]
        plt.plot(ts.values, i*np.ones(len(ts.values)),'b|')
        
    plt.xlabel('Time (s)')
    plt.ylabel('Neuron index')
    
    return fig