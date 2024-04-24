

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
  GammaConversionLoss_WithChilds = 0.0;
  ElePosAnnihilationLoss_WithChilds = 0.0;

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

  // LeftSFGDwithMometum = track->GetLeftSFGDwithMometum();
  
  // startedInSFGD = track->GetIsStartedInSFGD();
  // stoppedInSFGD = track->GetIsStoppedInSFGD();

  // type = TrackType::Undefined;

  // left_SFGD_with_Ek = compute_energy_from_momentum(PDG, LeftSFGDwithMometum.Mag());

  // NearestPtIDofParent = -1;
  // GParentID = -1;
  // Level = -1;
  left_SFGD_plane = "-";
  root_track_id = -999;
  sum_Ek0_of_childs = -999.0;
  // sum_Ek0_of_childs_dE = -999.0;
  // sum_Ek0_of_childs_dE_Ekleft = -999.0;
  // left_SFGD_with_Ek_wchilds = -999.0;

  edep_in_SFGD_wchilds = -999.0;
  left_SFGD_elepos_sumEk_childs = -999.0;
  left_SFGD_gamma_sumEk_childs = -999.0;
  // left_SFGD_neutron_sumEk_childs = -999.0;
  left_SFGD_other_sumEk_childs = -999.0;

  // dE_in_reflector = -999.0;
  // dE_in_fiber = -999.0;
  // dE_in_scintillator = -999.0;

  // EdepSumAtPt = -999.0;

  startedInSFGD = false;
  stoppedInSFGD = false;

  leftSFGDatPt = -1;
  returnedToSFGDatPt = -1;

  UntrackedEk0Loss = 0.0;
  UntrackedEk0Loss_wchilds = 0.0;
  UntrackedEk0Loss_gamma = 0.0;

  ReturnedToSFGDwithEk = 0.0;
  dEkOutSFGD = 0.0;
  dEkOutSFGD_wchilds = 0.0;

  /// TODO: handle tracks that left SFGD and returned to it several times
  // left_SFGD_with_Ek = -1.0;
  left_SFGD_with_Ek = 0.0;
  double _sum_Ek0_of_childs = 0.0;
  double _EdepSumAtPt = 0.0;
  int prev_pt_in_SFGD = -1;
  int has_untracked_particles = 0;

  double _dEkOutsideSFGD = 0.0;
  double _left_SFGD_with_ene = 0.0;

  TrackPoint * prev_pt_2 = nullptr;

  for (int ipt = 0; ipt < NPoints; ++ipt)
  {
    TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
    TrackPoint * pt = new TrackPoint(trackPoint, track);
    pt->SetKinEnergyFromMomentum(PDG);
    _EdepSumAtPt += pt->Edep;
    IonizationLoss += pt->Edep; // !!!LOSSES!!!
    pt->IncreasingEdep = _EdepSumAtPt;
    points_m.insert({pt->ID, pt});

    if (ipt == 0 && pt->IsIntoSFGD) startedInSFGD = true;
    else if (ipt == NPoints - 1 && pt->IsIntoSFGD) stoppedInSFGD = true;

    /// !!!LOSSES!!!
    {
      if (prev_pt_2)
      {
        if (!pt->IsIntoSFGD && prev_pt_2->IsIntoSFGD) // left SFGD
        {
          _left_SFGD_with_ene = prev_pt_2->Ek;
          // _left_SFGD_with_ene = pt->Ek;
        }

        else if (pt->IsIntoSFGD && !prev_pt_2->IsIntoSFGD && _left_SFGD_with_ene > 0.0)
        {
          // _dEkOutsideSFGD += (_left_SFGD_with_ene - pt->Ek);
          _dEkOutsideSFGD += (_left_SFGD_with_ene - prev_pt_2->Ek);
          _left_SFGD_with_ene = 0.0;
        }
      }

      // if (ipt == NPoints - 1 && pt->PostProcessName == "annihil" && abs(PDG) == 11)
      // {
      //   ElePosAnnihilationLoss -= 2.0 * ELECTRON_MASS;
      // }
      if (ipt == NPoints - 1 && pt->PostProcessName == "conv" && PDG == 22)
      {
        GammaConversionLoss += 2.0 * ELECTRON_MASS;
      }

      prev_pt_2 = pt;

      // 
      if ((pt->created_tracks).size())
      {
        // if (!(ipt == NPoints - 1 && pt->PostProcessName == "annihil" && abs(PDG) == 11))
        {        
          for (auto ctrit: pt->created_tracks)
          {
            int status = std::get<10>(ctrit);
            double InitialKineticEnergy_ = std::get<2>(ctrit);
            if (status == 0)
            {
              UntrackedParticlesSumEkLoss += InitialKineticEnergy_;
            } else {
              // SumEkOfChilds_PreLoss += InitialKineticEnergy_;
            }
          }
        }

        if (pt->PostProcessName == "annihil" && abs(PDG) == 11)
        {
          SumEkOfChilds_PreLoss -= 2.0 * ELECTRON_MASS;
        }
      }
    }
    ///~ !!!LOSSES!!!


    if ((pt->created_tracks).size())
    {
      for (auto ctrit: pt->created_tracks)
      {
        int status = std::get<10>(ctrit);
        double InitialKineticEnergy_ = std::get<2>(ctrit);
        if (status == 0)
        {
          has_untracked_particles++;
          // UntrackedParticlesSumEkLoss += InitialKineticEnergy_;
        }
      }

      // if (pt->PostProcessName == "annihil" && abs(PDG == 11))
      // {
      //   ElePosAnnihilationLoss += 2.0 * ELECTRON_MASS;
      // }

      points_related_to_childs_ptrs.push_back(pt);
      if ( /// Is it needed?
          pt->PostProcessName == "annihil"
          || pt->PostProcessName == "Decay"
        ) continue;



      // PreProcessName
      // PostProcessName

      // for (auto ctrit: pt->created_tracks)
      // {
      //   _sum_Ek0_of_childs += std::get<1>(ctrit);
      // }
    }

    
    
    if (ipt > 0)
    {
      TND280UpTrackPoint* prev_trackPoint = track->GetPoint(ipt-1);
      TrackPoint * prev_pt = new TrackPoint(prev_trackPoint, track);
      prev_pt->SetKinEnergyFromMomentum(PDG);
      if (!pt->IsIntoSFGD && prev_pt->IsIntoSFGD)
      {
        left_SFGD_with_Ek = prev_pt->Ek;
        leftSFGDatPt = prev_pt->ID;
      }
      // delete prev_pt;
    }
    

    // sum_Ek0_of_childs = _sum_Ek0_of_childs;

    if (pt->IsIntoSFGD && prev_pt_in_SFGD == 0) /// returned to SFGD
    {
      ReturnedToSFGDwithEk = pt->Ek;
      returnedToSFGDatPt = pt->ID;
      // if (left_SFGD_with_Ek)
      // dEkOutSFGD = left_SFGD_with_Ek - ReturnedToSFGDwithEk;
      
      // if (_left_SFGD_with_ene > 0.0)
      // {
      //   _dEkOutsideSFGD += (_left_SFGD_with_ene - pt->Ek);
      // }
    }

    prev_pt_in_SFGD = (pt->IsIntoSFGD)? 1 : 0;
    
  }

  /// !!!LOSSES!!!
  if (_left_SFGD_with_ene > 0.0) _dEkOutsideSFGD += _left_SFGD_with_ene;
  EkOutsideSFGDLoss = _dEkOutsideSFGD;
  ///~ !!!LOSSES!!!

  HasUntrackedParticles = has_untracked_particles;

  _left_SFGD_with_Ek = left_SFGD_with_Ek;

  // std::cout << " !!!!!!!!!!!! " << startedInSFGD << " " << stoppedInSFGD << " " << left_SFGD_with_Ek << std::endl;

  // dEkOutSFGD = left_SFGD_with_Ek - ReturnedToSFGDwithEk;

  /// Track left sfgd and then returned to it
  if (startedInSFGD && stoppedInSFGD && _left_SFGD_with_Ek > 0.0)
  {
    // left_SFGD_with_Ek = 0.0;
    // returnedToSFGDwithEk
    // double ElossOutSFGD = left_SFGD_with_Ek - returnedToSFGDwithEk;

    // if (pt->IsIntoSFGD && prev_pt_in_SFGD == 0) /// returned to SFGD
    // {
    //   ReturnedToSFGDwithEk = pt->Ek;
    //   // if (left_SFGD_with_Ek)
    //   dEkOutSFGD = left_SFGD_with_Ek - ReturnedToSFGDwithEk;
    // }

    dEkOutSFGD = _left_SFGD_with_Ek - ReturnedToSFGDwithEk;
    left_SFGD_with_Ek = 0.0;
  }

  if (ReturnedToSFGDwithEk > 0.0 && !startedInSFGD)
  {
    EnteredInSFGDwithEk = ReturnedToSFGDwithEk;
  }

}

Track::~Track()
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
  GammaConversionLoss_WithChilds = 0.0;
  ElePosAnnihilationLoss_WithChilds = 0.0;

  parent = nullptr;

  // type = TrackType::Undefined;

  // NearestPtIDofParent = -1;
  // GParentID = -1;
  // Level = -1;
  left_SFGD_plane = "-";
  root_track_id = -999;
  sum_Ek0_of_childs = -999.0;
  // sum_Ek0_of_childs_dE = -999.0;
  // sum_Ek0_of_childs_dE_Ekleft = -999.0;
  // left_SFGD_with_Ek_wchilds = -999.0;

  edep_in_SFGD_wchilds = -999.0;
  left_SFGD_elepos_sumEk_childs = -999.0;
  left_SFGD_gamma_sumEk_childs = -999.0;
  // left_SFGD_neutron_sumEk_childs = -999.0;
  left_SFGD_other_sumEk_childs = -999.0;

  // dE_in_reflector = -999.0;
  // dE_in_fiber = -999.0;
  // dE_in_scintillator = -999.0;

  // startedInSFGD = false;
  // stoppedInSFGD = false;

  UntrackedEk0Loss = 0.0;
  UntrackedEk0Loss_wchilds = 0.0;

  ReturnedToSFGDwithEk = 0.0;
  dEkOutSFGD = 0.0;
  EnteredInSFGDwithEk = 0.0;

  left_SFGD_with_Ek = 0.0;
  _left_SFGD_with_Ek = left_SFGD_with_Ek;

  dEkOutSFGD_wchilds = 0.0;
  UntrackedEk0Loss_gamma = 0.0;

  leftSFGDatPt = -1;
  returnedToSFGDatPt = -1;

  HasUntrackedParticles = 0;
}


void Track::print(std::string indent, bool with_points, bool my_condition)
{
  // std::stringstream parent_tracks_ids_ss;
  // parent_tracks_ids_ss << "";
  // if (parent_tracks_ids.size())
  // {
  //   for (auto tid: parent_tracks_ids)
  //   {
  //     parent_tracks_ids_ss << tid << ",";
  //   }
  // }

  // std::stringstream child_tracks_ids_ss_0;
  // child_tracks_ids_ss_0 << "";
  // if (daughter_tracks_ids.size())
  // {
  //   for (auto tid: daughter_tracks_ids)
  //   {
  //     child_tracks_ids_ss_0 << tid << ",";
  //   }
  // }

  // std::stringstream child_tracks_ids_ss;
  // child_tracks_ids_ss << "";
  // if (childs.size())
  // {
  //   for (auto ctrk: childs)
  //   {
  //     child_tracks_ids_ss << ctrk->ID << ",";
  //   }
  // }


  

  // double sum_ = (
  //           edep_in_SFGD_wchilds
  //           + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs
  //           + left_SFGD_neutron_sumEk_childs + left_SFGD_other_sumEk_childs
  //         );

  // double total_edep_sum_by_childs_recursive = edep_in_SFGD_wchilds
  //   + left_SFGD_elepos_sumEk_childs + left_SFGD_gamma_sumEk_childs + left_SFGD_other_sumEk_childs;
  // // double total_edep_sum_by_childs_recursive_wutek = total_edep_sum_by_childs_recursive + UntrackedEk0Loss;
  // double total_edep_sum_by_childs_recursive_wutek = total_edep_sum_by_childs_recursive + UntrackedEk0Loss_wchilds;
  // // double total_edep_sum_by_childs_ekin = dE_over_pts_in_sfgd + left_SFGD_with_Ek + sum_Ek0_of_childs + UntrackedEk0Loss;
  // double total_edep_sum_by_childs_ekin = dE_over_pts_in_sfgd + left_SFGD_with_Ek + sum_Ek0_of_childs + UntrackedEk0Loss_wchilds;

  if (my_condition)
    // if (abs(total_edep_sum_by_childs_recursive - InitKinEnergy) < 1.0e-3) return;
    if (abs(total_edep_sum_by_childs_recursive_wutek - InitKinEnergy) < 1.0e-3) return;

  std::cout 
    // << "\n\n\n"
    << indent << "== "
    // << " TID:" << std::setw(4) << ID
    // << " PID:" << std::setw(4) << ParentID
    // << " GPID:" << std::setw(4) << GParentID
    << BOLDGREEN << " (TPG:" << std::setw(4) << ID << ":" << std::setw(4) << ParentID /*<< ":" << std::setw(4) << GParentID*/ << ")" << RESET
    // << " " << BOLDCYAN << NearestPtIDofParent << RESET
    // << " L:" << std::setw(4) << Level
    << " PDG:" << std::setw(11) << BOLDYELLOW << PDG << RESET
    << " Ek0:" << std::setw(11) << InitKinEnergy
    << " Etot:" << std::setw(11) << TotalEnergy
    << " cosT0:" << std::setw(11) << InitCosTheta
    << " Npt:" << std::setw(5) << NPoints
    // << " [-> " << startedInSFGD << " . " << stoppedInSFGD << "] "
    << " [" << startedInSFGD << stoppedInSFGD << "] "
    << " (" << leftSFGDatPt << ", " << returnedToSFGDatPt << ")"
    << " dE_pts: " << std::setw(11) << dE_over_pts_in_sfgd
    // << " dE_pts_all: " << std::setw(11) << dE_over_all_pts
    << " lEk: " << std::setw(11) << left_SFGD_with_Ek
    << " Pl:" << left_SFGD_plane
    // << " LR:" << left_and_returned_to_SFGD
    // << " T:" << (int)type
    << " Nut:" << HasUntrackedParticles
    << " [" << StartPos.X() << "|" << StartPos.Y() << "|" << StartPos.Z() << "]"
    << " Proc:" << ProcessName
    << " RT:" << root_track_id
    << " \n" << indent
       << "   N childs: " << childs.size() << " sumEk0: " << sum_Ek0_of_childs
       << " || sumEk0_chlds+dE+leftEk+utEk = " << total_edep_sum_by_childs_ekin
       << " || sumEk0_chlds+dE+leftEk+utEk+utdEk_g = " << total_edep_sum_by_childs_ekin// + UntrackedEk0Loss_gamma
    << "\n" << indent
       << "   edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
       << " || dEwch+leftEk = " << edep_in_SFGD_wchilds + left_SFGD_with_Ek
       << " || dEwch+e_l+g_l+o_l = " << total_edep_sum_by_childs_recursive
       << " || dEwch+e_l+g_l+o_l+utEk = " << total_edep_sum_by_childs_recursive_wutek
       << " || dEwch+e_l+g_l+o_l+utEk+utdEk_g = " << total_edep_sum_by_childs_recursive_wutek// + UntrackedEk0Loss_gamma
    << "\n" << indent
       << "   Left sfgd: elepos " <<  left_SFGD_elepos_sumEk_childs
                   << ", gammas " << left_SFGD_gamma_sumEk_childs 
                   << ", others " << left_SFGD_other_sumEk_childs
    << "\n" << indent
       << "   Untracked particles Ek loss: " <<  UntrackedEk0Loss
       << ", with childs: " << UntrackedEk0Loss_wchilds
       << ", UntrackedEk0Loss_gamma: " << UntrackedEk0Loss_gamma
    << "\n" << indent
       << "   Returned to SFGD with Ek: " <<  ReturnedToSFGDwithEk
       << ", dEkOutSFGD: " << dEkOutSFGD
       << " (Left SFGD with Ek: " << _left_SFGD_with_Ek << ")"
       << " dEkOutSFGD_wchilds: " << dEkOutSFGD_wchilds
    << "\n" << indent
       << "   Losses   : "
          << BOLDRED    << "Ioniz: " << /*CYAN <<*/ IonizationLoss << RESET << ", "
          << BOLDGREEN  << "EkOut: " << /*CYAN <<*/ EkOutsideSFGDLoss << RESET << ", "
          << BOLDYELLOW << "UntrP: " << /*CYAN <<*/ UntrackedParticlesSumEkLoss << RESET << ", "
          << BOLDWHITE  << "GConv: " << /*CYAN <<*/ GammaConversionLoss << RESET << ", "
          << MAGENTA    << "SEk_C: " << /*CYAN <<*/ SumEkOfChilds_PreLoss << RESET << ", "
          << GREEN      << "EPann: " << /*CYAN <<*/ ElePosAnnihilationLoss << RESET << ", "
                        << "Sum  : " << /*CYAN <<*/ IonizationLoss
                                                + EkOutsideSFGDLoss
                                                + UntrackedParticlesSumEkLoss
                                                + GammaConversionLoss
                                                + SumEkOfChilds_PreLoss
                                                + ElePosAnnihilationLoss << RESET        
                        << " / " << InitKinEnergy
    << "\n" << indent
       << "   LossesWCH: "
          << BOLDRED    << "Ioniz: " << /*CYAN <<*/ IonizationLoss_WithChilds << RESET << ", "
          << BOLDGREEN  << "EkOut: " << /*CYAN <<*/ EkOutsideSFGDLoss_WithChilds << RESET << ", "
          << BOLDYELLOW << "UntrP: " << /*CYAN <<*/ UntrackedParticlesSumEkLoss_WithChilds << RESET << ", "
          << BOLDWHITE  << "GConv: " << /*CYAN <<*/ GammaConversionLoss_WithChilds << RESET << ", "
          << GREEN      << "EPann: " << /*CYAN <<*/ ElePosAnnihilationLoss_WithChilds << RESET << ", "
                        << "Sum  : " << /*CYAN <<*/ IonizationLoss_WithChilds
                                                + EkOutsideSFGDLoss_WithChilds
                                                + UntrackedParticlesSumEkLoss_WithChilds
                                                + GammaConversionLoss_WithChilds
                                                + ElePosAnnihilationLoss_WithChilds << RESET        
                        << " / " << InitKinEnergy

    // << "\n" << indent << "\t Parents: " << parent_tracks_ids_ss.str()
    //     << " Childs: " << child_tracks_ids_ss.str()
    //     << " Childs sumEk0: " << sum_Ek0_of_childs
    //     << " Childs sumEk0+dEpts: " << sum_Ek0_of_childs + dE_over_pts_in_sfgd
    //     // << " Childs sumEk0+dEpts: " << sum_Ek0_of_childs_dE
    //     // << " Childs sumEk0+dEpts+Ekleft: " << sum_Ek0_of_childs_dE_Ekleft
    // << "\n\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
    //     << " left_SFGD_with_Ek_wchilds: " << left_SFGD_with_Ek_wchilds

    // << BOLDMAGENTA << "\n" << indent << "\t edep_in_SFGD_wchilds: " << edep_in_SFGD_wchilds
    //     << RED << " left_SFGD_elepos: " << left_SFGD_elepos_sumEk_childs
    //     << GREEN <<" left_SFGD_gamma: " << left_SFGD_gamma_sumEk_childs
    //     // << BOLDBLUE <<" left_SFGD_neutron: " << left_SFGD_neutron_sumEk_childs
    //     << YELLOW <<" left_SFGD_other: " << left_SFGD_other_sumEk_childs
    //     // << BOLDCYAN <<" sum: " << sum_
    //     // << BOLDRED << " diff: " << sum_ - InitKinEnergy
    //    << RESET
  << std::endl;
  std::cout << indent << "   P0: [" << InitMom.X() << " " << InitMom.Y() << " " << InitMom.Z() << "]" << std::endl;
  std::cout << indent << "   P0: [" << LeftSFGDwithMometum.X() << " " << LeftSFGDwithMometum.Y() << " " << LeftSFGDwithMometum.Z() << "]" << std::endl;

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
      if (!pt.second->created_tracks.size()) continue;
      pt.second->print();
    }
  }
}


void Track::CollectEdepInSFGD(
  double& EdepInSFGD, double& ElePosLeftSFGD, double& GammaLeftSFGD, double& OthersLeftSFGD, double& UntrackedEkLoss, double &dEkOutSFGD_wchilds)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

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
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

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
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDeleposByChilds(ElePosLeftSFGD);
    }
  }

  if (left_SFGD_with_Ek != -1.0)
  {
    if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
    // else if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
    // else OthersLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectLeftSFGDgammaByChilds(double& GammaLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDgammaByChilds(GammaLeftSFGD);
    }
  }

  if (left_SFGD_with_Ek != -1.0)
  {
    // if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
    if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
    // else OthersLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectLeftSFGDothersByChilds(double& OthersLeftSFGD)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectLeftSFGDothersByChilds(OthersLeftSFGD);
    }
  }

  // EdepInSFGD += dE_over_pts_in_sfgd;
  if (left_SFGD_with_Ek != -1.0)
  {
    // if (abs(PDG) == 11) ElePosLeftSFGD += left_SFGD_with_Ek;
    // if (PDG == 22) GammaLeftSFGD += left_SFGD_with_Ek;
    if (abs(PDG) == 11 || PDG == 22) {  }
    else OthersLeftSFGD += left_SFGD_with_Ek;
  }
}

void Track::CollectUntrackedEkInSFGDByChilds(double& UntrackedEkLoss)
{
  for (auto pt_chld: point_child_m)
  {
    TrackPoint * pt = pt_chld.first;
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "conv") && chld_trks.size() )
    // {
    //   UntrackedEkLoss += 2.0 * ELECTRON_MASS;
    //   continue;
    // }

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

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
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

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
    // if ( (pt->ID == NPoints - 1) && (
    //       pt->PostProcessName == "annihil"
    //       // || pt->PostProcessName == "Decay"
    //     ) && (PDG == 22)) continue;

    std::vector<Track*> chld_trks = pt_chld.second;

    if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "conv") && chld_trks.size() )
    {
      UntrackedEkLoss_gamma += 2.0 * ELECTRON_MASS;
      continue;
    }

    if ( (pt->ID == NPoints - 1) && (PDG == -11) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "annihil") && chld_trks.size() ) continue;
    // if ( (pt->ID == NPoints - 1) && (PDG == 22) && (pt->PostProcessName == "phot") && chld_trks.size() ) continue;

    for (auto trk: chld_trks)
    {
      trk->CollectUntrackedEkInSFGDByChildsForGamma(UntrackedEkLoss_gamma);
    }
  }

  // dEkOutSFGD_wchilds =+ dEkOutSFGD;
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