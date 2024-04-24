
#include <TPaveText.h>
#include <TMarker.h>
#include <TPolyMarker.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include "EventDisplay.hh"

#include "utils.hh"

EventDisplay::EventDisplay()
{
  EventDisplayByTrackPointsEdep.h2_xy = new TH2F("XY plane", "", 56, 0.0, 56.0, 192, 0.0, 192.0);
  EventDisplayByTrackPointsEdep.h2_xz = new TH2F("XZ plane", "", 184, 0.0, 184.0, 192, 0.0, 192.0);
  EventDisplayByTrackPointsEdep.h2_yz = new TH2F("YZ plane", "", 184, 0.0, 184.0, 56, 0.0, 56.0);

  EventDisplayByTrackPointsEdep.h2_xy->SetTitle(";Y, cm;X, cm");
  EventDisplayByTrackPointsEdep.h2_yz->SetTitle(";Z, cm;Y, cm");
  EventDisplayByTrackPointsEdep.h2_xz->SetTitle("");

  EventDisplayByTrackPointsEdep.h2_xz->SetStats(0);
  EventDisplayByTrackPointsEdep.h2_xy->SetStats(0);
  EventDisplayByTrackPointsEdep.h2_yz->SetStats(0);

  EventDisplayByTrackPointsEdep.h2_xy->GetXaxis()->SetLabelSize(0.05);
  EventDisplayByTrackPointsEdep.h2_xy->GetYaxis()->SetLabelSize(0.05);
  EventDisplayByTrackPointsEdep.h2_xy->GetZaxis()->SetLabelSize(0.05);

  EventDisplayByTrackPointsEdep.h2_yz->GetXaxis()->SetLabelSize(0.07);
  EventDisplayByTrackPointsEdep.h2_yz->GetYaxis()->SetLabelSize(0.07);
  EventDisplayByTrackPointsEdep.h2_yz->GetZaxis()->SetLabelSize(0.07);

  EventDisplayByTrackPointsEdep.h2_xz->GetXaxis()->SetLabelSize(0.0);
  EventDisplayByTrackPointsEdep.h2_xz->GetYaxis()->SetLabelSize(0.0);
  EventDisplayByTrackPointsEdep.h2_xz->GetZaxis()->SetLabelSize(0.02);

  EventDisplayByTrackPointsEdep.h2_yz->SetTitleSize(0.1, "X");
  EventDisplayByTrackPointsEdep.h2_yz->SetTitleSize(0.1, "Y");
  EventDisplayByTrackPointsEdep.h2_yz->SetTitleSize(0.1, "Z");

  EventDisplayByTrackPointsEdep.h2_xy->SetTitleSize(0.07, "X");
  EventDisplayByTrackPointsEdep.h2_xy->SetTitleSize(0.07, "Y");




  EventDisplayByTrackPointsLocationG.mg_xy = new TMultiGraph();
  EventDisplayByTrackPointsLocationG.mg_yz = new TMultiGraph();
  EventDisplayByTrackPointsLocationG.mg_xz = new TMultiGraph();

  EventDisplayByTrackPointsLocationG.mg_xy->SetTitle(";Y, cm;X, cm");
  EventDisplayByTrackPointsLocationG.mg_yz->SetTitle(";Z, cm;Y, cm");
  EventDisplayByTrackPointsLocationG.mg_xz->SetTitle("");

  EventDisplayByTrackPointsLocationG.mg_xy->GetXaxis()->SetLabelSize(0.05);
  EventDisplayByTrackPointsLocationG.mg_xy->GetYaxis()->SetLabelSize(0.05);
  // EventDisplayByTrackPointsLocationG.mg_xy->GetZaxis()->SetLabelSize(0.05);

  EventDisplayByTrackPointsLocationG.mg_yz->GetXaxis()->SetLabelSize(0.07);
  EventDisplayByTrackPointsLocationG.mg_yz->GetYaxis()->SetLabelSize(0.07);
  // EventDisplayByTrackPointsLocationG.mg_yz->GetZaxis()->SetLabelSize(0.07);

  EventDisplayByTrackPointsLocationG.mg_xz->GetXaxis()->SetLabelSize(0.0);
  EventDisplayByTrackPointsLocationG.mg_xz->GetYaxis()->SetLabelSize(0.0);
  // EventDisplayByTrackPointsLocationG.mg_xz->GetZaxis()->SetLabelSize(0.02);

  EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->SetTitleSize(0.1, "X");
  EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->SetTitleSize(0.1, "Y");
  // EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->SetTitleSize(0.1, "Z");

  EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->SetTitleSize(0.07, "X");
  EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->SetTitleSize(0.07, "Y");

  EventDisplayByTrackPointsLocationG.mg_xy->GetXaxis()->SetLimits(0.0, 56.0); EventDisplayByTrackPointsLocationG.mg_xy->GetYaxis()->SetRangeUser(0.0, 192.0);
  EventDisplayByTrackPointsLocationG.mg_xz->GetXaxis()->SetLimits(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_xz->GetYaxis()->SetRangeUser(0.0, 192.0);
  EventDisplayByTrackPointsLocationG.mg_yz->GetXaxis()->SetLimits(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_yz->GetYaxis()->SetRangeUser(0.0, 56.0);
  

  c_event_display = new TCanvas(
    "c_event_display_by_track_points", "c_event_display_by_track_points", 1100, 1000);
}

EventDisplay::~EventDisplay()
{
  delete EventDisplayByTrackPointsEdep.h2_xy;
  delete EventDisplayByTrackPointsEdep.h2_xz;
  delete EventDisplayByTrackPointsEdep.h2_yz;
  EventDisplayByTrackPointsEdep.h2_xy = nullptr;
  EventDisplayByTrackPointsEdep.h2_xz = nullptr;
  EventDisplayByTrackPointsEdep.h2_yz = nullptr;

  // delete EventDisplayByTrackPointsLocationG.mg_xy;
  // delete EventDisplayByTrackPointsLocationG.mg_yz;
  // delete EventDisplayByTrackPointsLocationG.mg_xz;
  // EventDisplayByTrackPointsLocationG.mg_xy = nullptr;
  // EventDisplayByTrackPointsLocationG.mg_yz = nullptr;
  // EventDisplayByTrackPointsLocationG.mg_xz = nullptr;

  c_event_display->Clear();

  delete c_event_display;
  c_event_display = nullptr;
}

void EventDisplay::Save(std::string out_file_path, std::string canvas_title)
{  
  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the start of EventDisplay::Save: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
  }

  std::string out_file_path_pre = out_file_path + "(";
  std::string out_file_path_post = out_file_path + ")";

  c_event_display->cd();

  double yz_xz_start_xy_stop_x = 0.3;
  double xy_xz_start_yz_stop_y = 0.22;
  TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
  TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
  TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);
  display_pad_xz->Draw(); // top z -- lowest
  display_pad_xy->Draw(); // top z -- middle
  display_pad_yz->Draw(); // top z -- highest

  TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
  pt->AddText(canvas_title.c_str());
  pt->SetTextSize(0.03);
  pt->Draw();

  /// set color for pads (for debugging)
  // display_pad_xy->SetFillColor(kCyan);
  // display_pad_xz->SetFillColor(kYellow-10);
  // display_pad_yz->SetFillColor(kGreen-10);

  /// set xz pad borders
  display_pad_xz->SetLeftMargin(0.03);
  // display_pad_xz->SetRightMargin(0);
  display_pad_xz->SetTopMargin(0);
  display_pad_xz->SetBottomMargin(0.0);

  /// set xy pad borders
  display_pad_xy->SetLeftMargin(0.2);
  // display_pad_xy->SetRightMargin(0.2);
  display_pad_xy->SetRightMargin(0.15);
  display_pad_xy->SetTopMargin(0);
  display_pad_xy->SetBottomMargin(0.12);
  // display_pad_xy->SetBottomMargin(0.15);
  
  /// set yz pad borders
  display_pad_yz->SetLeftMargin(0.03);
  // display_pad_yz->SetRightMargin(0);
  display_pad_yz->SetTopMargin(0);
  display_pad_yz->SetBottomMargin(0.2);

  /// draw xz
  display_pad_xz->cd();
  EventDisplayByTrackPointsEdep.h2_xz->Draw("COLZ");
  /// draw xy
  display_pad_xy->cd();
  EventDisplayByTrackPointsEdep.h2_xy->Draw("COLZ");
  /// draw yz
  display_pad_yz->cd();
  EventDisplayByTrackPointsEdep.h2_yz->Draw("COLZ");

  c_event_display->Print(out_file_path_pre.c_str());
  display_pad_xz->Clear();
  display_pad_xy->Clear();
  display_pad_yz->Clear();


  // auto legend = new TLegend(0.2, 0.2, .8, .8);
  // auto h = new TH1F("", "", 1, 0, 1);
 
  // // legend->SetNColumns(2);
 
  // // legend->AddEntry(h, "Column 1 line 1", "l");
  // // legend->AddEntry(h, "Column 2 line 1", "l");
  // // legend->AddEntry(h, "Column 1 line 2", "l");
  // // legend->AddEntry(h, "Column 2 line 2", "l");
  // // TLegendEntry le1 = TLegendEntry(h, "Column 1 line 1", "l");
  // // le1.SetLineColor();
  // TLegendEntry * le_ele = legend->AddEntry(h, "e-", "l"); le_ele->SetFillColor(kRed);
  // TLegendEntry * le_pos = legend->AddEntry(h, "e+", "l"); le_pos->SetFillColor(particleToROOTColor(-11));
  // TLegendEntry * le_mu = legend->AddEntry(h, "mu-", "l"); le_mu->SetFillColor(particleToROOTColor(13));
  // TLegendEntry * le_amu = legend->AddEntry(h, "mu+", "l"); le_amu->SetFillColor(particleToROOTColor(-13));


  /// draw xz
  display_pad_xz->cd();
  EventDisplayByTrackPointsLocationG.mg_xz->Draw("AP");
  /// draw xy
  display_pad_xy->cd();
  EventDisplayByTrackPointsLocationG.mg_xy->Draw("AP");
  /// draw yz
  display_pad_yz->cd();
  EventDisplayByTrackPointsLocationG.mg_yz->Draw("AP");

  // legend->Draw();

  // x -- SetLimits
  // y -- SetRangeUser
  // EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->GetXaxis()->SetRangeUser(0.0, 56.0); EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 192.0);
  // EventDisplayByTrackPointsLocationG.mg_xz->GetHistogram()->GetXaxis()->SetRangeUser(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_xz->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 192.0);
  // EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->GetXaxis()->SetRangeUser(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 56.0);
  // EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->GetXaxis()->SetLimits(0.0, 56.0); EventDisplayByTrackPointsLocationG.mg_xy->GetHistogram()->GetYaxis()->SetLimits(0.0, 192.0);
  // EventDisplayByTrackPointsLocationG.mg_xz->GetHistogram()->GetXaxis()->SetLimits(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_xz->GetHistogram()->GetYaxis()->SetLimits(0.0, 192.0);
  // EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->GetXaxis()->SetLimits(0.0, 184.0); EventDisplayByTrackPointsLocationG.mg_yz->GetHistogram()->GetYaxis()->SetLimits(0.0, 56.0);
  
  // c_event_display->Update();

  c_event_display->Print(out_file_path_post.c_str());


  EventDisplayByTrackPointsEdep.h2_xz->Reset();
  EventDisplayByTrackPointsEdep.h2_xy->Reset();
  EventDisplayByTrackPointsEdep.h2_yz->Reset();

  EventDisplayByTrackPointsEdep.h2_xz->Clear();
  EventDisplayByTrackPointsEdep.h2_xy->Clear();
  EventDisplayByTrackPointsEdep.h2_yz->Clear();
  EventDisplayByTrackPointsLocationG.mg_xz->Clear();
  EventDisplayByTrackPointsLocationG.mg_xy->Clear();
  EventDisplayByTrackPointsLocationG.mg_yz->Clear();
  display_pad_xz->Clear();
  display_pad_xy->Clear();
  display_pad_yz->Clear();
  pt->Clear();

  delete display_pad_xz;
  delete display_pad_xy;
  delete display_pad_yz;
  delete pt;

  display_pad_xz = nullptr;
  display_pad_xy = nullptr;
  display_pad_yz = nullptr;
  pt = nullptr;

  c_event_display->Clear();

  c_event_display->Close();

  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the end of EventDisplay::Save: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
  }
}

TGraph * EventDisplay::CreateDisplayGraph(int color, double xmin, double xmax, double ymin, double ymax)
{
  TGraph * g = new TGraph();
  g->SetMarkerColor(color);
  // g->GetXaxis()->SetRangeUser(xmin, xmax);
  // g->GetYaxis()->SetRangeUser(ymin, ymax);
  return g;
}

void EventDisplay::FillDisplayGraphForParticle(TGraph * g, double x, double y, double z, int idx)
{
  g->SetPoint(idx, y, x);
  g->SetPoint(idx, z, x);
  g->SetPoint(idx, z, y);
}

void EventDisplay::FillEventDisplayByHits(double x, double y, double z, double hit_edep)
{

}


void EventDisplay::FillEventDisplayByTrackPointsEdep(double x, double y, double z, double point_edep)
{
  EventDisplayByTrackPointsEdep.h2_xy->Fill(y, x, point_edep);
  EventDisplayByTrackPointsEdep.h2_xz->Fill(z, x, point_edep);
  EventDisplayByTrackPointsEdep.h2_yz->Fill(z, y, point_edep);
}


/*
void EventDisplay::DrawEventDisplayByHits(std::string out_file_path, std::string canvas_title, bool save)
{

}

void EventDisplay::DrawEventDisplayByTrackPointsEdep(std::string out_file_path, std::string canvas_title, bool save)
{
  EventDisplayByTrackPointsEdep.canvas_title = canvas_title;
  Create2DhistDisplay(
    EventDisplayByTrackPointsEdep.canvas,
    EventDisplayByTrackPointsEdep.canvas_title,
    EventDisplayByTrackPointsEdep.h2_xy,
    EventDisplayByTrackPointsEdep.h2_yz,
    EventDisplayByTrackPointsEdep.h2_xz
  );

  if (save)
  {
    EventDisplayByTrackPointsEdep.canvas->SaveAs(out_file_path.c_str());
  }
}

void EventDisplay::Create2DhistDisplay(TCanvas* canvas, std::string canvas_title, TH2F* h2_xy, TH2F* h2_yz, TH2F* h2_xz)
{
  h2_xy->SetTitle(";Y, cm;X, cm");
  h2_yz->SetTitle(";Z, cm;Y, cm");
  h2_xz->SetTitle("");

  h2_xz->SetStats(0);
  h2_xy->SetStats(0);
  h2_yz->SetStats(0);

  h2_xy->GetXaxis()->SetLabelSize(0.05);
  h2_xy->GetYaxis()->SetLabelSize(0.05);
  h2_xy->GetZaxis()->SetLabelSize(0.05);

  h2_yz->GetXaxis()->SetLabelSize(0.07);
  h2_yz->GetYaxis()->SetLabelSize(0.07);
  h2_yz->GetZaxis()->SetLabelSize(0.07);

  h2_xz->GetXaxis()->SetLabelSize(0.0);
  h2_xz->GetYaxis()->SetLabelSize(0.0);
  h2_xz->GetZaxis()->SetLabelSize(0.02);

  h2_yz->SetTitleSize(0.1, "X");
  h2_yz->SetTitleSize(0.1, "Y");
  h2_yz->SetTitleSize(0.1, "Z");

  h2_xy->SetTitleSize(0.07, "X");
  h2_xy->SetTitleSize(0.07, "Y");
  
  canvas->cd();

  double yz_xz_start_xy_stop_x = 0.3;
  double xy_xz_start_yz_stop_y = 0.22;
  TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
  TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
  TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);
  display_pad_xz->Draw(); // top z -- lowest
  display_pad_xy->Draw(); // top z -- middle
  display_pad_yz->Draw(); // top z -- highest

  TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
  pt->AddText(canvas_title.c_str());
  pt->SetTextSize(0.03);
  pt->Draw();

  /// set color for pads (for debugging)
  // display_pad_xy->SetFillColor(kCyan);
  // display_pad_xz->SetFillColor(kYellow-10);
  // display_pad_yz->SetFillColor(kGreen-10);

  /// set xz pad borders
  display_pad_xz->SetLeftMargin(0.03);
  // display_pad_xz->SetRightMargin(0);
  display_pad_xz->SetTopMargin(0);
  display_pad_xz->SetBottomMargin(0.0);

  /// set xy pad borders
  display_pad_xy->SetLeftMargin(0.2);
  // display_pad_xy->SetRightMargin(0.2);
  display_pad_xy->SetRightMargin(0.15);
  display_pad_xy->SetTopMargin(0);
  display_pad_xy->SetBottomMargin(0.12);
  // display_pad_xy->SetBottomMargin(0.15);
  
  /// set yz pad borders
  display_pad_yz->SetLeftMargin(0.03);
  // display_pad_yz->SetRightMargin(0);
  display_pad_yz->SetTopMargin(0);
  display_pad_yz->SetBottomMargin(0.2);

  /// draw xz
  display_pad_xz->cd();
  h2_xz->Draw("COLZ");
  /// draw xy
  display_pad_xy->cd();
  h2_xy->Draw("COLZ");
  /// draw yz
  display_pad_yz->cd();
  h2_yz->Draw("COLZ");
}
// */


EventDataDrawing::EventDataDrawing()
{
  // h_n_vtx_out_tracks = nullptr;
  // h_n_vtx_out_protons = nullptr;
  // h_n_vtx_out_neutrons = nullptr;
  // h_n_vtx_out_pos_pions = nullptr;
  // h_n_vtx_out_neg_pions = nullptr;
  // h_n_vtx_out_neu_pions = nullptr;

  h_edep_of_primary_lepton_in_SFGD = nullptr;
  h_edep_of_all_particles_in_SFGD = nullptr;

  canvas = new TCanvas("EventDataDrawing");

  h_n_vtx_out_tracks = new TH1F(
    "h_n_vtx_out_tracks",
    "Number of outcoming vertex tracks;N outcoming vertex tracks;N events",
    20, 0, 20);

  h_n_vtx_out_protons = new TH1F(
    "h_n_vtx_out_protons",
    "Number of outcoming vertex protons;N outcoming vertex protons;N events",
    20, 0, 20);
  h_n_vtx_out_neutrons = new TH1F(
    "h_n_vtx_out_neutrons",
    "Number of outcoming vertex neutrons;N outcoming vertex neutrons;N events",
    20, 0, 20);
  h_n_vtx_out_pos_pions = new TH1F(
    "h_n_vtx_out_pos_pions",
    "Number of outcoming vertex pos_pions;N outcoming vertex #pi+;N events",
    20, 0, 20);
  h_n_vtx_out_neg_pions = new TH1F(
    "h_n_vtx_out_neg_pions",
    "Number of outcoming vertex neg_pions;N outcoming vertex #pi-;N events",
    20, 0, 20);
  h_n_vtx_out_neu_pions = new TH1F(
    "h_n_vtx_out_neu_pions",
    "Number of outcoming vertex neu_pions;N outcoming vertex #pi0;N events",
    20, 0, 20);
  h_n_vtx_out_neu_a_pions = new TH1F(
    "h_n_vtx_out_neu_a_pions",
    "Number of outcoming vertex neu_a_pions;N outcoming vertex #api0;N events",
    20, 0, 20);

  h_nu_energy = new TH1F(
    "h_nu_energy",
    "Neutrino energy;#nu energy, MeV;N events",
    100, 0, 2000.0);

  // h_nu_pdg = new TH1F(
  //   "h_nu_pdg",
  //   "Neutrino type;#nu energy, MeV;N events",
  //   100, 0, 2000.0);


  h_primary_lepton_angle = new TH1F(
    "h_primary_lepton_angle",
    "Primary lepton angle;#theta, rad;N events",
    140, -1.2, 1.2);

  h_primary_lepton_Ek0 = new TH1F(
    "h_primary_lepton_Ek0",
    "Primary lepton initial kinetic energy;Ekin, MeV;N events",
    100, 0.0, 2000.0);

  h_primary_lepton_collected_true_edep_by_childs = new TH1F(
    "h_primary_lepton_collected_true_edep_by_childs",
    "Primary lepton true collected edep (with all childs);dE, MeV;N events",
    100, 0.0, 2000.0);

  h_primary_lepton_collected_true_edep_by_childs_to_Ek0 = new TH1F(
    "h_primary_lepton_collected_true_edep_by_childs_to_Ek0",
    "Primary lepton true collected edep (with all childs)/Ek0;dE, MeV;N events",
    90, 0.0, 1.5);

  h_all_particles_collected_true_edep_by_childs_to_Ek0 = new TH1F(
    "h_all_particles_collected_true_edep_by_childs_to_Ek0",
    "All particles true collected edep (with all childs)/Ek0;dE, MeV;N events",
    90, 0.0, 1.5);


  h_sum_Ek_of_particles_leftSFGD = new TH1F(
    "h_sum_Ek_of_particles_leftSFGD",
    "Total kinetic energy of all particles left SFGD;Ekin, MeV;N events",
    100, 0.0, 2000.0);

  h_sum_Ek_of_photon_leftSFGD = new TH1F(
    "h_sum_Ek_of_photon_leftSFGD",
    "Total kinetic energy of all photons left SFGD;Ekin, MeV;N events",
    100, 0.0, 2000.0);

  h_sum_Ek_of_elepos_leftSFGD = new TH1F(
    "h_sum_Ek_of_elepos_leftSFGD",
    "Total kinetic energy of all e+e- left SFGD;Ekin, MeV;N events",
    100, 0.0, 2000.0);

  h_total_Ek_of_all_particles_leftSFGD = new TH1F(
    "h_total_Ek_of_all_particles_leftSFGD",
    "Total kinetic energy of all particles left SFGD;Ekin, MeV;N events",
    100, 0.0, 2000.0);

  h_total_ionization_loss_of_all_particles =  new TH1F(
    "h_total_ionization_loss_of_all_particles",
    "Total ionization loss of all particles in SFGD;dE, MeV;N events",
    100, 0.0, 2000.0);


  h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD = new TH2F(
    "h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD",
    "Diplot photons/e+e- left SFGD sum Ek;e+/e- Ek, MeV;#gamma Ek, MeV",
    100, 0.0, 2000.0, 100, 0.0, 2000.0);

  // h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD = new TH2F(
  //   "h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD",
  //   "Diplot photons/e+e- left SFGD sum Ek;e+/e- Ek, MeV;#gamma Ek, MeV",
  //   100, 0.0, 2000.0, 100, 0.0, 2000.0);

  h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0 = new TH2F(
    "h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0",
    "Diplot Total loss out SFGD/Ek0 to Ek0 for primary lepton;Initial kinetic energy, MeV;dE_outSFGD/Ek0",
    100, 0.0, 2000.0, 100, 0.0, 2.0);

  h2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep = new TH2F(
    "h2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep",
    "Diplot Total loss out SFGD to total collected edep in SFGD;dE, MeV;dE_outSFGD",
    100, 0.0, 2000.0, 100, 0.0, 2000.0);


  addHistogram(h_nu_energy, 1, -1);
  addHistogram(h_n_vtx_out_tracks, 1, -1);
  addHistogram(h_primary_lepton_angle, 1, -1);
  addHistogram(h_primary_lepton_Ek0, 1, -1);
  addHistogram(h_primary_lepton_collected_true_edep_by_childs, 1, -1);
  addHistogram(h_primary_lepton_collected_true_edep_by_childs_to_Ek0, 1, -1);
  addHistogram(h_all_particles_collected_true_edep_by_childs_to_Ek0, 1, -1);
  addHistogram(h_sum_Ek_of_particles_leftSFGD, 1, -1);
  addHistogram(h_sum_Ek_of_photon_leftSFGD, 1, -1);
  addHistogram(h_sum_Ek_of_elepos_leftSFGD, 1, -1);
  addHistogram(h_total_Ek_of_all_particles_leftSFGD, 1, -1);
  addHistogram(h_total_ionization_loss_of_all_particles, 1, -1);
  addHistogram(h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD, 2, -1);
  addHistogram(h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0, 2, -1);
  addHistogram(h2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep, 2, -1);
  addHistogram(h_n_vtx_out_protons, 1, -1);
  addHistogram(h_n_vtx_out_neutrons, 1, -1);
  addHistogram(h_n_vtx_out_pos_pions, 1, -1);
  addHistogram(h_n_vtx_out_neg_pions, 1, -1);
  addHistogram(h_n_vtx_out_neu_pions, 1, -1);
  addHistogram(h_n_vtx_out_neu_a_pions, 1, -1);
}

EventDataDrawing::~EventDataDrawing()
{
  // delete h_n_vtx_out_tracks;
  // delete h_n_vtx_out_protons;
  // delete h_n_vtx_out_neutrons;
  // delete h_n_vtx_out_pos_pions;
  // delete h_n_vtx_out_neg_pions;
  // delete h_n_vtx_out_neu_pions;
  // h_n_vtx_out_tracks = nullptr;
  // h_n_vtx_out_protons = nullptr;
  // h_n_vtx_out_neutrons = nullptr;
  // h_n_vtx_out_pos_pions = nullptr;
  // h_n_vtx_out_neg_pions = nullptr;
  // h_n_vtx_out_neu_pions = nullptr;

  // delete h_primary_lepton_angle;
  // delete h_primary_lepton_Ek0;
  // delete h_primary_lepton_collected_true_edep_by_childs;
  // delete h_primary_lepton_collected_true_edep_by_childs_to_Ek0;
  // h_primary_lepton_angle = nullptr;
  // h_primary_lepton_Ek0 = nullptr;
  // h_primary_lepton_collected_true_edep_by_childs = nullptr;
  // h_primary_lepton_collected_true_edep_by_childs_to_Ek0 = nullptr;

  // delete h_all_particles_collected_true_edep_by_childs_to_Ek0;
  // h_all_particles_collected_true_edep_by_childs_to_Ek0 = nullptr;

  // delete h_edep_of_primary_lepton_in_SFGD;
  // delete h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0;
  // delete h_edep_of_all_particles_in_SFGD;
  // delete h_total_Ek_of_all_particles_leftSFGD;
  // delete h_total_ionization_loss_of_all_particles;
  // h_edep_of_primary_lepton_in_SFGD = nullptr;
  // h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0 = nullptr;
  // h_edep_of_all_particles_in_SFGD = nullptr;
  // h_total_Ek_of_all_particles_leftSFGD = nullptr;
  // h_total_ionization_loss_of_all_particles = nullptr;

  // delete h_sum_Ek_of_particles_leftSFGD; // left and returned also?
  // delete h_sum_Ek_of_photon_leftSFGD;
  // delete h_sum_Ek_of_elepos_leftSFGD;
  // h_sum_Ek_of_particles_leftSFGD = nullptr;
  // h_sum_Ek_of_photon_leftSFGD = nullptr;
  // h_sum_Ek_of_elepos_leftSFGD = nullptr;

  // delete h2_edep_of_primary_lepton_in_SFGD_to_angle;
  // delete h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD;
  // h2_edep_of_primary_lepton_in_SFGD_to_angle = nullptr;
  // h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD = nullptr;

  for (auto item: _histograms)
  {
    // item.second.first->Reset();
    item.second.first->Clear();
    delete item.second.first;
    item.second.first = nullptr;
  }
  _histograms.clear();
}

void EventDataDrawing::addHistogram(TObject* hist, int category, int idx)
{
  if (_histograms.empty())
  {
    int new_index = (idx != -1)? idx : 0;
    _histograms.insert({new_index, {hist, category}});
    return;
  }

  if (idx != -1)
  {

  }
  else
  {
    int last_index = _histograms.rbegin()->first;
    _histograms.insert({last_index+1, {hist, category}});
  }


}


void EventDataDrawing::AdjustBinnings(double maxLeptonEk0)
{
  // double min_lepton_Ek0, max_lepton_Ek0;
  // int default_bins_Ek = 100;
  // h_primary_lepton_Ek0->GetMinimumAndMaximum(min_lepton_Ek0, max_lepton_Ek0);
  // max_lepton_Ek0 = h_primary_lepton_Ek0->GetMaximumStored();
  // std::cout << "min_lepton_Ek0: " << min_lepton_Ek0 << " max_lepton_Ek0: " << max_lepton_Ek0
  //           << " " << h_primary_lepton_Ek0->GetMaximumBin()
  //           // << " " << h_primary_lepton_Ek0->GetBinContent(h_primary_lepton_Ek0->GetMaximumBin())
  //           << " " << h_primary_lepton_Ek0->GetXaxis()->GetBinCenter(h_primary_lepton_Ek0->GetMaximumBin())
  //           << " " << h_primary_lepton_Ek0->GetXaxis()->GetBinLowEdge(h_primary_lepton_Ek0->GetMaximumBin())
  //           << " " << h_primary_lepton_Ek0->GetXaxis()->GetBinUpEdge(h_primary_lepton_Ek0->GetMaximumBin())
  //           << std::endl;

  // for (auto i = 0; i < 100; ++i)
  // {
  //   std::cout << "Bin " << i << ": " << h_primary_lepton_Ek0->GetBinContent(i) << std::endl;
  // }

  // h_primary_lepton_Ek0->SetMaximum(max_lepton_Ek0*1.3);
  // h_primary_lepton_Ek0->SetBins(default_bins_Ek*1.3, 0.0, max_lepton_Ek0*1.3);
  h_primary_lepton_Ek0->GetXaxis()->SetRangeUser(0.0, maxLeptonEk0*1.3);
  // h_primary_lepton_Ek0->SetBins(default_bins_Ek*1.3, 0.0, maxLeptonEk0*1.3);

  // h_primary_lepton_collected_true_edep_by_childs->SetMaximum(max_lepton_Ek0*1.3);
  // h_primary_lepton_collected_true_edep_by_childs->GetXaxis()->SetRangeUser(0.0, max_lepton_Ek0*1.3);
  // h_primary_lepton_collected_true_edep_by_childs->SetBins(default_bins_Ek*1.3, 0.0, max_lepton_Ek0*1.3);
  
  // double max_ene;
  // int max_bins = 100;
  // if (h_nu_energy->GetEntries())
  // {

  // }
}

void EventDataDrawing::SaveAs(std::string out_file_path)
{
  canvas->cd();

  for (auto item: _histograms)
  {
    int category = item.second.second;
    if (category == 1)
    {
      auto hist = (TH1F*)item.second.first;
      hist->SetFillColor(kRed);
    }
  }

  std::map<int, std::pair<TObject*, int>>::iterator it = _histograms.begin();
  if (_histograms.size() == 1)
  {
    it->second.first->Draw();
    canvas->SaveAs(out_file_path.c_str());
  }
  else
  {
    std::string out_file_path_pre = out_file_path + "(";
    auto h_pre = it->second.first;
    if (it->second.second == 1)
    {
      h_pre->Draw();
    }
    else if (it->second.second == 2) 
    {
      h_pre->Draw("COLZ");
    }
    canvas->Print(out_file_path_pre.c_str());

    it++;

    while (it != std::prev(_histograms.end()))
    {
      auto h = it->second.first;
      if (it->second.second == 1)
      {
        h->Draw();
      }
      else if (it->second.second == 2) 
      {
        h->Draw("COLZ");
      }
      canvas->Print(out_file_path.c_str());
      it++;
    }

    std::string out_file_path_post = out_file_path + ")";
    auto h_post = it->second.first;
    if (it->second.second == 1)
    {
      h_post->Draw();
    }
    else if (it->second.second == 2) 
    {
      h_post->Draw("COLZ");
    }
    canvas->Print(out_file_path_post.c_str());
  }
}


void EventDataDrawing::SaveAs_old(std::string out_file_path)
{
  /*
  std::vector<TH1F*> h1d_vect;

  if (h_n_vtx_out_tracks) h1d_vect.push_back(h_n_vtx_out_tracks);
  if (h_n_vtx_out_protons) h1d_vect.push_back(h_n_vtx_out_protons);
  if (h_n_vtx_out_neutrons) h1d_vect.push_back(h_n_vtx_out_neutrons);
  if (h_n_vtx_out_pos_pions) h1d_vect.push_back(h_n_vtx_out_pos_pions);
  if (h_n_vtx_out_neg_pions) h1d_vect.push_back(h_n_vtx_out_neg_pions);
  if (h_n_vtx_out_neu_pions) h1d_vect.push_back(h_n_vtx_out_neu_pions);

  if (h_n_vtx_out_neu_pions) h1d_vect.push_back(h_edep_of_primary_lepton_in_SFGD);
  if (h_n_vtx_out_neu_pions) h1d_vect.push_back(h_edep_of_all_particles_in_SFGD);

  canvas->cd();

  if (h1d_vect.size() == 1)
  {
    h1d_vect[0]->Draw();
    canvas->SaveAs(out_file_path.c_str());
  }
  else
  {
    std::string out_file_path_pre = out_file_path + "(";
    auto h_pre = h1d_vect[0];
    h_pre->Draw();
    canvas->Print(out_file_path_pre.c_str());

    for (auto i = 1; i < h1d_vect.size() - 2; ++i)
    {
      h1d_vect[i]->Draw();
      canvas->Print(out_file_path.c_str());
    }

    std::string out_file_path_post = out_file_path + ")";
    auto h_post = h1d_vect[h1d_vect.size() - 1];
    h_post->Draw();
    canvas->Print(out_file_path_post.c_str());
  }
  // */


  /*
  std::vector<TObject*> hd_vect;

  if (h_n_vtx_out_tracks) hd_vect.push_back(h_n_vtx_out_tracks);
  if (h_n_vtx_out_protons) hd_vect.push_back(h_n_vtx_out_protons);
  if (h_n_vtx_out_neutrons) hd_vect.push_back(h_n_vtx_out_neutrons);
  if (h_n_vtx_out_pos_pions) hd_vect.push_back(h_n_vtx_out_pos_pions);
  if (h_n_vtx_out_neg_pions) hd_vect.push_back(h_n_vtx_out_neg_pions);
  if (h_n_vtx_out_neu_pions) hd_vect.push_back(h_n_vtx_out_neu_pions);

  if (h_n_vtx_out_neu_pions) hd_vect.push_back(h_edep_of_primary_lepton_in_SFGD);
  if (h_n_vtx_out_neu_pions) hd_vect.push_back(h_edep_of_all_particles_in_SFGD);

  if (h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD) hd_vect.push_back(h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD);

  canvas->cd();

  if (hd_vect.size() == 1)
  {
    hd_vect[0]->Draw();
    canvas->SaveAs(out_file_path.c_str());
  }
  else
  {
    std::string out_file_path_pre = out_file_path + "(";
    auto h_pre = hd_vect[0];
    h_pre->Draw();
    canvas->Print(out_file_path_pre.c_str());

    for (auto i = 1; i < hd_vect.size() - 2; ++i)
    {
      hd_vect[i]->Draw();
      canvas->Print(out_file_path.c_str());
    }

    std::string out_file_path_post = out_file_path + ")";
    auto h_post = hd_vect[hd_vect.size() - 1];
    h_post->Draw();
    canvas->Print(out_file_path_post.c_str());
  }
  // */


  std::map<TObject*, int> hd_map;
  /// 1 -- 1d histogram
  /// 2 -- 2d histogram

  if (h_n_vtx_out_tracks) if (h_n_vtx_out_tracks->GetEntries()) hd_map.insert({h_n_vtx_out_tracks, 1});
  if (h_n_vtx_out_protons) if (h_n_vtx_out_protons->GetEntries()) hd_map.insert({h_n_vtx_out_protons, 1});
  if (h_n_vtx_out_neutrons) if (h_n_vtx_out_neutrons->GetEntries()) hd_map.insert({h_n_vtx_out_neutrons, 1});
  if (h_n_vtx_out_pos_pions) if (h_n_vtx_out_pos_pions->GetEntries()) hd_map.insert({h_n_vtx_out_pos_pions, 1});
  if (h_n_vtx_out_neg_pions) if (h_n_vtx_out_neg_pions->GetEntries()) hd_map.insert({h_n_vtx_out_neg_pions, 1});
  if (h_n_vtx_out_neu_pions) if (h_n_vtx_out_neu_pions->GetEntries()) hd_map.insert({h_n_vtx_out_neu_pions, 1});

  if (h_n_vtx_out_neu_pions) if (h_n_vtx_out_neu_pions->GetEntries()) hd_map.insert({h_edep_of_primary_lepton_in_SFGD, 1});
  if (h_n_vtx_out_neu_pions) if (h_n_vtx_out_neu_pions->GetEntries()) hd_map.insert({h_edep_of_all_particles_in_SFGD, 1});

  if (h_primary_lepton_angle) if (h_primary_lepton_angle->GetEntries()) hd_map.insert({h_primary_lepton_angle, 1});
  if (h_primary_lepton_Ek0) if (h_primary_lepton_Ek0->GetEntries()) hd_map.insert({h_primary_lepton_Ek0, 1});

  if (h_sum_Ek_of_particles_leftSFGD) if (h_sum_Ek_of_particles_leftSFGD->GetEntries()) hd_map.insert({h_sum_Ek_of_particles_leftSFGD, 1});
  if (h_sum_Ek_of_photon_leftSFGD) if (h_sum_Ek_of_photon_leftSFGD->GetEntries()) hd_map.insert({h_sum_Ek_of_photon_leftSFGD, 1});
  if (h_sum_Ek_of_elepos_leftSFGD) if (h_sum_Ek_of_elepos_leftSFGD->GetEntries()) hd_map.insert({h_sum_Ek_of_elepos_leftSFGD, 1});
  if (h_total_Ek_of_all_particles_leftSFGD) if (h_total_Ek_of_all_particles_leftSFGD->GetEntries()) hd_map.insert({h_total_Ek_of_all_particles_leftSFGD, 1});
  if (h_total_ionization_loss_of_all_particles) if (h_total_ionization_loss_of_all_particles->GetEntries()) hd_map.insert({h_total_ionization_loss_of_all_particles, 1});
  
  if (h_primary_lepton_collected_true_edep_by_childs)
    if (h_primary_lepton_collected_true_edep_by_childs->GetEntries())
      hd_map.insert({h_primary_lepton_collected_true_edep_by_childs, 1});
  if (h_primary_lepton_collected_true_edep_by_childs_to_Ek0)
    if (h_primary_lepton_collected_true_edep_by_childs_to_Ek0->GetEntries())
      hd_map.insert({h_primary_lepton_collected_true_edep_by_childs_to_Ek0, 1});

  if (h_all_particles_collected_true_edep_by_childs_to_Ek0)
    if (h_all_particles_collected_true_edep_by_childs_to_Ek0->GetEntries())
      hd_map.insert({h_all_particles_collected_true_edep_by_childs_to_Ek0, 1});
      

  if (h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0)
    if (h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0->GetEntries())
      hd_map.insert({h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0, 2});

  if (h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD)
    if (h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD->GetEntries())
      hd_map.insert({h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD, 2});

  canvas->cd();

  std::map<TObject*, int>::iterator it = hd_map.begin();
  if (hd_map.size() == 1)
  {
    it->first->Draw();
    canvas->SaveAs(out_file_path.c_str());
  }
  else
  {
    std::string out_file_path_pre = out_file_path + "(";
    auto h_pre = it->first;
    if (it->second == 1)
    {
      h_pre->Draw();
    }
    else if (it->second == 2) 
    {
      h_pre->Draw("COLZ");
    }
    canvas->Print(out_file_path_pre.c_str());

    it++;

    while (it != std::prev(hd_map.end()))
    {
      auto h = it->first;
      if (it->second == 1)
      {
        h->Draw();
      }
      else if (it->second == 2) 
      {
        h->Draw("COLZ");
      }
      canvas->Print(out_file_path.c_str());
      it++;
    }

    std::string out_file_path_post = out_file_path + ")";
    auto h_post = it->first;
    if (it->second == 1)
    {
      h_post->Draw();
    }
    else if (it->second == 2) 
    {
      h_post->Draw("COLZ");
    }
    canvas->Print(out_file_path_post.c_str());
  }
}



TH1F * EventDataDrawing::CreateTH1(std::vector<double>& data, std::string name, std::string title, int n_bins, double low, double heigh)
{
  TH1F * h = new TH1F(name.c_str(), title.c_str(),  n_bins, low, heigh);

  for (auto it: data)
  {
    if (it)
    h->Fill(it);
  }

  return h;
}


TH2F * EventDataDrawing::CreateTH2(std::vector<double>& dataX, std::vector<double>& dataY, std::string name, std::string title,
                     int n_binsX, double lowX, double heighX,
                     int n_binsY, double lowY, double heighY)
{
  TH2F * h2 = new TH2F(name.c_str(), title.c_str(), n_binsX, lowX, heighX, n_binsY, lowY, heighY);

  int N_items_vX = dataX.size();
  int N_items_vY = dataY.size();

  if (N_items_vX != N_items_vY) throw;

  for (auto i = 0; i < N_items_vX; ++i)
  {
    h2->Fill(dataX[i], dataY[i]);
  }

  return h2;
}