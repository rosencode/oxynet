# Oxynet

**oxynet** simulates the spiking activity of the magnocellular oxytocin network system, according to the model presented in ["Emergent Synchronous Bursting of Oxytocin Neuronal Network" by Rossoni et al. (2008)](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1000123)

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

Launch _oxynet_ without arguments to run a simulation with default parameters. Select option `-i` to provide a parameter file (see below for accepted format). 

The network connections are generated randomly within the code and saved on file `connections.csv` (select option `-t` to specify a different filename). 
The seed used to initialize the random number generator is saved with the other simulation parameters, thus allowing to run different simulations with the same network structure. 

The connection file is formatted as follows. On the first row is the triple (N_c, N_d, N_b) where N_c is the number of neurons, N_d is the number of dendrites on each neuron, and N_b is the number of bundles in the network. Each following row gives, for the i-th cell, a comma-separated list of indices specifying the bundles wherein the cell has its dendrites.

The spiking activity in the network is recorded by default on file `spike_times.csv` in the form of a list of (time, neuron_index) pairs. 
Select option `-o` to redirect the output to a different file. 

By default, all the parameters used in the simulation are saved on file `parameters.csv` as a list of (parameter_name, parameter_value) pairs. 

By selecting option `-g`, the code initializes and saves the network connections but execution is stopped before generating the temporal dynamics. This can be useful when studying different network topologies.

## Model parameters

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
