// Project: oxytnet 
// Parameters.cpp

#include "Parameters.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream> // stringstream
#include <iomanip> // setw, setprecision

using std::cin;     
using std::cout;
using std::endl;
using std::setw;        
using std::setprecision;

using std::ifstream;    
using std::ofstream;
using std::istream;     
using std::ostream;
using std::stringstream;

using std::map;         
using std::vector;
using std::string;

bool initialize_parameters(map_type &m)
{

    m["Ncells"]        = 48;       // (>0)
    m["Ndend"]         = 2;        // (>1)
    m["Nbun"]          = 12;       // (>0)
    m["tau_m"]         = 10.8;     // ms (>0)
    m["vrest"]         = -62.0;    // mV
    m["vthre0"]        = -50.0;    // mV
    m["a_E"]           = 0.0645;   // 0.0645 -> EPSP amplitude of 4mV for v=v_E
    m["a_I"]           = 0.22;     // 0.22 -> IPSP amplitude of 4mV for v=v_I
    m["hap_tau"]       = 12.5;     // ms (>0)
    m["hap_max"]       = 40.0;     // mV
    m["half_ahp"]      = 45.0;     //
    m["ahp_max"]       = 40.0;     // mV?
    m["ahp_tau"]       = 2000.0;   // ms  (>0)
    m["tau_ot"]        = 1000.0;   // ms  (>0)
    m["ot_to_dep"]     = 0.5;      //
    m["rel_delay"]     = 5.0;      // ms (>0)
    m["cann_thre"]     = 0.03;     //
    m["tau_cann"]      = 6000.0;   // ms (>0)
    m["ec_step"]       = 0.0025;   //
    m["res_max"]       = 200.0;    //
    m["tau_r"]         = 400000.0; // ms (>0)
    m["f_max"]         = 0.6;
    m["tau_rel"]       = 50.0;     // ms
    m["T_OT_max"]      = 25.0;     // mV
    m["k_r"]           = 0.045;
    m["re"]            = 80.0;     // Hz (>0)
    m["ri"]            = 80.0;     // Hz (>0)
    m["I_su"]          = 0.0005;
    m["t_end"]         = 300000;   // ms (>0)
    m["random_seed"]   = 1;        // set =0 to randomize each trial


    return true;

}

void show_parameters(const map_type &pars)
{
    cout << "Model parameters:\n\n";
    for (const auto &[key, value] : pars)
    { 
        cout << std::left << setw(15) << key << " =\t" << value << "\n";
    }
    cout << "\n";
}


void write_parameters(const map_type &pars, ofstream &ofs)
{
    for (const auto &[key, value] : pars)
    {
        ofs << key << ", " << value << "\n";
    }
}


bool save_parameters(const string& filename, const map_type &pars)
{
    ofstream out(filename);
    if(out){
        for (const auto &[key, value] : pars) {
            out << key << ", " << value << "\n";
        }
        out.close();
    } else 
    {
        cout << "Cannot save parameters to file " << filename << "\n";
        return false;
    }
    return true;
}


bool load_parameters(const string& filename, map_type &pars) 
{
    ifstream in(filename);
    if(in){
        read_model_parameters(pars, in);
        if(!validate_input_parameters(pars)){
            cout << "Some input parameters are out of bounds.\n";
            return false;
        }
    } else 
    {
        cout << "Cannot open parameter file "<< filename << "\n";
        return false;
    }
    return true;
}


void read_model_parameters(map_type &pars, std::istream& input_file)
{
    // Read input file data into a {par_name: par_value} map
    map_type file_pars;
    size_t pos = 0;
    std::string line, par_name;
    const std::string delimiter = "," , comment_id = "#";
    double par_value = 0.0;

    // read until end-of-file
    while(!getline(input_file, line).eof()){
        // ignore lines starting with # (comments) or not containing a delimiter
        if ((line.substr(0,comment_id.length()) != comment_id) && (pos = line.find(delimiter)) != std::string::npos) {
            // extract until first occurrence of delimiter
            par_name = line.substr(0, pos);
            // convert rest of line to stringstream and parse
            if (stringstream(line.substr(pos+1)) >> par_value) {
                file_pars[par_name] = par_value;
            } 
            else cout << " \n** Error in parsing input file: invalid value for parameter " << par_name  << " **\n\n";                 
        } 
        else cout<< "Skipping line " << line << " in input file." << "\n";
    }

    // iterate over model parameters and lookup keys in map
    map_type::iterator it = pars.begin(), it_found;
    while (it != pars.end())
    {
        // search for a match in file pars
        it_found = file_pars.find(it->first);
        if (it_found != file_pars.end())
        {
            it->second = it_found->second;
        }
        it++;
    }
}

bool validate_input_parameters(map_type &m){

    bool check = true;

    if ( m["Ncells"]       <  1      ||
         m["Ndend"]        <  1      ||
         m["Nbun"]         <  1      ||
         m["tau_m"]        <= 0      ||
         m["hap_tau"]      <= 0      ||
         m["ahp_tau"]      <= 0      ||
         m["tau_ot"]       <= 0      ||
         m["tau_rel"]      <= 0      ||
         m["rel_delay"]    <  0      ||
         m["tau_cann"]     <= 0      ||
         m["tau_r"]        <= 0      ||
         m["k_r"]          <= 0      ||
         m["k_r"]          > 1       ||
         m["f_max"]        > 1       ||
         m["re"]           <= 0      ||
         m["ri"]           <= 0 )    check = false;

    return check; 
}