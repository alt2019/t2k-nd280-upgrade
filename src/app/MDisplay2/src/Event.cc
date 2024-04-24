
#include <ranges>
#include "Event.hh"


Event::Event(EventDataDrawing * histCollection_, TND280UpEvent *nd280UpEvent, int eventId)
{
  CHECK_MEMORY = true;

  // double vm, rss;
  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the start of Event " << eventId << " constructor: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection_->v_memory_consumption.push_back(rss);
  }
  
  // EventDataDrawing * EventDisplay()
  eventDisplay = new EventDisplay();
  // histCollection = new EventDataDrawing();
  histCollection = histCollection_;

  // EventDisplay = nullptr;
  // std::unique_ptr<Track> primary_lepton;
  // std::unique_ptr<Track> highest_momentum_neg_track;
  // std::unique_ptr<Track> highest_momentum_pos_track;

  primary_lepton = nullptr;
  highest_momentum_neg_track = nullptr;
  highest_momentum_pos_track = nullptr;

  vtx_out_particles_totene = 0.0;
  neutrino_energy = -999.0;
  neutrino_pdg = -999.0;
  neutrino_target_pdg = -999;
  n_vtx_protons = -999;
  n_vtx_neutrons = -999;
  n_vtx_nuclei = -999;
  n_vtx_in_tracks = -999;
  n_vtx_out_tracks = -999;
  n_vtx_out_protons = 0;
  n_vtx_out_neutrons = 0;
  n_vtx_out_pos_pions = 0;
  n_vtx_out_neg_pions = 0;
  n_vtx_out_neu_pions = 0;
  Type = EventType::UNDEFINED;

  ID = eventId;
  NVertices = nd280UpEvent->GetNVertices();
  NTracks = nd280UpEvent->GetNTracks();
  NHits = nd280UpEvent->GetNHits();

  /// look at vertex
  ProcessVertex(nd280UpEvent);

  /// process tracks (first loop)
  std::map<int, double> momentum_of_tracks;
  std::map<int, std::tuple<int, double, double, double, double>> trk_mom_pt;
  for (Int_t trjID = 0; trjID < NTracks; trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    Track * trk = new Track(track);
    tracks_m.insert({trk->ID, trk});
    trk_mom_pt.insert({trk->ID, {trk->PDG, trk->InitKinEnergy,
                                 trk->InitMom.X(), trk->InitMom.Y(), trk->InitMom.Z()}});

    momentum_of_tracks.insert({trk->ID, (trk->InitMom).Mag()});

    if (trk->ParentID == 0 && (abs(trk->PDG) == 11 || abs(trk->PDG) == 13) )
    {
      // primary_lepton = std::make_unique<Track>(*trk);
      primary_lepton = trk;
    }
  }

  /// find highest_momentum_neg_track, highest_momentum_pos_track
  // FillHighestMomentumTracks(momentum_of_tracks);
  // FillTracksTree(trk_mom_pt);
  // FillTracksTree2();
  FillTracksTree3();

  /*
  /// loop over tracks
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    double EdepInSFGD = 0.0;
    double ElePosLeftSFGD = 0.0;
    double GammaLeftSFGD = 0.0;
    double OthersLeftSFGD = 0.0;
    double UntrackedEkLoss = 0.0;
    double dEkOutSFGD_wchilds = 0.0;
    double UntrackedEkLoss_gamma = 0.0;
    // trk->CollectEdepInSFGD(EdepInSFGD, ElePosLeftSFGD, GammaLeftSFGD, OthersLeftSFGD, UntrackedEkLoss, dEkOutSFGD_wchilds);
    trk->CollectEdepInSFGDbyChilds(EdepInSFGD);
    trk->CollectLeftSFGDeleposByChilds(ElePosLeftSFGD);
    trk->CollectLeftSFGDgammaByChilds(GammaLeftSFGD);
    trk->CollectLeftSFGDothersByChilds(OthersLeftSFGD);
    trk->CollectUntrackedEkInSFGDByChilds(UntrackedEkLoss);
    trk->CollectEdepOdReturnedTrksToSFGDByChilds(dEkOutSFGD_wchilds);
    trk->CollectUntrackedEkInSFGDByChildsForGamma(UntrackedEkLoss_gamma);
    
    trk->left_SFGD_elepos_sumEk_childs = ElePosLeftSFGD;
    trk->left_SFGD_gamma_sumEk_childs = GammaLeftSFGD;
    trk->left_SFGD_other_sumEk_childs = OthersLeftSFGD;
    trk->edep_in_SFGD_wchilds = EdepInSFGD;
    trk->UntrackedEk0Loss_wchilds = UntrackedEkLoss;
    trk->dEkOutSFGD_wchilds = dEkOutSFGD_wchilds;
    trk->UntrackedEk0Loss_gamma = UntrackedEkLoss_gamma;
  }

  /// loop over tracks
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    trk->SetControl();
  }
  // */

  double sum_elepos_left_SFGD_loss = 0.0;
  double sum_gamma_left_SFGD_loss = 0.0;
  double total_loss_of_particles_left_SFGD = 0.0;
  double total_ionization_loss_of_all_particles_in_SFGD = 0.0;
  double total_ionization_loss_of_all_particles_in_SFGD_FV = 0.0;
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    double EdepInSFGD = 0.0;
    double EdepInSFGD_InFV = 0.0;
    double ElePosLeftSFGD = 0.0;
    double GammaLeftSFGD = 0.0;
    double OthersLeftSFGD = 0.0;
    double UntrackedEkLoss = 0.0;
    // double UntrackedEkLoss_gamma = 0.0;
    trk->CollectEdepInSFGDbyChilds(EdepInSFGD);
    trk->IonizationLoss_WithChilds = EdepInSFGD;

    trk->CollectEdepInSFGDbyChilds_InFV(EdepInSFGD_InFV);
    trk->IonizationLoss_InFV_WithChilds = EdepInSFGD_InFV;

    trk->CollectUntrackedEkInSFGDByChilds(UntrackedEkLoss);
    trk->UntrackedParticlesSumEkLoss_WithChilds = UntrackedEkLoss;

    trk->CollectLeftSFGDeleposByChilds(ElePosLeftSFGD);
    trk->EkOutsideSFGDLoss_WithChilds_SplitByElePos = ElePosLeftSFGD;

    trk->CollectLeftSFGDgammaByChilds(GammaLeftSFGD);
    trk->EkOutsideSFGDLoss_WithChilds_SplitByGamma = GammaLeftSFGD;

    trk->CollectLeftSFGDothersByChilds(OthersLeftSFGD);
    trk->EkOutsideSFGDLoss_WithChilds_SplitByOthers = OthersLeftSFGD;

    trk->SetControl();

    if (trk->ParentID == 0)
    {
      sum_elepos_left_SFGD_loss += trk->EkOutsideSFGDLoss_WithChilds_SplitByElePos;
      sum_gamma_left_SFGD_loss += trk->EkOutsideSFGDLoss_WithChilds_SplitByGamma;
      total_ionization_loss_of_all_particles_in_SFGD += trk->IonizationLoss_WithChilds;
      total_ionization_loss_of_all_particles_in_SFGD_FV += trk->IonizationLoss_InFV_WithChilds;

      // total_loss_of_particles_left_SFGD += trk->EkOutsideSFGDLoss_WithChilds;
      total_loss_of_particles_left_SFGD += (
        trk->EkOutsideSFGDLoss_WithChilds_SplitByElePos +
        trk->EkOutsideSFGDLoss_WithChilds_SplitByGamma +
        trk->EkOutsideSFGDLoss_WithChilds_SplitByOthers
      );

      if (abs(trk->PDG) == 11 || abs(trk->PDG) == 13)
      {
        histCollection->h_primary_lepton_angle->Fill(trk->InitCosTheta);
        histCollection->h_primary_lepton_Ek0->Fill(trk->InitKinEnergy);

        histCollection->v_primary_lepton_angle.push_back(trk->InitCosTheta);
        histCollection->v_primary_lepton_Ek0.push_back(trk->InitKinEnergy);
        histCollection->v_edep_of_primary_lepton_in_SFGD.push_back(trk->dE_over_pts_in_sfgd);
      }
      
    }

    // histCollection->h_all_particles_collected_true_edep_by_childs_to_Ek0->Fill(
    //   trk->ControlSumOverEkLossComponents_WithChilds / trk->InitKinEnergy);
    // histCollection->v_all_particles_collected_true_edep_by_childs_to_Ek0.push_back(
    //   trk->ControlSumOverEkLossComponents_WithChilds / trk->InitKinEnergy);

    // if (trk->EkOutsideSFGDLoss) histCollection->h_sum_Ek_of_particles_leftSFGD->Fill(trk->EkOutsideSFGDLoss);
    // if (trk->EkOutsideSFGDLoss) histCollection->h_sum_Ek_of_photon_leftSFGD->Fill(trk->EkOutsideSFGDLoss);
    // if (trk->EkOutsideSFGDLoss) histCollection->h_sum_Ek_of_elepos_leftSFGD->Fill(trk->EkOutsideSFGDLoss);
  }

  histCollection->h2_sum_Ek_of_photon_leftSFGD_to_sum_Ek_of_elepos_leftSFGD->Fill(sum_gamma_left_SFGD_loss, sum_elepos_left_SFGD_loss);
  histCollection->h_sum_Ek_of_particles_leftSFGD->Fill(sum_gamma_left_SFGD_loss+sum_elepos_left_SFGD_loss);
  histCollection->h_sum_Ek_of_photon_leftSFGD->Fill(sum_gamma_left_SFGD_loss);
  histCollection->h_sum_Ek_of_elepos_leftSFGD->Fill(sum_elepos_left_SFGD_loss);
  histCollection->h_total_ionization_loss_of_all_particles->Fill(total_ionization_loss_of_all_particles_in_SFGD);
  histCollection->h_total_Ek_of_all_particles_leftSFGD->Fill(total_loss_of_particles_left_SFGD);

  histCollection->v_sum_Ek_of_particles_leftSFGD.push_back(sum_gamma_left_SFGD_loss+sum_elepos_left_SFGD_loss);
  histCollection->v_sum_Ek_of_photon_leftSFGD.push_back(sum_gamma_left_SFGD_loss);
  histCollection->v_sum_Ek_of_elepos_leftSFGD.push_back(sum_elepos_left_SFGD_loss);
  histCollection->v_total_ionization_loss_of_all_particles.push_back(total_ionization_loss_of_all_particles_in_SFGD);
  histCollection->v_total_ionization_loss_of_all_particles_infv.push_back(total_ionization_loss_of_all_particles_in_SFGD_FV);
  histCollection->v_total_Ek_of_all_particles_leftSFGD.push_back(total_loss_of_particles_left_SFGD);

  histCollection->h2_primary_lepton_tot_outSFGDloss_vs_total_collected_edep->Fill(
    total_ionization_loss_of_all_particles_in_SFGD,
    sum_elepos_left_SFGD_loss + sum_gamma_left_SFGD_loss);

  if (primary_lepton)
  {
    // histCollection->h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0->Fill(
    //   primary_lepton->InitKinEnergy, primary_lepton->EkOutsideSFGDLoss_WithChilds / primary_lepton->InitKinEnergy);
    histCollection->h2_primary_lepton_tot_outSFGDloss_to_Ek0_vs_Ek0->Fill(
      primary_lepton->InitKinEnergy, (
          primary_lepton->EkOutsideSFGDLoss_WithChilds_SplitByElePos +
          primary_lepton->EkOutsideSFGDLoss_WithChilds_SplitByGamma +
          primary_lepton->EkOutsideSFGDLoss_WithChilds_SplitByOthers
        ) / primary_lepton->InitKinEnergy);

    histCollection->h_primary_lepton_collected_true_edep_by_childs->Fill(primary_lepton->ControlSumOverEkLossComponents_WithChilds);
    histCollection->h_primary_lepton_collected_true_edep_by_childs_to_Ek0->Fill(
      primary_lepton->ControlSumOverEkLossComponents_WithChilds / primary_lepton->InitKinEnergy);

    histCollection->v_primary_lepton_collected_true_edep_by_childs.push_back(primary_lepton->ControlSumOverEkLossComponents_WithChilds);
    histCollection->v_primary_lepton_collected_true_edep_by_childs_to_Ek0.push_back(
      primary_lepton->ControlSumOverEkLossComponents_WithChilds / primary_lepton->InitKinEnergy);

    histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu.push_back({
      // histCollection->v_total_ionization_loss_of_all_particles,
      // histCollection->v_total_ionization_loss_of_all_particles_infv,
      total_ionization_loss_of_all_particles_in_SFGD,
      total_ionization_loss_of_all_particles_in_SFGD_FV,
      primary_lepton->InitKinEnergy,
      neutrino_energy
    });

    histCollection->v_Atotal_ionization_loss_of_all_particles_Ainfv_Alept_Ek0_Aenu_AtotvtxEk.push_back({
      total_ionization_loss_of_all_particles_in_SFGD,
      total_ionization_loss_of_all_particles_in_SFGD_FV,
      primary_lepton->InitKinEnergy,
      neutrino_energy,
      vtx_out_particles_totene
    });
  }

  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the end of Event " << eventId << " constructor: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection_->v_memory_consumption.push_back(rss);
  }
}

void Event::CreateDisplay(std::string out_fpath_base)
{
  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the start of Event::CreateDisplay (Event" << ID << "): " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection->v_memory_consumption.push_back(rss);

    std::cout << tracks_m.size() << std::endl;
  }

  /// loop over tracks and points -- fill event display
  // int pt_idx_global = 0;
  std::map<int, Track*>::reverse_iterator it = tracks_m.rbegin();
  // for (auto trk_item: tracks_m | std::views::reverse)
  // for (auto trk_item: tracks_m)
  std::vector<TGraph*> g_xy_v;
  std::vector<TGraph*> g_xz_v;
  std::vector<TGraph*> g_yz_v;

  // std::vector<TGraph*> g_xy_v_photons;
  // std::vector<TGraph*> g_xz_v_photons;
  // std::vector<TGraph*> g_yz_v_photons;
  while (it != tracks_m.rend())
  {
    Track * trk = it->second;
    // Track * trk = trk_item.second;
    // trk->SetControl();
    // trk->print("", false, false);

    int color = particleToROOTColor(trk->PDG);
    TGraph * g_DP_xy = eventDisplay->CreateDisplayGraph(color, 0.0, 56.0, 0.0, 192.0);
    TGraph * g_DP_xz = eventDisplay->CreateDisplayGraph(color, 0.0, 184.0, 0.0, 192.0);
    TGraph * g_DP_yz = eventDisplay->CreateDisplayGraph(color, 0.0, 184.0, 0.0, 56.0);

    g_xy_v.push_back(g_DP_xy);
    g_xz_v.push_back(g_DP_xz);
    g_yz_v.push_back(g_DP_yz);

    // if (trk->PDG == 22) {g_xy_v_photons.push_back(g_DP_xy);} else g_xy_v.push_back(g_DP_xy);
    // if (trk->PDG == 22) {g_xz_v_photons.push_back(g_DP_xz);} else g_xz_v.push_back(g_DP_xz);
    // if (trk->PDG == 22) {g_yz_v_photons.push_back(g_DP_yz);} else g_yz_v.push_back(g_DP_yz);

    for (auto pt_item: trk->points_m)
    {
      TrackPoint * pt = pt_item.second;
      if (!pt->IsIntoSFGD) continue;
      TVector3 coords_in_SFGD_cs = toSFGDCoordinateSystem(pt->AvgPosition);
      double x = coords_in_SFGD_cs.X();
      double y = coords_in_SFGD_cs.Y();
      double z = coords_in_SFGD_cs.Z();

      g_DP_xy->SetPoint(pt->ID, y, x);
      g_DP_xz->SetPoint(pt->ID, z, x);
      g_DP_yz->SetPoint(pt->ID, z, y);

      eventDisplay->FillEventDisplayByTrackPointsEdep(x, y, z, pt->Edep);
    }

    /// TODO: insert traks of photons in the very end of tracks collection (they must be first in TMultiGraph in order to not shadow other tracks)
    if (g_DP_xy->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xy->Add(g_DP_xy);
    if (g_DP_xz->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xz->Add(g_DP_xz);
    if (g_DP_yz->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_yz->Add(g_DP_yz);
    
    it++;
  }

  // for (auto trk_graph: g_xy_v_photons) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xy->Add(trk_graph);
  // for (auto trk_graph: g_xz_v_photons) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xz->Add(trk_graph);
  // for (auto trk_graph: g_yz_v_photons) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_yz->Add(trk_graph);

  // for (auto trk_graph: g_xy_v) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xy->Add(trk_graph);
  // for (auto trk_graph: g_xz_v) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_xz->Add(trk_graph);
  // for (auto trk_graph: g_yz_v) if (trk_graph->GetN()) eventDisplay->EventDisplayByTrackPointsLocationG.mg_yz->Add(trk_graph);

  std::stringstream out_fname_prefix_ss;
  out_fname_prefix_ss << out_fpath_base << "/event_display/Evt" << ID << ".pdf";
  std::stringstream canvas_title;
  canvas_title << "Event " << ID << " display. ";
  if (neutrino_pdg != -999) // neutrino event
  {
    canvas_title << "Nu: " << neutrino_pdg << ", " << std::setprecision(7) << neutrino_energy << " MeV";
  }
  else
  {
    if (primary_lepton)
      canvas_title << "Primary: " << primary_lepton->PDG << ", " << std::setprecision(7) << primary_lepton->InitKinEnergy << " MeV";
  }

  eventDisplay->Save(out_fname_prefix_ss.str(), canvas_title.str());

  for (auto it: g_xy_v)
  {
    it->Clear();
    delete it;
    it = nullptr;
  }
  g_xy_v.clear();
  for (auto it: g_xz_v)
  {
    it->Clear();
    delete it;
    it = nullptr;
  }
  g_xz_v.clear();
  for (auto it: g_yz_v)
  {
    it->Clear();
    delete it;
    it = nullptr;
  }
  g_yz_v.clear();

  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the end of Event::CreateDisplay (Event" << ID << "): " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection->v_memory_consumption.push_back(rss);
  }
}

// void Event::SaveDataHistograms(std::string output_dir_tag)
// {
//   std::string out_file_path = output_dir_tag + "/data-histograms.pdf";
//   histCollection->SaveAs(out_file_path);
// }

Event::~Event()
{
  // double vm, rss;
  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the start of Event " << ID << " destructor: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection->v_memory_consumption.push_back(rss);
  }

  // ID = -999;
  NVertices = -999;
  NHits = -999;
  NTracks = -999;

  vtx_out_particles_totene = 0.0;
  neutrino_energy = -999.0;
  neutrino_pdg = -999;
  neutrino_target_pdg = -999;
  n_vtx_protons = -999;
  n_vtx_neutrons = -999;
  n_vtx_nuclei = -999;
  n_vtx_in_tracks = -999;
  n_vtx_out_tracks = -999;
  n_vtx_out_protons = -999;
  n_vtx_out_neutrons = -999;
  n_vtx_out_pos_pions = -999;
  n_vtx_out_neg_pions = -999;
  n_vtx_out_neu_pions = -999;
  n_vtx_out_neu_apions = -999;

  Type = EventType::UNDEFINED;

  std::for_each(tracks_m.begin(), tracks_m.end(), 
    [] (decltype(tracks_m)::value_type const& p) { if (p.second) delete p.second; });
  // std::for_each(tracks_m.begin(), tracks_m.end(), 
  //   [] (decltype(tracks_m)::value_type& p) { if (p.second) { delete p.second; p.second = nullptr; } });
  tracks_m.clear();

  delete eventDisplay;
  eventDisplay = nullptr;

  if (CHECK_MEMORY)
  {
    process_mem_usage(vm, rss);
    std::cout << "\n\nMemory Consuption at the end of Event " << ID << " destructor: " << "VM " << vm << "; RSS " << rss << "\n\n" << std::endl;
    histCollection->v_memory_consumption.push_back(rss);
  }
}

void Event::ProcessVertex(TND280UpEvent *nd280UpEvent)
{
  for (int vtxID = 0; vtxID < NVertices; ++vtxID)
  {
    TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
    n_vtx_in_tracks = vtx->GetNInTracks();
    n_vtx_out_tracks = vtx->GetNOutTracks();

    for (int i = 0; i < n_vtx_in_tracks; ++i)
    {
      TND280UpTrack *trk = vtx->GetInTrack(i);
      int PDG = trk->GetPDG();
      double InitKinEnergy = trk->GetInitMom().Mag();
      // if (abs(PDG) != 12 && abs(PDG) != 14) {neutrino_target_pdg = PDG; continue;}
      switch (PDG)
      {
        case 12: {Type = EventType::NUE; neutrino_pdg = 12; neutrino_energy = InitKinEnergy; break;}
        case -12: {Type = EventType::ANUE; neutrino_pdg = -12; neutrino_energy = InitKinEnergy; break;}
        case 14: {Type = EventType::NUMU; neutrino_pdg = 14; neutrino_energy = InitKinEnergy; break;}
        case -14: {Type = EventType::ANUMU; neutrino_pdg = -14; neutrino_energy = InitKinEnergy; break;}
        default: {neutrino_target_pdg = PDG; break;} // doesn't work
      }
    }

    for (int i = 0; i < n_vtx_out_tracks; ++i)
    {
      TND280UpTrack *trk = vtx->GetOutTrack(i);
      int PDG = trk->GetPDG();
      std::cout << "<<< Out Track from vertex: " << PDG << std::endl;
      // double InitKinEnergy = trk->GetInitMom().Mag();
      switch (PDG)
      {
        case 2112: { n_vtx_out_neutrons++; break; }
        case 2212: { n_vtx_out_protons++; break; }
        case 211: { n_vtx_out_pos_pions++; break; }
        case -211: { n_vtx_out_neg_pions++; break; }
        case 111: { n_vtx_out_neu_pions++; break; }
        case -111: { n_vtx_out_neu_apions++; break; }
      }

      if (PDG < 1.0e6 - 1)
      {
        if (trk->GetInitMom().Mag()) vtx_out_particles_totene += compute_energy_from_momentum(PDG, trk->GetInitMom().Mag());
      }

      // vtx_out_particles_totene += trk->GetInitKinEnergy();
      // if (trk->GetInitMom()>Mag()) vtx_out_particles_totene += compute_energy_from_momentum(PDG, trk->GetInitMom());

      // from GENIE:
      // const int 	kPdgCompNuclCluster = 2000000300
      // const int 	kPdgHadronicBlob = 2000000002

        // 194   //
        // 195   // Note: PDG codes for nuclear targets can be computed using pdg::IonPdgCode(A,Z)
        // 196   // PDG2006 convention: 10LZZZAAAI
        // 197   // Define names for some commonly used nuclear PDG codes:
        // 198   //
        // 199   const int kPdgTgtFreeP     = 1000010010;
        // 200   const int kPdgTgtFreeN     = 1000000010;
        // 201   const int kPdgTgtDeuterium = 1000010020;
        // 202   const int kPdgTgtC12       = 1000060120;
        // 203   const int kPdgTgtO16       = 1000080160;
        // 204   const int kPdgTgtCa40      = 1000200400;
        // 205   const int kPdgTgtFe56      = 1000260560;
        // 206 
        // 207   //
        // 208   // PDG codes for GENIE special particles
        // 209   //
        // 210   const int kPdgHadronicSyst    =  2000000001; // DIS hadronic system before hadronization
        // 211   const int kPdgHadronicBlob    =  2000000002; // Unmodelled fraction of the hadronic system
        // 212   const int kPdgBindino         =  2000000101; // Binding energy subtracted from f/s nucleons
        // 213   const int kPdgCoulobtron      =  2000000102; // Coulomb energy subtracted from f/s leptons
        // 214   const int kPdgClusterNN       =  2000000200; // A nn cluster within a nucleus
        // 215   const int kPdgClusterNP       =  2000000201; // A np cluster within a nucleus
        // 216   const int kPdgClusterPP       =  2000000202; // A pp cluster within a nucleus
        // 217   const int kPdgCompNuclCluster =  2000000300; // Nucleon cluster before phase decay
        // 218   const int kPdgDarkMatter      =  2000010000; // Dark matter particle for GENIE Boosted Dark Matter mode
        // 219   const int kPdgAntiDarkMatter  = -2000010000; // Dark matter particle for GENIE Boosted Dark Matter mode
        // 220   const int kPdgMediator        =  2000010001; // Mediator particle for GENIE Boosted Dark Matter mode
        // 221   const int kPdgNHL             =  2000020000; // Neutral Heavy Lepton PDG code
        // 222   const int kPdgDarkNeutrino    =  2000030000; // Dark matter particle for GENIE Dark Neutrino mode
        // 223   const int kPdgAntiDarkNeutrino= -2000030000; // Dark matter particle for GENIE Dark Neutrino mode
        // 224   const int kPdgDNuMediator     =  2000030001; // Mediator particle for GENIE Dark Neutrino mode

    }
    // vtx->PrintVertex();

    histCollection->h_n_vtx_out_tracks->Fill(n_vtx_out_tracks);
    histCollection->v_n_vtx_out_tracks.push_back(n_vtx_out_tracks);

    if (n_vtx_out_protons > 0) {histCollection->h_n_vtx_out_protons->Fill(n_vtx_out_protons); histCollection->v_n_vtx_out_protons.push_back(n_vtx_out_protons);}
    if (n_vtx_out_neutrons > 0) {histCollection->h_n_vtx_out_neutrons->Fill(n_vtx_out_neutrons); histCollection->v_n_vtx_out_neutrons.push_back(n_vtx_out_neutrons);}
    if (n_vtx_out_pos_pions > 0) {histCollection->h_n_vtx_out_pos_pions->Fill(n_vtx_out_pos_pions); histCollection->v_n_vtx_out_pos_pions.push_back(n_vtx_out_pos_pions);}
    if (n_vtx_out_neg_pions > 0) {histCollection->h_n_vtx_out_neg_pions->Fill(n_vtx_out_neg_pions); histCollection->v_n_vtx_out_neg_pions.push_back(n_vtx_out_neg_pions);}
    if (n_vtx_out_neu_pions > 0) {histCollection->h_n_vtx_out_neu_pions->Fill(n_vtx_out_neu_pions); histCollection->v_n_vtx_out_neu_pions.push_back(n_vtx_out_neu_pions);}
    if (n_vtx_out_neu_apions > 0) {histCollection->h_n_vtx_out_neu_a_pions->Fill(n_vtx_out_neu_apions); histCollection->v_n_vtx_out_neu_a_pions.push_back(n_vtx_out_neu_apions);}
    if (neutrino_energy > 0) {histCollection->h_nu_energy->Fill(neutrino_energy); histCollection->v_nu_energy.push_back(neutrino_energy);}
  
    if (neutrino_energy > 0 && vtx_out_particles_totene > 0)
    {
      histCollection->v_vtx_out_particles_totene.push_back(vtx_out_particles_totene);
    }
  }
}


Track * Event::GetTrack(int TrackID)
{
  auto pos = tracks_m.find(TrackID);
  if (pos == tracks_m.end()) {
    return nullptr;
  }
  return pos->second;
}


void Event::FillHighestMomentumTracks(std::map<int, double>& momentum_of_tracks)
{
  /// find highest_momentum_neg_track, highest_momentum_pos_track
  std::vector<std::pair<int, double>> pairs;
  for (auto itr = momentum_of_tracks.begin(); itr != momentum_of_tracks.end(); ++itr)
    pairs.push_back(*itr);

  std::sort(
    pairs.begin(), pairs.end(),
    [=](std::pair<int, double>& a, std::pair<int, double>& b) { return a.second > b.second; }
  );

  bool found_hmnt = false;
  bool found_hmpt = false;
  for (auto item: pairs)
  {
    int tid = item.first;
    double mom = item.second;
    Track * trk = tracks_m.at(tid);
    switch (trk->PDG)
    {
      case 11: 
      case 13: 
      case -211:
      {
        if (found_hmnt && !found_hmpt) continue;
        // highest_momentum_neg_track = std::make_unique<Track>(*trk);
        highest_momentum_neg_track = trk;
        found_hmnt = true;
        break;
      }

      case -11: 
      case -13: 
      case 211:
      case 2212:
      {
        if (!found_hmnt && found_hmpt) continue;
        // highest_momentum_pos_track = std::make_unique<Track>(*trk);
        highest_momentum_pos_track = trk;
        found_hmpt = true;
        break;
      }
    }

    if (found_hmnt && found_hmpt) break;
  }
}


void Event::FillTracksTree3()
{
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;

    for (auto pt: trk->points_related_to_childs_ptrs) /// loop over points which have child tracks
    {
      std::vector<Track*> childs_at_pt;
      for (auto ct: pt->created_tracks) /// CreatedTrackAtPtDs
      {
        /// typedef std::vector<std::tuple<int, int,
        ///                        double, double, double, double,
        ///                        std::string,
        ///                        double, double, double,
        ///                        int>> CreatedTrackAtPtDs;

        int TrajTrkId_ = std::get<0>(ct);
        // int PDG_ = std::get<1>(ct);
        double InitialKineticEnergy_ = std::get<2>(ct);
        // double momX_ = std::get<3>(ct);
        // double momY_ = std::get<4>(ct);
        // double momZ_ = std::get<5>(ct);
        // std::string ProcessName_ = std::string(std::get<6>(ct));
        // double posX_ = std::get<7>(ct);
        // double posY_ = std::get<8>(ct);
        // double posZ_ = std::get<9>(ct);
        int status = std::get<10>(ct);

        if (status == 1)
        {
          if (tracks_m.find(TrajTrkId_) == tracks_m.end()) continue;
          Track * chld_trk = tracks_m.at(TrajTrkId_); // TO_CHECK
          childs_at_pt.push_back(chld_trk);
          if (pt->IsIntoSFGD)
          {
            trk->childs.push_back(chld_trk);
            trk->SumEkOfChilds_PreLoss += chld_trk->InitKinEnergy;
          }
        }
        else // status == 0
        {
          trk->UntrackedParticlesSumEkLoss += InitialKineticEnergy_;
        }
      }
      trk->point_child_m.insert({pt, childs_at_pt});

      if (pt->PostProcessName == "annihil" && abs(trk->PDG) == 11 && pt->IsIntoSFGD)
      {
        trk->SumEkOfChilds_PreLoss -= 2.0 * ELECTRON_MASS;
      }
      else if (pt->PostProcessName == "Decay" && abs(trk->PDG) == 13 && pt->IsIntoSFGD)
      {
        trk->SumEkOfChilds_PreLoss -= MUON_MASS;
      }
    }

    // double sumEkin_of_childs = 0.0;
    // for (auto chld_trk: trk->childs)
    // {
    //   sumEkin_of_childs += chld_trk->InitKinEnergy;
    // }
    // trk->SumEkOfChilds_PreLoss = sumEkin_of_childs;
  }
}


void Event::FillTracksTree(std::map<int, std::tuple<int, double, double, double, double>>& trk_mom_pt)
{
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;

    for (auto pt: trk->points_related_to_childs_ptrs) /// loop over points which have child tracks
    {
      std::vector<Track*> childs_at_pt;
      for (auto ct: pt->created_tracks)
      {
        for (auto tmpt: trk_mom_pt)
        {
          if (
                abs(std::get<0>(tmpt.second) - std::get<0>(ct)) < 1.0e-6
                && abs(std::get<1>(tmpt.second) - std::get<1>(ct)) < 1.0e-6
                && abs(std::get<2>(tmpt.second) - std::get<2>(ct)) < 1.0e-6
                && abs(std::get<3>(tmpt.second) - std::get<3>(ct)) < 1.0e-6
                && abs(std::get<4>(tmpt.second) - std::get<4>(ct)) < 1.0e-6
              )
          {
            int trackID = tmpt.first;
            Track * child_trk = tracks_m.at(trackID);
            (trk->childs).push_back(child_trk);
            pt->created_tracks_ids.push_back(child_trk->ID);
            pt->created_tracks_ptrs.push_back(child_trk);
            // trk->points_related_to_childs_ids.push_back(pt->ID);
            childs_at_pt.push_back(child_trk);
            // trk->point_child_m.insert({child_trk, pt});
          }
        }
      }
      trk->point_child_m.insert({pt, childs_at_pt});
    }
  }
}


void Event::print()
{
  std::cout << "TYPE      " << (int)Type << std::endl;
  std::cout << "ID        " << ID << std::endl;
  std::cout << "NVertices " << NVertices << std::endl;
  std::cout << "NHits     " << NHits << std::endl;
  std::cout << "NTracks   " << NTracks << std::endl;
  if (NVertices)
  {
    std::cout << "neutrino_energy     " << neutrino_energy << std::endl;
    std::cout << "neutrino_pdg        " << neutrino_pdg << std::endl;
    std::cout << "neutrino_target_pdg " << neutrino_target_pdg << std::endl;
    std::cout << "n_vtx_in_tracks     " << n_vtx_in_tracks << std::endl;
    std::cout << "n_vtx_out_tracks    " << n_vtx_out_tracks << std::endl;
    std::cout << "  #2212:" << CYAN << n_vtx_out_protons << RESET << " "
              << "#2112:" << CYAN << n_vtx_out_neutrons << RESET << " "
              << "#|111|:" << CYAN << n_vtx_out_neu_pions << RESET << " "
              << "#211:" << CYAN << n_vtx_out_pos_pions << RESET << " "
              << "#-211:" << CYAN << n_vtx_out_neg_pions << RESET << " "
    << std::endl;
  }

  std::cout << std::endl;
  if (primary_lepton) 
  {
    std::cout << "  ~~Primary lepton~~  " << std::endl;
    primary_lepton->print("", false);
    // primary_lepton->print("", true);
    std::cout << "  ~~Primary lepton~~  " << std::endl;
    // Ek0_prim_lepton = primary_lepton->InitKinEnergy;
    // dE_prim_lepton = primary_lepton->dE_over_pts_in_sfgd;
  }

  if (highest_momentum_neg_track) 
  {
    std::cout << "  ~~highest_momentum_neg_track~~  " << std::endl;
    highest_momentum_neg_track->print("", false);
    std::cout << "  ~~highest_momentum_neg_track~~  " << std::endl;
  }

  if (highest_momentum_pos_track) 
  {
    std::cout << "  ~~highest_momentum_pos_track~~  " << std::endl;
    highest_momentum_pos_track->print("", false);
    std::cout << "  ~~highest_momentum_pos_track~~  " << std::endl;
  }
  std::cout << std::endl;

  int n_correct_tracks = 0;
  int n_incorrect_tracks = 0;
  int n_correct_ele = 0;
  int n_incorrect_ele = 0;
  int n_correct_pos = 0;
  int n_incorrect_pos = 0;
  int n_correct_gamma = 0;
  int n_incorrect_gamma = 0;
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;

    // if (trk->PDG == 11) continue;
    // if (trk->PDG == -11) continue;
    // if (trk->PDG == 22) continue;
    // if (trk->PDG == 22 && trk->InitKinEnergy < 10.0) continue;
    // if (trk->PDG == 11 && trk->InitKinEnergy < 10.0) continue;
    // if (trk->PDG == 11 && trk->InitKinEnergy >= 10.0) continue;
    // if (trk->InitKinEnergy < 10.0) continue;

    trk->print("", true, true);
    // trk->print("", true, false);
    // trk->print("", false);

    if (abs(trk->total_edep_sum_by_childs_recursive_wutek - trk->InitKinEnergy) < 1.0e-3)
    {
      n_correct_tracks++;
      if      (trk->PDG ==  11) n_correct_ele++;
      else if (trk->PDG == -11) n_correct_pos++;
      else if (trk->PDG ==  22) n_correct_gamma++;
    }
    else
    {
      n_incorrect_tracks++;
      if      (trk->PDG ==  11) n_incorrect_ele++;
      else if (trk->PDG == -11) n_incorrect_pos++;
      else if (trk->PDG ==  22) n_incorrect_gamma++;
    }
  }

  std::cout << "\n N_correct_tracks  : " << n_correct_tracks
            << "\n N_incorrect_tracks: " << n_incorrect_tracks
            << "\n e-: correct: " << n_correct_ele << " incorrect: " << n_incorrect_ele
            << "\n e+: correct: " << n_correct_pos << " incorrect: " << n_incorrect_pos
            << "\n g : correct: " << n_correct_gamma << " incorrect: " << n_incorrect_gamma
            << std::endl;
}
