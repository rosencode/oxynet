#ifndef GUARD_Connections
#define GUARD_Connections

#include <ios>
#include <iostream>
#include <fstream>

#include <vector>
#include <string> 
#include <map>

std::vector<std::vector<int> > generate_network_connections(int Nc, int Nd, int Nb, uint32_t random_seed);
bool save_network_connections(const std::string& , int Nc, int Nd, int Nb, const std::vector<std::vector<int> >& );
void write_network_connections(const std::vector<std::vector<int> > &bun, std::ostream& os);

#endif