

#include "GenieAnalyzer.hh"

void analize_events_from_genie(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num,
    int stop_event_num)
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








void analize_events_from_genie_extended(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num,
    int stop_event_num)
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

  for (int eventId = start_event_num; eventId < stop_event_num; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;

    // if (eventId == 1588) continue; // error in map.at somewhere in tracks processing

    tinput->GetEntry(eventId);
    
    EventInfo evtInfo(nd280UpEvent, eventId);
    evtInfo.print();
  }
}


void analize_events_from_genie_new(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num,
    int stop_event_num)
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

    Event * evt = new Event(nd280UpEvent, eventId);
    evt->print();
  }
}