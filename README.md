# Oxynet

A C++ program to simulate the dynamics of the oxytocin network model described in [1]. Given a list of model parameters as inputs, oxynet simulates the model dynamics and outputs on a file a list of firing times for the neurons in the network.


## Requirements

## Building

`git clone https://github.com/rosencode/oxynet.git`

`./make install`


## Usage

`oxytnet [-i parameters_filename] [-o spiketimes_filename] [-t output_connections_filename] [-g]`

-g: stops before running the simulation


A table

Name | Value
v_{rest} | -65.0

## Created 
01 March 2021

## Last Revision 
28 March 2021

## Author
Enrico Rossoni (enrico.rossoni@gmail.com)

## References 
[1] Rossoni et al. (2008) Emergent Synchronous Bursting of Oxytocin Neuronal Network. PLoS Comput Biol 4(7): e1000123. doi:10.1371/journal.pcbi.1000123.
