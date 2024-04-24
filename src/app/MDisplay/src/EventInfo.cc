

#include "EventInfo.hh"


std::vector<TrackInfo> EventInfo::getTracksLeftSFGD()
{
  std::vector<TrackInfo> tracks_left_sfgd;
  for (auto trk_item: tracks)
  {
    TrackInfo trk = trk_item.second;
    if (trk.startedInSFGD && !trk.stoppedInSFGD) tracks_left_sfgd.push_back(trk);
  }

  return tracks_left_sfgd;
}


  void EventInfo::print()
  {
    std::cout << "ID        " << ID << std::endl;
    std::cout << "NVertices " << NVertices << std::endl;
    std::cout << "NHits     " << NHits << std::endl;
    std::cout << "NTracks   " << NTracks << std::endl;

    // for (Int_t trjID = 0; trjID < NTracks; trjID++)
    // {
    //   TrackInfo trk = tracks.at(trjID);
    //   trk.print();
    // }

    double Ek0_prim_lepton = -999.0;
    double dE_prim_lepton = -999.0;
    if (primary_lepton)
    {
      std::cout << "  ~~Primary lepton~~  " << std::endl;
      // (*primary_lepton).print();
      primary_lepton->print();
      std::cout << "  ~~Primary lepton~~  " << std::endl;
      Ek0_prim_lepton = primary_lepton->InitKinEnergy;
      dE_prim_lepton = primary_lepton->dE_over_pts_in_sfgd;
    }

    double ek0_sum_of_all_tracks = 0.0;
    double ek0_sum_of_tracks_started_in_sfgd = 0.0;
    double ek0_sum_of_primary_tracks = 0.0;
    double ek0_sum_childs_of_primary_lepton = 0.0;
    // double ek0_sum_childs_of_primary_lepton_left_sfgd = 0.0;
    for (auto trk_item: tracks)
    {
      TrackInfo trk = trk_item.second;
      ek0_sum_of_all_tracks += trk.InitKinEnergy;
      if (!trk.startedInSFGD && !trk.stoppedInSFGD) continue;
      ek0_sum_of_tracks_started_in_sfgd += trk.InitKinEnergy;
      if (trk.ParentID == 0) ek0_sum_of_primary_tracks += trk.InitKinEnergy;
      // if (trk.startedInSFGD && trk.stoppedInSFGD) continue;
      trk.print();

      int root_track_id = trk.root_track_id;
      if (root_track_id == 0) continue;
      int root_track_pdg = tracks.at(root_track_id).PDG;
      int n_childs = trk.daughter_tracks_ids.size();
      if (root_track_pdg == 11 && n_childs == 0)
      // if (n_childs == 0)
      {
        ek0_sum_childs_of_primary_lepton += trk.InitKinEnergy;
      }
    }
    std::cout << " ek0_sum_of_all_tracks: " << std::setw(11) << ek0_sum_of_all_tracks
              << "\n ek0_sum_of_tracks_started_in_sfgd: " << std::setw(11) << ek0_sum_of_tracks_started_in_sfgd
              << "\n ek0_sum_of_primary_tracks: " << std::setw(11) << ek0_sum_of_primary_tracks
              << "\n ek0_sum_childs_of_primary_lepton: " << std::setw(11) << ek0_sum_childs_of_primary_lepton
              << "\n Ek0_prim_lepton: " << std::setw(11) << Ek0_prim_lepton
              << "\n dE_prim_lepton: " << std::setw(11) << dE_prim_lepton
    << std::endl;
  }
























Event::Event(TND280UpEvent *nd280UpEvent, int eventId)
{
  EventDisplay = nullptr;
  // std::unique_ptr<Track> primary_lepton;
  // std::unique_ptr<Track> highest_momentum_neg_track;
  // std::unique_ptr<Track> highest_momentum_pos_track;
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
      // double InitKinEnergy = trk->GetInitMom().Mag();
      switch (PDG)
      {
        case 2112: { n_vtx_out_neutrons++; break; }
        case 2212: { n_vtx_out_protons++; break; }
        case 211: { n_vtx_out_pos_pions++; break; }
        case -211: { n_vtx_out_neg_pions++; break; }
        case 111: { n_vtx_out_neu_pions++; break; }
        case -111: { n_vtx_out_neu_pions++; break; }
      }
    }
    // vtx->PrintVertex();
  }

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
  FillHighestMomentumTracks(momentum_of_tracks);
  FillTracksTree(trk_mom_pt);

  /// loop over tracks
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    double EdepInSFGD = 0.0;
    double ElePosLeftSFGD = 0.0;
    double GammaLeftSFGD = 0.0;
    double OthersLeftSFGD = 0.0;
    trk->CollectEdepInSFGD(EdepInSFGD, ElePosLeftSFGD, GammaLeftSFGD, OthersLeftSFGD);
    trk->left_SFGD_elepos_sumEk_childs = ElePosLeftSFGD;
    trk->left_SFGD_gamma_sumEk_childs = GammaLeftSFGD;
    trk->left_SFGD_other_sumEk_childs = OthersLeftSFGD;
    trk->edep_in_SFGD_wchilds = EdepInSFGD;
  }
}

Event::~Event()
{
  ID = -999;
  NVertices = -999;
  NHits = -999;
  NTracks = -999;

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

  Type = EventType::UNDEFINED;

  std::for_each(tracks_m.begin(), tracks_m.end(), 
    [] (decltype(tracks_m)::value_type const& p) { delete p.second; });

  delete EventDisplay;
  EventDisplay = nullptr;
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

void Event::FillTracksTree(std::map<int, std::tuple<int, double, double, double, double>>& trk_mom_pt)
{
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    // for (auto pt_item: trk->points_m) /// loop over points in track
    // {
    //   if (!pt_item.second->created_tracks.size()) continue; /// do not consider points which do not have child tracks

    //   for (auto ct: pt_item.second->created_tracks)
    //   {
    //     for (auto tmpt: trk_mom_pt)
    //     {
    //       if (
    //             abs(std::get<0>(tmpt.second) - std::get<0>(ct)) < 1.0e-6
    //             && abs(std::get<1>(tmpt.second) - std::get<1>(ct)) < 1.0e-6
    //             && abs(std::get<2>(tmpt.second) - std::get<2>(ct)) < 1.0e-6
    //             && abs(std::get<3>(tmpt.second) - std::get<3>(ct)) < 1.0e-6
    //             && abs(std::get<4>(tmpt.second) - std::get<4>(ct)) < 1.0e-6
    //           )
    //       {
    //         int trackID = tmpt.first;
    //         Track * child_trk = tracks_m.at(trackID);
    //         (trk->childs).push_back(child_trk);
    //         pt_item.second->created_tracks_ids.push_back(child_trk->ID);
    //         pt_item.second->created_tracks_ptrs.push_back(child_trk);
    //         trk->points_related_to_childs_ids.push_back(pt_item.second->ID);
    //         // break;
    //       }
    //     }
    //   }
    // }

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
            trk->points_related_to_childs_ids.push_back(pt->ID);
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
    // primary_lepton->print("", false);
    primary_lepton->print("", true);
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

  
  for (auto trk_item: tracks_m)
  {
    Track * trk = trk_item.second;
    // trk->print("", true);
    trk->print("", false);
  }
}

