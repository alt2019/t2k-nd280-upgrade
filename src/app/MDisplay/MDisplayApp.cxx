

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>

#include <TString.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TString.h>

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

  for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    int TrackID = track->GetTrackID();
    int ParentID = track->GetParentID();
    std::string ProcessName = track->GetProcessName();
    int PDG = track->GetPDG();
    double InitKinEnergy = track->GetInitKinEnergy();

    std::cout << "Track: " << trjID << " " << TrackID << " " << ParentID << " " << PDG << " " << InitKinEnergy << " " << ProcessName
              << " " << track->GetNPoints()
              << std::endl;


    if (ParentID == 0) { primary_track = track; }

    if (ParentID == 1 && PDG == 22)
    {
      secondary_tracks.push_back(track);
    }

    if (ProcessName == "conv")
    {
      tertiary_tracks.push_back(track);
    }

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

  if (!tertiary_tracks.empty())
  {
    for (auto& track: tertiary_tracks)
    {
      // int TrackID = track->GetTrackID();
      int ParentID = track->GetParentID();
      // int GParentID = tid_gpid[TrackID];
      std::string ProcessName = track->GetProcessName();
      int PDG = track->GetPDG();
      double InitKinEnergy = track->GetInitKinEnergy();
      std::cout << PDG << " " << ParentID << " " << InitKinEnergy << std::endl;
    }
  }

  if (primary_track)
  {
    int NPoints = primary_track->GetNPoints();

    for (int ipt = 0; ipt < NPoints; ++ipt)
    {
      TND280UpTrackPoint* trackPoint = primary_track->GetPoint(ipt);
    }
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

  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

  std::size_t found = input_rootfilename.find_last_of("/\\");
  std::string path = input_rootfilename.substr(0,found+1);
  std::string file = input_rootfilename.substr(found+1);
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

  // test();

  return 0;
}