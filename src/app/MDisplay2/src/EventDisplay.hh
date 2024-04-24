/**
 * @file EventDisplay.hh
 *
 * @brief TODO
 *
 * @creation_date     { 28.03.2024 }
 * @modification_date ( 28.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef EVENT_DISPLAY_H
#define EVENT_DISPLAY_H

#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>

struct EventDisplayDataH
{
  TH2F* h2_xy;
  TH2F* h2_yz;
  TH2F* h2_xz;
};

struct EventDisplayDataG
{
  TMultiGraph* mg_xy;
  TMultiGraph* mg_yz;
  TMultiGraph* mg_xz;
};

class EventDisplay
{
  public:
    bool CHECK_MEMORY;
    double vm;
    double rss;

    EventDisplayDataH EventDisplayByHits;
    EventDisplayDataH EventDisplayByTrackPointsLocation;
    EventDisplayDataH EventDisplayByTrackPointsEdep;

    EventDisplayDataG EventDisplayByTrackPointsLocationG;

    TCanvas * c_event_display;

    EventDisplay();
    ~EventDisplay();

    void FillEventDisplayByHits(double x, double y, double z, double hit_edep);
    void FillEventDisplayByTrackPointsEdep(double x, double y, double z, double point_edep);

    // void DrawEventDisplayByHits(std::string out_file_path, std::string canvas_title, bool save);
    // void DrawEventDisplayByTrackPointsEdep(std::string out_file_path, std::string canvas_title, bool save);

    // void Create2DhistDisplay(TCanvas* canvas, std::string canvas_title, TH2F* h2_xy, TH2F* h2_yz, TH2F* h2_xz);
    // void Create2DhistDisplayByTrackPoints(TCanvas* canvas, std::string canvas_title);

    TGraph * CreateDisplayGraph(int color, double xmin, double xmax, double ymin, double ymax);
    void FillDisplayGraphForParticle(TGraph * g, double x, double y, double z, int idx);

    void Save(std::string out_file_path, std::string canvas_title);
    
};

class EventDataDrawing
{
  public:
    TCanvas* canvas;

    TH1F * h_n_vtx_out_tracks;
    TH1F * h_n_vtx_out_protons;
    TH1F * h_n_vtx_out_neutrons;
    TH1F * h_n_vtx_out_pos_pions;
    TH1F * h_n_vtx_out_neg_pions;
    TH1F * h_n_vtx_out_neu_pions;
    TH1F * h_n_vtx_out_neu_a_pions;

    TH1F * h_nu_energy;
    TH1F * h_nu_pdg;

    TH1F * h_edep_of_primary_lepton_in_SFGD;
    TH1F * h_edep_of_all_particles_in_SFGD;

    TH1F * h_primary_lepton_angle;
    TH1F * h_primary_lepton_Ek0;
    TH1F * h_primary_lepton_collected_true_edep_by_childs;
    TH1F * h_primary_lepton_collected_true_edep_by_childs_to_Ek0;

    TH1F * h_all_particles_collected_true_edep_by_childs_to_Ek0;

    TH1F * h_sum_Ek_of_particles_leftSFGD; // left and returned also?
    TH1F * h_sum_Ek_of_photon_leftSFGD;
    TH1F * h_sum_Ek_of_elepos_leftSFGD;
    TH1F * h_sum_Ek_of_neutron_leftSFGD;
    TH1F * h_total_Ek_of_all_particles_leftSFGD;
    TH1F * h_total_ionization_loss_of_all_particles;

    TH1F * h_edep_of_protons_in_SFGD; // for single proton
    TH1F * h_sum_edep_of_protons_in_SFGD; // sum of all protons

    TH2F * h2_edep_of_primary_lepton_in_SFGD_to_angle;
    TH2F * h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0;
    TH2F * h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD;
    TH2F * h2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep;


    std::vector<double> v_memory_consumption;

    std::vector<double> v_n_vtx_out_tracks;
    std::vector<double> v_n_vtx_out_protons;
    std::vector<double> v_n_vtx_out_neutrons;
    std::vector<double> v_n_vtx_out_pos_pions;
    std::vector<double> v_n_vtx_out_neg_pions;
    std::vector<double> v_n_vtx_out_neu_pions;
    std::vector<double> v_n_vtx_out_neu_a_pions;

    std::vector<double> v_nu_energy;
    std::vector<double> v_nu_pdg;

    std::vector<double> v_vtx_out_particles_totene;

    std::vector<double> v_edep_of_primary_lepton_in_SFGD;
    std::vector<double> v_edep_of_all_particles_in_SFGD;

    std::vector<double> v_primary_lepton_angle;
    std::vector<double> v_primary_lepton_Ek0;
    std::vector<double> v_primary_lepton_collected_true_edep_by_childs;
    std::vector<double> v_primary_lepton_collected_true_edep_by_childs_to_Ek0;

    std::vector<double> v_all_particles_collected_true_edep_by_childs_to_Ek0;

    std::vector<double> v_sum_Ek_of_particles_leftSFGD; // left and returned also?
    std::vector<double> v_sum_Ek_of_photon_leftSFGD;
    std::vector<double> v_sum_Ek_of_elepos_leftSFGD;
    std::vector<double> v_sum_Ek_of_neutron_leftSFGD;
    std::vector<double> v_total_Ek_of_all_particles_leftSFGD;
    std::vector<double> v_total_ionization_loss_of_all_particles;
    std::vector<double> v_total_ionization_loss_of_all_particles_infv;
    std::vector<std::tuple<double, double, double, double>> v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu;
    std::vector<std::tuple<double, double, double, double, double>> v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk;

    std::vector<double> v_edep_of_protons_in_SFGD; // for single proton
    std::vector<double> v_sum_edep_of_protons_in_SFGD; // sum of all protons

    // std::vector<std::pair> v2_edep_of_primary_lepton_in_SFGD_to_angle;
    // std::vector<std::pair> v2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0;
    // std::vector<std::pair> v2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD;
    // std::vector<std::pair> v2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep;

    EventDataDrawing();
    ~EventDataDrawing();

    void AdjustBinnings(double maxLeptonEk0);
    void SaveAs_old(std::string out_file_path);
    void SaveAs(std::string out_file_path);

    TH1F * CreateTH1(std::vector<double>& data, std::string name, std::string title, int n_bins, double low, double heigh);
    TH2F * CreateTH2(std::vector<double>& dataX, std::vector<double>& dataY, std::string name, std::string title,
                     int n_binsX, double lowX, double heighX,
                     int n_binsY, double lowY, double heighY);
  private:
    // std::map<TObject*, int> _histograms;
    std::map<int, std::pair<TObject*, int>> _histograms;
    void addHistogram(TObject* hist, int category, int idx = -1);
};

#endif