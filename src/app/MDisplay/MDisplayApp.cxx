

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
        (GPPDG == 11 & GParentID == 1 & PPDG == 22 & track_proc == "conv")
        // ||
        // (PDG == 22 & ParentID == 1)
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


int main(int argc, char **argv)
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
  if (res != 0)
  {
    return -1;
  }

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
  
  int NTreeEntries = tinput->GetEntries();

  if (argc == 4)
  {
    int eventIdToProcess = atoi(argv[3]);
    process_event(tinput, nd280UpEvent, eventIdToProcess, output_dir_tag, ei);
    return 0;
  }

  for (int i = 0; i < NTreeEntries; ++i)
  {
    process_event(tinput, nd280UpEvent, i, output_dir_tag, ei);
  }

  std::cout << "====================================" << std::endl;
  /// fill histograms here
  ofstream out_csv(out_csv_path);
  out_csv << "id,particle,L(cm),dE(MeV),dE_hits(MeV),dE/dx(MeV/cm),has_gamma->e+/e-\n";
  for (int i = 0; i < NTreeEntries; ++i)
  {
    std::cout << i << " P:" << ei.main_particle[i]
              << " L(cm):" << ei.main_particle_range[i]
              << " dE(MeV):" << ei.main_particle_edep[i]
              << " dE_hits(MeV):" << ei.main_particle_dedx_hits[i]
              << " dE/dx(MeV/cm):" << ei.main_particle_dedx[i]
              << " has_gamma->e+/e-:" << ei.main_particle_has_elepos_pair[i]
    << std::endl;

    out_csv << i << "," << ei.main_particle[i]
              << "," << ei.main_particle_range[i]
              << "," << ei.main_particle_edep[i]
              << "," << ei.main_particle_dedx_hits[i]
              << "," << ei.main_particle_dedx[i]
              << "," << ei.main_particle_has_elepos_pair[i]
    << "\n";
  }
  out_csv.close();

  // test();

  return 0;
}