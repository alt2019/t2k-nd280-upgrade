/**
 * @file ExtractSimulationInformation.hh
 *
 * @brief Obtainig of simulation parameters
 *
 * @creation_date     { --.--.2021 }
 * @modification_date ( 20.03.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef EXTRACT_SIMULATION_INFORMATION_H
#define EXTRACT_SIMULATION_INFORMATION_H

#include <fstream>
#include <regex>   // std::regex, std::smatch
#include <tuple>
#include <string>
#include <iomanip> // std::setw

#include <TTree.h>
#include <TFile.h>
#include <TString.h>

#include "color_definitions.hh"   // cprint
#include "particlesDefinition.hh" // compute_momentum_from_energy, compute_energy_from_momentum
#include "timer.hh"

using namespace std;
using namespace logging;


/***************************************************************************************************************
 * Converts any type to string (most suitable for float/double etc.)
 * 
 * @standard c++11/14/17
 * 
 * @creation_date     { 20.03.2021 }
 * @modification_date { 21.06.2021 }
 * 
 * @return[std::string] { parameter as string }
 ***************************************************************************************************************/
template <typename T> string tostr(const T& t) { 
   ostringstream os; 
   os << t; 
   return os.str(); 
} 


/***************************************************************************************************************
 * Stores information about simulation
 * 
 * @standard c++11/14/17
 * 
 * @creation_date     { --.--.2021 }
 * @modification_date { 20.03.2021 }
 * 
 * @return[std::string] { parameter as string }
 ***************************************************************************************************************/
struct SimulationInformationFromFile{
	std::string particle_name;
	std::string momentum_units;
	float momentum_amplitude;
	float momentum_value_x;
	float momentum_value_y;
	float momentum_value_z;

	float x_start_pos;
	float y_start_pos;
	float z_start_pos;
	std::string pos_units;

	float initial_kin_energy;
	std::string energy_units;

	float magnetic_field_x;
	std::string magnetic_field_units;

	/******************************************************************************
	 * Extracts information about simulation from name of the ROOT file via regex
	 * 
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { --.--.2021 }
	 * @modification_date { 20.03.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ******************************************************************************/
	SimulationInformationFromFile(std::string filename)
	{
		// 'ND280_proton_Px0y0z900uMeV_GPx0.5y0.5z-96.0ucm_Bx0.2y0z0utesla_n1-Exp0-Nexpt1.root'
		std::string template_momentum = "ND280_(\\w+(|\\-|\\+))_Px(\\d|\\d+)y(\\d|\\d+)z(\\d|\\d+)u(\\w+)"
								"_GPx((|\\-)\\d+.\\d+)y((|\\-)\\d+.\\d+)z((|\\-)\\d+.\\d+)u(\\w+)"
								"_Bx(((|\\-)\\d+(|.\\d+))|((|\\-)(\\d+)e(|\\-)\\d+))y((|\\-)\\d+(|.\\d+))z((|\\-)\\d+(|.\\d+))u(\\w+)"
								"_n(\\d+)-Exp0-Nexpt(\\d+).root";
		const std::regex regex_momentum(template_momentum);

		// 'ND280_proton_E100uMeV_GPx0.2375y0.3z-96.0ucm_Bx0.2y0z0utesla_n1000-Exp0-Nexpt1000.root'
		std::string template_energy = "ND280_(\\w+(|\\-|\\+))_E(\\d|\\d+)u(\\w+)"
								"_GPx((|\\-)\\d+.\\d+)y((|\\-)\\d+.\\d+)z((|\\-)\\d+.\\d+)u(\\w+)"
								"_Bx(((|\\-)\\d+(|.\\d+))|((|\\-)(\\d+)e(|\\-)\\d+))y((|\\-)\\d+(|.\\d+))z((|\\-)\\d+(|.\\d+))u(\\w+)"
								"_n(\\d+)-Exp0-Nexpt(\\d+).root";
		const std::regex regex_energy(template_energy);

		std::smatch match;
		if(regex_search(filename, match, regex_momentum)) {		
			for (unsigned i = 0; i < match.size(); ++i)
			{
				cout << " m" << i << " " << match.str(i) << endl;
			}

			particle_name = match.str(1);
			x_start_pos = stof(match.str(7));
			y_start_pos = stof(match.str(9));
			z_start_pos = stof(match.str(11));
			pos_units = match.str(13);
			magnetic_field_x = stof(match.str(14));
			momentum_value_x = stof(match.str(3));
			momentum_value_y = stof(match.str(4));
			momentum_value_z = stof(match.str(5));
			momentum_amplitude = sqrt(momentum_value_x * momentum_value_x + momentum_value_y * momentum_value_y + momentum_value_z * momentum_value_z);
			momentum_units = match.str(6);
			initial_kin_energy = compute_energy_from_momentum(TABLE_PARTICLE_NAME_PDG[particle_name], momentum_amplitude);
			energy_units = "MeV";
			magnetic_field_units = match.str(23);
		} else if (regex_search(filename, match, regex_energy)) {
			for (unsigned i = 0; i < match.size(); ++i)
			{
				cout << setw(5) << (" m" + to_string(i)) << "  " << match.str(i) << endl;
			}

			particle_name = match.str(1);
			x_start_pos = stof(match.str(5));
			y_start_pos = stof(match.str(7));
			z_start_pos = stof(match.str(9));
			magnetic_field_x = stof(match.str(12));
			pos_units = match.str(11);
			initial_kin_energy = stof(match.str(3));
			energy_units = match.str(4);
			momentum_amplitude = compute_momentum_from_energy(TABLE_PARTICLE_NAME_PDG[particle_name], initial_kin_energy);
			momentum_value_x = 0.0;
			momentum_value_y = 0.0;
			momentum_value_z = momentum_amplitude;
			momentum_units = "MeV/c";
		}
	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { --.--.2021 }
	 * @modification_date { --.--.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	void print(){
		cout << cprint(" Primary particle: ", RED) << setw(8) << particle_name << '\n'
			 << cprint(" Insertion position: ", BLUE)
			 	<< setw(7) << x_start_pos
			 	<< setw(7) << y_start_pos
			 	<< setw(7) << z_start_pos 
			 	<< setw(4) << " " + cprint(pos_units, GREEN) << '\n'
			 << setw(5) << cprint(" P :", BLUE) 
			 	<< setw(9) << momentum_value_x
			 	<< setw(9) << momentum_value_y
			 	<< setw(9) << momentum_value_z 
			 	<< setw(4) << " " + cprint(momentum_units, GREEN) << '\n'
			 << setw(5) << cprint("|P|:", BLUE) 
			 	<< setw(9) << momentum_amplitude 
			 	<< setw(4) << " " + cprint(momentum_units, GREEN) << "\n"
			 << setw(5) << cprint("T0 :", BLUE) 
			 	<< setw(9) << initial_kin_energy 
			 	<< setw(4) << " " + cprint(energy_units, GREEN) 
			<< endl;

	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { --.--.2021 }
	 * @modification_date { --.--.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 **********************************************/
	inline std::string getPositionAsString()
	{
		// return "x" + std::to_string(x_start_pos) + "y" + std::to_string(y_start_pos) + "z" + std::to_string(z_start_pos);
		// return "(" + std::to_string(x_start_pos) + ", " + std::to_string(y_start_pos) + ", " + std::to_string(z_start_pos) + ")";
		return "(" + tostr(x_start_pos) + ", " + tostr(y_start_pos) + ", " + tostr(z_start_pos) + ")";
	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 21.06.2021 }
	 * @modification_date { 21.06.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 **********************************************/
	inline std::string getPositionAsStringName()
	{
		return "x" + tostr(x_start_pos) + "y" + tostr(y_start_pos) + "z" + tostr(z_start_pos);
	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 20.03.2021 }
	 * @modification_date { 20.03.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	inline std::string getMagneticFieldAsString()
	{
		// return "Bx" + std::to_string(magnetic_field_x);
		return "Bx" + tostr(magnetic_field_x);
	}

	/************************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 21.06.2021 }
	 * @modification_date { 21.06.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	inline std::string getMomentumAmplitudeAsStringValue()
	{
		return tostr(momentum_amplitude).substr(0, 5);
	}

	/************************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 20.03.2021 }
	 * @modification_date { 21.06.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	inline std::string getMomentumAmplitudeAsString()
	{
		// return "modP" + std::to_string(momentum_amplitude).substr(0, 5);
		// return "modP" + tostr(momentum_amplitude).substr(0, 5);
		return "modP" + getMomentumAmplitudeAsStringValue();
	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 21.06.2021 }
	 * @modification_date { 21.06.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	inline std::string getInitialKineticEnergyAsStringValue()
	{
		return tostr(initial_kin_energy);
	}

	/***********************************************
	 * @standard c++11/14/17
	 * 
	 * @creation_date     { 20.03.2021 }
	 * @modification_date { 21.06.2021 }
	 * 
	 * @return[std::string] { parameter as string }
	 ***********************************************/
	inline std::string getInitialKineticEnergyAsString()
	{
		// return "K" + std::to_string(initial_kin_energy);
		// return "K" + tostr(initial_kin_energy);
		return "K" + getInitialKineticEnergyAsStringValue();
	}

};


/***************************************************************************************************************
 * @standard c++17
 * 
 * @creation_date     { 20.03.2021 }
 * @modification_date { 20.03.2021 }
 * 
 * @param[TString] prefix                         { path to root file }
 * @param[TString] specification                  { name of root file }
 * @return[tuple<TFile*, TTree*, TND280UpEvent*>] { tuple of file pointer, root tree pointer and events pointer }
 * 
 * @TODO: check if file pointer is needed to return
 ***************************************************************************************************************/
std::tuple<TFile*, TTree*, TND280UpEvent*> getEventsTreeFromRootFile(TString prefix, TString specification)
{
	TString input_root_file = prefix + specification;
	// WriteToLog(LT_INFO, "input root file path: ", (string)(input_root_file));
	WriteToLog(LT_DEBUG, "input root file path: ", (string)(input_root_file));

	TFile* file = new TFile(input_root_file.Data(), "READ"); // read file

	TTree* tree = (TTree*)file->Get("ND280upEvents"); // get events tree

	TND280UpEvent* nd280UpEvent = new TND280UpEvent(); // create TND280UpEvent pointer

	// set brunch for saving memory
	TBranch *branch = tree->GetBranch("Event");
	branch->SetAddress(&nd280UpEvent);
	branch->SetAutoDelete(kTRUE);

	// if      (__cplusplus == 201703L) return {file, tree, nd280UpEvent};
	// else if (__cplusplus == 201402L) return std::make_tuple(file, tree, nd280UpEvent);
	// else                             return std::make_tuple(file, tree, nd280UpEvent);
	return {file, tree, nd280UpEvent};
}

std::tuple<TFile*, TTree*, TND280UpEvent*> getEventsTreeFromRootFile(TString input_root_file)
{
	// TString input_root_file = prefix + specification;
	// WriteToLog(LT_INFO, "input root file path: ", (string)(input_root_file));
	WriteToLog(LT_DEBUG, "input root file path: ", (string)(input_root_file));

	TFile* file = new TFile(input_root_file.Data(), "READ"); // read file

	TTree* tree = (TTree*)file->Get("ND280upEvents"); // get events tree

	TND280UpEvent* nd280UpEvent = new TND280UpEvent(); // create TND280UpEvent pointer

	// set brunch for saving memory
	TBranch *branch = tree->GetBranch("Event");
	branch->SetAddress(&nd280UpEvent);
	branch->SetAutoDelete(kTRUE);

	// if      (__cplusplus == 201703L) return {file, tree, nd280UpEvent};
	// else if (__cplusplus == 201402L) return std::make_tuple(file, tree, nd280UpEvent);
	// else                             return std::make_tuple(file, tree, nd280UpEvent);
	return {file, tree, nd280UpEvent};
}


/*
TTree* getEventsTreeFromRootFile(TString prefix, TString specification)
{
	TString input_root_file = prefix + specification;
	cerr << __DATE__ << " " << __TIME__ << " [INFO] " << "input root file path: " << input_root_file << endl;

	unique_ptr<TFile> file(new TFile(input_root_file.Data(), "READ")); // read file
	TTree* tree = (TTree*)file->Get("ND280upEvents"); // get events tree

	return tree;
}
// */


/***************************************************************************************************************
 * @standard c++11?/14/17
 * 
 * @creation_date     { --.--.2021 }
 * @modification_date { --.--.2021 }
 * 
 * @return[std::string] { parameter as string }
 ***************************************************************************************************************/
std::string print_substr_det_name(std::string str)
{
	std::string sample = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/";
	auto sample_len = sample.length();
	if (str.find(sample) != std::string::npos) return str.substr(sample_len);
	else                                       return str;
}


#endif