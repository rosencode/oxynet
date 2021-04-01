from sys import argv
import matplotlib.pyplot as plt
import ot_stats

if len(argv) < 2:
    filename = input("Enter spike filename: ")
else:
    filename = argv[1]

try:
    
    # Load spike data into dataframe
    # --------------------------------
    df_spike_times = ot_stats.load_spikes(filename);
    t_first = df_spike_times['Spike_time'].min()
    t_last  = df_spike_times['Spike_time'].max()
    

    # Group spike times by neuron
    # --------------------------------
    by_neuron = df_spike_times.groupby('Neuron_index', sort = False)
    

    # Identify active cells
    # --------------------------------
    active_cells = list(by_neuron.groups.keys())
    n_cells      = len(active_cells)    
    print(f'Found {n_cells} active cells.\n')


    # Plot firing rates
    # --------------------------------
    nplots    = 3
    t_rates_1 = 0
    t_rates_2 = t_last
    bin_size  = 1
    fig1 = ot_stats.firing_rates(by_neuron, active_cells[0:nplots], t_rates_1, t_rates_2, bin_size);


    # Display raster plot
    # --------------------------------
    t_burst_start = 190
    t_burst_end   = 220
    fig2 = ot_stats.raster_plot(by_neuron, active_cells, t_1 = t_burst_start, t_2 = t_burst_end);
    
    # Show all figures

    # --------------------------------
    plt.show()

except FileNotFoundError:
    
    print(f"Spike times file {filename} not found.\n")





  