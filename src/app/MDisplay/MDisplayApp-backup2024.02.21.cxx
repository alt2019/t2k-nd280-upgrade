

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <cstdlib>

#include <TString.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TString.h>
#include <TMath.h>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"

#include "src/SVG.hh"

#include "src/old/ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
#include "src/old/AnalizeEvents.hh"
#include "src/old/SimulationAnalizer.hh"

void DR1(TString prefix, TString filename) {
	SimulationAnalizer SA{prefix, filename};

	SA.createSVGfilesInEvents(0, 100);
	// SA.printEvents(0, 10,
	// 			   1 /* print vertices*/
	// );
	SA.computeEnergeticProperties(0, 1000);
	// SA.computeAverageProperties(0, 1000);
	// SA.getEnergiesOfPrimaryParticles(0, 1000);

	std::cout << "!!! Output directory: " << SA.svgDirectoryRelativePath << std::endl;

	SA.clear();
}

// void DR1(TString input_root_file) {
// 	SimulationAnalizer SA{input_root_file};

// 	SA.createSVGfilesInEvents(0, 100);
// 	// SA.printEvents(0, 10,
// 	// 			   1 /* print vertices*/
// 	// );
// 	SA.computeEnergeticProperties(0, 1000);
// 	// SA.computeAverageProperties(0, 1000);
// 	// SA.getEnergiesOfPrimaryParticles(0, 1000);

// 	std::cout << "!!! Output directory: " << SA.svgDirectoryRelativePath << std::endl;

// 	SA.clear();
// }

void createSVGforTrueTracks(TTree *tinput, TND280UpEvent *nd280UpEvent, int eventId)
{

}

void createSVGforHits(TTree *tinput, TND280UpEvent *nd280UpEvent, int eventId)
{

}

std::string trackToSVG(TND280UpTrack* track)
{
  xmlns::XMLObject* xo = new xmlns::XMLObject("test", "");

  return "";
}


std::string getEventTopology(TND280UpEvent *nd280UpEvent)
{


  return "";
}


std::vector<double> computeTrackLengthInSuperFGD(TND280UpTrack* track)
{
  	float distance_betw_pts = 0.0;
		float total_distance_length = 0.0;
		float distance_betw_pts_z = 0.0;
		float total_distance_length_z = 0.0;
		float total_distance_length_ptstep = 0.0;
		float pt_pos_pre_x = 0.0;
		float pt_pos_pre_y = 0.0;
		float pt_pos_pre_z = 0.0;

		float TOF = 0.0, start_time = 0.0, stop_time = 0.0;

    int NPoints = track->GetNPoints();

    for (int ipt = 0; ipt < NPoints; ++ipt)
    {
      TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
      double Time = trackPoint->GetTime();
      TVector3 Momentum = trackPoint->GetMomentum();
      double Edep = trackPoint->GetEdep();
      double StepLength = trackPoint->GetStepLength();
      double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      string PhysVolName = trackPoint->GetPhysVolName();
      TVector3 PrePosition = trackPoint->GetPrePosition();
      TVector3 PostPosition = trackPoint->GetPostPosition();
      std::string PreProcessName = trackPoint->GetPreProcessName();
      std::string PostProcessName = trackPoint->GetPostProcessName();
      std::string MaterialName = trackPoint->GetMaterialName();
      int PointID = trackPoint->GetPointID();
      bool IsOnBoundary = trackPoint->IsOnBoundary();

      /// check if point is into superfgd
      if (PrePosition[0] > 960.0 || PrePosition[0] < -960.0) continue;
      if (PrePosition[1] > 280.0 || PrePosition[1] < -280.0) continue;
      if (PrePosition[2] > 920.0 || PrePosition[2] < -920.0) continue;

			distance_betw_pts = (PrePosition - TVector3(pt_pos_pre_x, pt_pos_pre_y, pt_pos_pre_z)).Mag();
			total_distance_length += distance_betw_pts;
			distance_betw_pts_z = abs(PrePosition[2] - pt_pos_pre_z);
			total_distance_length_z += distance_betw_pts_z;
			pt_pos_pre_x = PrePosition[0];
			pt_pos_pre_y = PrePosition[1];
			pt_pos_pre_z = PrePosition[2];
			total_distance_length_ptstep += StepLength; // true track length

			if (ipt == 0) start_time = Time;
			stop_time = Time;
		}

		// TOF = stop_time - start_time;
		// return {total_distance_length, total_distance_length_z, total_distance_length_ptstep};
		// return std::make_tuple(total_distance_length, total_distance_length_z, total_distance_length_ptstep);

    std::vector<double> track_length{total_distance_length, total_distance_length_z, total_distance_length_ptstep};

    return track_length;
}


struct EventsInfo
{
  std::map<int, TND280UpEvent*> events_map;

  /// true tracks (simulated)
  // std::map<int, int> mTrackID;
  // std::map<int, int> mTrackParentID;
  // std::map<int, int> mTrackGParentID;
  // std::map<int, int> mTrackPDG;
  // std::map<int, std::string> mTrackProcessName;
  // std::map<int, double> mTrackInitKinEnergy;

  /// hits (simulated)

  std::vector<std::string> main_particle;
  std::vector<double> main_particle_edep;
  std::vector<double> main_particle_range;
  std::vector<double> main_particle_dedx;
  std::vector<double> main_particle_dedx_hits;
  std::vector<bool> main_particle_has_elepos_pair;

  std::vector<std::vector<double>> main_track_pt_z;
  std::vector<std::vector<double>> main_track_de;

  TH1F* cube0_edep;
  TH1F* cube1_edep;
  TH1F* cube2_edep;
  TH1F* cube3_edep;
};


void process_event(TTree *tinput, TND280UpEvent *nd280UpEvent, int eventId, std::string out_path_tag, EventsInfo& ei)
{
  std::cout << "\nEvent " << eventId << std::endl;
  tinput->GetEntry(eventId);

  int NHits = nd280UpEvent->GetNHits();
  int NTracks = nd280UpEvent->GetNTracks();

  std::cout << "NHits " << NHits << std::endl;
  std::cout << "NTracks " << NTracks << std::endl;

  TND280UpTrack* primary_track;
  std::vector<TND280UpTrack*> secondary_tracks; // gammas
  std::vector<TND280UpTrack*> tertiary_tracks; // e+/e- from gamma conversion process

  std::map<int, int> track_parent_id;
  std::map<int, int> track_gparent_id;
  std::map<int, std::array<int, 7>> track_g_parent_id;

  bool event_has_elepos_pair = false;

  for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    int TrackID = track->GetTrackID();
    int ParentID = track->GetParentID();
    std::string ProcessName = track->GetProcessName();
    int PDG = track->GetPDG();
    double InitKinEnergy = track->GetInitKinEnergy();

    // std::cout << "Track: " << trjID << " " << TrackID << " " << ParentID << " " << PDG << " " << InitKinEnergy << " " << ProcessName
    //           << " " << track->GetNPoints()
    //           << std::endl;

    track_parent_id.insert({TrackID, ParentID});

    if (ParentID == 0) { primary_track = track; }

    if (ParentID == 1 && PDG == 22)
    {
      secondary_tracks.push_back(track);
    }

    if (ProcessName == "conv")
    {
      tertiary_tracks.push_back(track);
    }

    std::array<int, 7> tids{trjID, TrackID, ParentID, -1, PDG, -1, -1};
    // track_g_parent_id.insert({trjID, tids});
    track_g_parent_id.insert({TrackID, tids});

    // if (ParentID != 0)
    // {
    //   TND280UpTrack* parent_track = nd280UpEvent->GetTrack(ParentID);
    //   int PTrackID = parent_track->GetTrackID();
    //   std::cout << PTrackID << " " << ParentID << std::endl;
    //   // if (PTrackID != ParentID) throw;
    //   int GParentID = parent_track->GetParentID();
    //   std::string PProcessName = parent_track->GetProcessName();
    //   int PPDG = parent_track->GetPDG();
    //   double PInitKinEnergy = parent_track->GetInitKinEnergy();
    // }
    
  }

  // std::cout << "-------------------------" << std::endl;
  // for (auto it: track_g_parent_id)
  // {
  //   std::cout << " TID:" << it.first
  //             << " ID:" << (it.second)[0]
  //             << " TID:" << (it.second)[1]
  //             << " PID:" << (it.second)[2]
  //             << " GPID:" << (it.second)[3]
  //             << " PDG:" << (it.second)[4]
  //             << std::endl;
  // }
  // std::cout << "-------------------------" << std::endl;

  for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    int TrackID = track->GetTrackID();
    int ParentID = track->GetParentID();

    // track_gparent_id.insert({track_parent_id.at(ParentID), ParentID});

    // auto & parent_ids_arr = track_g_parent_id[ParentID];

    // auto ids_arr = track_g_parent_id[TrackID];
    // auto & ids_arr = track_g_parent_id[TrackID];
    // ids_arr[3] = parent_ids_arr[2];
    // track_g_parent_id[TrackID][3] = parent_ids_arr[2];
    track_g_parent_id[TrackID][3] = track_g_parent_id[ParentID][2];
    track_g_parent_id[TrackID][5] = track_g_parent_id[ParentID][4];
  }

  std::cout << "-------------------------" << std::endl;
  for (auto it: track_g_parent_id)
  {
    int TrackID = it.first;
    int TrackID_id = it.second[0];
    int ParentID = it.second[2];
    int GParentID = it.second[3];
    int GParentID_id = track_g_parent_id[GParentID][0];
    int PDG = (it.second)[4];
    int PPDG = (it.second)[5];

    TND280UpTrack* gparent_track = nd280UpEvent->GetTrack(GParentID_id);
    int GPPDG = gparent_track->GetPDG();
    std::string gparent_track_proc = gparent_track->GetProcessName();

    TND280UpTrack* track = nd280UpEvent->GetTrack(TrackID_id);
    std::string track_proc = track->GetProcessName();

    if (
        // (GPPDG == 11 & GParentID == 1 & PPDG == 22 & track_proc == "conv")
        // ||
        (PDG == 22 & ParentID == 1)
        // || 
        // (TrackID == 1)
      )
    {
      event_has_elepos_pair = true;
      std::cout << " TID:" << TrackID
                << " ID:" << (it.second)[0]
                << " TID:" << TrackID
                << " PID:" << ParentID
                << " GPID:" << GParentID
                << " PDG:" << PDG
                << " PPDG:" << PPDG
                << " GPPDG:" << GPPDG
                << std::endl;
    }

    // std::cout << " TID:" << TrackID
    //           << " ID:" << (it.second)[0]
    //           << " TID:" << (it.second)[1]
    //           << " PID:" << (it.second)[2]
    //           << " GPID:" << (it.second)[3]
    //           << " PDG:" << (it.second)[4]
    //           << " PPDG:" << (it.second)[5]
    //           << " GPPDG:" << GPPDG
    //           << std::endl;

    
  }
  std::cout << "-------------------------" << std::endl;

  // for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  // {
  //   TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
  
  //   int TrackID = track->GetTrackID();
  //   int ParentID = track->GetParentID();
  //   // int GParentID = track_gparent_id.at();
  // }



  // if (!tertiary_tracks.empty())
  // {
  //   for (auto& track: tertiary_tracks)
  //   {
  //     // int TrackID = track->GetTrackID();
  //     int ParentID = track->GetParentID();
  //     // int GParentID = tid_gpid[TrackID];
  //     std::string ProcessName = track->GetProcessName();
  //     int PDG = track->GetPDG();
  //     double InitKinEnergy = track->GetInitKinEnergy();
  //     std::cout << PDG << " " << ParentID << " " << InitKinEnergy << std::endl;
  //   }
  // }

  if (primary_track)
  {
    int NPoints = primary_track->GetNPoints();
    int TrackID = primary_track->GetTrackID();
    int ParentID = primary_track->GetParentID();
    std::string ProcessName = primary_track->GetProcessName();
    int PDG = primary_track->GetPDG();
    double InitKinEnergy = primary_track->GetInitKinEnergy();
    double Range = primary_track->GetRange();
    double SDTotalEnergyDeposit = primary_track->GetSDTotalEnergyDeposit();
    double SDLength = primary_track->GetSDLength();
    double LengthTarget1 = primary_track->GetLengthTarget1();
    double EdepTarget1 = primary_track->GetEdepTarget1();

    std::string particle_repr = "";
    switch (PDG)
    {
      case 11: { particle_repr = "e-"; break; }
      case -11: { particle_repr = "e+"; break; }
      case -13: { particle_repr = "mu+"; break; }
      case 13: { particle_repr = "mu-"; break; }
    }

    ei.main_particle.push_back(particle_repr);
    ei.main_particle_edep.push_back(EdepTarget1); // MeV
    ei.main_particle_range.push_back(LengthTarget1 / 10.0); // cm
    ei.main_particle_dedx.push_back(EdepTarget1 / LengthTarget1 * 10.0); // MeV/cm

    std::cout << "Track: "
              << " TID:" << TrackID
              << " PID:" << ParentID
              << " PDG:" << PDG
              << " Ek0:" << InitKinEnergy
              << " Pros:" << ProcessName
              << " Npt:" << NPoints
              << " R:" << Range
              << " dEsd:" << SDTotalEnergyDeposit
              // << " " << SDLength
              << " Ltg1:" << LengthTarget1
              << " dEtg1:" << EdepTarget1
              << std::endl;

    double pts_edep = 0.0;
    double pts_len = 0.0;
    double prevX = 0.0, prevY = 0.0, prevZ = 0.0;
    for (int ipt = 0; ipt < NPoints; ++ipt)
    {
      TND280UpTrackPoint* trackPoint = primary_track->GetPoint(ipt);
      double Time = trackPoint->GetTime();
      TVector3 Momentum = trackPoint->GetMomentum();
      double Edep = trackPoint->GetEdep();
      double StepLength = trackPoint->GetStepLength();
      double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      string PhysVolName = trackPoint->GetPhysVolName();
      TVector3 PrePosition = trackPoint->GetPrePosition();
      TVector3 PostPosition = trackPoint->GetPostPosition();
      std::string PreProcessName = trackPoint->GetPreProcessName();
      std::string PostProcessName = trackPoint->GetPostProcessName();
      std::string MaterialName = trackPoint->GetMaterialName();
      int PointID = trackPoint->GetPointID();
      bool IsOnBoundary = trackPoint->IsOnBoundary();
      std::cout << "  Tp:" << PointID
                << " |M|:" << Momentum.Mag()
                << " dE:" << Edep
                << " dL:" << StepLength
                << " " << PreProcessName << " " << PostProcessName << " " << MaterialName
                << " Bnd:" << IsOnBoundary
                << " X:" << PrePosition.X() << " Y:" << PrePosition.Y() << " Z:" << PrePosition.Z()
                << " " << PhysVolName
                << std::endl;

      double dLtoPrev = TMath::Sqrt(
        TMath::Power(prevX - PrePosition.X(), 2.0) + TMath::Power(prevY - PrePosition.Y(), 2.0) + TMath::Power(prevZ - PrePosition.Z(), 2.0)
      );
      // double dL = (PostPosition - PrePosition).Mag();

      pts_edep += Edep;
      // pts_len += StepLength;
      // pts_len += dLtoPrev + (PostPosition - PrePosition).Mag();
      pts_len += (PostPosition - PrePosition).Mag();
      // pts_len += dL;

      prevX = PrePosition.X();
      prevY = PrePosition.Y();
      prevZ = PrePosition.Z();
    }

    std::cout << "  Pts dE: " << pts_edep << " L: " << pts_len << std::endl;
  }

  // ei.main_particle_dedx_hits
  double total_hits_edep = 0.0;

  // int NHits = nd280UpEvent->GetNHits();
  /// loop over hits
  for (int ihit = 0; ihit < NHits; ++ihit)
  // for (int ihit = 0; ihit < 0; ++ihit)
  {
    TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
    double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
    double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
    double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
    double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
    TVector3 lightPos(posX,posY,posZ); // needed for ApplyResponse
    double charge = 1.; // assumption on hit charge; needed for ApplyResponse
    double edep = nd280UpHit->GetEnergyDeposit();
    double steplength = nd280UpHit->GetTrackLength();
    std::string detname = nd280UpHit->GetDetName();
    total_hits_edep += edep;
  }

  ei.main_particle_dedx_hits.push_back(total_hits_edep);
  ei.main_particle_has_elepos_pair.push_back(event_has_elepos_pair);
}




bool isPtIntoSFGD(TVector3 pt)
{
  double X = pt.X();
  double Y = pt.Y();
  double Z = pt.Z();
  /// depending on point position ///
	if (X > 960.0 || X < -960.0) return false;
	if (Y > 280.0 || Y < -280.0) return false;
	if (Z > 920.0 || Z < -920.0) return false;

	/// depending on point volume /// NOT YET IMPLEMENTED ///
	return true;
}


void process_event_v2(TTree *tinput, TND280UpEvent *nd280UpEvent, int eventId, std::string out_path_tag, EventsInfo& ei)
{
  std::cout << "\nEvent " << eventId << std::endl;
  tinput->GetEntry(eventId);

  int NHits = nd280UpEvent->GetNHits();
  int NTracks = nd280UpEvent->GetNTracks();

  std::cout << "NHits " << NHits << std::endl;
  std::cout << "NTracks " << NTracks << std::endl;

  TND280UpTrack* primary_track;
  std::vector<TND280UpTrack*> secondary_tracks; // gammas
  std::vector<TND280UpTrack*> tertiary_tracks; // e+/e- from gamma conversion process

  std::map<int, int> track_parent_id;
  std::map<int, int> track_gparent_id;
  std::map<int, std::array<int, 7>> track_g_parent_id;

  bool event_has_elepos_pair = false;

  std::vector<std::vector<double>> main_track_pt_z;
  std::vector<std::vector<double>> main_track_de;

  // TH2* h2 = new TH2F("h2", "h2 title", 92, 0.0, 92.0, 200, 0, 200);
  

  // for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  for (Int_t trjID = 0; trjID < 1; trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    int TrackID = track->GetTrackID();
    int ParentID = track->GetParentID();
    std::string ProcessName = track->GetProcessName();
    int PDG = track->GetPDG();
    double InitKinEnergy = track->GetInitKinEnergy();

    int NPoints = track->GetNPoints();

    std::cout << " TID:" << TrackID
              << " PID:" << ParentID
              << " PDG:" << PDG
              << " Proc:" << ProcessName
              << " Ek0:" << InitKinEnergy
              << " Npt:" << NPoints
    << std::endl;

    std::vector<double> mt_pt_z;
    std::vector<double> mt_de;

    std::vector<double> mt_pt_z_cubes;
    std::vector<double> mt_de_cubes;
    double dz_prev = 0.0;
    // double dz = 0.0;
    double de_cube = 0.0;
    double cube_ini_z_pos = 0.0;
    double cube_fin_z_pos = 10.0;
    int zLayer = 0;

    for (int ipt = 0; ipt < NPoints; ++ipt)
    {
      TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
      TVector3 PrePosition = trackPoint->GetPrePosition();
      TVector3 PostPosition = trackPoint->GetPostPosition();
      if (!isPtIntoSFGD(PrePosition)) continue;


      double Time = trackPoint->GetTime();
      TVector3 Momentum = trackPoint->GetMomentum();
      double Edep = trackPoint->GetEdep();
      double StepLength = trackPoint->GetStepLength();
      double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      string PhysVolName = trackPoint->GetPhysVolName();
      std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1";
      if (PhysVolName.find(to_replace) != std::string::npos)
        PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"~");
      std::string PreProcessName = trackPoint->GetPreProcessName();
      std::string PostProcessName = trackPoint->GetPostProcessName();
      std::string MaterialName = trackPoint->GetMaterialName();
      int PointID = trackPoint->GetPointID();
      bool IsOnBoundary = trackPoint->IsOnBoundary();
      // std::cout << "  Tp:" << PointID
      //           << " |M|:" << Momentum.Mag()
      //           << " dE:" << Edep
      //           << " dL:" << StepLength
      //           << " " << PreProcessName << " " << PostProcessName << " " << MaterialName
      //           << " Bnd:" << IsOnBoundary
      //           //  << " X:" << PrePosition.X()
      //           //   << " Y:" << PrePosition.Y()
      //             << " Z:" << PrePosition.Z()
      //           //  << " X:" << PostPosition.X()
      //           //   << " Y:" << PostPosition.Y()
      //             << " Z:" << PostPosition.Z()
      //           << " " << PhysVolName
      //           << std::endl;

      TVector3 AvgPos = (PostPosition + PrePosition);
      AvgPos *= 0.5;

      

      // mt_pt_z.push_back(PrePosition.Z());
      mt_pt_z.push_back(AvgPos.Z());
      mt_de.push_back(Edep);

      // if (PrePosition.Z() - dz_prev > 10.0)
      // if (AvgPos.Z() - dz_prev > 10.0)
      // {
      //   mt_de_cubes.push_back(de_cube);
      //   mt_pt_z_cubes.push_back(dz_prev);
      //   de_cube = Edep;
      //   // dz_prev = PrePosition.Z();
      //   dz_prev = AvgPos.Z();
      // } else {
      //   de_cube += Edep;
      // }      

      if ( AvgPos.Z() < cube_fin_z_pos && AvgPos.Z() >= cube_ini_z_pos )
      {
        de_cube += Edep;
      } else {
        mt_de_cubes.push_back(de_cube);
        mt_pt_z_cubes.push_back(dz_prev);
        zLayer++;
        cube_ini_z_pos += 10.0; // mm
        cube_fin_z_pos += 10.0;
        // if (cube_fin_z_pos > 920.0) break;
        de_cube = Edep;
        dz_prev = AvgPos.Z();
      }
      
    }

    main_track_pt_z.push_back(mt_pt_z);
    main_track_de.push_back(mt_de);

    for (int i = 0; i < mt_pt_z_cubes.size(); ++i)
    {
      std::cout << " Cube:" << i
                << " Z:" << mt_pt_z_cubes[i]
                << " dE:" << mt_de_cubes[i]
                << std::endl;
    }

    ei.cube0_edep->Fill(mt_de_cubes[0]);
    ei.cube1_edep->Fill(mt_de_cubes[1]);
    ei.cube2_edep->Fill(mt_de_cubes[2]);
    ei.cube3_edep->Fill(mt_de_cubes[3]);
  }

  
}



void test()
{
  /*
  xmlns::XMLObject* xo = new xmlns::XMLObject("test", "");

  xmlns::XMLObject* xo5 = new xmlns::XMLObject(nullptr, xmlns::XMLObjectType::Text, "", "some text");

  xmlns::XMLObject* xo1 = new xmlns::XMLObject("path", "shgdjfsfhsjhdfjshf");
  xo->addChild(xo1);
  xo->addChild(xo5);

  xmlns::XMLAttributes svg_obj_attributes{
		{"type", "image/svg+xml"},
		{"id", "main-vis-area"},
		{"data", ""},
		{"width", "2690"},
		{"height", "2610"},
	};

  xmlns::XMLObject* xo2 = new xmlns::XMLObject("s", "dahgs kd", svg_obj_attributes);
  xo1->addChild(xo2);

  std::string xo_s = xo->render(2);
  std::cout << xo_s << std::endl;

  xo->parse(xo_s);
  // */

  std::string svg_xml_sample = R"SVG(<?xml-stylesheet type="text/css" href="../svg.css" ?>

  <svg version="1.1" xmlns="http://www.w3.org/2000/svg" data-eventid="0" data-trackscount="-11:1,11:194,12:1,22:20,1000060120:1" id="main-svg-wrapper">
    <g id="plane-xz">
        <svg version="1.1" xmlns="http://www.w3.org/2000/svg" id="svg-plane-xz" width="1920" height="1840" viewBox="-960, -920, 1920, 1840">
  <g id="sim-det-resp-activated-cubes-xz">
    <rect class="activated-cube xz" x="-950" y="-180" width="10" height="10" fill="hsl(229.262177, 100%, 50%)" data-energy="0.018920"><title>18.9 KeV at cube (-95 -18)</title></rect>
  </g></svg></g></svg>
  <svg version="1.1" xmlns="http://www.w3.org/2000/svg" data-eventid="0" data-trackscount="-11:1,11:194,12:1,22:20,1000060120:1" id="main-svg-wrapper">
    <g id="plane-xz">
        <svg version="1.1" xmlns="http://www.w3.org/2000/svg" id="svg-plane-xz" width="1920" height="1840" viewBox="-960, -920, 1920, 1840">
  <g id="sim-det-resp-activated-cubes-xz">
    <rect class="activated-cube xz" x="-950" y="-180" width="10" height="10" fill="hsl(229.262177, 100%, 50%)" data-energy="0.018920"><title>18.9 KeV at cube (-95 -18)</title></rect>
  </g></svg></g></svg>
  )SVG";

 xmlns::XMLObject* xo3 = new xmlns::XMLObject(svg_xml_sample);

 xmlns::XMLObject* xo4 = new xmlns::XMLObject(nullptr, xmlns::XMLObjectType::Text, "", "some text");
 std::cout << xo4->render(2) << std::endl;

  // delete xo, xo1, xo2;
}

int main_v2(int argc, char **argv)
{
  // test();
  // return 2;

  std::cout << "Usage:" << std::endl;
  std::cout << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> <output file path with tag> [<event number to process>]" << std::endl;

  if (argc != 3 && argc != 4)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  std::string input_rootfilename = argv[1];
  std::string output_dir_tag = argv[2];

  std::stringstream cmd; cmd << "mkdir " << output_dir_tag;
  int res = std::system(cmd.str().c_str());
  // if (res != 0)
  // {
  //   return -1;
  // }

  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

  std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  // std::size_t found = input_rootfilename.find_last_of("/\\");
  // std::string path = input_rootfilename.substr(0,found+1);
  // std::string file = input_rootfilename.substr(found+1);
  // std::cout << " path: " << str.substr(0,found) << '\n';
  // std::cout << " file: " << str.substr(found+1) << '\n';

  // DR1(path, file); // causes segfault
  // return 2;

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  EventsInfo ei;

  // double dEmax = 3.0;
  double dEmax = 5.0;
  ei.cube0_edep = new TH1F("cube0_dE", "cube0 dE", 100, 0.0, dEmax);
  ei.cube1_edep = new TH1F("cube1_dE", "cube1 dE", 100, 0.0, dEmax);
  ei.cube2_edep = new TH1F("cube2_dE", "cube2 dE", 100, 0.0, dEmax);
  ei.cube3_edep = new TH1F("cube3_dE", "cube3 dE", 100, 0.0, dEmax);
  
  int NTreeEntries = tinput->GetEntries();

  if (argc == 4)
  {
    int eventIdToProcess = atoi(argv[3]);
    // process_event(tinput, nd280UpEvent, eventIdToProcess, output_dir_tag, ei);
    process_event_v2(tinput, nd280UpEvent, eventIdToProcess, output_dir_tag, ei);
    return 0;
  }

  // for (int i = 0; i < NTreeEntries; ++i)
  for (int i = 0; i < min(NTreeEntries, 200); ++i)
  {
    // process_event(tinput, nd280UpEvent, i, output_dir_tag, ei);
    process_event_v2(tinput, nd280UpEvent, i, output_dir_tag, ei);
  }

  std::string out_fname_cube0 = output_dir_tag + "/cube0_edep-200ev.pdf";
  std::string out_fname_cube1 = output_dir_tag + "/cube1_edep-200ev.pdf";
  std::string out_fname_cube2 = output_dir_tag + "/cube2_edep-200ev.pdf";
  std::string out_fname_cube3 = output_dir_tag + "/cube3_edep-200ev.pdf";


  TCanvas* c0 = new TCanvas("cube0_edep;dE/dx, MeV;N events", "cube0_edep", 1080, 720);
  c0->cd();
  ei.cube0_edep->Draw();
  ei.cube0_edep->SetFillColor(kRed);
  c0->SaveAs(out_fname_cube0.c_str());
  c0->Close();

  TCanvas* c1 = new TCanvas("cube1_edep;dE/dx, MeV;N events", "cube1_edep", 1080, 720);
  c1->cd();
  ei.cube1_edep->Draw();
  ei.cube1_edep->SetFillColor(kRed);
  c1->SaveAs(out_fname_cube1.c_str());
  c1->Close();

  TCanvas* c2 = new TCanvas("cube2_edep;dE/dx, MeV;N events", "cube2_edep", 1080, 720);
  c2->cd();
  ei.cube2_edep->Draw();
  ei.cube2_edep->SetFillColor(kRed);
  c2->SaveAs(out_fname_cube2.c_str());
  c2->Close();

  TCanvas* c3 = new TCanvas("cube3_edep;dE/dx, MeV;N events", "cube3_edep", 1080, 720);
  c3->cd();
  ei.cube3_edep->Draw();
  ei.cube3_edep->SetFillColor(kRed);
  c3->SaveAs(out_fname_cube3.c_str());
  c3->Close();

  // std::cout << "====================================" << std::endl;
  // ofstream out_csv(out_csv_path);
  // out_csv << "id,particle,L(cm),dE(MeV),dE_hits(MeV),dE/dx(MeV/cm),has_gamma->e+/e-\n";
  // for (int i = 0; i < NTreeEntries; ++i)
  // {
  //   std::cout << i << " P:" << ei.main_particle[i]
  //             << " L(cm):" << ei.main_particle_range[i]
  //             << " dE(MeV):" << ei.main_particle_edep[i]
  //             << " dE_hits(MeV):" << ei.main_particle_dedx_hits[i]
  //             << " dE/dx(MeV/cm):" << ei.main_particle_dedx[i]
  //             << " has_gamma->e+/e-:" << ei.main_particle_has_elepos_pair[i]
  //   << std::endl;

  //   out_csv << i << "," << ei.main_particle[i]
  //             << "," << ei.main_particle_range[i]
  //             << "," << ei.main_particle_edep[i]
  //             << "," << ei.main_particle_dedx_hits[i]
  //             << "," << ei.main_particle_dedx[i]
  //             << "," << ei.main_particle_has_elepos_pair[i]
  //   << "\n";
  // }
  // out_csv.close();

  // test();

  return 0;
}



int main(int argc, char **argv)
{

  std::cout << "Usage:" << std::endl;
  std::cout << "  MDisplayApp <input ROOT file name (from GEANT4 simulation)> <output file path with tag> [<event number to process>]" << std::endl;

  if (argc != 3 && argc != 4)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  std::string input_rootfilename = argv[1];
  std::string output_dir_tag = argv[2];

  std::stringstream cmd; cmd << "mkdir " << output_dir_tag;
  int res = std::system(cmd.str().c_str());
  // if (res != 0)
  // {
  //   return -1;
  // }

  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

  std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  EventsInfo ei;

  // double dEmax = 3.0;
  double dEmax = 5.0;
  ei.cube0_edep = new TH1F("cube0_dE", "cube0 dE", 100, 0.0, dEmax);
  ei.cube1_edep = new TH1F("cube1_dE", "cube1 dE", 100, 0.0, dEmax);
  ei.cube2_edep = new TH1F("cube2_dE", "cube2 dE", 100, 0.0, dEmax);
  ei.cube3_edep = new TH1F("cube3_dE", "cube3 dE", 100, 0.0, dEmax);

  int NeventsWOgamma = 0;
  
  int NTreeEntries = tinput->GetEntries();

  if (argc == 4)
  {
    int eventIdToProcess = atoi(argv[3]);
    // process_event(tinput, nd280UpEvent, eventIdToProcess, output_dir_tag, ei);
    process_event_v2(tinput, nd280UpEvent, eventIdToProcess, output_dir_tag, ei);
    return 0;
  }

  // for (int i = 0; i < NTreeEntries; ++i)
  for (int eventId = 0; eventId < min(NTreeEntries, 200); ++eventId)
  {
    // process_event(tinput, nd280UpEvent, i, output_dir_tag, ei);
    // process_event_v2(tinput, nd280UpEvent, i, output_dir_tag, ei);

    std::cout << "\nEvent " << eventId << std::endl;
    tinput->GetEntry(eventId);

    int NHits = nd280UpEvent->GetNHits();
    int NTracks = nd280UpEvent->GetNTracks();

    std::cout << "NHits " << NHits << std::endl;
    std::cout << "NTracks " << NTracks << std::endl;

    TND280UpTrack* primary_track;
    std::vector<TND280UpTrack*> secondary_tracks; // gammas
    std::vector<TND280UpTrack*> tertiary_tracks; // e+/e- from gamma conversion process

    std::map<int, int> track_parent_id;
    std::map<int, int> track_gparent_id;
    std::map<int, std::array<int, 7>> track_g_parent_id;

    bool event_has_elepos_pair = false;

    std::vector<std::vector<double>> main_track_pt_z;
    std::vector<std::vector<double>> main_track_de;

    bool electron_has_photon = false;

    for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
    // for (Int_t trjID = 0; trjID < 1; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

      int TrackID = track->GetTrackID();
      int ParentID = track->GetParentID();
      std::string ProcessName = track->GetProcessName();
      int PDG = track->GetPDG();
      double InitKinEnergy = track->GetInitKinEnergy();

      int NPoints = track->GetNPoints();

      std::cout << " TID:" << TrackID
                << " PID:" << ParentID
                << " PDG:" << PDG
                << " Proc:" << ProcessName
                << " Ek0:" << InitKinEnergy
                << " Npt:" << NPoints
      << std::endl;


      if (ParentID == 1) // secondary track
      {
        if (PDG == 22) // if secondary track is photon, which event is not needed to consider
        {
          electron_has_photon = true;
        }
      }

      if (ParentID == 0)
      {
        primary_track = track;
      }
    } /// end loop over tracks

    /// primary track
    // if (electron_has_photon) continue; // do not do the following if electron has photon
    // NeventsWOgamma++;
    std::vector<double> mt_pt_z;
    std::vector<double> mt_de;

    std::vector<double> mt_pt_z_cubes;
    std::vector<double> mt_de_cubes;
    double dz_prev = 0.0;
    // double dz = 0.0;
    double de_cube = 0.0;
    double cube_ini_z_pos = 0.0;
    double cube_fin_z_pos = 10.0;
    int zLayer = 0;

    int TrackID = primary_track->GetTrackID();
    int ParentID = primary_track->GetParentID();
    std::string ProcessName = primary_track->GetProcessName();
    int PDG = primary_track->GetPDG();
    double InitKinEnergy = primary_track->GetInitKinEnergy();

    int NPoints = primary_track->GetNPoints();

    std::cout << " TID:" << TrackID
              << " PID:" << ParentID
              << " PDG:" << PDG
              << " Proc:" << ProcessName
              << " Ek0:" << InitKinEnergy
              << " Npt:" << NPoints
    << std::endl;

    for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
    {
      TND280UpTrackPoint* trackPoint = primary_track->GetPoint(ipt);
      TVector3 PrePosition = trackPoint->GetPrePosition();
      TVector3 PostPosition = trackPoint->GetPostPosition();
      if (!isPtIntoSFGD(PrePosition)) continue;

      double Time = trackPoint->GetTime();
      TVector3 Momentum = trackPoint->GetMomentum();
      double Edep = trackPoint->GetEdep();
      double StepLength = trackPoint->GetStepLength();
      double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      string PhysVolName = trackPoint->GetPhysVolName();
      std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1";
      if (PhysVolName.find(to_replace) != std::string::npos)
        PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"~");
      std::string PreProcessName = trackPoint->GetPreProcessName();
      std::string PostProcessName = trackPoint->GetPostProcessName();
      std::string MaterialName = trackPoint->GetMaterialName();
      int PointID = trackPoint->GetPointID();
      bool IsOnBoundary = trackPoint->IsOnBoundary();
      // std::cout << "  Tp:" << PointID
      //           << " |M|:" << Momentum.Mag()
      //           << " dE:" << Edep
      //           << " dL:" << StepLength
      //           << " " << PreProcessName << " " << PostProcessName << " " << MaterialName
      //           << " Bnd:" << IsOnBoundary
      //           //  << " X:" << PrePosition.X()
      //           //   << " Y:" << PrePosition.Y()
      //             << " Z:" << PrePosition.Z()
      //           //  << " X:" << PostPosition.X()
      //           //   << " Y:" << PostPosition.Y()
      //             << " Z:" << PostPosition.Z()
      //           << " " << PhysVolName
      //           << std::endl;

      TVector3 AvgPos = (PostPosition + PrePosition);
      AvgPos *= 0.5;

            mt_pt_z.push_back(AvgPos.Z());
      mt_de.push_back(Edep);

      if ( AvgPos.Z() < cube_fin_z_pos && AvgPos.Z() >= cube_ini_z_pos )
      {
        de_cube += Edep;
      } else {
        mt_de_cubes.push_back(de_cube);
        mt_pt_z_cubes.push_back(dz_prev);
        zLayer++;
        cube_ini_z_pos += 10.0; // mm
        cube_fin_z_pos += 10.0;
        // if (cube_fin_z_pos > 920.0) break;
        de_cube = Edep;
        dz_prev = AvgPos.Z();
      }
      
    }

    main_track_pt_z.push_back(mt_pt_z);
    main_track_de.push_back(mt_de);

    for (int i = 0; i < mt_pt_z_cubes.size(); ++i)
    {
      std::cout << " Cube:" << i
                << " Z:" << mt_pt_z_cubes[i]
                << " dE:" << mt_de_cubes[i]
                << std::endl;
    }

    ei.cube0_edep->Fill(mt_de_cubes[0]);
    ei.cube1_edep->Fill(mt_de_cubes[1]);
    ei.cube2_edep->Fill(mt_de_cubes[2]);
    ei.cube3_edep->Fill(mt_de_cubes[3]);

  }

  std::cout << "== NeventsWOgamma == " << NeventsWOgamma << std::endl;

  std::string out_fname_cube0 = output_dir_tag + "/cube0_edep-200ev.pdf";
  std::string out_fname_cube1 = output_dir_tag + "/cube1_edep-200ev.pdf";
  std::string out_fname_cube2 = output_dir_tag + "/cube2_edep-200ev.pdf";
  std::string out_fname_cube3 = output_dir_tag + "/cube3_edep-200ev.pdf";


  TCanvas* c0 = new TCanvas("cube0_edep;dE/dx, MeV;N events", "cube0_edep", 1080, 720);
  c0->cd();
  ei.cube0_edep->Draw();
  ei.cube0_edep->SetFillColor(kRed);
  c0->SaveAs(out_fname_cube0.c_str());
  c0->Close();

  TCanvas* c1 = new TCanvas("cube1_edep;dE/dx, MeV;N events", "cube1_edep", 1080, 720);
  c1->cd();
  ei.cube1_edep->Draw();
  ei.cube1_edep->SetFillColor(kRed);
  c1->SaveAs(out_fname_cube1.c_str());
  c1->Close();

  TCanvas* c2 = new TCanvas("cube2_edep;dE/dx, MeV;N events", "cube2_edep", 1080, 720);
  c2->cd();
  ei.cube2_edep->Draw();
  ei.cube2_edep->SetFillColor(kRed);
  c2->SaveAs(out_fname_cube2.c_str());
  c2->Close();

  TCanvas* c3 = new TCanvas("cube3_edep;dE/dx, MeV;N events", "cube3_edep", 1080, 720);
  c3->cd();
  ei.cube3_edep->Draw();
  ei.cube3_edep->SetFillColor(kRed);
  c3->SaveAs(out_fname_cube3.c_str());
  c3->Close();

  // std::cout << "====================================" << std::endl;
  // ofstream out_csv(out_csv_path);
  // out_csv << "id,particle,L(cm),dE(MeV),dE_hits(MeV),dE/dx(MeV/cm),has_gamma->e+/e-\n";
  // for (int i = 0; i < NTreeEntries; ++i)
  // {
  //   std::cout << i << " P:" << ei.main_particle[i]
  //             << " L(cm):" << ei.main_particle_range[i]
  //             << " dE(MeV):" << ei.main_particle_edep[i]
  //             << " dE_hits(MeV):" << ei.main_particle_dedx_hits[i]
  //             << " dE/dx(MeV/cm):" << ei.main_particle_dedx[i]
  //             << " has_gamma->e+/e-:" << ei.main_particle_has_elepos_pair[i]
  //   << std::endl;

  //   out_csv << i << "," << ei.main_particle[i]
  //             << "," << ei.main_particle_range[i]
  //             << "," << ei.main_particle_edep[i]
  //             << "," << ei.main_particle_dedx_hits[i]
  //             << "," << ei.main_particle_dedx[i]
  //             << "," << ei.main_particle_has_elepos_pair[i]
  //   << "\n";
  // }
  // out_csv.close();

  // test();

  return 0;
}