

#include <iostream> // std::cout, std::endl
#include <iomanip> // std::setw
#include <fstream>
#include <string> // std::string
#include <sstream> // std::stringstream
#include <cstdlib> // atoi

#include <vector>
#include <array>
#include <map>

#include <TString.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TMath.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPad.h>
#include <TStyle.h>
#include <TPaveText.h>

#include <G4String.hh>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"

#include "src/SVG.hh"
#include "src/utils.hh"

// #include "src/old/ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
// #include "src/old/AnalizeEvents.hh"
// #include "src/old/SimulationAnalizer.hh"
// #include "src/old/particlesDefinition.hh"
#include "src/ParticlesDefinition.hh"

#include "src/ParticleGunAnalyzer.hh"
#include "src/GenieAnalyzer.hh"

// #include "src/PointInfo.hh"
// #include "src/TrackInfo.hh"
#include "src/EventInfo.hh"
// #include "src2/Event.hh"


std::map<std::tuple<int,std::string>, TH1F*> generate_hists_map(double max_ene, int n_bins)
{
  std::map<std::tuple<int,std::string>, TH1F*> hists_map;



  hists_map[{22, "B"}] = new TH1F(
    "#gamma left sfgd in B kinetic energy distribution",
    "#gamma left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "T"}] = new TH1F(
    "#gamma left sfgd in T kinetic energy distribution",
    "#gamma left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "FWD"}] = new TH1F(
    "#gamma left sfgd in FWD kinetic energy distribution",
    "#gamma left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "BWD"}] = new TH1F(
    "#gamma left sfgd in BWD kinetic energy distribution",
    "#gamma left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "L"}] = new TH1F(
    "#gamma left sfgd in L kinetic energy distribution",
    "#gamma left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "R"}] = new TH1F(
    "#gamma left sfgd in R kinetic energy distribution",
    "#gamma left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "-"}] = new TH1F(
    "#gamma left sfgd in Undefined plane kinetic energy distribution",
    "#gamma left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



  hists_map[{11, "B"}] = new TH1F(
    "e- left sfgd in B kinetic energy distribution",
    "e- left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "T"}] = new TH1F(
    "e- left sfgd in T kinetic energy distribution",
    "e- left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "FWD"}] = new TH1F(
    "e- left sfgd in FWD kinetic energy distribution",
    "e- left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "BWD"}] = new TH1F(
    "e- left sfgd in BWD kinetic energy distribution",
    "e- left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "L"}] = new TH1F(
    "e- left sfgd in L kinetic energy distribution",
    "e- left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "R"}] = new TH1F(
    "e- left sfgd in R kinetic energy distribution",
    "e- left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "-"}] = new TH1F(
    "e- left sfgd in Undefined plane kinetic energy distribution",
    "e- left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



hists_map[{-11, "B"}] = new TH1F(
    "e+ left sfgd in B kinetic energy distribution",
    "e+ left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "T"}] = new TH1F(
    "e+ left sfgd in T kinetic energy distribution",
    "e+ left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "FWD"}] = new TH1F(
    "e+ left sfgd in FWD kinetic energy distribution",
    "e+ left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "BWD"}] = new TH1F(
    "e+ left sfgd in BWD kinetic energy distribution",
    "e+ left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "L"}] = new TH1F(
    "e+ left sfgd in L kinetic energy distribution",
    "e+ left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "R"}] = new TH1F(
    "e+ left sfgd in R kinetic energy distribution",
    "e+ left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "-"}] = new TH1F(
    "e+ left sfgd in Undefined plane kinetic energy distribution",
    "e+ left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );


  hists_map[{2212, "B"}] = new TH1F(
    "proton left sfgd in B kinetic energy distribution",
    "proton left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "T"}] = new TH1F(
    "proton left sfgd in T kinetic energy distribution",
    "proton left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "FWD"}] = new TH1F(
    "proton left sfgd in FWD kinetic energy distribution",
    "proton left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "BWD"}] = new TH1F(
    "proton left sfgd in BWD kinetic energy distribution",
    "proton left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "L"}] = new TH1F(
    "proton left sfgd in L kinetic energy distribution",
    "proton left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "R"}] = new TH1F(
    "proton left sfgd in R kinetic energy distribution",
    "proton left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "-"}] = new TH1F(
    "proton left sfgd in Undefined plane kinetic energy distribution",
    "proton left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



  hists_map[{2112, "B"}] = new TH1F(
    "neutron left sfgd in B kinetic energy distribution",
    "neutron left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "T"}] = new TH1F(
    "neutron left sfgd in T kinetic energy distribution",
    "neutron left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "FWD"}] = new TH1F(
    "neutron left sfgd in FWD kinetic energy distribution",
    "neutron left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "BWD"}] = new TH1F(
    "neutron left sfgd in BWD kinetic energy distribution",
    "neutron left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "L"}] = new TH1F(
    "neutron left sfgd in L kinetic energy distribution",
    "neutron left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "R"}] = new TH1F(
    "neutron left sfgd in R kinetic energy distribution",
    "neutron left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "-"}] = new TH1F(
    "neutron left sfgd in Undefined plane kinetic energy distribution",
    "neutron left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );


  hists_map[{-1, "B"}] = new TH1F(
    "tracks left sfgd in B kinetic energy distribution",
    "tracks left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "T"}] = new TH1F(
    "tracks left sfgd in T kinetic energy distribution",
    "tracks left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "FWD"}] = new TH1F(
    "tracks left sfgd in FWD kinetic energy distribution",
    "tracks left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "BWD"}] = new TH1F(
    "tracks left sfgd in BWD kinetic energy distribution",
    "tracks left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "L"}] = new TH1F(
    "tracks left sfgd in L kinetic energy distribution",
    "tracks left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "R"}] = new TH1F(
    "tracks left sfgd in R kinetic energy distribution",
    "tracks left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "-"}] = new TH1F(
    "tracks left sfgd in Undefined plane kinetic energy distribution",
    "tracks left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  hists_map[{-1, "B"}]->SetFillColor(kBlue);
  hists_map[{-1, "T"}]->SetFillColor(kCyan);
  hists_map[{-1, "FWD"}]->SetFillColor(kGreen);
  hists_map[{-1, "BWD"}]->SetFillColor(kRed);
  hists_map[{-1, "L"}]->SetFillColor(kYellow);
  hists_map[{-1, "R"}]->SetFillColor(kOrange);
  hists_map[{-1, "-"}]->SetFillColor(kBlack);

  hists_map[{22, "B"}]->SetFillColor(kBlue);
  hists_map[{22, "T"}]->SetFillColor(kCyan);
  hists_map[{22, "FWD"}]->SetFillColor(kGreen);
  hists_map[{22, "BWD"}]->SetFillColor(kRed);
  hists_map[{22, "L"}]->SetFillColor(kYellow);
  hists_map[{22, "R"}]->SetFillColor(kOrange);
  hists_map[{22, "-"}]->SetFillColor(kBlack);

  hists_map[{11, "B"}]->SetFillColor(kBlue);
  hists_map[{11, "T"}]->SetFillColor(kCyan);
  hists_map[{11, "FWD"}]->SetFillColor(kGreen);
  hists_map[{11, "BWD"}]->SetFillColor(kRed);
  hists_map[{11, "L"}]->SetFillColor(kYellow);
  hists_map[{11, "R"}]->SetFillColor(kOrange);
  hists_map[{11, "-"}]->SetFillColor(kBlack);

  hists_map[{-11, "B"}]->SetFillColor(kBlue);
  hists_map[{-11, "T"}]->SetFillColor(kCyan);
  hists_map[{-11, "FWD"}]->SetFillColor(kGreen);
  hists_map[{-11, "BWD"}]->SetFillColor(kRed);
  hists_map[{-11, "L"}]->SetFillColor(kYellow);
  hists_map[{-11, "R"}]->SetFillColor(kOrange);
  hists_map[{-11, "-"}]->SetFillColor(kBlack);

  hists_map[{2212, "B"}]->SetFillColor(kBlue);
  hists_map[{2212, "T"}]->SetFillColor(kCyan);
  hists_map[{2212, "FWD"}]->SetFillColor(kGreen);
  hists_map[{2212, "BWD"}]->SetFillColor(kRed);
  hists_map[{2212, "L"}]->SetFillColor(kYellow);
  hists_map[{2212, "R"}]->SetFillColor(kOrange);
  hists_map[{2212, "-"}]->SetFillColor(kBlack);

  hists_map[{2112, "B"}]->SetFillColor(kBlue);
  hists_map[{2112, "T"}]->SetFillColor(kCyan);
  hists_map[{2112, "FWD"}]->SetFillColor(kGreen);
  hists_map[{2112, "BWD"}]->SetFillColor(kRed);
  hists_map[{2112, "L"}]->SetFillColor(kYellow);
  hists_map[{2112, "R"}]->SetFillColor(kOrange);
  hists_map[{2112, "-"}]->SetFillColor(kBlack);

  return hists_map;
}


void draw_1d_hist(TH1F* h, std::string out_fname, std::string canvas_title, bool setLogX = false, bool setLogY = false)
{
  TCanvas* c = new TCanvas(canvas_title.c_str(), canvas_title.c_str(), 1080, 720);
  c->cd();
  if (setLogX) gPad->SetLogx();
  if (setLogY) gPad->SetLogy();
  h->Draw();
  h->SetFillColor(kRed);
  c->SaveAs(out_fname.c_str());
  c->Close();
}


void analize_events_from_genie_v2(std::string input_rootfilename, std::string output_dir_tag)
{
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  
  // std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // double hist_max_edep = 3000.0;
  // double hist_max_edep = 1000.0;
  // double hist_max_edep = 500.0;
  // double hist_max_edep = 500.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  // double hist_max_edep = 5000.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  double hist_max_edep = 3000.0;
  int hist_max_bins_edep = 100;

  /// Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  TH1F* h_lepton_angle_distribution = new TH1F(
    "lepton angle distribution",
    "Lepton angle distribution;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_edep = new TH1F(
    "lepton edep",
    "Lepton energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );

  /*
  TH1F* h_lepton_angle_distribution_z = new TH1F(
    "lepton angle distribution z",
    "Lepton angle distribution z;cos #theta;N events", 
    60, -1.5, 1.5
  );
  TH1F* h_lepton_angle_distribution_x = new TH1F(
    "lepton angle distribution x",
    "Lepton angle distribution x;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_all_charged_partciles_edep = new TH1F(
    "all charged particles edep",
    "All charged particles energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );


  TH1F* h_vtx_distr_x = new TH1F(
    "X-vtx-distr",
    "Distribution of vertex position by X;Vertex X position, cm;N events", 
    200, -105.0, 105.0
  );
  TH1F* h_vtx_distr_y = new TH1F(
    "Y-vtx-distr",
    "Distribution of vertex position by Y;Vertex Y position, cm;N events", 
    80, -40.0, 40.0
    // 200, -100.0, 100.0
  );
  TH1F* h_vtx_distr_z = new TH1F(
    "Z-vtx-distr",
    "Distribution of vertex position by Z;Vertex Z position, cm;N events", 
    200, -105.0, 105.0
  );

  TH2F* h_angle_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_energy_primary_lepton_diplot_2",
    "Angle-energy;cos #theta;E, MeV", 
    // 60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
    60, -1.5, 1.5, 100, 0, 1000.0
  );

  TH2F* h_angle_initek_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_Ek0_primary_lepton_diplot_2",
    "Angle-initial kinetic energy;cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0.0, hist_max_edep
  );

  TH2F* h_angle_energy_all_charged_particles_diplot_2 = new TH2F(
    "angle_energy_all_charged_particles_diplot_2",
    "Angle-energy (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  TH2F* h_angle_Ek0_all_charged_particles_diplot_2 = new TH2F(
    "angle_Ek0_all_charged_particles_diplot_2",
    "Angle-Ek0 (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );
  // */

  // double max_ene = 2000.0;
  double max_ene = 3000.0;
  int n_bins = 100;

  TH1F* h_energies_of_gammas_left_sfgd = new TH1F(
    "#gamma left sfgd kinetic energy distribution",
    "#gamma left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_ele_left_sfgd = new TH1F(
    "e- left sfgd kinetic energy distribution",
    "e- left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_prot_left_sfgd = new TH1F(
    "proton left sfgd kinetic energy distribution",
    "proton left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_neut_left_sfgd = new TH1F(
    "neutron left sfgd knetic energy distribution",
    "neutron left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_tracks_left_sfgd = new TH1F(
    "track left sfgd kinetic energy distribution",
    "track left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_ele_left_sfgd_per_event = new TH1F(
    "e- left sfgd per event",
    "e- left sfgd per event;N electrons;N events", 
    50, 0, 50
  );

  TH1F* h_gamma_left_sfgd_per_event = new TH1F(
    "#gamma left sfgd per event",
    "#gamma left sfgd per event;N #gamma;N events", 
    100, 0, 100
  );

  TH1F* h_ele_left_sfgd_carried_ene = new TH1F(
    "e- left sfgd carried energy",
    "e- left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  TH1F* h_gamma_left_sfgd_carried_ene = new TH1F(
    "#gamma left sfgd carried energy",
    "#gamma left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  TH1F* h_all_tracks_left_sfgd_carried_ene = new TH1F(
    "all tracks left sfgd carried energy",
    "all tracks left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  TH2F* h_Egamma_E_primary_lepton_escape = new TH2F(
    "h_Egamma_E_primary_lepton_escape",
    "E_#gamma escape vs E_e escape;E_e escape, MeV;E_#gamma escape, MeV", 
    // 60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
    n_bins, 0, max_ene, n_bins, 0, max_ene
  );

  TH2F* h_escape_Egamma_e_to_primary_lepton_dE = new TH2F(
    "h_escape_Egamma_e_to_primary_lepton_dE",
    "E_#gamma+e escape vs primary lepton dE;primary lepton dE in SFGD, MeV;E_e + E_#gamma escape, MeV", 
    // n_bins, 0, max_ene, n_bins, 0, max_ene
    n_bins, 0, 500.0, n_bins, 0, max_ene
  );

  TH2F* h_escape_all_tracks_to_primary_lepton_Ek0 = new TH2F(
    "h_escape_all_tracks_to_primary_lepton_Ek0",
    "E escape of all tracks to primary lepton initial Ek in SFGD relation;primary lepton initial Ek in SFGD, MeV;E escape of all tracks/Ek0, MeV", 
    // n_bins, 0, max_ene, n_bins, 0, 2.0
    n_bins, 0, max_ene, n_bins, 0, 3.0
  );

  std::map<std::tuple<int,std::string>, TH1F*> hists_map = generate_hists_map(max_ene, n_bins);


  int main_particle_PDG = -1;

  int NTreeEntries = tinput->GetEntries();

  
  for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // for (int eventId = 1; eventId < 2; ++eventId)
  // for (int eventId = 1; eventId < 10; ++eventId)
  // for (int eventId = 1; eventId < 100; ++eventId)
  // for (int eventId = 1; eventId < 500; ++eventId)
  // for (int eventId = 1; eventId < 1000; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;

    if (eventId == 1588) continue; // error in map.at somewhere in tracks processing

    tinput->GetEntry(eventId);
    
    EventInfo evtInfo(nd280UpEvent, eventId);
    evtInfo.print();

    // create_display(int trackID, bool by_track_points, bool by_hits, bool as_svg)
    evtInfo.create_display(-999, true, false, false, output_dir_tag);

    // h_lepton_angle_distribution
    // std::unique_ptr<TrackInfo> primary_lepton = evtInfo.primary_lepton;
    // TrackInfo * primary_lepton = evtInfo.primary_lepton;
    if (evtInfo.primary_lepton)
    {
      h_lepton_angle_distribution->Fill(evtInfo.primary_lepton->InitCosTheta);
      h_lepton_edep->Fill(evtInfo.primary_lepton->dE_over_pts_in_sfgd);
    }

    std::vector<TrackInfo> TracksLeftSFGD = evtInfo.getTracksLeftSFGD();
    int n_out_ele = 0;
    int n_out_gamma = 0;
    double ene_carried_by_left_ele = 0.0;
    double ene_carried_by_left_pos = 0.0;
    double ene_carried_by_left_gamma = 0.0;
    double total_energy_carried_by_all_left_tracks = 0.0;

    for (auto trk: TracksLeftSFGD)
    {
      double Ek_out = trk.left_SFGD_with_Ek;
      if (trk.PDG == 22)
      {
        h_energies_of_gammas_left_sfgd->Fill(Ek_out);
        hists_map.at({22, trk.left_SFGD_plane})->Fill(Ek_out);
        n_out_gamma++;
        ene_carried_by_left_gamma += Ek_out;
      }
      else if (trk.PDG == 11)
      {
        h_energies_of_ele_left_sfgd->Fill(Ek_out);
        hists_map.at({11, trk.left_SFGD_plane})->Fill(Ek_out);
        n_out_ele++;
        ene_carried_by_left_ele += Ek_out;
      }
      else if (trk.PDG == -11)
      {
        // h_energies_of_ele_left_sfgd->Fill(Ek_out);
        hists_map.at({-11, trk.left_SFGD_plane})->Fill(Ek_out);
        // n_out_ele++;
        ene_carried_by_left_pos += Ek_out;
      }
      else if (trk.PDG == 2212)
      {
        h_energies_of_prot_left_sfgd->Fill(Ek_out);
        hists_map.at({2212, trk.left_SFGD_plane})->Fill(Ek_out);
      }
      else if (trk.PDG == 2112)
      {
        h_energies_of_neut_left_sfgd->Fill(Ek_out);
        hists_map.at({2112, trk.left_SFGD_plane})->Fill(Ek_out);
      }
      h_energies_of_tracks_left_sfgd->Fill(Ek_out);
      hists_map.at({-1, trk.left_SFGD_plane})->Fill(Ek_out);
      total_energy_carried_by_all_left_tracks += Ek_out;
    }

    h_ele_left_sfgd_per_event->Fill(n_out_ele);
    h_gamma_left_sfgd_per_event->Fill(n_out_gamma);
    h_ele_left_sfgd_carried_ene->Fill(ene_carried_by_left_ele);
    h_gamma_left_sfgd_carried_ene->Fill(ene_carried_by_left_gamma);
    h_all_tracks_left_sfgd_carried_ene->Fill(total_energy_carried_by_all_left_tracks);

    double energy_carried_by_left_e = ene_carried_by_left_ele + ene_carried_by_left_pos;
    h_Egamma_E_primary_lepton_escape->Fill(energy_carried_by_left_e, ene_carried_by_left_gamma);

    double Ee_Egamma = energy_carried_by_left_e + ene_carried_by_left_gamma;
    if (evtInfo.primary_lepton)
    {
      h_escape_Egamma_e_to_primary_lepton_dE->Fill(
        evtInfo.primary_lepton->dE_over_pts_in_sfgd, Ee_Egamma);
      h_escape_all_tracks_to_primary_lepton_Ek0->Fill(
        evtInfo.primary_lepton->InitKinEnergy,
        total_energy_carried_by_all_left_tracks / evtInfo.primary_lepton->InitKinEnergy);
    }
    

  }

  std::string out_fname_primary_lepton_angle_distr = output_dir_tag + "/primary_lepton_angle_distr.pdf";
  draw_1d_hist(h_lepton_angle_distribution, out_fname_primary_lepton_angle_distr, "primary lepton angle distribution");

  std::string out_fname_primary_lepton_dedx_distr = output_dir_tag + "/primary_lepton_dedx_distr.pdf";
  draw_1d_hist(h_lepton_edep, out_fname_primary_lepton_dedx_distr, "primary lepton angle distribution");

  std::string out_fname_gammas_left_sfgd = output_dir_tag + "/gammas_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_gammas_left_sfgd, out_fname_gammas_left_sfgd, "#gamma left SFGD Ek", false, true);

  std::string out_fname_ele_left_sfgd = output_dir_tag + "/e-_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_ele_left_sfgd, out_fname_ele_left_sfgd, "e- left SFGD Ek", false, true);

  std::string out_fname_prot_left_sfgd = output_dir_tag + "/prot_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_prot_left_sfgd, out_fname_prot_left_sfgd, "prot left SFGD Ek", false, true);

  std::string out_fname_trks_left_sfgd = output_dir_tag + "/trks_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_tracks_left_sfgd, out_fname_trks_left_sfgd, "tracks left SFGD Ek", false, true);

  std::string out_fname_n_ele_left_sfgd = output_dir_tag + "/e-_left_sfgd-per_event.pdf";
  draw_1d_hist(h_ele_left_sfgd_per_event, out_fname_n_ele_left_sfgd, "e- left SFGD per event");

  std::string out_fname_n_gamma_left_sfgd = output_dir_tag + "/gamma_left_sfgd-per_event.pdf";
  draw_1d_hist(h_gamma_left_sfgd_per_event, out_fname_n_gamma_left_sfgd, "#gamma left SFGD per event");

  std::string out_fname_ele_left_sfgd_carrene = output_dir_tag + "/e-_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_ele_left_sfgd_carried_ene, out_fname_ele_left_sfgd_carrene, "e- left SFGD carried energy");

  std::string out_fname_gamma_left_sfgd_carrene = output_dir_tag + "/gamma_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_gamma_left_sfgd_carried_ene, out_fname_gamma_left_sfgd_carrene, "#gamma left SFGD carried energy");

  std::string out_fname_all_tracks_left_sfgd_carrene = output_dir_tag + "/all_tracks_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_all_tracks_left_sfgd_carried_ene, out_fname_all_tracks_left_sfgd_carrene, "all tracks left SFGD carried energy");


  
  THStack hs_gamma("hs_gamma","#gamma left SFGD Ek;Ekin, MeV;N events");
  THStack hs_ele("hs_ele","e- left SFGD Ek;Ekin, MeV;N events");
  THStack hs_prot("hs_prot","proton left SFGD Ek;Ekin, MeV;N events");
  THStack hs_neut("hs_neut","neutron left SFGD Ek;Ekin, MeV;N events");
  THStack hs_trks("hs_trks","tracks left SFGD Ek;Ekin, MeV;N events");

  TLegend *hs_gamma_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_ele_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_prot_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_neut_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_trks_leg = new TLegend(0.6,0.6,0.9,0.9);

  for (auto &item: hists_map)
  {
    std::tuple<int, std::string> pdg_plane = item.first;
    int pdg = std::get<0>(pdg_plane);
    std::string plane = std::get<1>(pdg_plane);
    TH1F* h = item.second;
    int N_entries = h->GetEntries();
    std::stringstream leg_item;
    leg_item << "left at " << plane << "; " << N_entries << " events";
    h->SetMinimum(0.0);
    // h->SetMaximum(100);

    if (pdg == 22)
    {
      hs_gamma.Add(h);
      hs_gamma_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 11)
    {
      hs_ele.Add(h);
      hs_ele_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 2212)
    {
      hs_prot.Add(h);
      hs_prot_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 2112)
    {
      hs_neut.Add(h);
      hs_neut_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == -1)
    {
      hs_trks.Add(h);
      hs_trks_leg->AddEntry(h, leg_item.str().c_str());
    }
  }

  std::string out_fname_hs_gamma = output_dir_tag + "/gamma_left_sfgd-plane_hs.pdf";
  TCanvas* c_gamma_left_sfgd_hs = new TCanvas("gamma_left_sfgd-plane", "gamma_left_sfgd-plane", 1080, 720);
  c_gamma_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_gamma.Draw();
  hs_gamma_leg->Draw();
  c_gamma_left_sfgd_hs->SaveAs(out_fname_hs_gamma.c_str());
  c_gamma_left_sfgd_hs->Close();

  std::string out_fname_hs_ele = output_dir_tag + "/e-_left_sfgd-plane_hs.pdf";
  TCanvas* c_ele_left_sfgd_hs = new TCanvas("e-_left_sfgd-plane", "e-_left_sfgd-plane", 1080, 720);
  c_ele_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_ele.Draw();
  hs_ele_leg->Draw();
  c_ele_left_sfgd_hs->SaveAs(out_fname_hs_ele.c_str());
  c_ele_left_sfgd_hs->Close();

  std::string out_fname_hs_prot = output_dir_tag + "/proton_left_sfgd-plane_hs.pdf";
  TCanvas* c_prot_left_sfgd_hs = new TCanvas("proton_left_sfgd-plane", "proton_left_sfgd-plane", 1080, 720);
  c_prot_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_prot.Draw();
  hs_prot_leg->Draw();
  c_prot_left_sfgd_hs->SaveAs(out_fname_hs_prot.c_str());
  c_prot_left_sfgd_hs->Close();

  std::string out_fname_hs_neut = output_dir_tag + "/neutron_left_sfgd-plane_hs.pdf";
  TCanvas* c_neut_left_sfgd_hs = new TCanvas("neutron_left_sfgd-plane", "neutron_left_sfgd-plane", 1080, 720);
  c_neut_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_neut.Draw();
  hs_neut_leg->Draw();
  c_neut_left_sfgd_hs->SaveAs(out_fname_hs_neut.c_str());
  c_neut_left_sfgd_hs->Close();

  std::string out_fname_hs_tracks = output_dir_tag + "/tracks_left_sfgd-plane_hs.pdf";
  TCanvas* c_trks_left_sfgd_hs = new TCanvas("tracks_left_sfgd-plane", "tracks_left_sfgd-plane", 1080, 720);
  c_trks_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_trks.Draw();
  hs_trks_leg->Draw();
  // c_trks_left_sfgd_hs->BuildLegend();
  c_trks_left_sfgd_hs->SaveAs(out_fname_hs_tracks.c_str());
  c_trks_left_sfgd_hs->Close();


  std::string out_fname_e_gamma_left_sfgd_2d = output_dir_tag + "/e_gamma_left_sfgd-diplot_ene.pdf";
  TCanvas* c_e_gamma_left_sfgd_2d = new TCanvas("e_gamma_left_sfgd-diplot_ene", "e_gamma_left_sfgd-diplot_ene", 1080, 720);
  c_e_gamma_left_sfgd_2d->cd();
  // gPad->SetLogy();
  h_Egamma_E_primary_lepton_escape->Draw("COLZ");
  c_e_gamma_left_sfgd_2d->SaveAs(out_fname_e_gamma_left_sfgd_2d.c_str());
  c_e_gamma_left_sfgd_2d->Close();

  std::string out_fname_escape_Egamma_e_to_primary_lepton_dE = output_dir_tag + "/escape_Egamma_e_to_primary_lepton_dE-diplot_ene.pdf";
  TCanvas* c_escape_Egamma_e_to_primary_lepton_dE = new TCanvas(
    "escape_Egamma_e_to_primary_lepton_dE-diplot_ene",
    "escape_Egamma_e_to_primary_lepton_dE-diplot_ene", 1080, 720);
  c_escape_Egamma_e_to_primary_lepton_dE->cd();
  // gPad->SetLogy();
  h_escape_Egamma_e_to_primary_lepton_dE->Draw("COLZ");
  c_escape_Egamma_e_to_primary_lepton_dE->SaveAs(out_fname_escape_Egamma_e_to_primary_lepton_dE.c_str());
  c_escape_Egamma_e_to_primary_lepton_dE->Close();

  std::string out_fname_escape_all_tracks_to_primary_lepton_Ek0 = output_dir_tag + "/escape_all_tracks_to_primary_lepton_Ek0-diplot_ene.pdf";
  TCanvas* c_escape_all_tracks_to_primary_lepton_Ek0 = new TCanvas(
    "escape_all_tracks_to_primary_lepton_Ek0-diplot_ene",
    "escape_all_tracks_to_primary_lepton_Ek0-diplot_ene", 1080, 720);
  c_escape_all_tracks_to_primary_lepton_Ek0->cd();
  // gPad->SetLogy();
  h_escape_all_tracks_to_primary_lepton_Ek0->Draw("COLZ");
  c_escape_all_tracks_to_primary_lepton_Ek0->SaveAs(out_fname_escape_all_tracks_to_primary_lepton_Ek0.c_str());
  c_escape_all_tracks_to_primary_lepton_Ek0->Close();
  
}


void analize_events_from_genie(std::string input_rootfilename, std::string output_dir_tag, std::string test)
{
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  
  // std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // double hist_max_edep = 3000.0;
  // double hist_max_edep = 1000.0;
  // double hist_max_edep = 500.0;
  // double hist_max_edep = 500.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  // double hist_max_edep = 5000.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  double hist_max_edep = 3000.0;
  int hist_max_bins_edep = 100;

  /// Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  TH1F* h_lepton_angle_distribution = new TH1F(
    "lepton angle distribution",
    "Lepton angle distribution;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_angle_distribution_z = new TH1F(
    "lepton angle distribution z",
    "Lepton angle distribution z;cos #theta;N events", 
    60, -1.5, 1.5
  );
  TH1F* h_lepton_angle_distribution_x = new TH1F(
    "lepton angle distribution x",
    "Lepton angle distribution x;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_edep = new TH1F(
    "lepton edep",
    "Lepton energy deposit;E deposited, MeV;N events", 
    // hist_max_bins_edep, 0.0, hist_max_edep
    100, 0.0, 1000
  );

  TH1F* h_all_charged_partciles_edep = new TH1F(
    "all charged particles edep",
    "All charged particles energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );


  TH1F* h_vtx_distr_x = new TH1F(
    "X-vtx-distr",
    "Distribution of vertex position by X;Vertex X position, cm;N events", 
    200, -105.0, 105.0
  );
  TH1F* h_vtx_distr_y = new TH1F(
    "Y-vtx-distr",
    "Distribution of vertex position by Y;Vertex Y position, cm;N events", 
    80, -40.0, 40.0
    // 200, -100.0, 100.0
  );
  TH1F* h_vtx_distr_z = new TH1F(
    "Z-vtx-distr",
    "Distribution of vertex position by Z;Vertex Z position, cm;N events", 
    200, -105.0, 105.0
  );

  TH2F* h_angle_energy_primary_lepton_diplot = new TH2F(
    "angle_energy_primary_lepton_diplot",
    "Angle-energy;E, MeV, MeV;cos #theta", 
    hist_max_bins_edep, 0, hist_max_edep, 60, -1.5, 1.5
  );
  TH2F* h_angle_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_energy_primary_lepton_diplot_2",
    "Angle-energy;cos #theta;E, MeV", 
    // 60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
    60, -1.5, 1.5, 100, 0, 1000.0
  );

  TH2F* h_angle_initek_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_Ek0_primary_lepton_diplot_2",
    "Angle-initial kinetic energy;cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0.0, hist_max_edep
  );

  TH2F* h_angle_energy_all_charged_particles_diplot = new TH2F(
    "angle_energy_all_charged_particles_diplot",
    "Angle-energy (all charged particles);E, MeV;cos #theta", 
    hist_max_bins_edep, 0, hist_max_edep, 60, -1.5, 1.5
  );
  TH2F* h_angle_energy_all_charged_particles_diplot_2 = new TH2F(
    "angle_energy_all_charged_particles_diplot_2",
    "Angle-energy (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  TH2F* h_angle_Ek0_all_charged_particles_diplot_2 = new TH2F(
    "angle_Ek0_all_charged_particles_diplot_2",
    "Angle-Ek0 (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  

  int main_particle_PDG = -1;

  int NTreeEntries = tinput->GetEntries();
  // for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // {
  //   std::cout << "\nEvent " << eventId << std::endl;
  //   tinput->GetEntry(eventId);

  //   int NVertices = nd280UpEvent->GetNVertices();

  //   for (int vtxID = 0; vtxID < NVertices; ++vtxID)
  //   {
  //     TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
  //     // vtx->PrintVertex();

  //     TND280UpTrack *trk0 = vtx->GetInTrack(0);
  //     TND280UpTrack *trk1 = vtx->GetInTrack(1);

  //     // std::cout << " Vertex input Track 0: " << trk0->GetPDG() << " " << trk0->GetInitMom().Mag() << std::endl;
  //     // std::cout << " Vertex input Track 1: " << trk1->GetPDG() << " " << trk1->GetInitMom().Mag() << std::endl;

  //     TVector3 vtx_pos = vtx->GetPosition();

  //     // std::cout << " Vertex position: " << vtx_pos.X() << " " << vtx_pos.Y() << " " << vtx_pos.Z() << std::endl;

  //     h_vtx_distr_x->Fill(vtx_pos.X() / 10.0);
  //     h_vtx_distr_y->Fill(vtx_pos.Y() / 10.0);
  //     h_vtx_distr_z->Fill(vtx_pos.Z() / 10.0);
  //     // h_vtx_distr_y->Fill(vtx_pos.Z() / 10.0);
  //     // h_vtx_distr_z->Fill(vtx_pos.Y() / 10.0);
  //   }
  // }


  // for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // for (int eventId = 1; eventId < 2; ++eventId)
  for (int eventId = 1; eventId < 10; ++eventId)
  // for (int eventId = 1; eventId < 100; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;
    tinput->GetEntry(eventId);

    int NVertices = nd280UpEvent->GetNVertices();
    int NHits = nd280UpEvent->GetNHits();
    int NTracks = nd280UpEvent->GetNTracks();

    std::cout << "NVertices " << NVertices << std::endl;
    std::cout << "NHits     " << NHits << std::endl;
    std::cout << "NTracks   " << NTracks << std::endl;

    TND280UpTrack* t_primary_lepton_track = nullptr;
    // std::vector<TND280UpTrack*> t_primary_tracks;

    bool skip_event = false;
    for (int vtxID = 0; vtxID < NVertices; ++vtxID)
    {
      TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
      // vtx->PrintVertex();

      TND280UpTrack *trk0 = vtx->GetInTrack(0);
      TND280UpTrack *trk1 = vtx->GetInTrack(1);

      std::cout << " Vertex input Track 0: " << trk0->GetPDG() << " " << trk0->GetInitMom().Mag() << std::endl;
      std::cout << " Vertex input Track 1: " << trk1->GetPDG() << " " << trk1->GetInitMom().Mag() << std::endl;

      TVector3 vtx_pos = vtx->GetPosition();

      std::cout << " Vertex position: " << vtx_pos.X() << " " << vtx_pos.Y() << " " << vtx_pos.Z() << std::endl;

      if (vtx_pos.X() < -960.0+20.0 || vtx_pos.X() > 960.0-20.0) skip_event = true;
      if (vtx_pos.Z() < -920.0+20.0 || vtx_pos.Z() > 920.0-20.0) skip_event = true;
      if (vtx_pos.Y() < -280.0+16.0+20.0 || vtx_pos.Y() > 280.0+16.0-20.0) skip_event = true;

      h_vtx_distr_x->Fill(vtx_pos.X() / 10.0);
      h_vtx_distr_y->Fill(vtx_pos.Y() / 10.0);
      h_vtx_distr_z->Fill(vtx_pos.Z() / 10.0);
      // h_vtx_distr_y->Fill(vtx_pos.Z() / 10.0);
      // h_vtx_distr_z->Fill(vtx_pos.Y() / 10.0);
    }

    if (skip_event) continue;

    double sum_of_Ek0_for_out_vertex_particles = 0.0;
    double sum_of_Ek0_for_all_particles = 0.0;
    double sum_of_mom_for_out_vertex_particles = 0.0;
    double sum_of_mom_for_all_particles = 0.0;
    double total_deposited_energy_by_charged_particles = 0.0;

    std::map<int, TND280UpTrack*> tracks_map;
    std::map<int, double> tracks_edep_over_pts;

    int n_started_and_stopped_in_sfgd = 0;
    int n_stopped_in_sfgd_from_out = 0;
    int n_started_and_not_stopped_in_sfgd = 0;
    int n_other = 0;

    for (Int_t trjID = 0; trjID < NTracks; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
      int ParentID = track->GetParentID();
      int PDG = track->GetPDG();
      int NPoints = track->GetNPoints();
      int TrackID = track->GetTrackID();
      std::string ProcessName = track->GetProcessName();
      double InitKinEnergy = track->GetInitKinEnergy();
      double InitCosTheta = track->GetInitCosTheta();
      TVector3 InitMom = track->GetInitMom();

      std::cout 
                // << "\n\n\n"
                << "== "
                << " TID:" << TrackID
                << " PID:" << ParentID
                << " PDG:" << PDG
                << " Proc:" << ProcessName
                << " Ek0:" << InitKinEnergy
                << " cosT0:" << InitCosTheta
                << " Npt:" << NPoints
      << std::endl;

      sum_of_Ek0_for_all_particles += InitKinEnergy;
      sum_of_mom_for_all_particles += InitMom.Mag();

      tracks_map.insert({TrackID, track});

      bool is_started_inside_sfgd = false;
      bool is_stopped_inside_sfgd = false;

      // if (PDG != 22 && PDG != 2112 && PDG < 1000000000)
      {
        double trk_edep_over_pts = 0.0;
        double trk_edep_over_pts_full = 0.0;
        for (int ipt = 0; ipt < NPoints; ++ipt)
        {
          TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);

          // std::string material = trackPoint->GetMaterialName();

          // double Edep = trackPoint->GetEdep();
          // total_deposited_energy_by_charged_particles += Edep;

          /////
          int ptId = trackPoint->GetPointID();
          // double ptTime = trackPoint->GetTime();
          TVector3 ptMomentum = trackPoint->GetMomentum();
          double ptEdep = trackPoint->GetEdep();
          double ptStepLength = trackPoint->GetStepLength();
          // double ptStepDeltaLyz = trackPoint->GetStepDeltaLyz();
          // int ptStepStatus = trackPoint->GetStepStatus();
          // std::string ptPhysVolName = trackPoint->GetPhysVolName();
          std::string ptLogVolName = trackPoint->GetLogVolName();
          // TVector3 ptPrePosition = trackPoint->GetPrePosition();
          // TVector3 ptPostPosition = trackPoint->GetPostPosition();
          // bool ptIsOnBoundary = trackPoint->IsOnBoundary();
          // std::string ptPreProcessName = trackPoint->GetPreProcessName();
          // std::string ptPostProcessName = trackPoint->GetPostProcessName();
          // std::string ptMaterialName = trackPoint->GetMaterialName();
          
          bool is_in_sfgd = isPtInsideSFGD(ptLogVolName);

          std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ";
          size_t pos = ptLogVolName.find(to_replace);
          if (pos != std::string::npos) ptLogVolName.replace(pos, to_replace.length(), "~");

          double Ek_at_step = compute_energy_from_momentum(PDG, ptMomentum.Mag());
          /////

          /////
          std::string det = trackPoint->GetLogVolName();
          if (ipt == 0)
          {
            if (isPtInsideSFGD(det)) is_started_inside_sfgd = true;
          }
          else if (ipt == NPoints-1)
          {
            if (isPtInsideSFGD(det)) is_stopped_inside_sfgd = true;
          }

          if (isPtInsideSFGD(det))
          {
            double Edep = trackPoint->GetEdep();
            total_deposited_energy_by_charged_particles += Edep;
            trk_edep_over_pts += Edep;
          }
          /////
          trk_edep_over_pts_full += ptEdep;

          // if (NPoints < 100)
          {
            std::cout << "\t" << ptId << " "
                      << "dE: " << std::setw(10) << ptEdep << " "
                      << "dL: " << std::setw(10) << ptStepLength << " "
                      << "|Mom|: " << std::setw(10) << ptMomentum.Mag() << " "
                      << "|Ek|: " << std::setw(10) << Ek_at_step << " "
                      << "inSFGD: " << is_in_sfgd << " "
                      // << "Mat: "<< ptMaterialName
                      << "V: "<< ptLogVolName
                      << std::endl;

            // if (0) // do not print
            // {
            //   std::cout << std::setprecision(5) << std::fixed << std::showpoint
            //             << std::setw(5) << "  Tp:" << PointID
            //             << std::setw(11) << " |M|:" << Momentum.Mag()
            //             << std::setw(11) << " dE:" << Edep
            //             << std::setw(11) << " dL:" << StepLength
            //             << std::setw(11) << " dLpt:" << dLpt
            //               << std::setw(20) << " " << PreProcessName << " "
            //               << std::setw(20) << PostProcessName << " "
            //               << std::setw(20) << MaterialName
            //             << std::setw(2) << " Bnd:" << IsOnBoundary
            //             //  << " X:" << PrePosition.X()
            //             //   << " Y:" << PrePosition.Y()
            //               << std::setw(8) << " Z:" << PrePosition.Z()
            //             //  << " X:" << PostPosition.X()
            //             //   << " Y:" << PostPosition.Y()
            //               << std::setw(8) << " Z:" << PostPosition.Z()
            //               << std::setw(8) << " avgZ:" << zAvgPos
            //             << " " << PhysVolName
            //             << std::endl;
            // }
          } 
        }
        tracks_edep_over_pts.insert({TrackID, trk_edep_over_pts});

        std::cout << "  dE_over_pt:" << trk_edep_over_pts << " MeV"
                  << "  dE_over_pt_full:"  << trk_edep_over_pts_full << " MeV"
                  << "\n   Started in SFGD: " << is_started_inside_sfgd << " "
                  << " Stopped in SFGD: " << is_stopped_inside_sfgd << " "
                  << std::endl;
      }

      

      if (is_started_inside_sfgd && is_stopped_inside_sfgd)
      {
        n_started_and_stopped_in_sfgd++;
      }
      else if (!is_started_inside_sfgd && is_stopped_inside_sfgd)
      {
        n_stopped_in_sfgd_from_out++;
      }
      else if (is_started_inside_sfgd && !is_stopped_inside_sfgd)
      {
        n_started_and_not_stopped_in_sfgd++;
      }
      else // (!is_started_inside_sfgd && !is_stopped_inside_sfgd)
      {
        n_other++;
      }

      // if (ParentID != 0) continue;

      if (ParentID == 0)
      {
        if (abs(PDG) == 11 || abs(PDG) == 13)
        {
          t_primary_lepton_track = track;
          main_particle_PDG = PDG;
        }
        sum_of_Ek0_for_out_vertex_particles += InitKinEnergy;
        sum_of_mom_for_out_vertex_particles += InitMom.Mag();
      }

      if (ParentID == 0)
      {
        std::cout 
                  // << "\n\n\n"
                  << " TID:" << TrackID
                  << " PID:" << ParentID
                  << " PDG:" << PDG
                  << " Proc:" << ProcessName
                  << " Ek0:" << InitKinEnergy
                  << " cosT0:" << InitCosTheta
                  << " Npt:" << NPoints
        << std::endl;
      }
      
    }

    /*
    if (total_deposited_energy_by_charged_particles > 500.0)
    {

      for (Int_t trjID = 0; trjID < NTracks; trjID++)
      {
        TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
        int ParentID = track->GetParentID();
        int PDG = track->GetPDG();
        int NPoints = track->GetNPoints();
        int TrackID = track->GetTrackID();
        std::string ProcessName = track->GetProcessName();
        double InitKinEnergy = track->GetInitKinEnergy();
        double InitCosTheta = track->GetInitCosTheta();
        TVector3 InitMom = track->GetInitMom();
        // double StepLength = trackPoint->GetStepLength();

        std::cout << " (" << std::setw(5) << TrackID << "," << std::setw(5) << ParentID << ") "
                  << " PDG: " << std::setw(11) << PDG << " "
                  << " Ek0:" << std::setw(10) << InitKinEnergy << " "
                  // << " dE/dx_pts:" << std::setw(10) << tracks_edep_over_pts[TrackID]
                  << " dE/dx_pts:" << std::setw(10) << energy_best_units(tracks_edep_over_pts[TrackID])
                  // << " R:" << std::setw(10) << 
                  << std::endl;
      }

      continue;
    }
    // */


    for (auto& item: tracks_map)
    {
      int TrackID = item.first;
      TND280UpTrack* track = item.second;
      int ParentId = track->GetParentID();

      [[maybe_unused]] int p_PDG = track->GetPDG();
      [[maybe_unused]] int p_NPoints = track->GetNPoints();
      [[maybe_unused]] int p_TrackID = track->GetTrackID();
      [[maybe_unused]] std::string p_ProcessName = track->GetProcessName();
      [[maybe_unused]] double p_InitKinEnergy = track->GetInitKinEnergy();
      [[maybe_unused]] double p_InitCosTheta = track->GetInitCosTheta();


      if (tracks_map.find(ParentId) != tracks_map.end())
      {
        TND280UpTrack* parent_track = tracks_map[ParentId];
        int GParentId = parent_track->GetParentID();

        [[maybe_unused]] int p_PDG = parent_track->GetPDG();
        [[maybe_unused]] int p_NPoints = parent_track->GetNPoints();
        [[maybe_unused]] int p_TrackID = parent_track->GetTrackID();
        [[maybe_unused]] std::string p_ProcessName = parent_track->GetProcessName();
        [[maybe_unused]] double p_InitKinEnergy = parent_track->GetInitKinEnergy();
        [[maybe_unused]] double p_InitCosTheta = parent_track->GetInitCosTheta();

        if (tracks_map.find(GParentId) != tracks_map.end())
        {
          TND280UpTrack* gparent_track = tracks_map[GParentId];

          [[maybe_unused]] int gp_ParentID = gparent_track->GetParentID();
          [[maybe_unused]] int gp_PDG = gparent_track->GetPDG();
          [[maybe_unused]] int gp_NPoints = gparent_track->GetNPoints();
          [[maybe_unused]] int gp_TrackID = gparent_track->GetTrackID();
          [[maybe_unused]] std::string gp_ProcessName = gparent_track->GetProcessName();
          [[maybe_unused]] double gp_InitKinEnergy = gparent_track->GetInitKinEnergy();
          [[maybe_unused]] double gp_InitCosTheta = gparent_track->GetInitCosTheta();
        }
      }
      
    }


    if (t_primary_lepton_track)
    {
      int ParentID = t_primary_lepton_track->GetParentID();
      int PDG = t_primary_lepton_track->GetPDG();
      int NPoints = t_primary_lepton_track->GetNPoints();
      int TrackID = t_primary_lepton_track->GetTrackID();
      std::string ProcessName = t_primary_lepton_track->GetProcessName();
      double InitKinEnergy = t_primary_lepton_track->GetInitKinEnergy();
      double InitCosTheta = t_primary_lepton_track->GetInitCosTheta();
      TVector3 InitMom = t_primary_lepton_track->GetInitMom();

      TVector3 init_direction(
        InitMom.X() / InitMom.Mag(),
        InitMom.Y() / InitMom.Mag(),
        InitMom.Z() / InitMom.Mag()
      );

      h_lepton_angle_distribution->Fill(InitCosTheta);

      // double xy_val = TMath::Power(init_direction.X(), 2.0) + TMath::Power(init_direction.Y(), 2.0); 
      // double xy_angle_cos = init_direction.X() / TMath::Sqrt(xy_val);

      h_lepton_angle_distribution_z->Fill(init_direction.Z());
      h_lepton_angle_distribution_x->Fill(init_direction.X());

      double lepton_edep = 0.0;
      for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
      {
        TND280UpTrackPoint* trackPoint = t_primary_lepton_track->GetPoint(ipt);

        std::string det = trackPoint->GetLogVolName();
        if (!isPtInsideSFGD(det)) continue;

        double Edep = trackPoint->GetEdep();
        // total_edep_of_all_tracks += Edep;
        lepton_edep += Edep;
      }

      h_lepton_edep->Fill(lepton_edep);
      h_all_charged_partciles_edep->Fill(total_deposited_energy_by_charged_particles);
      h_angle_energy_primary_lepton_diplot->Fill(lepton_edep, init_direction.Z());
      h_angle_energy_primary_lepton_diplot_2->Fill(init_direction.Z(), lepton_edep);
      h_angle_energy_all_charged_particles_diplot->Fill(total_deposited_energy_by_charged_particles, init_direction.Z());
      h_angle_energy_all_charged_particles_diplot_2->Fill(init_direction.Z(), total_deposited_energy_by_charged_particles);
      h_angle_initek_energy_primary_lepton_diplot_2->Fill(init_direction.Z(), InitMom.Mag());
      h_angle_Ek0_all_charged_particles_diplot_2->Fill(init_direction.Z(), sum_of_Ek0_for_all_particles);
    }
    

    std::cout << "Total Energy of outgoing particles with ParentID = 0: " << sum_of_Ek0_for_out_vertex_particles << std::endl;
    std::cout << "Total Mom of outgoing particles with ParentID = 0: " << sum_of_mom_for_out_vertex_particles << std::endl;
    std::cout << "Total Energy of all particles: " << sum_of_Ek0_for_all_particles << std::endl;
    std::cout << "Total Mom of all particles: " << sum_of_mom_for_all_particles << std::endl;

    std::cout << "\nn_started_and_stopped_in_sfgd: " << n_started_and_stopped_in_sfgd
              << "\nn_stopped_in_sfgd_from_out: " << n_stopped_in_sfgd_from_out
              << "\nn_started_and_not_stopped_in_sfgd: " << n_started_and_not_stopped_in_sfgd
              << "\nn_other: " << n_other
              << std::endl;
    
  }

  return;

  std::map<int, std::string> particle_pdg_name{
    { 11, "e-"},
    {-11, "e+"},
    { 13, "mu-"},
    {-13, "mu+"},
  };

  h_lepton_angle_distribution->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution;cos #theta;N events").c_str());
  h_lepton_angle_distribution_z->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution z;cos #theta;N events").c_str());
  h_lepton_angle_distribution_x->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution x;cos #theta;N events").c_str());
  h_lepton_edep->SetTitle((particle_pdg_name[main_particle_PDG] + " energy deposit;E deposited, MeV;N events").c_str());

  std::string out_fname_lepton_angle_z = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle_z.pdf";
  TCanvas* c_lepton_angle_z = new TCanvas("lepton_angle_z", "lepton angle z", 1080, 720);
  c_lepton_angle_z->cd();
  h_lepton_angle_distribution_z->Draw();
  h_lepton_angle_distribution_z->SetFillColor(kRed);
  c_lepton_angle_z->SaveAs(out_fname_lepton_angle_z.c_str());
  c_lepton_angle_z->Close();

  std::string out_fname_lepton_angle_x = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle_x.pdf";
  TCanvas* c_lepton_angle_x = new TCanvas("lepton_angle_x", "lepton angle x", 1080, 720);
  c_lepton_angle_x->cd();
  h_lepton_angle_distribution_x->Draw();
  h_lepton_angle_distribution_x->SetFillColor(kRed);
  c_lepton_angle_x->SaveAs(out_fname_lepton_angle_x.c_str());
  c_lepton_angle_x->Close();

  std::string out_fname_lepton_angle = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle.pdf";
  TCanvas* c_lepton_angle = new TCanvas("lepton_angle", "lepton angle", 1080, 720);
  c_lepton_angle->cd();
  h_lepton_angle_distribution->Draw();
  h_lepton_angle_distribution->SetFillColor(kRed);
  c_lepton_angle->SaveAs(out_fname_lepton_angle.c_str());
  c_lepton_angle->Close();

  std::string out_fname_lepton_edep = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_edep.pdf";
  TCanvas* c_lepton_edep = new TCanvas("lepton_angle", "lepton angle", 1080, 720);
  c_lepton_edep->cd();
  h_lepton_edep->Draw();
  h_lepton_edep->SetFillColor(kRed);
  c_lepton_edep->SaveAs(out_fname_lepton_edep.c_str());
  c_lepton_edep->Close();

  std::string out_fname_all_charged_partciles_edep = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-all_charged_partciles_edep.pdf";
  TCanvas* c_all_charged_partciles_edep = new TCanvas("all charged particles edep", "all charged particles edep", 1080, 720);
  c_all_charged_partciles_edep->cd();
  h_all_charged_partciles_edep->Draw();
  h_all_charged_partciles_edep->SetFillColor(kRed);
  c_all_charged_partciles_edep->SaveAs(out_fname_all_charged_partciles_edep.c_str());
  c_all_charged_partciles_edep->Close();

  std::string out_fname_angle_energy_primary_lepton_diplot = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_primary_lepton.pdf";
  TCanvas* c_angle_energy_primary_lepton_diplot = new TCanvas("lepton_angle_energy_diplot", "lepton angle", 1080, 720);
  c_angle_energy_primary_lepton_diplot->cd();
  h_angle_energy_primary_lepton_diplot->Draw("COLZ");
  h_angle_energy_primary_lepton_diplot->SetFillColor(kRed);
  c_angle_energy_primary_lepton_diplot->SaveAs(out_fname_angle_energy_primary_lepton_diplot.c_str());
  c_angle_energy_primary_lepton_diplot->Close();

  std::string out_fname_angle_energy_primary_lepton_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_primary_lepton_2.pdf";
  TCanvas* c_angle_energy_primary_lepton_diplot_2 = new TCanvas("lepton_angle_energy_diplot_2", "lepton angle", 1080, 720);
  c_angle_energy_primary_lepton_diplot_2->cd();
  h_angle_energy_primary_lepton_diplot_2->Draw("COLZ");
  h_angle_energy_primary_lepton_diplot_2->SetFillColor(kRed);
  c_angle_energy_primary_lepton_diplot_2->SaveAs(out_fname_angle_energy_primary_lepton_diplot_2.c_str());
  c_angle_energy_primary_lepton_diplot_2->Close();

  std::string out_fname_angle_Ek0_primary_lepton_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_Ek0_primary_lepton_2.pdf";
  TCanvas* c_angle_Ek0_primary_lepton_diplot_2 = new TCanvas("lepton_angle_Ek0_diplot_2", "lepton angle", 1080, 720);
  c_angle_Ek0_primary_lepton_diplot_2->cd();
  h_angle_initek_energy_primary_lepton_diplot_2->Draw("COLZ");
  h_angle_initek_energy_primary_lepton_diplot_2->SetFillColor(kRed);
  c_angle_Ek0_primary_lepton_diplot_2->SaveAs(out_fname_angle_Ek0_primary_lepton_diplot_2.c_str());
  c_angle_Ek0_primary_lepton_diplot_2->Close();

  std::string out_fname_angle_energy_all_charged_particles_diplot = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_all_charged_particles.pdf";
  TCanvas* c_angle_energy_all_charged_particles_diplot = new TCanvas("angle_energy_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_energy_all_charged_particles_diplot->cd();
  h_angle_energy_all_charged_particles_diplot->Draw("COLZ");
  h_angle_energy_all_charged_particles_diplot->SetFillColor(kRed);
  c_angle_energy_all_charged_particles_diplot->SaveAs(out_fname_angle_energy_all_charged_particles_diplot.c_str());
  c_angle_energy_all_charged_particles_diplot->Close();

  std::string out_fname_angle_energy_all_charged_particles_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_all_charged_particles_2.pdf";
  TCanvas* c_angle_energy_all_charged_particles_diplot_2 = new TCanvas("angle_energy_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_energy_all_charged_particles_diplot_2->cd();
  h_angle_energy_all_charged_particles_diplot_2->Draw("COLZ");
  h_angle_energy_all_charged_particles_diplot_2->SetFillColor(kRed);
  c_angle_energy_all_charged_particles_diplot_2->SaveAs(out_fname_angle_energy_all_charged_particles_diplot_2.c_str());
  c_angle_energy_all_charged_particles_diplot_2->Close();

  std::string out_fname_angle_Ek0_all_charged_particles_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_Ek0_all_charged_particles_2.pdf";
  TCanvas* c_angle_Ek0_all_charged_particles_diplot_2 = new TCanvas("angle_Ek0_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_Ek0_all_charged_particles_diplot_2->cd();
  h_angle_Ek0_all_charged_particles_diplot_2->Draw("COLZ");
  h_angle_Ek0_all_charged_particles_diplot_2->SetFillColor(kRed);
  c_angle_Ek0_all_charged_particles_diplot_2->SaveAs(out_fname_angle_Ek0_all_charged_particles_diplot_2.c_str());
  c_angle_Ek0_all_charged_particles_diplot_2->Close();

  std::string out_fname_vtx_pos = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-vtx_pos.pdf";
  TCanvas* c_vtx_pos = new TCanvas("h_edep_slices_per_cube_vs_zL_2", "Vertex position", 1080, 720);
  c_vtx_pos->Divide(2,2);
  c_vtx_pos->cd(1);
  // gPad->SetLogy();
  h_vtx_distr_x->Draw();
  h_vtx_distr_x->SetFillColor(kRed);
  c_vtx_pos->cd(2);
  // gPad->SetLogy();
  h_vtx_distr_y->Draw();
  h_vtx_distr_y->SetFillColor(kRed);
  c_vtx_pos->cd(3);
  // gPad->SetLogy();
  h_vtx_distr_z->Draw();
  h_vtx_distr_z->SetFillColor(kRed);
  // c_vtx_pos->cd(4);
  // gPad->SetLogy();
  // h_edep_at_30cm_cube->Draw();
  // h_edep_at_30cm_cube->SetFillColor(kRed);
  c_vtx_pos->SaveAs(out_fname_vtx_pos.c_str());
  c_vtx_pos->Close();
}


int main(int argc, char **argv)
{

  std::cout << "Usage:" << std::endl;
  // std::cout << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> \
  //                             <output file path with tag> \
  //                             [<event number to process>]"
  // << std::endl;
  std::cout << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> \
                              <output file path with tag> \
                              [<start event number>]\
                              [<stop event number>]"
            << "\nOR\n"
            << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> \
                              <output file path with tag> \
                              [<number of events to process from 0>]"
  << std::endl;

  // if (argc != 3 && argc != 4)
  if (argc < 3 || argc > 5)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  std::string input_rootfilename = argv[1];
  std::string output_dir_tag = argv[2];

  int start_event = -1;
  int stop_event = -1;

  if (argc == 5)
  {
    start_event = atoi(argv[3]);
    stop_event = atoi(argv[4]);
  }
  else if (argc == 4)
  {
    start_event = 0;
    stop_event = atoi(argv[3]);
  }

  std::stringstream cmd; cmd << "mkdir " << output_dir_tag;
  int res = std::system(cmd.str().c_str());
  if (res != 0) // if error (directory exists or so)
  {
    // return -1;
  }
  std::stringstream cmd2; cmd2 << "mkdir " << output_dir_tag << "/event_display";
  int res2 = std::system(cmd2.str().c_str());
  if (res2 != 0) // if error (directory exists or so)
  {
    // return -1;
  }

  // analize_events_from_particle_gun(input_rootfilename, output_dir_tag); 
  // analize_events_from_particle_gun_1(input_rootfilename, output_dir_tag); 
  // analize_events_from_genie(input_rootfilename, output_dir_tag, ""); 
  // analize_events_from_genie_v2(input_rootfilename, output_dir_tag);
  // analize_events_from_genie_extended(input_rootfilename, output_dir_tag, start_event, stop_event);
  analize_events_from_genie_new(input_rootfilename, output_dir_tag, start_event, stop_event);

  // analize_events(input_rootfilename, output_dir_tag, start_event, stop_event);
  
  return 0;
}