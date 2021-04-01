# Oxynet: a C++ simulation engine for the oxytocin network system  

**oxynet** simulates the spiking activity of the magnocellular oxytocin network system, based on the model presented in ["Emergent Synchronous Bursting of Oxytocin Neuronal Network" by Rossoni et al. (2008)](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1000123)

## Requirements

* A C++17 compliant compiler

## Installation

```bash
$ git clone https://github.com/rosencode/oxynet.git
$ cd oxynet
$ make install
```

## Usage

`./oxynet [-i parameters_file] [-o spiketimes_file] [-t connections_file] [-g]`

Launching _oxynet_ without arguments runs a simulation with the default set of parameters as reported in [1], displaying synchronized bursting. 
To change the model parameters, select option `-i` with the parameter file. The latter should be formatted as a list of (name, value) pairs, as shown below. By default, all the parameters used in a simulation are saved on file `parameters.csv`.

The network connections are generated randomly upon execution and are saved on file `connections.csv` (select option `-t` to specify a different filename). 
The seed used to initialize the random number generator is saved with the other simulation parameters, thus allowing to run different simulations with the same network structure. 

The connection file is formatted as follows:
* On the first row is the triple (N_c, N_d, N_b) where N_c is the number of neurons, N_d is the number of dendrites on each neuron, and N_b is the number of bundles in the network;
* On each following is a comma-separated list of indices specifying the bundles wherein the i-th cell has its dendrites.

The spiking activity in the network is recorded by default on file `spike_times.csv` as of a list of (time, neuron_index) pairs preceded by a header row. 
Select option `-o ` to redirect the output to a different file. 

When the option `-g` is selected, _oxynet_ initializes and saves the network connections but the execution is stopped before generating the temporal dynamics. This can be useful for studying different network structures.

## Model parameters

The input file should be formatted as a list of (name, value) pairs. Parameter names should be exactly as follows: 

Name | Value (default) | Description
---- | --------------- | -----------
Ncells | 48 | Number of neurons 
Nbun | 12 | Number of bundles 
Ndend | 2 | Number of dendrites per cell 
vrest |  -62 | Resting potential (mV) 
vthre0 | -50 | Spike threshold baseline (mV) 
tau_m | 10.8 | Membrane time constant (ms)
I_su  | 0.0005 | Suckling input
a_E | 0.0645 | Excitatory PSP amplitude for v = v_E (mV)
a_I | 0.22 | Inhibitory PSP amplitude for v = v_E (mV)
re | 80 | Excitatory synaptic input rate (Hz)
ri | 80 | Inhibitory synaptic input rate (Hz)
ahp_max | 40 | AHP, maximum amplitude (mV)
ahp_tau | 2000 | AHP, time constant (ms)
cann_thre | 0.03 | EC threshold for synaptic attenuation
ec_step | 0.0025 | EC increase per unit oxytocin release
f_max | 0.6 | Maximum fractional attenuation of synaptic input rate 
tau_rel | 50 | Maximum interspike interval for release (ms) 
half_ahp | 45 | AHP, half-activation constant
hap_max | 40 | HAP, maximum amplitude (mV)
hap_tau | 12.5 | HAP, time constant (ms)
T_OT_max | 25 | Maximum OT-induced depolarization (mV)
tau_g_ot | 1000 | Time decay of OT-induced depolarization (ms)
k_r | 0.045 | Maximum fraction of dendritic stores released per spike
ot_to_dep | 0.5 | Depolarization per unit oxytocin release (mV)
rel_delay | 5 | Time delay for OT release (ms)
tau_cann | 6000 | Time constant for EC decay (ms)
tau_r | 400000 | Time constant for priming (ms)
t_end | 300000 | Simulation time (ms) 
random_seed | 1 | Seed for random number generation. Set 0 to randomize at each run

The file may include as few parameters as required. Note that the current version of `oxynet` will only run simulations with Ndend = 2, but will still generate network connections with any value Ndend>1.

## Created 
01 March 2021

## Last Revision 
02 April 2021

## Author
Enrico Rossoni (enrico.rossoni@gmail.com)

## References 
[1] Rossoni et al. (2008) Emergent Synchronous Bursting of Oxytocin Neuronal Network. PLoS Comput Biol 4(7): e1000123. doi:10.1371/journal.pcbi.1000123.
