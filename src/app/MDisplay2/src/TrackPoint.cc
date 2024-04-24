

#include "TrackPoint.hh"

TrackPoint::TrackPoint(TND280UpTrackPoint* trackPoint, TND280UpTrack* ownerTrack)
{
  ID = trackPoint->GetPointID();
  Time = trackPoint->GetTime();
  Momentum = trackPoint->GetMomentum();
  Edep = trackPoint->GetEdep();
  StepLength = trackPoint->GetStepLength();
  StepDeltaLyz = trackPoint->GetStepDeltaLyz();
  StepStatus = trackPoint->GetStepStatus();
  PhysVolName = trackPoint->GetPhysVolName();
  LogVolName = trackPoint->GetLogVolName();
  PrePosition = trackPoint->GetPrePosition();
  PostPosition = trackPoint->GetPostPosition();
  IsOnBoundary = trackPoint->IsOnBoundary();
  PreProcessName = trackPoint->GetPreProcessName();
  PostProcessName = trackPoint->GetPostProcessName();
  MaterialName = trackPoint->GetMaterialName();
  PreProcessType = (int)(trackPoint->GetPreProcessType());
  PostProcessType = (int)(trackPoint->GetPostProcessType());

  // created_tracks_ids = trackPoint->GetSecondariesIds();
  created_tracks = trackPoint->GetSecondariesIds();
    
  OwnerTrackId = ownerTrack->GetTrackID();

  AvgPosition = (PrePosition + PostPosition) * 0.5;
    
  // IsIntoSFGD = isPtInsideSFGD(LogVolName);
  IsIntoSFGD = _isIntoSFGD();
  IsIntoSFGDFV = isPtInSFGDFV(toSFGDCoordinateSystem(AvgPosition));

  LogVolNameShort = getLocationMaterialStr();

  Ek = -1.0;
  // dEk_with_prev_pt = 0.0;

  IncreasingEdep = -1.0;

  IsFirstPtOfTrack = false;
  IsLastPtOfTrack = false;
  IsFirstPtOfTrack_InSFGD = false;
  IsLastPtOfTrack_InSFGD = false;

  idx = -1;
}

TrackPoint::~TrackPoint()
{
  for (auto it: created_tracks_ptrs)
  {
    if (!it) continue;
    delete it;
    it = nullptr;
  }
}

void TrackPoint::print()
{
  // if (dEk_dE_diff > 1.0e-7 && dEk_dE_diff <= 1.0e-5) std::cout << YELLOW;
  // else if (dEk_dE_diff > 1.0e-7) std::cout << GREEN;
  std::cout << "\t"
    << std::setw(3) << idx << " " << std::setw(3) << ID << " "
    << "dE: " << std::setw(12) << Edep << " "
    << "dL: " << std::setw(12) << StepLength << " "
    << "|Mom|: " << std::setw(12) << Momentum.Mag() << " "
    << "|Ek|: " << std::setw(12) << Ek << " "
    << "dEsum: " << std::setw(12) << RED << IncreasingEdep << RESET << " "
    // << "dEk: " << std::setw(12) << dEk_with_prev_pt << " "
    // << "dEk: " << std::setw(12) << dEk_dE_diff << " "
    // << "inSFGD: " << IsIntoSFGD << " "
    // << " [X:" << AvgPosition.X() << " Y:" << AvgPosition.Y() << " Z:" << AvgPosition.Z() << "] "
    << " Mat: " << std::setw(7) << MaterialToShort(MaterialName)
    << " V: " << std::setw(12) << LogVolNameShort
    << " P: " << std::setw(7) << ProcessNameToShort(PreProcessName)
    << " " << std::setw(7) << ProcessNameToShort(PostProcessName)
    << " " << PreProcessType
    << " " << PostProcessType
    << "| inSFGD: " << IsIntoSFGD << " "
    << " [X:" << std::setw(10) << AvgPosition.X() << " Y:" << std::setw(10) << AvgPosition.Y() << " Z:" << std::setw(10) << AvgPosition.Z() << "] "
    << " onB: " << IsOnBoundary
    << " [" << IsFirstPtOfTrack << IsLastPtOfTrack << IsFirstPtOfTrack_InSFGD << IsLastPtOfTrack_InSFGD << "] "

    // << "\n\t" << LogVolName << " " << IsIntoSFGD
    << std::endl;
  // if (dEk_dE_diff > 1.0e-7) std::cout << RESET;
  // if (created_tracks_ids.size())
  // {
  //   std::stringstream created_tracks_ss;
  //   for (auto trkid: created_tracks_ids)
  //   {
  //     created_tracks_ss << trkid << " ";
  //   }
  //   std::cout << "  Secondaries at this point: " << created_tracks_ss.str() << std::endl;
  // }
  if (created_tracks.size())
  {
    std::stringstream created_tracks_ss;
    for (auto trkid: created_tracks)
    {
      // std::vector<std::tuple<int, int, double, double, double, double, std::string, double, double, double, int>> to_save;
      ///                       ID   PDG  Ek0     Px      Py      Pz      ProcName     X       Y       Z       Status
      // created_tracks_ss << "[" << trkid.first << "," << trkid.second << "] ";
      int TrajTrkId_ = std::get<0>(trkid);
      int PDG_ = std::get<1>(trkid);
      double InitialKineticEnergy_ = std::get<2>(trkid);
      double momX_ = std::get<3>(trkid);
      double momY_ = std::get<4>(trkid);
      double momZ_ = std::get<5>(trkid);
      std::string ProcessName_ = std::string(std::get<6>(trkid));
      double posX_ = std::get<7>(trkid);
      double posY_ = std::get<8>(trkid);
      double posZ_ = std::get<9>(trkid);
      int status = std::get<10>(trkid);
      created_tracks_ss << BOLDBLUE << "[" << RESET
                          << TrajTrkId_ << ","
                          << CYAN << PDG_ << RESET << ","
                          << GREEN << InitialKineticEnergy_ << RESET << ","
                          << momX_ << ","
                          << momY_ << ","
                          << momZ_ << ","
                          << MAGENTA << ProcessName_ << RESET << ","
                          << posX_ << ","
                          << posY_ << ","
                          << posZ_ << ","
                          << RED << status << RESET << ""
                        << BOLDBLUE << "]" << RESET;

      // created_tracks_ss << "[" << std::get<0>(trkid) << ","
      //                          << std::get<1>(trkid) << ","
      //                          << std::get<2>(trkid) << ","
      //                          << std::get<3>(trkid) << ","
      //                          << std::get<4>(trkid) << ","
      //                   << "] ";
      
    }

    std::cout << "\t  Secondaries at this point: " << created_tracks_ss.str() << std::endl;

    // std::stringstream created_tracks_ids_ss;
    // for (auto trkid: created_tracks_ids)
    // {
    //   created_tracks_ids_ss << trkid << ",";
    // }
    // std::cout << "  Secondaries at this point: " << created_tracks_ids_ss.str() << std::endl;
    
    // for (auto trk: created_tracks_ptrs)
    // {
    //   std::cout << "[" << trk->ID << "," << trk->PDG << "," << trk->InitKinEnergy << "] ";
    // }
  }
}


std::string TrackPoint::getLocationMaterialStr()
{
  /*
   * location_material:
   * - undefined = -1
   * - scintillator = 0
   * - reflector = 1
   * - fiberX = 2
   * - fiberY = 3
   * - fiberZ = 4
   * - hole = 5
   */
  std::string location_material = LogVolName;
  if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion/Core") location_material = "Scintillator";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion") location_material = "Reflector";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberX") location_material = "FiberX";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberY") location_material = "FiberY";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberZ") location_material = "FiberZ";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole") location_material = "Hole";
  else if (LogVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint") location_material = "?CubeScint?";
  return location_material;
}

bool TrackPoint::_isIntoSFGD()
{
  if (
    //    LogVolName.contains("/t2k/OA/Magnet/Basket/target1/TargetUniform")
    // || LogVolName.contains("/t2k/OA/Magnet/Basket/target1/SuperFGD1")
    // || LogVolName.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform")
    // || LogVolName.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1")
       (LogVolName.find("/t2k/OA/Magnet/Basket/target1/TargetUniform") != std::string::npos)
    || (LogVolName.find("/t2k/OA/Magnet/Basket/target1/SuperFGD1") != std::string::npos)
    || (LogVolName.find("/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") != std::string::npos)
    || (LogVolName.find("/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1") != std::string::npos)
  ) return true;

  return false;
}

// bool TrackPoint::_isIntoSFGDFV(TVector3 pt_in_sfgd_cs)
// {
//   if (
//          (pt_in_sfgd_cs.X() <= 192.0 - 2.0 && pt_in_sfgd_cs.X() >= 0.0 + 2.0)
//       && (pt_in_sfgd_cs.Y() <= 56.0 - 2.0 && pt_in_sfgd_cs.Y() >= 0.0 + 2.0)
//       && (pt_in_sfgd_cs.Z() <= 184.0 - 2.0 && pt_in_sfgd_cs.Z() >= 0.0 + 2.0)
//   ) return true;

//   return false;
// }


std::string TrackPoint::MaterialToShort(std::string mat)
{
  std::string _short = mat;
  if      (mat == "Scintillator") _short = "M_Scint";
  else if (mat == "FGDScintillator") _short = "M_Refl";
  else if (mat == "FiberCore") _short = "M_Fiber";
  return _short;
}

std::string TrackPoint::ProcessNameToShort(std::string proc)
{
  std::string _short = proc;
  if      (proc == "Transportation") _short = "P_Tr";
  else if (proc == "UserMaxStep") _short = "M_UMS";
  return _short;
}


