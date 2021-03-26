// Project: oxytnet 
// main.cpp


#include <ios>
#include <iostream>
#include <fstream>
#include <sstream> // stringstream
#include <iomanip> // setw
#include <vector>
#include <string> 
#include <map>
#include <random>    // default_random_engine
#include <time.h>   // time
#include <math.h> // pow, exp
#include <unistd.h>     // printf
#include <stdlib.h>     // exit
#include <getopt.h>     // getopt

#include "Parameters.h"
#include "Connections.h"

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
using std::vector;      
using std::string;

struct configs 
{
  string    inParsFile;
  string    connFile;
  string    outParsFile;
  string    spikeFile;
  uint      SPIKE_TIMES_BUFFERSIZE;
};

struct spike_event
{
    uint index;
    double time;
};

void write_spikes(std::ofstream& , std::vector<spike_event>::const_iterator , std::vector<spike_event>::const_iterator );

int main(int argc, char **argv)
{
    struct configs cfgs = 
    {
      .inParsFile               = "",
      .connFile                 = "connections.csv",
      .outParsFile              = "parameters.csv",
      .spikeFile                = "spike_times.csv",
      .SPIKE_TIMES_BUFFERSIZE   = 10000
    };

    bool run_simulation_flag = true;

    // Initialise model parameters to default values
    map_type model_pars = initialize_model_parameters();


    // Parse command-line options

    int opt;
    while ((opt = getopt(argc, argv, "ho:i:c:t:g")) != -1)
    {
        switch (opt) {

            case '?':
            case 'h':
            default: /* '?' */
                cout << "Usage: " << argv[0] << "\n [-i parameters_filename] \n [-o spiketimes_filename] \n [-t output_connections_filename] [-g]\n";
                exit(EXIT_FAILURE);

            case 'o':
                cfgs.spikeFile = optarg;
                continue;

            case 'i':
                cfgs.inParsFile = optarg;
                if (!load_parameters(cfgs.inParsFile, model_pars))
                    exit(EXIT_FAILURE);
                continue;

            case 't':
                cfgs.connFile = optarg;
                continue;      

            case 'g':
                run_simulation_flag = false;
                continue;                            
        }
    }

    const uint   ISI_BUFFER_SIZE = 2;
    const double ISI_BUFFER_INIT = 10000.0;       // ms


    // **********************************************************
    // GENERATE NETWORK STRUCTURE
    // **********************************************************  

    uint Nc = uint(model_pars["Ncells"]);
    uint Nb = uint(model_pars["Nbun"]);
    uint Nd = uint(model_pars["Ndend"]);   

    // assign random seed
    uint32_t random_seed;
    if(model_pars["random_seed"] != 0) {
        random_seed = uint32_t(model_pars["random_seed"]);
    } else {
        random_seed = std::time(NULL);   
    }

    vector<vector<int> > bundles = generate_network_connections(Nc, Nd, Nb, random_seed);
    cout << "A random network structure was generated with seed " << random_seed << "\n\n";   

    // Redefine model parameters Nc, Nd, Nb
    model_pars["Ncells"]  = Nc;
    model_pars["Nbun"]    = Nb;
    model_pars["Ndend"]   = Nd;      
    
    // Write simulation parameters to file
    save_parameters(cfgs.outParsFile, model_pars);
    
    // Display simulation parameters on standard output
    show_parameters(model_pars);

    // Write network connections to file
    if(save_network_connections(cfgs.connFile, Nc, Nd, Nb, bundles)){
        cout << "Network structure saved to file " << cfgs.connFile << "\n\n";
    }    

    if (!run_simulation_flag) exit(EXIT_SUCCESS);

    // **********************************************************
    // DEFINE EQUATION CONSTANTS
    // **********************************************************  

    if (Nd != 2) {
        cout << "Cannot run simulation with Nd != 2. Exiting.\n";
        exit(EXIT_FAILURE);
    }

    const double dt             = 1.0;  // ms   
    
    const double VE             = 0.0;
    const double VI             = -80.0;

    const double VREST          = model_pars["vrest"];
    const double VTHRE0         = model_pars["vthre0"];

    const double V_C_1          = dt /  model_pars["tau_m"];
    const double V_C_2          = (1.0 - V_C_1);
    const double V_C_3          = model_pars["vrest"] * V_C_1;

    const double AMP_EXC        = model_pars["a_E"];
    const double AMP_INH        = model_pars["a_I"];

    const int    N_ATT_INPUT    = (int) model_pars["n_att_input"];
    const double YY_BAR         = pow(model_pars["cann_thre"],model_pars["n_att_input"]);
    const double LAMBDA_EXC     = model_pars["re"] / 1000.0;
    const double LAMBDA_INH     = model_pars["ri"] / 1000.0;

    const double LAMBDA_EXC_DT  = dt * LAMBDA_EXC;
    const double LAMBDA_INH_DT  = dt * LAMBDA_INH;

    const double HAP_MAX        = model_pars["hap_max"];
    const double HAP_TAU        = model_pars["hap_tau"];

    const int    NAHP           = (int) model_pars["nahp"];
    const double AHP_MAX        = model_pars["ahp_max"];
    const double AHP_C_1        = dt/ model_pars["ahp_tau"];
    const double AHP_C_2        = (1.0 - AHP_C_1);
    const double YYY            = pow(model_pars["half_ahp"], NAHP);

    const double F_MAX          = model_pars["f_max"];
    const double K_2            = model_pars["k_2"];

    const double G_OT_C_1       = dt / model_pars["tau_g_ot"];
    const double G_OT_C_2       = (1.0 - G_OT_C_1);
    const double OT_TO_DEP      = model_pars["ot_to_dep"];

    const double REL_DELAY      = model_pars["rel_delay"];
    const double F_TH           = model_pars["f_th"];
    const double KPP_3          = model_pars["kpp_3"];

    const double RES_C_1        = dt / model_pars["tau_r"];
    const double RES_C_2        = (1.0 - RES_C_1);
    const double RES_C_3        = dt * model_pars["I_su"];

    const double ATT_INPUT_C_1  = dt / model_pars["tau_cann"];
    const double ATT_INPUT_C_2  = (1.0 - ATT_INPUT_C_1);
    const double EC_STEP        = model_pars["ec_step"];

    // **********************************************************
    // INITIALISE STATE VARIABLES
    // **********************************************************  

    double t = 0.0;                                                  // time (ms)

    vector<double>              v(Nc, VREST);                        // membrane potential (mV)

    vector<double>              vthreac(Nc, VTHRE0);                 // total adaptive spike threshold 
    vector<double>              AHP(Nc, 0.0);                        // After-hyperpolarization potential state

    vector<double>              att_input(Nb, 0.0);                  // EC-induced synaptic input attenuation

    vector<double>              ot(Nb, 0.0);                         // 
    vector<double>              ot_rel(Nb, 0.0);                     //
    vector<bool>                released(Nc, true);                  //

    vector<vector<double> >     res(Nc, vector<double>(Nd, 0.0));         // OT dendritic reserves
    vector<vector<double> >     g_OT(Nc, vector<double>(Nd, 0.0));        // 
    vector<double>              ttime(Nc, ISI_BUFFER_INIT);          // time interval since last spike (ms)
    vector<vector<double> >     isi_buffer(Nc, vector<double>(ISI_BUFFER_SIZE, ISI_BUFFER_INIT));

    // **********************************************************
    // INITIALIZE POISSON INPUTS
    // **********************************************************  

    std::default_random_engine generator(random_seed);

    typedef std::poisson_distribution<int>::param_type ptype;
    
    std::poisson_distribution<int> poisson_exc_1(LAMBDA_EXC_DT);
    std::poisson_distribution<int> poisson_inh_1(LAMBDA_INH_DT);

    std::poisson_distribution<int> poisson_exc_2(LAMBDA_EXC_DT);
    std::poisson_distribution<int> poisson_inh_2(LAMBDA_INH_DT);        


    // **********************************************************
    // Ancillary variables
    // **********************************************************  

    double rel = 0.0;
    double OTinput = 0.0;
    double zzz = 0.0; 
    double yy1 = 0.0, yy2 = 0.0;
    double att_1 = 1.0, att_2 = 1.0;
    int    nepsp_1 = 0, nipsp_1 = 0;
    int    nepsp_2 = 0, nipsp_2 = 0;
    double syn_inp = 0.0;


    // **********************************************************
    // SPIKE OUTPUT
    // **********************************************************  

    // Initialize spike times vector
    // vector<spike_event> spikes(SPIKE_TIMES_BUFFERSIZE);
    vector<spike_event> spikes(cfgs.SPIKE_TIMES_BUFFERSIZE);

    auto iter_spikes = spikes.begin();
    auto iter_spikes_end = spikes.end();

    ofstream spiketimes_file(cfgs.spikeFile);
    if(!spiketimes_file){
        cout << "Cannot open file " << cfgs.spikeFile << " for recording spikes.\n";
        exit(EXIT_FAILURE);
    }

    spiketimes_file << "Neuron_index" << "," << "Spike_time" << "\n";

    // **********************************************************
    // SOLVE MODEL EQUATION
    // **********************************************************  

    cout << "Simulation started.\n\n";

    time_t t_sim_start = std::time(NULL);

    while (t < model_pars["t_end"])
    {

        // Update time
        t = t + dt;
 
        for (uint i = 0; i < Nc; ++i)
        {
            // Reset the amount of oxytocin released
            rel = 0.0;  

            // Update the time interval since last spike
            ttime[i] = ttime[i] + dt;

            // Update the ISI buffer for delayed release
            isi_buffer[i][0] += dt;     

            // **********************************************************
            // SYNAPTIC INPUT (Time-dependent Poisson)
            // **********************************************************  

            // Calculate synaptic input attenuation
            
            yy1       = pow(att_input[bundles[i][0]], N_ATT_INPUT);
            yy2       = pow(att_input[bundles[i][1]], N_ATT_INPUT);

            att_1     = 1.0 - F_MAX*yy1/(yy1 + YY_BAR);
            att_2     = 1.0 - F_MAX*yy2/(yy2 + YY_BAR);
 
            // Update the rates

            poisson_exc_1.param(ptype(LAMBDA_EXC_DT*att_1));
            poisson_inh_1.param(ptype(LAMBDA_INH_DT*att_1));

            poisson_exc_2.param(ptype(LAMBDA_EXC_DT*att_2));
            poisson_inh_2.param(ptype(LAMBDA_INH_DT*att_2));
            
            // Generate Poisson distributed event counts

			nepsp_1 = poisson_exc_1(generator);
			nipsp_1 = poisson_inh_1(generator);
			
            nepsp_2 = poisson_exc_2(generator);
			nipsp_2 = poisson_inh_2(generator);

            // Compute total synaptic input

            syn_inp = AMP_EXC * (VE - v[i]) * (nepsp_1 + nepsp_2) 
                    - AMP_INH * (v[i] - VI) * (nipsp_1 + nipsp_2);

            // **********************************************************
            // MEMBRANE POTENTIAL
            // **********************************************************       

            // Original equation
            // v[i] = v[i] + dt * (model_pars["vrest"] - v[i]) / model_pars["tau_m"] + syn_inp;

            v[i] = V_C_2 * v[i]  + V_C_3 + syn_inp;

            // **********************************************************
            // OT INPUT
            // **********************************************************       

            OTinput = 0.0;     
            for (uint k = 0; k < Nd; ++k)
            {
                // Original equation
                // g_OT[i][k] = g_OT[i][k] - dt*g_OT[i][k]/model_pars["tau_g_ot"] + model_pars["ot_to_dep"]*ot[bun[i][k]];
                g_OT[i][k] = G_OT_C_2 * g_OT[i][k] + OT_TO_DEP * ot[bundles[i][k]];
                if (g_OT[i][k] > K_2) g_OT[i][k] = K_2; // limit OT-induced depolarization
                OTinput = OTinput + 0.5 * g_OT[i][k];
            }

            // **********************************************************
            // AHP
            // **********************************************************  

            // Original equation
            // AHP[i]	= AHP[i] - dt*AHP[i] / model_pars["ahp_tau"];
            AHP[i]	=  AHP_C_2 * AHP[i];

            // **********************************************************
            // Dynamic spike threshold
            // **********************************************************  

            zzz = pow(AHP[i], NAHP);
            vthreac[i] = VTHRE0 + HAP_MAX*exp(-ttime[i]/HAP_TAU) + AHP_MAX * zzz / (zzz + YYY);
            vthreac[i] = vthreac[i] - OTinput;

            // **********************************************************
            // SPIKE CHECK
            // **********************************************************       

            if (v[i] > vthreac[i])
            { 

                AHP[i] = AHP[i] + 1.0;
                // Reset membrane potential after a spike
                v[i] = VREST;

                // Oxytocin not yet released
                released[i] = false;

                // Reset interval since last spike
                ttime[i] = 0.0;

                // Cycle buffer
                isi_buffer[i][1] = isi_buffer[i][0];
                isi_buffer[i][0] = 0.0;

                // Record spike time for neuron firing (in ms)
                (*iter_spikes).time = t;
                (*iter_spikes).index = i;
                ++iter_spikes;

                // Write spike times if buffer is full
                if (iter_spikes != iter_spikes_end)
                {
                    // pass
                }
                else
                {
                    cout << "t = " << t << " - Writing spike times to file.\n";
                    write_spikes(spiketimes_file, spikes.begin(), iter_spikes);
                    iter_spikes = spikes.begin();
                }
            } // end spike check

            // **********************************************************
            // DELAYED DENDRITIC OT RELEASE
            // **********************************************************

            if ( !released[i] && ttime[i]>REL_DELAY )
            {
                released[i] = true;
                if (isi_buffer[i][1] < F_TH) rel =  KPP_3 * res[i][0];
            }
         
            // **********************************************************
            // UPDATE DENDRITIC VARIABLES
            // **********************************************************            

            for (uint k = 0; k < Nd; ++k)
            {                
                // Original equation
                // res[i][k] = res[i][k] - dt * res[i][k] / model_pars["tau_r"] + dt * model_pars["I_su"] - rel;
                res[i][k] = RES_C_2 * res[i][k] + RES_C_3 - rel;

                ot_rel[bundles[i][k]] = ot_rel[bundles[i][k]] + rel;
            }
        
        } // End loop on neurons 

	// **********************************************************
	// UPDATE BUNDLE STATES
	// **********************************************************

        for (uint j = 0; j < Nb; ++j)
        {
            ot[j] = ot_rel[j];

            // Original equation
            // att_input[j] = att_input[j] - dt * att_input[j] / model_pars["tau_cann"] + model_pars["ec_step"] * ot_rel[j];

            att_input[j] = att_input[j] * ATT_INPUT_C_2 + EC_STEP * ot_rel[j];                
            ot_rel[j] = 0.0; // reset total OT released in bundle j
        }

    } // End loop on time

    // Write any spike times left in the buffer 
    write_spikes(spiketimes_file, spikes.begin(), iter_spikes);

    cout << "\nSimulation completed in " << difftime(std::time(NULL),t_sim_start) << "s\n\n";

    if (spiketimes_file.is_open()){
        spiketimes_file.close();
        cout << "Spike times recorded on file " << cfgs.spikeFile << "\n";
    }

    exit(EXIT_SUCCESS);
}

void write_spikes(ofstream &ofs, vector<spike_event>::const_iterator it_begin, vector<spike_event>::const_iterator it_end)
{
    if (ofs.is_open())
    {
        for (vector<spike_event>::const_iterator iter = it_begin; iter != it_end; ++iter)
        {
            ofs << iter->index << "," << iter->time << "\n";
        }
    }
    else cout << "Cannot access output file\n";
}