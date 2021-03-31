# Oxynet

**oxynet** simulates the spiking activity of the magnocellular oxytocin network, according to the model presented in in Rossoni et al. (2008) Emergent Synchronous Bursting of Oxytocin Neuronal Network. PLoS Comput Biol 4(7): e1000123. doi:10.1371/journal.pcbi.1000123.

## Requirements

* A C++17 compliant compiler
* make

## Installation

```bash
$ git clone https://github.com/rosencode/oxynet.git
$ cd oxynet
$ make install
```

## Usage

`./oxynet [-i parameters_file] [-o spiketimes_file] [-t connections_file] [-g]`

Launch `oxynet` without arguments to run a simulation of the oxytocin model with default parameters. 

Within the code, a set of network connections is generated at random (the random seed is saved as simulation parameter). 
By default, oxynet saves the resulting network structure on file `connections.csv`; use flag `-t` to specify a different filename. 

Inside `network_connections_file`, the first row is in the form (N_c, N_d, N_b) where N_c is the number of neurons, N_d is the number of dendrites per each neuron, and N_b is the number of bundles in the network. The following rows give, for each cell, a comma-separated list of bunlde indices specifying which bundle each dendrite is projecting to.

The spiking activity in the network is recorded by default in file `spike_times.csv` in the form of a list of (time, neuron_index) pais. 
Use flag `-o` to redirect the output to a different file. 

By default, the parameters used in the simulation are saved in the file `parameters.csv` as a list of (parameter_name, parameter_value) pairs. To run a model  simulation with a different set of parameters, use flag `-i` passing a parameter file conforming to the same format.

Using flag `-g`, the code initialized and saves the network structure but stops before generating the temporal dynamics. This can be useful when studying dirrent network topologies, especially with large networks.

## Model description

Parameter | Value
------------ | -------------
v_{rest} | -65.0 mV
... | ...

## Created 
01 March 2021

## Last Revision 
28 March 2021

## Author
Enrico Rossoni (enrico.rossoni@gmail.com)

## References 
[1] Rossoni et al. (2008) Emergent Synchronous Bursting of Oxytocin Neuronal Network. PLoS Comput Biol 4(7): e1000123. doi:10.1371/journal.pcbi.1000123.
