

#include "Track.hh"


Track::Track(TND280UpTrack* track)
{
  /// losses
  IonizationLoss = 0.0;
  UntrackedParticlesSumEkLoss = 0.0;
  EkOutsideSFGDLoss = 0.0;
  GammaConversionLoss = 0.0;
  ElePosAnnihilationLoss = 0.0;
  SumEkOfChilds_PreLoss = 0.0;
  IonizationLoss_WithChilds = 0.0;
  UntrackedParticlesSumEkLoss_WithChilds = 0.0;
  EkOutsideSFGDLoss_WithChilds = 0.0;
  EkOutsideSFGDLoss_WithChilds_SplitByElePos = 0.0;
  EkOutsideSFGDLoss_WithChilds_SplitByGamma = 0.0;
  EkOutsideSFGDLoss_WithChilds_SplitByNeutron = 0.0;
  EkOutsideSFGDLoss_WithChilds_SplitByOthers = 0.0;
  GammaConversionLoss_WithChilds = 0.0;
  ElePosAnnihilationLoss_WithChilds = 0.0;
  ControlSumOverEkLossComponents = 0.0;

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

  left_SFGD_plane = "-";

  // startedInSFGD = track->GetIsStartedInSFGD();
  // stoppedInSFGD = track->GetIsStoppedInSFGD();
  startedInSFGD = false;
  stoppedInSFGD = false;

  TrackPoint * prev_pt = nullptr;
  double _EdepSumAtPt = 0.0;
  double _EdepSumAtPtInSFGDFV = 0.0;
  double _left_SFGD_with_ene = 0.0;
  double _dEkOutsideSFGD = 0.0;
  bool isTrackIntoSFGD_atPt = false;
  for (int ipt = 0; ipt < NPoints; ++ipt)
  {
    TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
    TrackPoint * pt = new TrackPoint(trackPoint, track);
    pt->SetKinEnergyFromMomentum(PDG);
    _EdepSumAtPt += pt->Edep;
    // if (isPtInSFGDFV(toSFGDCoordinateSystem(pt->AvgPosition))) _EdepSumAtPtInSFGDFV += pt->Edep;
    if (pt->IsIntoSFGDFV) _EdepSumAtPtInSFGDFV += pt->Edep;
    pt->IncreasingEdep = _EdepSumAtPt;
    pt->idx = ipt;
    points_m.insert({pt->ID, pt});

    // delete trackPoint; // causes extremly high memory loss. Why?
    // trackPoint = nullptr;

    if (ipt == 0) pt->IsFirstPtOfTrack = true;
    if (ipt == NPoints - 1) pt->IsLastPtOfTrack = true;

    if (ipt == 0 && pt->IsIntoSFGD) {startedInSFGD = true; isTrackIntoSFGD_atPt = true; pt->IsFirstPtOfTrack_InSFGD = true;}
    else if (ipt == NPoints - 1 && pt->IsIntoSFGD) {stoppedInSFGD = true; pt->IsLastPtOfTrack_InSFGD = true;}

    if (pt->IsIntoSFGD) IonizationLoss += pt->Edep; // !!!LOSSES!!!

    if (ipt == NPoints - 1 && pt->PostProcessName == "conv" && PDG == 22 && pt->IsIntoSFGD)
    {
      GammaConversionLoss += 2.0 * ELECTRON_MASS; // !!!LOSSES!!!
    }

    if (prev_pt)
    {
      if (!pt->IsIntoSFGD && prev_pt->IsIntoSFGD) // left SFGD
      {
        _left_SFGD_with_ene = prev_pt->Ek;
        // _left_SFGD_with_ene = pt->Ek;
        left_SFGD_with_Ek = prev_pt->Ek;
        // left_SFGD_with_Ek = pt->Ek;
      }
      else if (pt->IsIntoSFGD && !prev_pt->IsIntoSFGD && _left_SFGD_with_ene > 0.0) // returned to SFGD
      {
        _dEkOutsideSFGD += (_left_SFGD_with_ene - pt->Ek);
        // _dEkOutsideSFGD += (_left_SFGD_with_ene - prev_pt->Ek);
        _left_SFGD_with_ene = 0.0;
      }
    }

    // if (isTrackIntoSFGD_atPt && !pt->IsIntoSFGD) // left SFGD
    // {
    // }

    /// if point has related child tracks
    if ((pt->created_tracks).size())
    {
      points_related_to_childs_ptrs.push_back(pt);
    }

    prev_pt = pt;
  }

  dE_over_pts_in_sfgd_fv = _EdepSumAtPtInSFGDFV;
  IonizationLoss_InFV = _EdepSumAtPtInSFGDFV;

  // if (prev_pt) { delete prev_pt; prev_pt = nullptr; }

  // int ipt = 0;
  // for (auto pt_item: points_m)
  // {

  // }

  if (_left_SFGD_with_ene > 0.0) _dEkOutsideSFGD += _left_SFGD_with_ene;
  EkOutsideSFGDLoss = _dEkOutsideSFGD;
  // if (startedInSFGD && !stoppedInSFGD)
  //   std::cout << "!!!!!!!! " << ID << " " << EkOutsideSFGDLoss << std::endl;
}


Track::~Track()
{
  /// losses
  parent = nullptr;

  // std::map<int, TrackPoint*> points_m;
  for (auto item: points_m)
  {
    if (!item.second) continue;
    delete item.second;
    item.second = nullptr;
  }
  points_m.clear();

  // for (auto item: point_child_m)
  // {
  //   /// problem is also here
  //   /// free(): double free detected in tcache 2
  //   // if (item.first) { delete item.first; }
  //   // auto _ = item.first;
  //   // if (_)
  //   // {
  //   //   delete _;
  //   //   _ = nullptr;
  //   // }

  //   /// problem is somewhere here
  //   /// free(): double free detected in tcache 2
  //   // for (auto nested_item: item.second)
  //   // {
  //   //   if (!nested_item) continue;
  //   //   delete nested_item;
  //   //   nested_item = nullptr;
  //   // }
  //   item.second.clear();
  // }
  point_child_m.clear();

  // for (auto item: childs)
  // {
  //   if (!item) continue;
  //   delete item;
  //   item = nullptr;
  // }
  childs.clear();

  /// problem is also here
  /// free(): double free detected in tcache 2
  // for (auto item: points_related_to_childs_ptrs)
  // {
  //   if (!item) continue;
  //   delete item;
  //   item = nullptr;
  // }
  points_related_to_childs_ptrs.clear();
}


void Track::print(std::string indent, bool with_points, bool my_condition)
{
  // if (my_condition)
  //   // if (abs(total_edep_sum_by_childs_recursive - InitKinEnergy) < 1.0e-3) return;
  //   if (abs(total_edep_sum_by_childs_recursive_wutek - InitKinEnergy) < 1.0e-3) return;
  
  std::string mark;
  if (abs(ControlSumOverEkLossComponents - InitKinEnergy) < 1.0e-3)
  {
    mark = std::string(BOLDGREEN) + "TRUE" + std::string(RESET);
    // return;
  }
  else if (ControlSumOverEkLossComponents < InitKinEnergy)
  {
    mark = std::string(BOLDYELLOW) + "FALSE" + std::string(CYAN) + "<" + std::string(RESET);
  }
  else if (ControlSumOverEkLossComponents > InitKinEnergy)
  {
    mark = std::string(BOLDYELLOW) + "FALSE" + std::string(CYAN) + ">" + std::string(RESET);
  }

  std::string mark2;
  if (abs(ControlSumOverEkLossComponents_WithChilds - InitKinEnergy) < 1.0e-3)
  {
    mark2 = std::string(BOLDGREEN) + "TRUE" + std::string(RESET);
    // return;
  }
  else if (ControlSumOverEkLossComponents_WithChilds < InitKinEnergy)
  {
    mark2 = std::string(BOLDYELLOW) + "FALSE" + std::string(CYAN) + "<" + std::string(RESET);
  }
  else if (ControlSumOverEkLossComponents_WithChilds > InitKinEnergy)
  {
    mark2 = std::string(BOLDYELLOW) + "FALSE" + std::string(CYAN) + ">" + std::string(RESET);
  }

  

  std::cout 
    // << "\n\n\n"
    << indent << "== "
    << BOLDGREEN << " (TPG:" << std::setw(4) << ID << ":" << std::setw(4) << ParentID /*<< ":" << std::setw(4) << GParentID*/ << ")" << RESET
    << " PDG:" << std::setw(11) << BOLDYELLOW << PDG << RESET
    << " Ek0:" << std::setw(11) << InitKinEnergy
    << " Etot:" << std::setw(11) << TotalEnergy
    << " cosT0:" << std::setw(11) << InitCosTheta
    << " Npt:" << std::setw(5) << NPoints
    << " Nchld: " << std::setw(3) << childs.size()
    << " [" << startedInSFGD << stoppedInSFGD << "] "
    // << " (" << leftSFGDatPt << ", " << returnedToSFGDatPt << ")"
    << " dE_pts: " << std::setw(11) << dE_over_pts_in_sfgd
    << " lEk: " << std::setw(11) << left_SFGD_with_Ek
    << " Pl:" << left_SFGD_plane
    // << " LR:" << left_and_returned_to_SFGD
    // << " Nut:" << HasUntrackedParticles
    << " [" << StartPos.X() << "|" << StartPos.Y() << "|" << StartPos.Z() << "]"
    << " Proc:" << ProcessName

    << "\n" << indent
       << "   Losses   : "
          << BOLDRED    << "Ioniz: " << /*CYAN <<*/ std::setw(12) << IonizationLoss << RESET << ", "
          << BOLDGREEN  << "EkOut: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss << RESET << ", "
          << BOLDYELLOW << "UntrP: " << /*CYAN <<*/ std::setw(12) << UntrackedParticlesSumEkLoss << RESET << ", "
          << BOLDCYAN  << "GConv: " << /*CYAN <<*/ std::setw(12) << GammaConversionLoss << RESET << ", "
          << MAGENTA    << "SEk_C: " << /*CYAN <<*/ std::setw(12) << SumEkOfChilds_PreLoss << RESET << ", "
          // << GREEN      << "EPann: " << /*CYAN <<*/ ElePosAnnihilationLoss << RESET << ", "
                        << BOLDRED << "Sum  : " << /*CYAN <<*/ ControlSumOverEkLossComponents
                                                // << RESET        
                        << " / " << InitKinEnergy << RESET << " " << mark << " "
    << "\n" << indent
       << "   LossesWC : "
          << BOLDRED    << "Ioniz: " << /*CYAN <<*/ std::setw(12) << IonizationLoss_WithChilds << RESET << ", "
          << BOLDGREEN  << "EkOut: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss_WithChilds << RESET << ", "
            << BOLDGREEN  << " [e: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss_WithChilds_SplitByElePos << RESET << ", "
            << BOLDGREEN  << " g: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss_WithChilds_SplitByGamma << RESET << ", "
            // << BOLDGREEN  << " n: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss_WithChilds_SplitByNeutron << RESET << ", "
            << BOLDGREEN  << " o: " << /*CYAN <<*/ std::setw(12) << EkOutsideSFGDLoss_WithChilds_SplitByOthers << RESET << "] "
          << BOLDYELLOW << "UntrP: " << /*CYAN <<*/ std::setw(12) << UntrackedParticlesSumEkLoss_WithChilds << RESET << ", "
          << BOLDCYAN  << "GConv: " << /*CYAN <<*/ std::setw(12) << GammaConversionLoss_WithChilds << RESET << ", "
                        << BOLDRED << "Sum  : " << /*CYAN <<*/ ControlSumOverEkLossComponents_WithChilds
                        << " / " << InitKinEnergy << RESET << " " << mark2 << " "
  << std::endl;

  if (with_points)
  { 
    for (auto pt: points_m)
    {
      if (!pt.second->created_tracks.size()) continue;
      pt.second->print();
    }
  }
}


void Track::SetControl()
{
  // total_edep_sum_by_childs_recursive = edep_in_SFGD_wchilds
  //   + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs + left_SFGD_other_sumEk_childs;
  // total_edep_sum_by_childs_recursive_wutek = total_edep_sum_by_childs_recursive + UntrackedEk0Loss_wchilds
  //   + dEkOutSFGD_wchilds
  //   + UntrackedEk0Loss_gamma;
  // total_edep_sum_by_childs_ekin = dE_over_pts_in_sfgd + left_SFGD_with_Ek + sum_Ek0_of_childs + UntrackedEk0Loss_wchilds
  //   + dEkOutSFGD
  //   + UntrackedEk0Loss_gamma;

  ControlSumOverEkLossComponents = IonizationLoss
    + EkOutsideSFGDLoss
    + UntrackedParticlesSumEkLoss
    + GammaConversionLoss
    + SumEkOfChilds_PreLoss
    // + ElePosAnnihilationLoss
  ;
  
  ControlSumOverEkLossComponents_WithChilds = IonizationLoss_WithChilds
    + EkOutsideSFGDLoss_WithChilds_SplitByElePos
    + EkOutsideSFGDLoss_WithChilds_SplitByGamma
    + EkOutsideSFGDLoss_WithChilds_SplitByOthers
    + UntrackedParticlesSumEkLoss_WithChilds
    + GammaConversionLoss_WithChilds
  ;
}

///
// -11: problem in annihil -> total collected edep larger by 1.1 MeV (1 mass of electrons)
void Track::CollectEdepInSFGDbyChilds(double& EdepInSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    // if ( (pt->idx == NPoints - 1) && !pt->IsIntoSFGD ) continue;
    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD ) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectEdepInSFGDbyChilds(EdepInSFGD);
    }
  }

  EdepInSFGD += IonizationLoss;
}

void Track::CollectEdepInSFGDbyChilds_InFV(double& EdepInSFGD_InFV)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    // if ( (pt->idx == NPoints - 1) && !pt->IsIntoSFGD ) continue;
    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD ) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectEdepInSFGDbyChilds_InFV(EdepInSFGD_InFV);
    }
  }

  EdepInSFGD_InFV += IonizationLoss_InFV;
}

void Track::CollectUntrackedEkInSFGDByChilds(double& UntrackedEkLoss)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD ) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() && !pt->IsIntoSFGD ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectUntrackedEkInSFGDByChilds(UntrackedEkLoss);
    }
  }

  UntrackedEkLoss += UntrackedParticlesSumEkLoss;
}

void Track::CollectLeftSFGDeleposByChilds(double& ElePosLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDeleposByChilds(ElePosLeftSFGD);
    }
  }

  if (abs(PDG) == 11) ElePosLeftSFGD += EkOutsideSFGDLoss;
}


void Track::CollectLeftSFGDgammaByChilds(double& GammaLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDgammaByChilds(GammaLeftSFGD);
    }
  }

  if (PDG == 22) GammaLeftSFGD += EkOutsideSFGDLoss;
}

void Track::CollectLeftSFGDothersByChilds(double& OthersLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( !pt->IsIntoSFGD ) continue;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() && !pt->IsIntoSFGD ) continue;
    // if ( (pt->idx == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (abs(PDG) == 13) && (pt->PostProcessName == "Decay") && chld_trks.size() ) continue;
    if ( (pt->idx == NPoints - 1) && (PDG == 13) && (pt->PostProcessName == "muMinusCaptureAtRest") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDothersByChilds(OthersLeftSFGD);
    }
  }

  if (!(abs(PDG) == 11 || PDG == 22)) OthersLeftSFGD += EkOutsideSFGDLoss;
}


/*
void Track::CollectEdepInSFGD(
  double& EdepInSFGD, double& ElePosLeftSFGD, double& GammaLeftSFGD, double& OthersLeftSFGD, double& UntrackedEkLoss, double &dEkOutSFGD_wchilds)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectEdepInSFGD(EdepInSFGD, ElePosLeftSFGD, GammaLeftSFGD, OthersLeftSFGD, UntrackedEkLoss, dEkOutSFGD_wchilds);
    }
  }

  EdepInSFGD += dE_over_pts_in_sfgd;
  if (left_SFGD_with_Ek != -1.0)
  {
    if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
    else if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
    else OthersLeftSFGD += left_SFGD_with_Ek;
  }

  UntrackedEkLoss += UntrackedEk0Loss;
  dEkOutSFGD_wchilds =+ dEkOutSFGD;
}


void Track::CollectEdepInSFGDbyChilds(double& EdepInSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectEdepInSFGDbyChilds(EdepInSFGD);
    }
  }

  EdepInSFGD += dE_over_pts_in_sfgd;
}

void Track::CollectLeftSFGDeleposByChilds(double& ElePosLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDeleposByChilds(ElePosLeftSFGD);
    }
  }

  if (left_SFGD_with_Ek != -1.0)
  {
    if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectLeftSFGDgammaByChilds(double& GammaLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDgammaByChilds(GammaLeftSFGD);
    }
  }

  if (left_SFGD_with_Ek != -1.0)
  {
    if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectLeftSFGDothersByChilds(double& OthersLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDothersByChilds(OthersLeftSFGD);
    }
  }

  if (left_SFGD_with_Ek != -1.0)
  {
    if (abs(PDG) == 11 || PDG == 22) {  }
    else OthersLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectUntrackedEkInSFGDByChilds(double& UntrackedEkLoss)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectUntrackedEkInSFGDByChilds(UntrackedEkLoss);
    }
  }

  UntrackedEkLoss += UntrackedEk0Loss;
}

void Track::CollectEdepOdReturnedTrksToSFGDByChilds(double& dEkOutSFGD_wchilds)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectEdepOdReturnedTrksToSFGDByChilds(dEkOutSFGD_wchilds);
    }
  }

  dEkOutSFGD_wchilds =+ dEkOutSFGD;
}


void Track::CollectUntrackedEkInSFGDByChildsForGamma(double& UntrackedEkLoss_gamma)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "conv") && chld_trks.size() )
    {
      UntrackedEkLoss_gamma += 2.0 * ELECTRON_MASS;
      continue;
    }

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectUntrackedEkInSFGDByChildsForGamma(UntrackedEkLoss_gamma);
    }
  }
}

void Track::SetControl()
{
  total_edep_sum_by_childs_recursive = edep_in_SFGD_wchilds
    + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs + left_SFGD_other_sumEk_childs;
  total_edep_sum_by_childs_recursive_wutek = total_edep_sum_by_childs_recursive + UntrackedEk0Loss_wchilds
    + dEkOutSFGD_wchilds
    + UntrackedEk0Loss_gamma;
  total_edep_sum_by_childs_ekin = dE_over_pts_in_sfgd + left_SFGD_with_Ek + sum_Ek0_of_childs + UntrackedEk0Loss_wchilds
    + dEkOutSFGD
    + UntrackedEk0Loss_gamma;
}
// */