/**
 * @file ParticlesDefinition.hh
 *
 * @brief Definition of particles and their properties
 *
 * @creation_date     { 24.03.2024 }
 * @modification_date ( 24.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef PARTICLES_DEFINITION_2_C
#define PARTICLES_DEFINITION_2_C


#include <unordered_map>
#include <iomanip>      // std::setprecision
#include <cmath>

// #include "old/color_definitions.hh" // print_green
#include "ColorDefinitions.hh" // print_green

using namespace std;


//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 
#define MUON_MASS 105.658 // MeV
#define PION_MASS 139.571 // MeV
#define PROTON_MASS 938.272 // MeV
#define ELECTRON_MASS 0.511 // MeV

//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 
struct Particle {
	string Name;
	string MassUnit;
	double Mass;
	int PDG;

	Particle(string name, string mu, double m, int pdg) : Name(name), MassUnit(mu), Mass(m), PDG(pdg) {};
};


//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 
double compute_energy_from_momentum(int pdg, double momentum /* MeV/c */);
double compute_momentum_from_energy(int pdg, double energy /* MeV */);
double compute_energy_from_momentum(double mass, double momentum /* MeV/c */);
double compute_momentum_from_energy(double mass, double energy /* MeV */);
double compute_total_energy(int pdg, double kinetic_energy /* MeV */);

//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 
struct UValue
{
	double value;
	string unit;
};

UValue energy_with_best_units(double energy); // default value is in MeV
UValue mom_amp_with_best_units(double mom_amp); // default value is in MeV

//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 

std::string strInitKinEnergyWithBestValues(double energy);
std::string strTrackLengthWithBestValues(double length);

#endif