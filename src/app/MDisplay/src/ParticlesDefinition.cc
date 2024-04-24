
#include "ParticlesDefinition.hh"


// /*
Particle muon        = Particle("mu-"        , "MeV", 105.658, 13  );
Particle antimuon    = Particle("mu+"        , "MeV", 105.658, -13 );
Particle pion        = Particle("pi-"        , "MeV", 139.571, -211);
Particle apion       = Particle("pi+"        , "MeV", 139.571, 211 );
Particle proton      = Particle("proton"     , "MeV", 938.272, 2212);
Particle neutron     = Particle("neutron"    , "MeV", 939.5  , 2112);
Particle electron    = Particle("e-"         , "MeV", 0.511  , 11  );
Particle positron    = Particle("e+"         , "MeV", 0.511  , 11  );
Particle nu_e        = Particle("nu_e"       , "MeV", 0.0    , 12  );
Particle anti_nu_e   = Particle("anti_nu_e"  , "MeV", 0.0   , -12  );
Particle nu_mu       = Particle("nu_mu"      , "MeV", 0.0    , 14  );
Particle anti_nu_mu  = Particle("anti_nu_mu" , "MeV", 0.0   , -14  );
Particle nu_tau      = Particle("nu_tau"     , "MeV", 0.0    , 16  );
Particle anti_nu_tau = Particle("anti_nu_tau", "MeV", 0.0   , -16  );
Particle photon      = Particle("photon"     , "MeV", 0.0   , 22   );



std::unordered_map<int, Particle> PARTICLE_MASS = {
	{13  , muon       },
	{-13 , antimuon   },
	{-211, pion       },
	{ 211, apion      },
	{2212, proton     },
	{2112, neutron    },
	{11  , electron   },
	{-11 , positron   },
	{12  , nu_e       },
	{-12 , anti_nu_e  },
	{14  , nu_mu      },
	{-14 , anti_nu_mu },
	{16  , nu_tau     },
	{-16 , anti_nu_tau},
	{22  , photon     },
};


std::unordered_map<int, double> TABLE_PARTICLE_PDG_MASS = {
	{13  , 105.658}, // muon
	{-13 , 105.658}, // antimuon
	{-211, 139.571}, // pion
	{ 211, 139.571}, // pion
	{2212, 938.272}, // proton
	{2112, 939.5  }, // neutron
	{11  , 0.511  }, // electron
	{-11 , 0.511  }, // positron
	{12  , 0.0    }, // nu_e
	{-12 , 0.0    }, // anti_nu_e
	{14  , 0.0    }, // nu_mu
	{-14 , 0.0    }, // anti_nu_mu
	{16  , 0.0    }, // nu_tau
	{-16 , 0.0    }, // anti_nu_tau
	{22  , 0.0    }, // photon
};

std::unordered_map<std::string, double> TABLE_PARTICLE_NAME_MASS = {
	{"mu-"        , 105.658}, // muon
	{"mu+"        , 105.658}, // antimuon
	{"pi-"        , 139.571}, // pion
	{"pi+"        , 139.571}, // pion
	{"proton"     , 938.272}, // proton
	{"neutron"    , 939.5  }, // neutron
	{"e-"         , 0.511  }, // electron
	{"e-"         , 0.511  }, // positron
	{"nu_e"       , 0.0    }, // nu_e
	{"anti_nu_e"  , 0.0    }, // anti_nu_e
	{"nu_mu"      , 0.0    }, // nu_mu
	{"anti_nu_mu" , 0.0    }, // anti_nu_mu
	{"nu_tau"     , 0.0    }, // nu_tau
	{"anti_nu_tau", 0.0    }, // anti_nu_tau
	{"photon"     , 0.0    }, // photon
};

std::unordered_map<std::string, int> TABLE_PARTICLE_NAME_PDG = {
	{"mu-"        , 13  }, // muon
	{"mu+"        , -13 }, // antimuon
	{"pi-"        , -211}, // pion
	{"pi+"        ,  211}, // pion
	{"proton"     , 2212}, // proton
	{"neutron"    , 2112}, // neutron
	{"e-"         , 11  }, // electron
	{"e+"         , -11 }, // positron
	{"nu_e"       , 12  }, // nu_e
	{"anti_nu_e"  , -12 }, // anti_nu_e
	{"nu_mu"      , 14  }, // nu_mu
	{"anti_nu_mu" , -14 }, // anti_nu_mu
	{"nu_tau"     , 16  }, // nu_tau
	{"anti_nu_tau", -16 }, // anti_nu_tau
	{"photon"     , 22  }, // photon
};


std::unordered_map<int, std::string> TABLE_PARTICLE_PDG_NAME = {
	{13  , "mu-"        }, // muon
	{-13 , "mu+"        }, // muon
	{-211, "pi-"        }, // pion
	{ 211, "pi+"        }, // pion
	{2212, "proton"     }, // proton
	{2112, "neutron"    }, // proton
	{11  , "e-"         }, // electron
	{-11 , "e+"         }, // positron
	{12  , "nu_e"       }, // nu_e
	{-12 , "anti_nu_e"  }, // anti_nu_e
	{14  , "nu_mu"      }, // nu_mu
	{-14 , "anti_nu_mu" }, // anti_nu_mu
	{16  , "nu_tau"     }, // nu_tau
	{-16 , "anti_nu_tau"}, // anti_nu_tau
	{22  , "photon"     }, // photon
};


std::unordered_map<int, std::string> TABLE_PARTICLE_PDG_DEFAULT_COLOR = {
	{11  , "#FF0000"}, // e-
	{-11 , "#0000FF"}, // e+
	
	{12  , "#FFFFFF"}, // nu_e
	{-12 , "#FFFFFF"}, // a_nu_e
	{13  , "#FFFFFF"}, // mu-
	{-13 , "#FFFFFF"}, // mu+
	{14  , "#FFFFFF"}, // nu_mu
	{-14 , "#FFFFFF"}, // a_nu_mu
	{16  , "#FFFFFF"}, // nu_tau
	{-16 , "#FFFFFF"}, // a_nu_tau
	
	{2212, "#FFFF00"}, // proton
	{2112, "#22FF00"}, // neutron
	{-211, "#22FF00"}, // pi-
	{211 , "#22FF00"}, // pi+

	{22  , "#00FF))"}, // pi+
};


// std::unordered_map<int, std::string> TABLE_PARTICLE_PDG_SVG_LINE_STYLE = {
// 	{11  , "#FF0000"}, // e-
// 	{-11 , "#0000FF"}, // e+
	
// 	{12  , "#FFFFFF"}, // nu_e
// 	{-12 , "#FFFFFF"}, // a_nu_e
// 	{13  , "#FFFFFF"}, // mu-
// 	{-13 , "#FFFFFF"}, // mu+
// 	{14  , "#FFFFFF"}, // nu_mu
// 	{-14 , "#FFFFFF"}, // a_nu_mu
// 	{16  , "#FFFFFF"}, // nu_tau
// 	{-16 , "#FFFFFF"}, // a_nu_tau
	
// 	{2212, "#FFFF00"}, // proton
// 	{2112, "#22FF00"}, // neutron
// 	{-211, "#22FF00"}, // pi-
// 	{211 , "#22FF00"}, // pi+
// };


// //------------------------------------------------------------------------------------ 
// //------------------------------------------------------------------------------------ 
double compute_energy_from_momentum(int pdg, double momentum /* MeV/c */){
	if (TABLE_PARTICLE_PDG_MASS.find(pdg) == TABLE_PARTICLE_PDG_MASS.end()) return -1.0;
	double mass = TABLE_PARTICLE_PDG_MASS[pdg];
	return sqrt(momentum * momentum + mass * mass) - mass;
}

double compute_momentum_from_energy(int pdg, double energy /* MeV */){
	if (TABLE_PARTICLE_PDG_MASS.find(pdg) == TABLE_PARTICLE_PDG_MASS.end()) return -1.0;
	double mass = TABLE_PARTICLE_PDG_MASS[pdg];
	return sqrt((mass + energy) * (mass + energy) - mass * mass);
}

double compute_energy_from_momentum(double mass, double momentum /* MeV/c */){
	return sqrt(momentum * momentum + mass * mass) - mass;
}

double compute_momentum_from_energy(double mass, double energy /* MeV */){
	return sqrt((mass + energy) * (mass + energy) - mass * mass);
}

double compute_total_energy(int pdg, double kinetic_energy /* MeV */){
	if (TABLE_PARTICLE_PDG_MASS.find(pdg) == TABLE_PARTICLE_PDG_MASS.end()) return -1.0;
	double mass = TABLE_PARTICLE_PDG_MASS[pdg];
	return kinetic_energy + mass;
}


//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 
UValue energy_with_best_units(double energy) // default value is in MeV
{
	UValue res;
	if (energy >= 1.0e3)                  {res.value = energy        ; res.unit = print_green(" GeV"); return res;}
	if (energy >= 1.0 && energy < 1.0e3)  {res.value = energy        ; res.unit = print_green(" MeV"); return res;}
	if (energy >= 1.0e-3 && energy < 1.0) {res.value = energy * 1.0e3; res.unit = print_green(" KeV"); return res;}
	if (energy >= 1e-9 && energy < 1e-6)  {res.value = energy * 1e6  ; res.unit = print_green("  eV"); return res;}
	if (energy >= 1e-6 && energy < 1e-3)  {res.value = energy * 1e6  ; res.unit = print_green("  eV"); return res;}
	if (energy < 1e-9)                    {res.value = 0.0           ; res.unit = print_green("  eV"); return res;}
	res.value = energy; res.unit = print_green(" MeV"); return res;
}


UValue mom_amp_with_best_units(double mom_amp) // default value is in MeV
{
	UValue res;

	if      (mom_amp >= 1.0)       {res.value = mom_amp        ; res.unit = print_green(" MeV/c");}
	else if (mom_amp * 1.0e3 >= 1) {res.value = mom_amp * 1.0e3; res.unit = print_green(" KeV/c");}
	else if (mom_amp * 1.0e6 >= 1) {res.value = mom_amp * 1.0e6; res.unit = print_green("  eV/c");}
	else if (mom_amp >= 1.0e3)     {res.value = mom_amp        ; res.unit = print_green(" GeV/c");}
	else                           {res.value = mom_amp        ; res.unit = print_green(" MeV/c");}

	return res;
}

//------------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------------ 

std::string strInitKinEnergyWithBestValues(double energy)
{
	std::stringstream res;
	if      (energy >= 1.0e3)                     (res << std::setprecision(3) << energy / 1.0e3 << " GeV");
	else if (energy < 1.0e3  && energy >= 1.0)    (res << std::setprecision(3) << energy         << " MeV");
	else if (energy < 1.0    && energy >= 1.0e-3) (res << std::setprecision(3) << energy * 1.0e3 << " KeV");
	else if (energy < 1.0e-3 && energy >= 1.0e-6) (res << std::setprecision(3) << energy * 1.0e6 << " eV" );
	else if (energy < 1.0e-6 && energy >= 1.0e-9) (res << std::setprecision(3) << energy * 1.0e9 << " meV");
	else                                          (res << "0 eV");
	return res.str();
}


std::string strTrackLengthWithBestValues(double length)
{
	std::stringstream res;
	if      (length >= 1.0e3)                     (res << std::setprecision(3) << length / 1.0e3 << " m"  );
	else if (length < 1.0e3  && length >= 10.0)   (res << std::setprecision(3) << length / 10.0  << " cm" );
	else if (length < 10.0   && length >= 1.0)    (res << std::setprecision(3) << length         << " mm" );
	else if (length < 1.0    && length >= 1.0e-3) (res << std::setprecision(3) << length * 1.0e3 << " mkm");
	else if (length < 1.0e-3 && length >= 1.0e-6) (res << std::setprecision(3) << length * 1.0e6 << " nm" );
	else if (length < 1.0e-6 && length >= 1.0e-9) (res << std::setprecision(3) << length * 1.0e9 << " pm" );
	else                                          (res << "0 mm");
	return res.str();
}


// */