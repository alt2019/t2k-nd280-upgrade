
#include "PointInfo.hh"

int PointInfo::getLocationMaterial()
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
  int location_material = -1;
  if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion/Core") location_material = 0;
  else if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion") location_material = 1;
  else if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberX") location_material = 2;
  else if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberY") location_material = 3;
  else if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberZ") location_material = 4;
  else if (PhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole") location_material = 5;
  return location_material;
}

std::string PointInfo::getLocationMaterialStr()
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
    
  IsIntoSFGD = isPtInsideSFGD(LogVolName);

  LogVolNameShort = getLocationMaterialStr();

  Ek = -1.0;
  dEk_with_prev_pt = 0.0;

  IncreasingEdep = -999.0;
}

TrackPoint::~TrackPoint()
{

}

void TrackPoint::print()
{
  // if (dEk_dE_diff > 1.0e-7 && dEk_dE_diff <= 1.0e-5) std::cout << YELLOW;
  // else if (dEk_dE_diff > 1.0e-7) std::cout << GREEN;
  std::cout << "\t"
    << ID << " "
    << "dE: " << std::setw(12) << Edep << " "
    << "dL: " << std::setw(12) << StepLength << " "
    << "|Mom|: " << std::setw(12) << Momentum.Mag() << " "
    << "|Ek|: " << std::setw(12) << Ek << " "
    << "dEsum: " << std::setw(12) << IncreasingEdep << " "
    // << "dEk: " << std::setw(12) << dEk_with_prev_pt << " "
    // << "dEk: " << std::setw(12) << dEk_dE_diff << " "
    << "inSFGD: " << IsIntoSFGD << " "
    << " [X:" << AvgPosition.X() << " Y:" << AvgPosition.Y() << " Z:" << AvgPosition.Z() << "] "
    << " Mat: "<< MaterialName
    << " V: "<< LogVolNameShort
    << " P: " << PreProcessName
    << " " << PostProcessName
    << " " << PreProcessType
    << " " << PostProcessType
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
      // created_tracks_ss << "[" << trkid.first << "," << trkid.second << "] ";
      created_tracks_ss << "[" << std::get<0>(trkid) << ","
                               << std::get<1>(trkid) << ","
                               << std::get<2>(trkid) << ","
                               << std::get<3>(trkid) << ","
                               << std::get<4>(trkid) << ","
                        << "] ";
    }

    std::cout << "  Secondaries at this point: " << created_tracks_ss.str() << std::endl;

    std::stringstream created_tracks_ids_ss;
    for (auto trkid: created_tracks_ids)
    {
      created_tracks_ids_ss << trkid << ",";
    }
    std::cout << "  Secondaries at this point: " << created_tracks_ids_ss.str() << std::endl;
    
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
