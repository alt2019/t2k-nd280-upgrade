

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

#include "src/utils.hh"

#include "src/ParticlesDefinition.hh"

// #include "src/ParticleGunAnalyzer.hh"
// #include "src/GenieAnalyzer.hh"

// #include "src/PointInfo.hh"
// #include "src/TrackInfo.hh"
#include "src/Event.hh"


void analize_events_from_files_list(
  std::string input_list_of_root_files,
  std::string output_dir_tag,
  int start_event_num,
  int stop_event_num
);


int main(int argc, char **argv)
{

  std::cout << "Usage:" << std::endl;
  // std::cout << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> \
  //                             <output file path with tag> \
  //                             [<event number to process>]"
  // << std::endl;
  std::cout << "  MDisplayApp <list of ROOT files as text file> \
                              <output file path with tag> \
                              [<start event number>]\
                              [<stop event number>]"
            << "\nOR\n"
            << "  MDisplayApp <list of ROOT files as text file> \
                              <output file path with tag> \
                              [<number of events to process from 0>]"
  << std::endl;

  if (argc < 3 || argc > 5)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  // std::string input_rootfilename = argv[1];
  std::string input_list_of_root_files = argv[1];
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

  std::stringstream cmd; cmd << "mkdir -p " << output_dir_tag;
  int res = std::system(cmd.str().c_str());
  if (res != 0) // if error (directory exists or so)
  {
    // return -1;
  }
  std::stringstream cmd2; cmd2 << "mkdir -p " << output_dir_tag << "/event_display";
  int res2 = std::system(cmd2.str().c_str());
  if (res2 != 0) // if error (directory exists or so)
  {
    // return -1;
  }

  analize_events_from_files_list(
    input_list_of_root_files,
    output_dir_tag,
    start_event,
    stop_event
  );

  // analize_events_from_particle_gun(input_rootfilename, output_dir_tag); 
  // analize_events_from_particle_gun_1(input_rootfilename, output_dir_tag); 
  // analize_events_from_genie(input_rootfilename, output_dir_tag, ""); 
  // analize_events_from_genie_v2(input_rootfilename, output_dir_tag);
  // analize_events_from_genie_extended(input_rootfilename, output_dir_tag, start_event, stop_event);
  // analize_events_from_genie_new(input_rootfilename, output_dir_tag, start_event, stop_event);

  // analize_events(input_rootfilename, output_dir_tag, start_event, stop_event);
  
  // std::ifstream infile(input_list_of_root_files);

  // std::vector<TFile*> input_files_v;
  // std::vector<TTree*> input_trees_v;
  // std::vector<TND280UpEvent*> input_eventobj_v;

  // std::string input_root_file;
  // while (std::getline(infile, input_root_file))
  // {
  //     // std::istringstream iss(line);
  //     // int a, b;
  //     // if (!(iss >> a >> b)) { break; } // error

  //     // process pair (a,b)

  //     // analize_events(input_rootfilename, output_dir_tag, start_event, stop_event);

  //     std::cout << "Reading the input ROOT file: " << input_root_file << std::endl;
  //     TFile *finput = new TFile(input_root_file.c_str(),"READ");
      
  //     TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //     /// Declare the input true event object
  //     TND280UpEvent *nd280UpEvent = new TND280UpEvent();

  //     int NTreeEntries = tinput->GetEntries();
  // }
  
  return 0;
}

void analize_events_from_files_list(
  std::string input_list_of_root_files,
  std::string output_dir_tag,
  int start_event_num,
  int stop_event_num
)
{
  std::ifstream infile(input_list_of_root_files);

  if (start_event_num == -1 && stop_event_num == -1)
  {
    start_event_num = 0;
    // stop_event_num = NTreeEntries;
  }
  else if (start_event_num != -1 && stop_event_num == -1)
  {
    stop_event_num = start_event_num;
    start_event_num = 0;
  }
  // if (stop_event_num > NTreeEntries) stop_event_num = NTreeEntries;

  // TH1::AddDirectory(false);

  EventDataDrawing * histCollection = new EventDataDrawing();

  double maxLeptonEk0 = 0.0;

  // std::vector<double> primary_lepton_Ek0;

  int event_index = -1;
  std::string input_root_file;
  while (std::getline(infile, input_root_file))
  {
    std::cout << "Reading the input ROOT file: " << input_root_file << std::endl;
    TFile *finput = new TFile(input_root_file.c_str(),"READ");
    
    TTree *tinput = (TTree*) finput->Get("ND280upEvents");

    /// Declare the input true event object
    TND280UpEvent *nd280UpEvent = new TND280UpEvent();
    tinput->SetBranchAddress("Event",&nd280UpEvent);
    int NTreeEntries = tinput->GetEntries();

    for (int i = 0; i < NTreeEntries; ++i)
    {
      event_index++;
      if (event_index < start_event_num) continue;
      if (stop_event_num != -1 && event_index > stop_event_num) break;

      if (nd280UpEvent->GetNVertices() > 1)
      {
        std::cout << "This event has " << nd280UpEvent->GetNVertices() << " vertices" << std::endl;
        continue;
      }

      // tinput->GetEntry(eventId);
      tinput->GetEntry(i);

      // Event * evt = new Event(histCollection, nd280UpEvent, eventId);
      Event * evt = new Event(histCollection, nd280UpEvent, event_index);
      // evt->CreateDisplay(output_dir_tag);
      // evt->SaveDataHistograms(output_dir_tag);
      evt->print();

      if (evt->primary_lepton)
      {
        if (evt->primary_lepton->InitKinEnergy > maxLeptonEk0) maxLeptonEk0 = evt->primary_lepton->InitKinEnergy;
      }

      delete evt;
      evt = nullptr;
    }

    finput->Close();
    // delete tinput;
    // tinput = NULL;
  }

  // histCollection->AdjustBinnings(maxLeptonEk0);

  std::string out_file_path = output_dir_tag + "/data-histograms.pdf";
  histCollection->SaveAs(out_file_path);



  std::cout 
    << std::setw(50) << "v_memory_consumption " << histCollection->v_memory_consumption.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_tracks " << histCollection->v_n_vtx_out_tracks.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_protons " << histCollection->v_n_vtx_out_protons.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_neutrons " << histCollection->v_n_vtx_out_neutrons.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_pos_pions " << histCollection->v_n_vtx_out_pos_pions.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_neg_pions " << histCollection->v_n_vtx_out_neg_pions.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_neu_pions " << histCollection->v_n_vtx_out_neu_pions.size() << "\n"
    << std::setw(50) << "v_n_vtx_out_neu_a_pions " << histCollection->v_n_vtx_out_neu_a_pions.size() << "\n"
    << std::setw(50) << "v_nu_energy " << histCollection->v_nu_energy.size() << "\n"
    << std::setw(50) << "v_nu_pdg " << histCollection->v_nu_pdg.size() << "\n"
    << std::setw(50) << "v_edep_of_primary_lepton_in_SFGD " << histCollection->v_edep_of_primary_lepton_in_SFGD.size() << "\n"
    << std::setw(50) << "v_edep_of_all_particles_in_SFGD " << histCollection->v_edep_of_all_particles_in_SFGD.size() << "\n"
    << std::setw(50) << "v_primary_lepton_angle " << histCollection->v_primary_lepton_angle.size() << "\n"
    << std::setw(50) << "v_primary_lepton_Ek0 " << histCollection->v_primary_lepton_Ek0.size() << "\n"
    << std::setw(50) << "v_primary_lepton_collected_true_edep_by_childs " << histCollection->v_primary_lepton_collected_true_edep_by_childs.size() << "\n"
    << std::setw(50) << "v_primary_lepton_collected_true_edep_by_childs_to_Ek0 " << histCollection->v_primary_lepton_collected_true_edep_by_childs_to_Ek0.size() << "\n"
    << std::setw(50) << "v_all_particles_collected_true_edep_by_childs_to_Ek0 " << histCollection->v_all_particles_collected_true_edep_by_childs_to_Ek0.size() << "\n"
    << std::setw(50) << "v_sum_Ek_of_particles_leftSFGD " << histCollection->v_sum_Ek_of_particles_leftSFGD.size() << "\n"
    << std::setw(50) << "v_sum_Ek_of_photon_leftSFGD " << histCollection->v_sum_Ek_of_photon_leftSFGD.size() << "\n"
    << std::setw(50) << "v_sum_Ek_of_elepos_leftSFGD " << histCollection->v_sum_Ek_of_elepos_leftSFGD.size() << "\n"
    << std::setw(50) << "v_sum_Ek_of_neutron_leftSFGD " << histCollection->v_sum_Ek_of_neutron_leftSFGD.size() << "\n"
    << std::setw(50) << "v_total_Ek_of_all_particles_leftSFGD " << histCollection->v_total_Ek_of_all_particles_leftSFGD.size() << "\n"
    << std::setw(50) << "v_total_ionization_loss_of_all_particles " << histCollection->v_total_ionization_loss_of_all_particles.size() << "\n"
    << std::setw(50) << "v_total_ionization_loss_of_all_particles_infv " << histCollection->v_total_ionization_loss_of_all_particles_infv.size() << "\n"
    << std::setw(50) << "v_edep_of_protons_in_SFGD " << histCollection->v_edep_of_protons_in_SFGD.size() << "\n"
    << std::setw(50) << "v_sum_edep_of_protons_in_SFGD " << histCollection->v_sum_edep_of_protons_in_SFGD.size() << "\n"
  << std::endl;


  ////////////////////////////////////////////////////////
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  ////////////////////////////////////////////////////////

  // TH1F * CreateTH1(std::vector<double>& data, std::string name, std::string title, int n_bins, double low, double heigh)
  // TH2F * CreateTH2(std::vector<double>& dataX, std::vector<double>& dataY, std::string name, std::string title,
  //                    int n_binsX, double lowX, double heighX,
  //                    int n_binsY, double lowY, double heighY)

  // TH1F * histCollection->CreateTH1(std::vector<double>& data, std::string name, std::string title, int n_bins, double low, double heigh)

  // v_edep_of_primary_lepton_in_SFGD
  // v_edep_of_all_particles_in_SFGD
  // v_primary_lepton_Ek0
  // v_primary_lepton_collected_true_edep_by_childs

  int n_energy_bins = 100;

  double primary_lepton_Ek0_max = *max_element(histCollection->v_primary_lepton_Ek0.begin(), histCollection->v_primary_lepton_Ek0.end());
  
  TH1F * _h_primary_lepton_Ek0 = histCollection->CreateTH1(
    histCollection->v_primary_lepton_Ek0,
    "_h_primary_lepton_Ek0",
    "Primary lepton initial kinetic energy;Ekin, MeV;N events",
    (int)n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  TH1F * _h_edep_of_primary_lepton_in_SFGD = histCollection->CreateTH1(
    histCollection->v_edep_of_primary_lepton_in_SFGD,
    std::string{"_h_edep_of_primary_lepton_in_SFGD"},
    std::string{"Primary lepton collected edep in SFGD;dE, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  TH1F * _h_primary_lepton_collected_true_edep_by_childs = histCollection->CreateTH1(
    histCollection->v_primary_lepton_collected_true_edep_by_childs,
    std::string{"_h_primary_lepton_collected_true_edep_by_childs"},
    std::string{"Primary lepton true collected edep (with all childs);dE, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  

  TH1F * _h_total_ionization_loss_of_all_particles = histCollection->CreateTH1(
    histCollection->v_total_ionization_loss_of_all_particles,
    std::string{"_h_total_ionization_loss_of_all_particles"},
    std::string{"Total ionization loss of all particles;dE, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  TH1F * _h_total_ionization_loss_of_all_particles_infv = histCollection->CreateTH1(
    histCollection->v_total_ionization_loss_of_all_particles_infv,
    std::string{"_h_total_ionization_loss_of_all_particles_infv"},
    std::string{"Total ionization loss of all particles in Fiducial Volume;dE, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  TH1F * _h_sum_Ek_of_elepos_leftSFGD = histCollection->CreateTH1(
    histCollection->v_sum_Ek_of_elepos_leftSFGD,
    std::string{"_h_sum_Ek_of_elepos_leftSFGD"},
    std::string{"Sum Ekin of all e+e- left SFGD;E, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  TH1F * _h_sum_Ek_of_photon_leftSFGD = histCollection->CreateTH1(
    histCollection->v_sum_Ek_of_photon_leftSFGD,
    std::string{"_h_sum_Ek_of_photon_leftSFGD"},
    std::string{"Sum Ekin of all gamma left SFGD;E, MeV;N events"},
    n_energy_bins*1.3, 0.0, primary_lepton_Ek0_max*1.3);

  // histCollection->v_primary_lepton_collected_true_edep_by_childs_to_Ek0
  // histCollection->v_nu_energy
  std::vector<double> v_dEfvtoE0_vs_Enu;
  std::vector<double> v_dEtoE0_vs_Enu;
  std::vector<double> v_dEtoE0_vs_Enu_relatedEnu;
  std::vector<double> v_dEfvtoE0_vs_leptEk0;
  std::vector<double> v_dEtoE0_vs_leptEk0;
  std::vector<double> v_dEtoE0_vs_leptEk0_relatedleptEk0;
  
  /*
  for (auto i = 0; i < histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu.size(); ++i)
  {
    double dEioniz = std::get<0>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu[i]);
    double dEioniz_fv = std::get<1>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu[i]);
    double leptEk0 = std::get<2>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu[i]);
    double nuE = std::get<3>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu[i]);

    if (leptEk0 > 0.0)
    {
      // v_dEtoE0_vs_leptEk0.push_back(dEioniz / leptEk0);
      // v_dEfvtoE0_vs_leptEk0.push_back(dEioniz_fv / leptEk0);
      // v_dEtoE0_vs_leptEk0_relatedleptEk0.push_back(leptEk0);

      if (nuE > 0.0)
      {
        v_dEtoE0_vs_Enu.push_back(dEioniz / leptEk0);
        v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / leptEk0);
        v_dEtoE0_vs_Enu_relatedEnu.push_back(nuE);

        if (dEioniz / nuE > 1.0) continue;
        v_dEtoE0_vs_Enu.push_back(dEioniz / nuE);
        v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / nuE);
        v_dEtoE0_vs_leptEk0_relatedleptEk0.push_back(leptEk0);
      }
    }
    
    // if (nuE > 0.0)
    // {
    //   if (dEioniz / nuE > 1.0) continue;

    //   v_dEtoE0_vs_Enu.push_back(dEioniz / nuE);
    //   v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / nuE);
    //   v_dEtoE0_vs_Enu_relatedEnu.push_back(nuE);

    //   // if (leptEk0 > 0.0)
    //   // {
    //   //   v_dEtoE0_vs_leptEk0.push_back(dEioniz / leptEk0);
    //   //   v_dEfvtoE0_vs_leptEk0.push_back(dEioniz_fv / leptEk0);
    //   //   v_dEtoE0_vs_leptEk0_relatedleptEk0.push_back(leptEk0);
    //   // }
    // }
  }
  double max_nu_ene = *std::max_element(v_dEtoE0_vs_Enu_relatedEnu.begin(), v_dEtoE0_vs_Enu_relatedEnu.end());
  double max_ratio = *std::max_element(v_dEtoE0_vs_Enu.begin(), v_dEtoE0_vs_Enu.end());
  TH2F * _h2_dEtoE0_vs_Enu = histCollection->CreateTH2(
    v_dEtoE0_vs_Enu_relatedEnu,
    v_dEtoE0_vs_Enu,
    std::string("_h2_dEtoE0_vs_Enu"), std::string("dE_tot/Ek0_lept;E_#nu, MeV;dE_tot/Ek0_lept"),
    (int)(100*1.3), 0.0, max_nu_ene*1.3,
    // 130, 0.0, 1.3
    130, 0.0, max_ratio*1.3
  );

  TH2F * _h2_dEfvtoE0_vs_Enu = histCollection->CreateTH2(
    v_dEtoE0_vs_Enu_relatedEnu,
    v_dEfvtoE0_vs_Enu,
    std::string("_h2_dEfvtoE0_vs_Enu"), std::string("dEfv_tot/Ek0_lept;E_#nu, MeV;dEfv_tot/Ek0_lept"),
    (int)(100*1.3), 0.0, max_nu_ene*1.3,
    // 130, 0.0, 1.3
    130, 0.0, max_ratio*1.3
  );

  // double max_lept_Ek0 = *std::max_element(v_dEtoE0_vs_leptEk0_relatedleptEk0.begin(), v_dEtoE0_vs_leptEk0_relatedleptEk0.end());
  // TH2F * _h2_dEtoE0_vs_leptEk0 = histCollection->CreateTH2(
  //   v_dEtoE0_vs_leptEk0_relatedleptEk0,
  //   v_dEtoE0_vs_leptEk0,
  //   std::string("_h2_dEtoE0_vs_leptEk0"), std::string("dE_tot/Ek0_lept;Ek0_lept, MeV;dE_tot/Ek0_lept"),
  //   (int)(100*1.3), 0.0, max_lept_Ek0*1.3,
  //   130, 0.0, 1.3);

  // TH2F * _h2_dEfvtoE0_vs_leptEk0 = histCollection->CreateTH2(
  //   v_dEtoE0_vs_leptEk0_relatedleptEk0,
  //   v_dEtoE0_vs_leptEk0,
  //   std::string("_h2_dEfvtoE0_vs_leptEk0"), std::string("dE_tot/Ek0_lept;Ek0_lept, MeV;dE_tot/Ek0_lept"),
  //   (int)(100*1.3), 0.0, max_lept_Ek0*1.3,
  //   130, 0.0, 1.3);

  // double max_nu_ene = *std::max_element(v_dEtoE0_vs_Enu_relatedEnu.begin(), v_dEtoE0_vs_Enu_relatedEnu.end());
  // TH2F * _h2_dEtoE0_vs_Enu = histCollection->CreateTH2(
  //   v_dEtoE0_vs_Enu_relatedEnu,
  //   v_dEtoE0_vs_Enu,
  //   std::string("_h2_dEtoE0_vs_Enu"), std::string("dE_tot/E_#nu;E_#nu, MeV;dE_tot/E_#nu"),
  //   (int)(100*1.3), 0.0, max_nu_ene*1.3,
  //   130, 0.0, 1.3);

  // TH2F * _h2_dEfvtoE0_vs_Enu = histCollection->CreateTH2(
  //   v_dEtoE0_vs_Enu_relatedEnu,
  //   v_dEfvtoE0_vs_Enu,
  //   std::string("_h2_dEfvtoE0_vs_Enu"), std::string("dEfv_tot/E_#nu;E_#nu, MeV;dEfv_tot/E_#nu"),
  //   (int)(100*1.3), 0.0, max_nu_ene*1.3,
  //   130, 0.0, 1.3);

  // double max_nu_ene = *std::max_element(v_dEtoE0_vs_Enu_relatedEnu.begin(), v_dEtoE0_vs_Enu_relatedEnu.end());
  // double max_ratio = *std::max_element(v_dEtoE0_vs_Enu.begin(), v_dEtoE0_vs_Enu.end());
  // TH2F * _h2_dEtoE0_vs_Enu = histCollection->CreateTH2(
  //   v_dEtoE0_vs_Enu_relatedEnu,
  //   v_dEtoE0_vs_Enu,
  //   std::string("_h2_dEtoE0_vs_Ek0lept"), std::string("dEioniz/E_tot;Ek0_lept, MeV;dE_tot/Ek0_lept"),
  //   (int)(100*1.3), 0.0, max_nu_ene*1.3,
  //   // 130, 0.0, 1.3
  //   130, 0.0, max_ratio*1.3
  // );

  // TH2F * _h2_dEfvtoE0_vs_Enu = histCollection->CreateTH2(
  //   v_dEtoE0_vs_Enu_relatedEnu,
  //   v_dEfvtoE0_vs_Enu,
  //   std::string("_h2_dEfvtoE0_vs_Ek0lept"), std::string("dEionizfv/E_tot;Ek0_lept, MeV;dEfv_tot/Ek0_lept"),
  //   (int)(100*1.3), 0.0, max_nu_ene*1.3,
  //   // 130, 0.0, 1.3
  //   130, 0.0, max_ratio*1.3
  // );
  // */

  for (auto i = 0; i < histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk.size(); ++i)
  {
    double dEioniz = std::get<0>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk[i]);
    double dEioniz_fv = std::get<1>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk[i]);
    double leptEk0 = std::get<2>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk[i]);
    double nuE = std::get<3>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk[i]);
    double tot_vtx_Ek = std::get<4>(histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk[i]);

    std::cout << "tot_vtx_Ek: " << i << " " << tot_vtx_Ek << std::endl;

    if (leptEk0 > 0.0) // if has lepton
    {
      // v_dEtoE0_vs_leptEk0.push_back(dEioniz / leptEk0);
      // v_dEfvtoE0_vs_leptEk0.push_back(dEioniz_fv / leptEk0);
      // v_dEtoE0_vs_leptEk0_relatedleptEk0.push_back(leptEk0);

      if (nuE > 0.0) // do not remember why
      {
        // v_dEtoE0_vs_Enu.push_back(dEioniz / tot_vtx_Ek);
        // v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / tot_vtx_Ek);
        // v_dEtoE0_vs_Enu_relatedEnu.push_back(nuE);

        // if (dEioniz / tot_vtx_Ek > 1.0) continue;
        v_dEtoE0_vs_Enu.push_back(dEioniz / tot_vtx_Ek);
        v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / tot_vtx_Ek);
        // if (dEioniz / nuE > 1.0) continue;
        // v_dEtoE0_vs_Enu.push_back(dEioniz / nuE);
        // v_dEfvtoE0_vs_Enu.push_back(dEioniz_fv / nuE);
        v_dEtoE0_vs_leptEk0_relatedleptEk0.push_back(leptEk0);
      }
    }
  }

  std::cout 
    << std::setw(50) << "v_dEtoE0_vs_Enu " << v_dEtoE0_vs_Enu.size() << "\n"
    << std::setw(50) << "v_dEfvtoE0_vs_Enu " << v_dEfvtoE0_vs_Enu.size() << "\n"
    << std::setw(50) << "v_dEtoE0_vs_leptEk0_relatedleptEk0 " << v_dEtoE0_vs_leptEk0_relatedleptEk0.size() << "\n"
  << std::endl;

  // double max_nu_ene = *std::max_element(v_dEtoE0_vs_Enu_relatedEnu.begin(), v_dEtoE0_vs_Enu_relatedEnu.end());
  double max_lept_Ek0 = *std::max_element(v_dEtoE0_vs_leptEk0_relatedleptEk0.begin(), v_dEtoE0_vs_leptEk0_relatedleptEk0.end());
  double max_ratio = *std::max_element(v_dEtoE0_vs_Enu.begin(), v_dEtoE0_vs_Enu.end());
  TH2F * _h2_dEtoE0_vs_Enu = histCollection->CreateTH2(
    v_dEtoE0_vs_leptEk0_relatedleptEk0,
    v_dEtoE0_vs_Enu,
    std::string("_h2_dEtoE0_vs_Enu"), std::string("dEioniz/E_#nu;Ek0_lept, MeV;dEioniz/E_#nu"),
    (int)(100*1.3), 0.0, max_lept_Ek0*1.3,
    // 130, 0.0, 1.3
    130, 0.0, max_ratio*1.3
  );

  TH2F * _h2_dEfvtoE0_vs_Enu = histCollection->CreateTH2(
    v_dEtoE0_vs_leptEk0_relatedleptEk0,
    v_dEfvtoE0_vs_Enu,
    std::string("_h2_dEfvtoE0_vs_Enu"), std::string("dEionizfv/E_#nu;Ek0_lept, MeV;dEionizfv/E_#nu"),
    (int)(100*1.3), 0.0, max_lept_Ek0*1.3,
    // 130, 0.0, 1.3
    130, 0.0, max_ratio*1.3
  );

  // std::cout << "!!! DONE !!!" << std::endl;

  // double max_ene_all_particles = 0.0;
  /// for neutrino events
  // double neutrino_max_ene = *max_element(v_primary_lepton_Ek0.begin(), v_primary_lepton_Ek0.end());

  

  std::string out_file_path_extended_hists_v2 = output_dir_tag + "/data-histograms-exteneded-v2.pdf";
  std::string out_file_path_extended_hists_v2_open = out_file_path_extended_hists_v2 + "(";
  std::string out_file_path_extended_hists_v2_close = out_file_path_extended_hists_v2 + ")";

  TCanvas * c0 = new TCanvas("extended histograms v2", "extended histograms v2", 1920, 1080);
  c0->cd();

  _h_primary_lepton_Ek0->SetFillColor(kRed);
  _h_primary_lepton_Ek0->Draw();
  c0->Print(out_file_path_extended_hists_v2_open.c_str());
  c0->Clear();
  _h_primary_lepton_Ek0->SetFillColor(0);

  _h_edep_of_primary_lepton_in_SFGD->SetFillColor(kRed);
  _h_edep_of_primary_lepton_in_SFGD->Draw();
  c0->Print(out_file_path_extended_hists_v2.c_str());
  c0->Clear();
  _h_edep_of_primary_lepton_in_SFGD->SetFillColor(0);

  _h_primary_lepton_collected_true_edep_by_childs->SetFillColor(kRed);
  _h_primary_lepton_collected_true_edep_by_childs->Draw();
  // c0->Print(out_file_path_extended_hists_v2_close.c_str());
  c0->Print(out_file_path_extended_hists_v2.c_str());
  c0->Clear();
  _h_primary_lepton_collected_true_edep_by_childs->SetFillColor(0);


  gPad->SetLogy(1);
  _h_total_ionization_loss_of_all_particles->SetLineColor(kRed);
  _h_total_ionization_loss_of_all_particles_infv->SetLineColor(kOrange);
  _h_sum_Ek_of_elepos_leftSFGD->SetLineColor(kBlue);
  _h_sum_Ek_of_photon_leftSFGD->SetLineColor(kGreen-4);
  _h_primary_lepton_collected_true_edep_by_childs->SetLineColor(kBlack);
  _h_primary_lepton_collected_true_edep_by_childs->SetStats(0);

  std::vector<double> maxvals;
  maxvals.push_back(_h_total_ionization_loss_of_all_particles->GetMaximum());
  maxvals.push_back(_h_total_ionization_loss_of_all_particles_infv->GetMaximum());
  maxvals.push_back(_h_sum_Ek_of_elepos_leftSFGD->GetMaximum());
  maxvals.push_back(_h_sum_Ek_of_photon_leftSFGD->GetMaximum());
  maxvals.push_back(_h_primary_lepton_collected_true_edep_by_childs->GetMaximum());
  double max = *std::max_element(maxvals.begin(), maxvals.end());

  _h_primary_lepton_collected_true_edep_by_childs->Draw();
  _h_total_ionization_loss_of_all_particles_infv->Draw("same");
  _h_total_ionization_loss_of_all_particles->Draw("same");
  _h_sum_Ek_of_elepos_leftSFGD->Draw("same");
  _h_sum_Ek_of_photon_leftSFGD->Draw("same");
  _h_primary_lepton_collected_true_edep_by_childs->GetYaxis()->SetRangeUser(1.0e-1, max*1.3);
  c0->BuildLegend(0.75, 0.7, 1.0, 0.9);
  // c0->Print(out_file_path_extended_hists_v2_close.c_str());
  c0->Print(out_file_path_extended_hists_v2.c_str());
  c0->Clear();
  _h_total_ionization_loss_of_all_particles->SetLineColor(0);
  _h_total_ionization_loss_of_all_particles_infv->SetLineColor(0);
  _h_sum_Ek_of_elepos_leftSFGD->SetLineColor(0);
  _h_sum_Ek_of_photon_leftSFGD->SetLineColor(0);
  _h_primary_lepton_collected_true_edep_by_childs->SetLineColor(0);
  gPad->SetLogy(0);

  c0->Clear();

  _h2_dEtoE0_vs_Enu->Draw("COLZ");
  c0->Print(out_file_path_extended_hists_v2.c_str());
  c0->Clear();

  _h2_dEfvtoE0_vs_Enu->Draw("COLZ");
  c0->Print(out_file_path_extended_hists_v2_close.c_str());
  c0->Clear();

  // _h2_dEtoE0_vs_leptEk0->Draw("COLZ");
  // c0->Print(out_file_path_extended_hists_v2.c_str());
  // c0->Clear();

  // _h2_dEfvtoE0_vs_leptEk0->Draw("COLZ");
  // c0->Print(out_file_path_extended_hists_v2_close.c_str());
  // c0->Clear();

  delete c0;
  c0 = nullptr;

  ////////////////////////////////////////////////////////
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  ////////////////////////////////////////////////////////

  /// drawing histograms
  std::string out_file_path_extended_hists = output_dir_tag + "/data-histograms-exteneded.pdf";
  std::string out_file_path_extended_hists_open = out_file_path_extended_hists + "(";
  std::string out_file_path_extended_hists_close = out_file_path_extended_hists + ")";

  TCanvas * c = new TCanvas("extended histograms", "extended histograms", 1920, 1080);
  c->cd();

  // const Int_t n = const_cast<const Int_t&>(histCollection->v_memory_consumption.size());
  const Int_t n = histCollection->v_memory_consumption.size();
  // if (n)
  // {
    Double_t xvect[n];
    Double_t yvect[n];
    for (auto i = 0; i < histCollection->v_memory_consumption.size(); ++i)
    {
      xvect[i] = i;
      yvect[i] = histCollection->v_memory_consumption[i];
    }
    TGraph graph = TGraph(n, xvect, yvect);
    // graph.Draw("al"); // draw the graph as a line
    graph.Draw("ap"); // draw the graph with points
  // }
  

  c->Print(out_file_path_extended_hists_open.c_str());
  c->Clear();

  std::vector<double> maxvals_vtx;
  histCollection->h_n_vtx_out_tracks->SetFillColor(0); histCollection->h_n_vtx_out_tracks->SetLineColor(kBlack);
  histCollection->h_n_vtx_out_protons->SetFillColor(0); histCollection->h_n_vtx_out_protons->SetLineColor(kGreen);
  histCollection->h_n_vtx_out_neutrons->SetFillColor(0); histCollection->h_n_vtx_out_neutrons->SetLineColor(kTeal);
  histCollection->h_n_vtx_out_pos_pions->SetFillColor(0); histCollection->h_n_vtx_out_pos_pions->SetLineColor(kYellow);
  histCollection->h_n_vtx_out_neg_pions->SetFillColor(0); histCollection->h_n_vtx_out_neg_pions->SetLineColor(kOrange);
  histCollection->h_n_vtx_out_neu_pions->SetFillColor(0); histCollection->h_n_vtx_out_neu_pions->SetLineColor(kBlue);
  histCollection->h_n_vtx_out_neu_a_pions->SetFillColor(0); histCollection->h_n_vtx_out_neu_a_pions->SetLineColor(kCyan);

  maxvals_vtx.push_back(histCollection->h_n_vtx_out_tracks->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_protons->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_neutrons->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_pos_pions->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_neg_pions->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_neu_pions->GetMaximum());
  maxvals_vtx.push_back(histCollection->h_n_vtx_out_neu_a_pions->GetMaximum());
  double max_vtx = *std::max_element(maxvals_vtx.begin(), maxvals_vtx.end());


  histCollection->h_n_vtx_out_tracks->Draw();
  if (histCollection->h_n_vtx_out_protons->GetEntries()) histCollection->h_n_vtx_out_protons->Draw("same");
  if (histCollection->h_n_vtx_out_neutrons->GetEntries()) histCollection->h_n_vtx_out_neutrons->Draw("same");
  if (histCollection->h_n_vtx_out_pos_pions->GetEntries()) histCollection->h_n_vtx_out_pos_pions->Draw("same");
  if (histCollection->h_n_vtx_out_neg_pions->GetEntries()) histCollection->h_n_vtx_out_neg_pions->Draw("same");
  if (histCollection->h_n_vtx_out_neu_pions->GetEntries()) histCollection->h_n_vtx_out_neu_pions->Draw("same");
  if (histCollection->h_n_vtx_out_neu_a_pions->GetEntries()) histCollection->h_n_vtx_out_neu_a_pions->Draw("same");
  histCollection->h_n_vtx_out_tracks->GetYaxis()->SetRangeUser(1.0e-1, max_vtx*1.3);

  TLegend * leg = c->BuildLegend(0.5, 0.5, 0.9, 0.7);
  // leg->PaintBox(0.5, 0.5, 0.7, 0.7);
  // leg->DrawBox(0.5, 0.5, 0.7, 0.7);
  // leg->DrawPave(0.5, 0.5, 0.7, 0.7);
  // leg->SetBBoxCenterX(0.7);
  // leg->SetBBoxCenterY(0.7);

  // c->Print(out_file_path_extended_hists_open.c_str());
  c->Print(out_file_path_extended_hists.c_str());
  c->Clear();


  c->Print(out_file_path_extended_hists_close.c_str());
  // c->Clear();

  delete c;
  c = nullptr;
}















/*
void analize_events(
  std::string input_rootfilename,
  std::string output_dir_tag,
  int start_event_num = -1,
  int stop_event_num = -1
);

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
  // analize_events_from_genie_new(input_rootfilename, output_dir_tag, start_event, stop_event);

  analize_events(input_rootfilename, output_dir_tag, start_event, stop_event);
  
  return 0;
}


void analize_events(
  std::string input_rootfilename,
  std::string output_dir_tag,
  int start_event_num,
  int stop_event_num
)
{
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  /// Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  int NTreeEntries = tinput->GetEntries();

  if (start_event_num == -1 && stop_event_num == -1)
  {
    start_event_num = 0;
    stop_event_num = NTreeEntries;
  }
  else if (start_event_num != -1 && stop_event_num == -1)
  {
    stop_event_num = start_event_num;
    start_event_num = 0;
  }
  if (stop_event_num > NTreeEntries) stop_event_num = NTreeEntries;

  EventDataDrawing * histCollection = new EventDataDrawing();

  for (int eventId = start_event_num; eventId < stop_event_num; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;

    if (nd280UpEvent->GetNVertices() > 1)
    {
      std::cout << "This event has " << nd280UpEvent->GetNVertices() << " vertices" << std::endl;
      continue;
    }

    // if (eventId == 1588) continue; // error in map.at somewhere in tracks processing

    tinput->GetEntry(eventId);

    Event * evt = new Event(histCollection, nd280UpEvent, eventId);
    evt->CreateDisplay(output_dir_tag);
    // evt->SaveDataHistograms(output_dir_tag);
    evt->print();
  }

  std::string out_file_path = output_dir_tag + "/data-histograms.pdf";
  histCollection->SaveAs(out_file_path);
}
// */