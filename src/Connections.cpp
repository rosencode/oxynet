// Project: oxynet 
// Connections.cpp

#include "Connections.h"

#include <vector>
#include <string>    
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>      // stringstream
#include <random>       // seed_seq, mt19937, sample

using std::vector;      
using std::string;
using std::ifstream;    
using std::ofstream;
using std::istream;     
using std::ostream;
using std::cout;


vector<vector<int> > generate_network_connections(int Nc, int Nd, int Nb, uint32_t random_seed)
{
    vector<vector<int> > connections(Nc, vector<int>(Nd));

    vector<int> bundle_list(Nb);
    for (uint i = 0; i != Nb; ++i) 
        bundle_list[i] = i;

    std::seed_seq seeder{random_seed};
    std::mt19937 my_rng(seeder);

    for (uint i = 0; i < Nc; ++i)
    {
        std::sample(bundle_list.begin(), bundle_list.end(), connections[i].begin(), Nd, my_rng);                    
    }
    return connections;
}



void write_network_connections(const vector<vector<int> > &bun, std::ostream& os)
{
    for (auto i = 0; i != bun.size(); ++i) {
        for (auto j = 0; j != bun[i].size() - 1; ++j) {
            os << bun[i][j] << ", ";
        }
        os << bun[i][bun[i].size() - 1] << "\n";
    }
}


bool save_network_connections(const string& filename, int Nc, int Nd, int Nb, const vector<vector<int> > &bun){

    ofstream out(filename);
    if(out){
        out << Nc << "," << Nd << "," << Nb << "\n";
        for (auto cell = 0; cell != bun.size(); ++cell)
        {
            for (auto dend = 0; dend != bun[cell].size() - 1; ++dend)
            {
                out << bun[cell][dend] << ", ";
            }
            out << bun[cell][bun[cell].size() - 1] << "\n";
        }
    } else {
        cout << "Cannot open file " << filename << "\n";
        return false;
    }
    return true;
}
