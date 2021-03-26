#ifndef GUARD_Parameters
#define GUARD_Parameters

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

typedef std::map<std::string, double> map_type;

map_type initialize_model_parameters();

void show_parameters(const map_type& );
void write_parameters(const map_type& , std::ofstream& );
void read_model_parameters(map_type& , std::istream& );

bool save_parameters(const std::string& , const map_type& );
bool load_parameters(const std::string& , map_type& );
bool validate_input_parameters(map_type& );

#endif