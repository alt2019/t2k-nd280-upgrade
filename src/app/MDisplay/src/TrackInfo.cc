

#include "TrackInfo.hh"


TrackInfo::TrackInfo(TND280UpTrack* track)
{
  ParentID = track->GetParentID();
  PDG = track->GetPDG();
  NPoints = track->GetNPoints();
  ID = track->GetTrackID();
  ProcessName = track->GetProcessName();
  InitKinEnergy = track->GetInitKinEnergy();
  InitCosTheta = track->GetInitCosTheta();
  InitMom = track->GetInitMom();
  double te = compute_total_energy(PDG, InitKinEnergy);
  TotalEnergy = (te != -1)? te : -1.0;

  type = TrackType::Undefined;

  NearestPtIDofParent = -1;
  GParentID = -1;
  Level = -1;
  left_SFGD_plane = "-";
  root_track_id = -999;
  sum_Ek0_of_childs = -999.0;
  sum_Ek0_of_childs_dE = -999.0;
  sum_Ek0_of_childs_dE_Ekleft = -999.0;
  // edep_in_SFGD_wchilds = -999.0;
  left_SFGD_with_Ek_wchilds = -999.0;

  edep_in_SFGD_wchilds = -999.0;
  left_SFGD_elepos_sumEk_childs = -999.0;
  left_SFGD_gamma_sumEk_childs = -999.0;
  left_SFGD_neutron_sumEk_childs = -999.0;
  left_SFGD_other_sumEk_childs = -999.0;

  dE_in_reflector = -999.0;
  dE_in_fiber = -999.0;
  dE_in_scintillator = -999.0;

  startedInSFGD = false;
  stoppedInSFGD = false;

  /*
  * not set: status = -1
  * if in SFGD: status = 0
  * if left SFGD: status = 1
  */
  int status = -1; 
  left_and_returned_to_SFGD = false;

  /// first loop over points
  dE_over_pts_in_sfgd = 0.0;
  dE_over_all_pts = 0.0;
  double prev_pt_Ek = 0.0;
  for (int ipt = 0; ipt < NPoints; ++ipt)
  {
    TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
    PointInfo pt(trackPoint, track);
    pt.SetKinEnergyFromMomentum(PDG);
    if (ipt != 0) pt.dEk_with_prev_pt = TMath::Abs(pt.Ek - prev_pt_Ek);
    pt.dEk_dE_diff = pt.dEk_with_prev_pt - pt.Edep;
    if ((PDG == 2112 && pt.dEk_dE_diff >= 1.0e-5) || (pt.dEk_dE_diff > 1.0e-7)) points_with_large_dEk.push_back(pt);
    
    points.push_back(pt);
    points_m.insert({pt.ID, pt});

    prev_pt_Ek = pt.Ek;
    dE_over_all_pts += pt.Edep;

    if (pt.IsIntoSFGD)
    {
      dE_over_pts_in_sfgd += pt.Edep; // collect edep in SFGD
      if (ipt == NPoints-1) {stoppedInSFGD = true;}
      else if (ipt == 0) {startedInSFGD = true;}
    }

    /// check if track left SFGD and later returned
    switch (status)
    {
      case -1:
      {
        if (pt.IsIntoSFGD) status = 0;
        else status = 1;
        break;
      }
      case 1:
      {
        if (pt.IsIntoSFGD) // returned to SFGD
        {
          left_and_returned_to_SFGD = true;
          status = 0;
        }
        break;
      }
      case 0:
      {
        if (!pt.IsIntoSFGD) status = 1; // left SFGD
        break;
      }
    }

    switch (pt.getLocationMaterial())
    {
      case -1: // point outside sfgd
      {
        break;
      }
      case 0: // scintillator
      {
        dE_in_scintillator += pt.Edep;
        break;
      }
      case 1: // reflector
      {
        dE_in_reflector += pt.Edep;
        break;
      }
      case 2: // fiber X
      {
        dE_in_fiber += pt.Edep;
        break;
      }
      case 3: // fiber Y
      {
        dE_in_fiber += pt.Edep;
        break;
      }
      case 4: // fiber Z
      {
        dE_in_fiber += pt.Edep;
        break;
      }
      case 5: // hole
      {
        break;
      }
    }

    if (ipt == 0)
    {
      StartPos = pt.AvgPosition;
    }
  }

  left_SFGD_with_Ek = -1.0;
  if (startedInSFGD && !stoppedInSFGD)
  {
    for (int i = NPoints - 1; i >= 1; i--)
    {
      PointInfo pt = points[i];
      PointInfo prev_pt = points[i-1];

      if (!pt.IsIntoSFGD && prev_pt.IsIntoSFGD)
      {
        left_SFGD_with_Ek = prev_pt.Ek;
        TVector3 avgpos = pt.PostPosition;
        left_SFGD_plane = getDetectorPlaneOfPoint(avgpos);
        break;
      }
    }
  }
  else if (startedInSFGD && stoppedInSFGD)
  {
    if (PDG == 11 && NPoints == 2) type = TrackType::DeltaElectron;
  }
}


void TrackInfo::SetChilds(std::vector<int>& childs, std::map<int, TrackInfo>& tracks)
{
  daughter_tracks_ids = childs;

  // sum_Ek0_of_childs
  if (childs.size())
  {
    sum_Ek0_of_childs = 0.0;
    for (auto cid: childs)
    {
      // sum_Ek0_of_childs += tracks[cid].InitKinEnergy; // doesn't work
      sum_Ek0_of_childs += tracks.at(cid).InitKinEnergy;
    }
    sum_Ek0_of_childs_dE = sum_Ek0_of_childs + dE_over_pts_in_sfgd;
    if (left_SFGD_with_Ek != -1.0)
    {
      sum_Ek0_of_childs_dE_Ekleft = sum_Ek0_of_childs_dE + left_SFGD_with_Ek;
    }
  }
}


void TrackInfo::SetParents(std::vector<int>& parents)
{
  parent_tracks_ids = parents;
  if (parents.size() >= 2)
  {
    root_track_id = parents[parents.size()-2];
  }
  else root_track_id = 0;
}


void TrackInfo::print()
{
  std::stringstream parent_tracks_ids_ss;
  parent_tracks_ids_ss << "";
  if (parent_tracks_ids.size())
  {
    for (auto tid: parent_tracks_ids)
    {
      parent_tracks_ids_ss << tid << ",";
    }
  }

  std::stringstream child_tracks_ids_ss;
  child_tracks_ids_ss << "";
  if (daughter_tracks_ids.size())
  {
    for (auto tid: daughter_tracks_ids)
    {
      child_tracks_ids_ss << tid << ",";
    }
  }

  double sum_ = (
            edep_in_SFGD_wchilds
            + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs
            + left_SFGD_neutron_sumEk_childs + left_SFGD_other_sumEk_childs
          );

  std::cout 
    // << "\n\n\n"
    << "== "
    // << " TID:" << std::setw(4) << ID
    // << " PID:" << std::setw(4) << ParentID
    // << " GPID:" << std::setw(4) << GParentID
    << BOLDGREEN << " (TPG:" << std::setw(4) << ID << ":" << std::setw(4) << ParentID << ":" << std::setw(4) << GParentID << ")" << RESET
    << " " << BOLDCYAN << NearestPtIDofParent << RESET
    // << " L:" << std::setw(4) << Level
    << " PDG:" << std::setw(11) << BOLDYELLOW << PDG << RESET
    << " Ek0:" << std::setw(11) << InitKinEnergy
    << " Etot:" << std::setw(11) << TotalEnergy
    << " cosT0:" << std::setw(11) << InitCosTheta
    << " Npt:" << std::setw(5) << NPoints
    // << " [-> " << startedInSFGD << " . " << stoppedInSFGD << "] "
    << " [" << startedInSFGD << stoppedInSFGD << "] "
    << " dE_pts: " << std::setw(11) << dE_over_pts_in_sfgd
    << " dE_pts_all: " << std::setw(11) << dE_over_all_pts
    << " lEk: " << std::setw(11) << left_SFGD_with_Ek
    << " Pl:" << left_SFGD_plane
    << " LR:" << left_and_returned_to_SFGD
    << " T:" << (int)type
    << " [" << StartPos.X() << "|" << StartPos.Y() << "|" << StartPos.Z() << "]"
    << " Proc:" << ProcessName
    << " RT:" << root_track_id
    << "\n\t Parents: " << parent_tracks_ids_ss.str()
        << " Childs: " << child_tracks_ids_ss.str()
        // << " Childs sumEk0: " << sum_Ek0_of_childs
        // << " Childs sumEk0+dEpts: " << sum_Ek0_of_childs_dE
        // << " Childs sumEk0+dEpts+Ekleft: " << sum_Ek0_of_childs_dE_Ekleft
    // << "\n\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
    //     << " left_SFGD_with_Ek_wchilds: " << left_SFGD_with_Ek_wchilds

    << BOLDMAGENTA << "\n\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
        << RED << " left_SFGD_elepos: " << left_SFGD_elepos_sumEk_childs
        << GREEN <<" left_SFGD_gamma: " << left_SFGD_gamma_sumEk_childs
        << BOLDBLUE <<" left_SFGD_neutron: " << left_SFGD_neutron_sumEk_childs
        << YELLOW <<" left_SFGD_other: " << left_SFGD_other_sumEk_childs
        << BOLDCYAN <<" sum: " << sum_
        << BOLDRED << " diff: " << sum_ - InitKinEnergy
        << RESET
  << std::endl;
  for (auto pt: points)
  {
    pt.print();
  }
}


int TrackInfo::GetNearestPtID(TVector3 pt_pos)
{
  int idx = -1;
  double min_diff = 10.0e10;
  // for (auto pt: points)
  for (auto pt: points_with_large_dEk)
  {
    // double diff = TMath::Abs((pt.AvgPosition - pt_pos).Mag());
    double diff = TMath::Abs((pt.PrePosition - pt_pos).Mag());
    if (diff < min_diff)
    {
      idx = pt.ID;
      min_diff = diff;
    }
  }
  return idx;
}


void TrackInfo::computeTotalEdepInSFGDbyChilds(
  double &edep_in_SFGD,
  double &sum_Ek0_of_gammas_left_SFGD,
  double &sum_Ek0_of_elepos_left_SFGD,
  double &sum_Ek0_of_neutrons_left_SFGD,
  double &sum_Ek0_of_other_left_SFGD,
  std::map<int, TrackInfo>& tracks)
{
  std::cout << " ~~ " << ID << " " << ParentID << " " << PDG << " " << edep_in_SFGD << std::endl;
  // for (auto cid: daughter_tracks_ids)
  // {
  //   TrackInfo ctr = tracks.at(cid);
  //   ctr.computeTotalEdepInSFGDbyChilds(
  //     edep_in_SFGD,
  //     sum_Ek0_of_gammas_left_SFGD,
  //     sum_Ek0_of_elepos_left_SFGD,
  //     sum_Ek0_of_neutrons_left_SFGD,
  //     sum_Ek0_of_other_left_SFGD,
  //     tracks
  //   );
  // }

  // if (!startedInSFGD && !stoppedInSFGD) return;

  edep_in_SFGD += dE_over_pts_in_sfgd;

  if (left_SFGD_with_Ek != -1.0)
  {
    if (startedInSFGD && stoppedInSFGD) edep_in_SFGD = -99999999999.0;

    switch (PDG)
    {
      case 11:
      {
        sum_Ek0_of_elepos_left_SFGD += left_SFGD_with_Ek;
        break;
      }
      case -11:
      {
        sum_Ek0_of_elepos_left_SFGD += left_SFGD_with_Ek;
        break;
      }
      case 22:
      {
        sum_Ek0_of_gammas_left_SFGD += left_SFGD_with_Ek;
        break;
      }
      case 2112:
      {
        sum_Ek0_of_neutrons_left_SFGD += left_SFGD_with_Ek;
        break;
      }
      default:
      {
        sum_Ek0_of_other_left_SFGD += left_SFGD_with_Ek;
        break;
      }
    }

    // return;
  }

  for (auto cid: daughter_tracks_ids)
  {
    TrackInfo ctr = tracks.at(cid);
    if (!ctr.startedInSFGD && !ctr.stoppedInSFGD) continue;
    ctr.computeTotalEdepInSFGDbyChilds(
      edep_in_SFGD,
      sum_Ek0_of_gammas_left_SFGD,
      sum_Ek0_of_elepos_left_SFGD,
      sum_Ek0_of_neutrons_left_SFGD,
      sum_Ek0_of_other_left_SFGD,
      tracks
    );
  }
}

void TrackInfo::computeTotalEdepInSFGDbyChilds_v2(
  double &edep_in_SFGD,
  double &sum_Ek0_of_gammas_left_SFGD,
  double &sum_Ek0_of_elepos_left_SFGD,
  double &sum_Ek0_of_neutrons_left_SFGD,
  double &sum_Ek0_of_other_left_SFGD,
  std::map<int, TrackInfo>& tracks)
{
  std::cout << " ~~ " << ID << " " << ParentID << " " << PDG << " " << edep_in_SFGD << std::endl;

  edep_in_SFGD += dE_over_pts_in_sfgd;

  for (auto cid: daughter_tracks_ids)
  {
    TrackInfo ctr = tracks.at(cid);
    
    if (!ctr.startedInSFGD && !ctr.stoppedInSFGD) continue;
    if (ctr.left_SFGD_with_Ek != -1.0)
    {
      switch (ctr.PDG)
      {
        case 11:
        {
          sum_Ek0_of_elepos_left_SFGD += ctr.left_SFGD_with_Ek;
          break;
        }
        case -11:
        {
          sum_Ek0_of_elepos_left_SFGD += ctr.left_SFGD_with_Ek;
          break;
        }
        case 22:
        {
          sum_Ek0_of_gammas_left_SFGD += ctr.left_SFGD_with_Ek;
          break;
        }
        case 2112:
        {
          sum_Ek0_of_neutrons_left_SFGD += ctr.left_SFGD_with_Ek;
          break;
        }
        default:
        {
          sum_Ek0_of_other_left_SFGD += ctr.left_SFGD_with_Ek;
          break;
        }
      }
      continue;
    }

    ctr.computeTotalEdepInSFGDbyChilds(
      edep_in_SFGD,
      sum_Ek0_of_gammas_left_SFGD,
      sum_Ek0_of_elepos_left_SFGD,
      sum_Ek0_of_neutrons_left_SFGD,
      sum_Ek0_of_other_left_SFGD,
      tracks
    );
  }
}

void TrackInfo::processChild_test(std::map<int, TrackInfo>& tracks)
{
  std::cout << RED << ">>> P " << RESET << ID << " " << ParentID << " " << PDG << std::endl;
  if (!daughter_tracks_ids.size()) return;

  for (auto cid: daughter_tracks_ids)
  {
    TrackInfo ctr = tracks.at(cid);
    ctr.processChild_test(tracks);
  }
}


void TrackInfo::collectEdepByChilds(std::map<int, TrackInfo>& tracks)
{
  std:cout << "\n ~T: " << ID << " " << PDG << std::endl;
  double edep_in_SFGD = 0.0;
  double sum_Ek0_of_gammas_left_SFGD = 0.0;
  double sum_Ek0_of_elepos_left_SFGD = 0.0;
  double sum_Ek0_of_neutrons_left_SFGD = 0.0;
  double sum_Ek0_of_other_left_SFGD = 0.0;


  computeTotalEdepInSFGDbyChilds(
  // computeTotalEdepInSFGDbyChilds_v2(
    edep_in_SFGD,
    sum_Ek0_of_gammas_left_SFGD,
    sum_Ek0_of_elepos_left_SFGD,
    sum_Ek0_of_neutrons_left_SFGD,
    sum_Ek0_of_other_left_SFGD,
    tracks
  );

  edep_in_SFGD_wchilds = edep_in_SFGD + dE_over_pts_in_sfgd;
  left_SFGD_elepos_sumEk_childs = sum_Ek0_of_elepos_left_SFGD;
  left_SFGD_gamma_sumEk_childs = sum_Ek0_of_gammas_left_SFGD;
  left_SFGD_neutron_sumEk_childs = sum_Ek0_of_neutrons_left_SFGD;
  left_SFGD_other_sumEk_childs = sum_Ek0_of_other_left_SFGD;
}


void TrackInfo::MapPtToChild(int nearest_pt_id_of_parent_trk, int ChildTrackID)
{
  PointInfo pt = points_m.at(nearest_pt_id_of_parent_trk);
  pt.ChildTrackID = ChildTrackID;
  points_m.erase(pt.ID);
  points_m.insert({pt.ID, pt});
}



Track::Track(TND280UpTrack* track)
{
  parent = nullptr;

  ParentID = track->GetParentID();
  PDG = track->GetPDG();
  NPoints = track->GetNPoints();
  ID = track->GetTrackID();
  ProcessName = track->GetProcessName();
  InitKinEnergy = track->GetInitKinEnergy();
  InitCosTheta = track->GetInitCosTheta();
  InitMom = track->GetInitMom();
  double te = compute_total_energy(PDG, InitKinEnergy);
  TotalEnergy = (te != -1)? te : -1.0;

  Len_over_pts_in_sfgd = track->GetLengthInSFGD();
  dE_over_pts_in_sfgd = track->GetEdepInSFGD();

  TVector3 LeftSFGDwithMometum = track->GetLeftSFGDwithMometum();
  
  startedInSFGD = track->GetIsStartedInSFGD();
  stoppedInSFGD = track->GetIsStoppedInSFGD();

  type = TrackType::Undefined;

  // left_SFGD_with_Ek = compute_energy_from_momentum(PDG, LeftSFGDwithMometum.Mag());

  NearestPtIDofParent = -1;
  GParentID = -1;
  Level = -1;
  left_SFGD_plane = "-";
  root_track_id = -999;
  sum_Ek0_of_childs = -999.0;
  sum_Ek0_of_childs_dE = -999.0;
  sum_Ek0_of_childs_dE_Ekleft = -999.0;
  left_SFGD_with_Ek_wchilds = -999.0;

  edep_in_SFGD_wchilds = -999.0;
  left_SFGD_elepos_sumEk_childs = -999.0;
  left_SFGD_gamma_sumEk_childs = -999.0;
  left_SFGD_neutron_sumEk_childs = -999.0;
  left_SFGD_other_sumEk_childs = -999.0;

  dE_in_reflector = -999.0;
  dE_in_fiber = -999.0;
  dE_in_scintillator = -999.0;

  // EdepSumAtPt = -999.0;

  // startedInSFGD = false;
  // stoppedInSFGD = false;

  double _sum_Ek0_of_childs = 0.0;
  double _EdepSumAtPt = 0.0;
  for (int ipt = 0; ipt < NPoints; ++ipt)
  {
    TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
    TrackPoint * pt = new TrackPoint(trackPoint, track);
    pt->SetKinEnergyFromMomentum(PDG);
    _EdepSumAtPt += pt->Edep;
    pt->IncreasingEdep = _EdepSumAtPt;
    points_m.insert({pt->ID, pt});

    if ((pt->created_tracks).size())
    {
      points_related_to_childs_ptrs.push_back(pt);
      if (
          pt->PostProcessName == "annihil"
          || pt->PostProcessName == "Decay"
        ) continue;

      // PreProcessName
      // PostProcessName
      for (auto ctrit: pt->created_tracks)
      {
        _sum_Ek0_of_childs += std::get<1>(ctrit);
      }
    }

    if (ipt > 0)
    {
      TND280UpTrackPoint* prev_trackPoint = track->GetPoint(ipt-1);
      TrackPoint * prev_pt = new TrackPoint(prev_trackPoint, track);
      prev_pt->SetKinEnergyFromMomentum(PDG);
      if (!pt->IsIntoSFGD && prev_pt->IsIntoSFGD)
      {
        left_SFGD_with_Ek = prev_pt->Ek;
      }
      // delete prev_pt;
    }

    sum_Ek0_of_childs = _sum_Ek0_of_childs;
  }

  // left_SFGD_with_Ek = -1.0;
  // if (startedInSFGD && !stoppedInSFGD)
  // {
  //   for (int i = NPoints - 1; i >= 1; i--)
  //   {
  //     PointInfo pt = points[i];
  //     PointInfo prev_pt = points[i-1];

  //     if (!pt.IsIntoSFGD && prev_pt.IsIntoSFGD)
  //     {
  //       left_SFGD_with_Ek = prev_pt.Ek;
  //       TVector3 avgpos = pt.PostPosition;
  //       left_SFGD_plane = getDetectorPlaneOfPoint(avgpos);
  //       break;
  //     }
  //   }
  // }

  
}

Track::~Track()
{
  parent = nullptr;

  type = TrackType::Undefined;

  NearestPtIDofParent = -1;
  GParentID = -1;
  Level = -1;
  left_SFGD_plane = "-";
  root_track_id = -999;
  sum_Ek0_of_childs = -999.0;
  sum_Ek0_of_childs_dE = -999.0;
  sum_Ek0_of_childs_dE_Ekleft = -999.0;
  left_SFGD_with_Ek_wchilds = -999.0;

  edep_in_SFGD_wchilds = -999.0;
  left_SFGD_elepos_sumEk_childs = -999.0;
  left_SFGD_gamma_sumEk_childs = -999.0;
  left_SFGD_neutron_sumEk_childs = -999.0;
  left_SFGD_other_sumEk_childs = -999.0;

  dE_in_reflector = -999.0;
  dE_in_fiber = -999.0;
  dE_in_scintillator = -999.0;

  startedInSFGD = false;
  stoppedInSFGD = false;
}

void Track::print(std::string indent, bool with_points)
{
  std::stringstream parent_tracks_ids_ss;
  parent_tracks_ids_ss << "";
  if (parent_tracks_ids.size())
  {
    for (auto tid: parent_tracks_ids)
    {
      parent_tracks_ids_ss << tid << ",";
    }
  }

  // std::stringstream child_tracks_ids_ss_0;
  // child_tracks_ids_ss_0 << "";
  // if (daughter_tracks_ids.size())
  // {
  //   for (auto tid: daughter_tracks_ids)
  //   {
  //     child_tracks_ids_ss_0 << tid << ",";
  //   }
  // }

  std::stringstream child_tracks_ids_ss;
  child_tracks_ids_ss << "";
  if (childs.size())
  {
    for (auto ctrk: childs)
    {
      child_tracks_ids_ss << ctrk->ID << ",";
    }
  }


  

  double sum_ = (
            edep_in_SFGD_wchilds
            + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs
            + left_SFGD_neutron_sumEk_childs + left_SFGD_other_sumEk_childs
          );

  std::cout 
    // << "\n\n\n"
    << indent << "== "
    // << " TID:" << std::setw(4) << ID
    // << " PID:" << std::setw(4) << ParentID
    // << " GPID:" << std::setw(4) << GParentID
    << BOLDGREEN << " (TPG:" << std::setw(4) << ID << ":" << std::setw(4) << ParentID << ":" << std::setw(4) << GParentID << ")" << RESET
    << " " << BOLDCYAN << NearestPtIDofParent << RESET
    // << " L:" << std::setw(4) << Level
    << " PDG:" << std::setw(11) << BOLDYELLOW << PDG << RESET
    << " Ek0:" << std::setw(11) << InitKinEnergy
    << " Etot:" << std::setw(11) << TotalEnergy
    << " cosT0:" << std::setw(11) << InitCosTheta
    << " Npt:" << std::setw(5) << NPoints
    // << " [-> " << startedInSFGD << " . " << stoppedInSFGD << "] "
    << " [" << startedInSFGD << stoppedInSFGD << "] "
    << " dE_pts: " << std::setw(11) << dE_over_pts_in_sfgd
    // << " dE_pts_all: " << std::setw(11) << dE_over_all_pts
    << " lEk: " << std::setw(11) << left_SFGD_with_Ek
    << " Pl:" << left_SFGD_plane
    << " LR:" << left_and_returned_to_SFGD
    << " T:" << (int)type
    << " [" << StartPos.X() << "|" << StartPos.Y() << "|" << StartPos.Z() << "]"
    << " Proc:" << ProcessName
    << " RT:" << root_track_id
    << "\n" << indent << "\t Parents: " << parent_tracks_ids_ss.str()
        << " Childs: " << child_tracks_ids_ss.str()
        << " Childs sumEk0: " << sum_Ek0_of_childs
        << " Childs sumEk0+dEpts: " << sum_Ek0_of_childs + dE_over_pts_in_sfgd
        // << " Childs sumEk0+dEpts: " << sum_Ek0_of_childs_dE
        // << " Childs sumEk0+dEpts+Ekleft: " << sum_Ek0_of_childs_dE_Ekleft
    // << "\n\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
    //     << " left_SFGD_with_Ek_wchilds: " << left_SFGD_with_Ek_wchilds

    << BOLDMAGENTA << "\n" << indent << "\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
        << RED << " left_SFGD_elepos: " << left_SFGD_elepos_sumEk_childs
        << GREEN <<" left_SFGD_gamma: " << left_SFGD_gamma_sumEk_childs
        // << BOLDBLUE <<" left_SFGD_neutron: " << left_SFGD_neutron_sumEk_childs
        << YELLOW <<" left_SFGD_other: " << left_SFGD_other_sumEk_childs
        // << BOLDCYAN <<" sum: " << sum_
        // << BOLDRED << " diff: " << sum_ - InitKinEnergy
        << RESET
  << std::endl;
  std::cout << indent << "   P0: [" << InitMom.X() << " " << InitMom.Y() << " " << InitMom.Z() << "]" << std::endl;

  // for (auto pt_chld: point_child_m)
  // {
  //   std::string indent_new_pt = "     " + indent;
  //   std::string indent_new = "       " + indent;

  //   TrackPoint * pt = pt_chld.first;
  //   std::cout << indent_new_pt << "Point " << pt->ID << ":" << pt->created_tracks_ptrs.size() << std::endl;

  //   std::vector<Track*> chld_trks = pt_chld.second;
  //   for (auto trk: chld_trks)
  //   {
  //     trk->print(indent_new, false);
  //   }
  // }

  if (with_points)
  { 
    for (auto pt: points_m)
    {
      pt.second->print();
    }
  }
}


void Track::CollectEdepInSFGD(double& EdepInSFGD, double& ElePosLeftSFGD, double& GammaLeftSFGD, double& OthersLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;
    if ( (pt->ID == NPoints - 1) && (
          pt->PostProcessName == "annihil"
          || pt->PostProcessName == "Decay"
        )) continue;

    std::vector<Track*> chld_trks = pt_chld.second;
    for (auto trk: chld_trks)
    {
      trk->CollectEdepInSFGD(EdepInSFGD, ElePosLeftSFGD, GammaLeftSFGD, OthersLeftSFGD);
    }
  }

  EdepInSFGD += dE_over_pts_in_sfgd;
  if (left_SFGD_with_Ek != -1.0)
  {
    if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
    else if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
    else OthersLeftSFGD += left_SFGD_with_Ek;
  }
}

