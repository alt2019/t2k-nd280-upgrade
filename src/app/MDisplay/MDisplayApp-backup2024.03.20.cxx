

#include <iostream>
#include <iomanip>
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
#include <TMath.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPad.h>
#include <TStyle.h>
#include <TPaveText.h>

#include <G4String.hh>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"

#include "src/SVG.hh"
// #include "src/utils.hh"

#include "src/old/ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
#include "src/old/AnalizeEvents.hh"
#include "src/old/SimulationAnalizer.hh"
#include "src/old/particlesDefinition.hh"


// bool isPtInsideSFGD(std::string detname)
bool isPtInsideSFGD(G4String detname)
{
  if (
    detname.contains("/t2k/OA/Magnet/Basket/target1/TargetUniform") ||
    detname.contains("/t2k/OA/Magnet/Basket/target1/SuperFGD1") || 
    detname.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") ||
    detname.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1")
  ) return true;

  return false;
}

std::map<std::tuple<int,std::string>, TH1F*> generate_hists_map(double max_ene, int n_bins)
{
  std::map<std::tuple<int,std::string>, TH1F*> hists_map;



  hists_map[{22, "B"}] = new TH1F(
    "#gamma left sfgd in B kinetic energy distribution",
    "#gamma left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "T"}] = new TH1F(
    "#gamma left sfgd in T kinetic energy distribution",
    "#gamma left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "FWD"}] = new TH1F(
    "#gamma left sfgd in FWD kinetic energy distribution",
    "#gamma left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "BWD"}] = new TH1F(
    "#gamma left sfgd in BWD kinetic energy distribution",
    "#gamma left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "L"}] = new TH1F(
    "#gamma left sfgd in L kinetic energy distribution",
    "#gamma left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "R"}] = new TH1F(
    "#gamma left sfgd in R kinetic energy distribution",
    "#gamma left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{22, "-"}] = new TH1F(
    "#gamma left sfgd in Undefined plane kinetic energy distribution",
    "#gamma left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



  hists_map[{11, "B"}] = new TH1F(
    "e- left sfgd in B kinetic energy distribution",
    "e- left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "T"}] = new TH1F(
    "e- left sfgd in T kinetic energy distribution",
    "e- left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "FWD"}] = new TH1F(
    "e- left sfgd in FWD kinetic energy distribution",
    "e- left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "BWD"}] = new TH1F(
    "e- left sfgd in BWD kinetic energy distribution",
    "e- left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "L"}] = new TH1F(
    "e- left sfgd in L kinetic energy distribution",
    "e- left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "R"}] = new TH1F(
    "e- left sfgd in R kinetic energy distribution",
    "e- left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{11, "-"}] = new TH1F(
    "e- left sfgd in Undefined plane kinetic energy distribution",
    "e- left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



hists_map[{-11, "B"}] = new TH1F(
    "e+ left sfgd in B kinetic energy distribution",
    "e+ left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "T"}] = new TH1F(
    "e+ left sfgd in T kinetic energy distribution",
    "e+ left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "FWD"}] = new TH1F(
    "e+ left sfgd in FWD kinetic energy distribution",
    "e+ left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "BWD"}] = new TH1F(
    "e+ left sfgd in BWD kinetic energy distribution",
    "e+ left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "L"}] = new TH1F(
    "e+ left sfgd in L kinetic energy distribution",
    "e+ left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "R"}] = new TH1F(
    "e+ left sfgd in R kinetic energy distribution",
    "e+ left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-11, "-"}] = new TH1F(
    "e+ left sfgd in Undefined plane kinetic energy distribution",
    "e+ left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );


  hists_map[{2212, "B"}] = new TH1F(
    "proton left sfgd in B kinetic energy distribution",
    "proton left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "T"}] = new TH1F(
    "proton left sfgd in T kinetic energy distribution",
    "proton left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "FWD"}] = new TH1F(
    "proton left sfgd in FWD kinetic energy distribution",
    "proton left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "BWD"}] = new TH1F(
    "proton left sfgd in BWD kinetic energy distribution",
    "proton left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "L"}] = new TH1F(
    "proton left sfgd in L kinetic energy distribution",
    "proton left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "R"}] = new TH1F(
    "proton left sfgd in R kinetic energy distribution",
    "proton left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2212, "-"}] = new TH1F(
    "proton left sfgd in Undefined plane kinetic energy distribution",
    "proton left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );



  hists_map[{2112, "B"}] = new TH1F(
    "neutron left sfgd in B kinetic energy distribution",
    "neutron left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "T"}] = new TH1F(
    "neutron left sfgd in T kinetic energy distribution",
    "neutron left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "FWD"}] = new TH1F(
    "neutron left sfgd in FWD kinetic energy distribution",
    "neutron left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "BWD"}] = new TH1F(
    "neutron left sfgd in BWD kinetic energy distribution",
    "neutron left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "L"}] = new TH1F(
    "neutron left sfgd in L kinetic energy distribution",
    "neutron left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "R"}] = new TH1F(
    "neutron left sfgd in R kinetic energy distribution",
    "neutron left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{2112, "-"}] = new TH1F(
    "neutron left sfgd in Undefined plane kinetic energy distribution",
    "neutron left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );


  hists_map[{-1, "B"}] = new TH1F(
    "tracks left sfgd in B kinetic energy distribution",
    "tracks left sfgd in B kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "T"}] = new TH1F(
    "tracks left sfgd in T kinetic energy distribution",
    "tracks left sfgd in T kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "FWD"}] = new TH1F(
    "tracks left sfgd in FWD kinetic energy distribution",
    "tracks left sfgd in FWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "BWD"}] = new TH1F(
    "tracks left sfgd in BWD kinetic energy distribution",
    "tracks left sfgd in BWD kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "L"}] = new TH1F(
    "tracks left sfgd in L kinetic energy distribution",
    "tracks left sfgd in L kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "R"}] = new TH1F(
    "tracks left sfgd in R kinetic energy distribution",
    "tracks left sfgd in R kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );
  hists_map[{-1, "-"}] = new TH1F(
    "tracks left sfgd in Undefined plane kinetic energy distribution",
    "tracks left sfgd in Undefined plane kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  hists_map[{-1, "B"}]->SetFillColor(kBlue);
  hists_map[{-1, "T"}]->SetFillColor(kCyan);
  hists_map[{-1, "FWD"}]->SetFillColor(kGreen);
  hists_map[{-1, "BWD"}]->SetFillColor(kRed);
  hists_map[{-1, "L"}]->SetFillColor(kYellow);
  hists_map[{-1, "R"}]->SetFillColor(kOrange);
  hists_map[{-1, "-"}]->SetFillColor(kBlack);

  hists_map[{22, "B"}]->SetFillColor(kBlue);
  hists_map[{22, "T"}]->SetFillColor(kCyan);
  hists_map[{22, "FWD"}]->SetFillColor(kGreen);
  hists_map[{22, "BWD"}]->SetFillColor(kRed);
  hists_map[{22, "L"}]->SetFillColor(kYellow);
  hists_map[{22, "R"}]->SetFillColor(kOrange);
  hists_map[{22, "-"}]->SetFillColor(kBlack);

  hists_map[{11, "B"}]->SetFillColor(kBlue);
  hists_map[{11, "T"}]->SetFillColor(kCyan);
  hists_map[{11, "FWD"}]->SetFillColor(kGreen);
  hists_map[{11, "BWD"}]->SetFillColor(kRed);
  hists_map[{11, "L"}]->SetFillColor(kYellow);
  hists_map[{11, "R"}]->SetFillColor(kOrange);
  hists_map[{11, "-"}]->SetFillColor(kBlack);

  hists_map[{-11, "B"}]->SetFillColor(kBlue);
  hists_map[{-11, "T"}]->SetFillColor(kCyan);
  hists_map[{-11, "FWD"}]->SetFillColor(kGreen);
  hists_map[{-11, "BWD"}]->SetFillColor(kRed);
  hists_map[{-11, "L"}]->SetFillColor(kYellow);
  hists_map[{-11, "R"}]->SetFillColor(kOrange);
  hists_map[{-11, "-"}]->SetFillColor(kBlack);

  hists_map[{2212, "B"}]->SetFillColor(kBlue);
  hists_map[{2212, "T"}]->SetFillColor(kCyan);
  hists_map[{2212, "FWD"}]->SetFillColor(kGreen);
  hists_map[{2212, "BWD"}]->SetFillColor(kRed);
  hists_map[{2212, "L"}]->SetFillColor(kYellow);
  hists_map[{2212, "R"}]->SetFillColor(kOrange);
  hists_map[{2212, "-"}]->SetFillColor(kBlack);

  hists_map[{2112, "B"}]->SetFillColor(kBlue);
  hists_map[{2112, "T"}]->SetFillColor(kCyan);
  hists_map[{2112, "FWD"}]->SetFillColor(kGreen);
  hists_map[{2112, "BWD"}]->SetFillColor(kRed);
  hists_map[{2112, "L"}]->SetFillColor(kYellow);
  hists_map[{2112, "R"}]->SetFillColor(kOrange);
  hists_map[{2112, "-"}]->SetFillColor(kBlack);

  return hists_map;
}



struct EventsInfo
{
  std::map<int, TND280UpEvent*> events_map;

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


struct PointInfo
{
  int ID;
  double Time;
  TVector3 Momentum;
  double Ek;
  double Edep;
  double StepLength;
  double StepDeltaLyz;
  int StepStatus;
  TVector3 PrePosition;
  TVector3 PostPosition;
  TVector3 AvgPosition;
  bool IsOnBoundary;
  std::string PhysVolName;
  std::string LogVolName;
  std::string LogVolNameShort;
  std::string PreProcessName;
  std::string PostProcessName;
  std::string MaterialName;
  bool IsIntoSFGD;

  PointInfo(TND280UpTrackPoint* trackPoint, TND280UpTrack* ownerTrack)
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

    AvgPosition = (PrePosition + PostPosition) * 0.5;
    
    IsIntoSFGD = isPtInsideSFGD(LogVolName);

    std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ";
    size_t pos = LogVolName.find(to_replace);
    LogVolNameShort = LogVolName;
    if (pos != std::string::npos) LogVolNameShort.replace(pos, to_replace.length(), "~");

    Ek = -1.0;
  }

  void SetKinEnergyFromMomentum(int OwnerTrackPDG) { Ek = compute_energy_from_momentum(OwnerTrackPDG, Momentum.Mag()); }

  void print()
  {
    std::cout << "\t"
      << ID << " "
      << "dE: " << std::setw(10) << Edep << " "
      << "dL: " << std::setw(10) << StepLength << " "
      << "|Mom|: " << std::setw(10) << Momentum.Mag() << " "
      << "|Ek|: " << std::setw(10) << Ek << " "
      << "inSFGD: " << IsIntoSFGD << " "
      // << "Mat: "<< MaterialName
      << "V: "<< LogVolNameShort
      << std::endl;
  }
};


struct TrackInfo
{
  int ID;
  int ParentID;
  int GParentID;
  int PDG;
  int NPoints;
  int Level;
  double InitKinEnergy;
  double InitCosTheta;
  double TotalEnergy;
  TVector3 InitMom;
  TVector3 StartPos;
  std::string ProcessName;
  // std::map<int, PointInfo> points; // {pt_id, pt}
  std::vector<PointInfo> points;
  std::vector<int> daughter_tracks_ids;
  bool startedInSFGD;
  bool stoppedInSFGD;
  double dE_over_pts_in_sfgd;
  double dE_over_all_pts;
  double left_SFGD_with_Ek;
  std::string left_SFGD_plane;

  TrackInfo(TND280UpTrack* track)
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

    GParentID = -1;
    Level = -1;
    left_SFGD_plane = "-";

    startedInSFGD = false;
    stoppedInSFGD = false;

    dE_over_pts_in_sfgd = 0.0;
    dE_over_all_pts = 0.0;
    for (int ipt = 0; ipt < NPoints; ++ipt)
    {
      TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);
      PointInfo pt(trackPoint, track);
      pt.SetKinEnergyFromMomentum(PDG);
      points.push_back(pt);

      if (pt.IsIntoSFGD) dE_over_pts_in_sfgd += pt.Edep;
      dE_over_all_pts += pt.Edep;

      if (pt.IsIntoSFGD)
      {
        if (ipt == NPoints-1) {stoppedInSFGD = true;}
        else if (ipt == 0) {startedInSFGD = true;}
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
          // std::cout << avgpos.X() << " " << avgpos.Y() << " " << avgpos.Z() << std::endl;
          if      (avgpos.X() >=  960.0     ) left_SFGD_plane = "R"; // right YZ+X
          else if (avgpos.X() <= -960.0     ) left_SFGD_plane = "L"; // left YZ-X
          else if (avgpos.Z() <= -920.0     ) left_SFGD_plane = "BWD"; // backward XY+Z
          else if (avgpos.Z() >=  920.0     ) left_SFGD_plane = "FWD"; // forward XY-Z
          // else if (avgpos.Y() >=  280.0+16.0) left_SFGD_plane = "T"; // top XZ+Y
          // else if (avgpos.Y() <= -280.0+16.0) left_SFGD_plane = "B"; // bottom XZ-Y
          else if (avgpos.Y() >=  280.0) left_SFGD_plane = "T"; // top XZ+Y
          else if (avgpos.Y() <= -280.0) left_SFGD_plane = "B"; // bottom XZ-Y
          // std::cout << avgpos.X() << " " << avgpos.Y() << " " << avgpos.Z() << " " << left_SFGD_plane << std::endl;
          break;
        }
      }
    }
  }

  void SetGParentID(int gparentid) { GParentID = gparentid; };
  void SetLevel(int level) { Level = level; };
  void AddDaughterTrack(int track_id) { daughter_tracks_ids.push_back(track_id); }

  void print()
  {
    std::cout 
      // << "\n\n\n"
      << "== "
      << " TID:" << std::setw(4) << ID
      << " PID:" << std::setw(4) << ParentID
      << " GPID:" << std::setw(4) << GParentID
      // << " (TPG:" << std::setw(4) << ID << ":" << std::setw(4) << ParentID << ":" << std::setw(4) << GParentID << ")"
      // << " L:" << std::setw(4) << Level
      << " PDG:" << std::setw(11) << PDG
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
      << " [" << StartPos.X() << "|" << StartPos.Y() << "|" << StartPos.Z() << "]"
      << " Proc:" << ProcessName
    << std::endl;

    // for (auto pt: points)
    // {
    //   pt.print();
    // }
  }
};

struct EventInfo
{
  int NVertices;
  int NHits;
  int NTracks;

  // TrackInfo *primary_lepton;
  std::unique_ptr<TrackInfo> primary_lepton;
  // TrackInfo primary_lepton;

  std::map<int, TrackInfo> tracks; // {trk_id, trk}

  EventInfo(TND280UpEvent *nd280UpEvent)
  {
    NVertices = nd280UpEvent->GetNVertices();
    NHits = nd280UpEvent->GetNHits();
    NTracks = nd280UpEvent->GetNTracks();

    for (int vtxID = 0; vtxID < NVertices; ++vtxID)
    {
      TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
      // vtx->PrintVertex();

      TND280UpTrack *trk0 = vtx->GetInTrack(0);
      TND280UpTrack *trk1 = vtx->GetInTrack(1);

      std::cout << " Vertex input Track 0: " << trk0->GetPDG() << " " << trk0->GetInitMom().Mag() << std::endl;
      std::cout << " Vertex input Track 1: " << trk1->GetPDG() << " " << trk1->GetInitMom().Mag() << std::endl;

      TVector3 vtx_pos = vtx->GetPosition();

      std::cout << " Vertex position: " << vtx_pos.X() << " " << vtx_pos.Y() << " " << vtx_pos.Z() << std::endl;
    }

    // primary_lepton = nullptr;

    std::vector<TrackInfo> tracks_v;
    std::vector<TrackInfo> primary_tracks_v;
    std::vector<int> primary_tracks_ids_v;
    std::vector<TrackInfo> remained_tracks;
    for (Int_t trjID = 0; trjID < NTracks; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
      TrackInfo trk(track);
      // TrackInfo trk = TrackInfo(track);

      tracks_v.push_back(trk);

      tracks.insert({trk.ID, trk});
      // tracks[trk.ID] = trk;

      // std::cout << " ---- " << trk.ID << " " << trk.ParentID << " " << trk.PDG << std::endl;

      if (trk.ParentID == 0 && (TMath::Abs(trk.PDG) == 11 || TMath::Abs(trk.PDG) == 13) )
      {
        // primary_lepton = &trk;
        primary_lepton = std::make_unique<TrackInfo>(trk);
        // std::cout << " ---- " << trk.ID << " " << trk.ParentID << " " << trk.PDG << std::endl;
      }

      if (trk.ParentID == 0)
      {
        primary_tracks_v.push_back(trk);
        primary_tracks_ids_v.push_back(trk.ID);
      } else {
        remained_tracks.push_back(trk);
      }
    }

    std::map<int, TrackInfo> new_tracks_map;
    for (auto item: tracks)
    {
      TrackInfo trk = item.second;
      int trk_parentid = trk.ParentID;
      if (trk_parentid == 0)
      {
        trk.SetGParentID(-1);
      }
      // else if (tracks.find(trk_parentid) == tracks.end())
      // {
      //   trk.SetGParentID(-2);
      // }
      else
      {
        TrackInfo parent_trk = tracks.at(trk_parentid);
        trk.SetGParentID(parent_trk.ParentID);
      }
      // trk.print();
      new_tracks_map.insert({trk.ID, trk});
    }

    tracks.clear();
    tracks = new_tracks_map;



    // if (remained_tracks.size())
    // {
    //   for (auto trk: remained_tracks)
    //   {
    //     if (std::find(primary_tracks_ids_v.begin(), primary_tracks_ids_v.end(), trk.ID) != primary_tracks_ids_v.end())
    //     {
    //     }
    //   }
    // }

    // for (auto item: tracks) // error here, to check
    // {
    //   TrackInfo trk = item.second;
    //   int trk_parentid = trk.ParentID;
    //   // if (trk_parentid == 0) { trk.SetGParentID(0); continue; }
    //   if (trk_parentid == 0)
    //   {
    //     // tracks.at(trk.ID).SetGParentID(0);
    //     continue;
    //   }
    //   TrackInfo parent_trk = tracks.at(trk_parentid);
    //   trk.SetGParentID(parent_trk.ParentID);
    //   tracks[trk_parentid] = parent_trk;
    // }

    // TrackInfo trk = tracks[0];
    // auto item = tracks[0];
    // TrackInfo trk = tracks.at(0);

    

  }

  void print()
  {
    std::cout << "NVertices " << NVertices << std::endl;
    std::cout << "NHits     " << NHits << std::endl;
    std::cout << "NTracks   " << NTracks << std::endl;

    // for (Int_t trjID = 0; trjID < NTracks; trjID++)
    // {
    //   TrackInfo trk = tracks.at(trjID);
    //   trk.print();
    // }

    if (primary_lepton)
    {
      std::cout << "  ~~Primary lepton~~  " << std::endl;
      // (*primary_lepton).print();
      primary_lepton->print();
      std::cout << "  ~~Primary lepton~~  " << std::endl;
    }

    double ek0_sum_of_all_tracks = 0.0;
    double ek0_sum_of_tracks_started_in_sfgd = 0.0;
    double ek0_sum_of_primary_tracks = 0.0;
    for (auto trk_item: tracks)
    {
      TrackInfo trk = trk_item.second;
      ek0_sum_of_all_tracks += trk.InitKinEnergy;
      if (!trk.startedInSFGD && !trk.stoppedInSFGD) continue;
      ek0_sum_of_tracks_started_in_sfgd += trk.InitKinEnergy;
      if (trk.ParentID == 0) ek0_sum_of_primary_tracks += trk.InitKinEnergy;
      // if (trk.startedInSFGD && trk.stoppedInSFGD) continue;
      trk.print();

    }
    std::cout << " ek0_sum_of_all_tracks: " << std::setw(11) << ek0_sum_of_all_tracks
              << "\n ek0_sum_of_tracks_started_in_sfgd: " << std::setw(11) << ek0_sum_of_tracks_started_in_sfgd
              << "\n ek0_sum_of_primary_tracks: " << std::setw(11) << ek0_sum_of_primary_tracks
    << std::endl;
  }

  std::vector<TrackInfo> getTracksLeftSFGD()
  {
    std::vector<TrackInfo> tracks_left_sfgd;
    for (auto trk_item: tracks)
    {
      TrackInfo trk = trk_item.second;
      // if (!trk.startedInSFGD && !trk.stoppedInSFGD) continue;
      // if (trk.startedInSFGD && trk.stoppedInSFGD) continue;
      // trk.print();
      if (trk.startedInSFGD && !trk.stoppedInSFGD) tracks_left_sfgd.push_back(trk);
    }

    return tracks_left_sfgd;
  }
};


std::string energy_best_units(double energy/*MeV*/)
{
  std::stringstream ss;
  if (energy >= 1000.0)
  {
    ss << std::setprecision(6) << energy / 1000.0 << "_GeV";
  }
  else if (energy >= 0.001 && energy < 1.0)
  {
    ss << std::setprecision(6) << energy * 1000.0 << "_KeV";
  }
  else if (energy >= 0.000001 && energy < 0.001)
  {
    ss << std::setprecision(6) << energy * 1000000.0 << "_eV";
  }
  else
  {
    ss << std::setprecision(6) << energy << "_MeV";
  }

  return ss.str();
}

void draw_1d_hist(TH1F* h, std::string out_fname, std::string canvas_title, bool setLogX = false, bool setLogY = false)
{
  TCanvas* c = new TCanvas(canvas_title.c_str(), canvas_title.c_str(), 1080, 720);
  c->cd();
  if (setLogX) gPad->SetLogx();
  if (setLogY) gPad->SetLogy();
  h->Draw();
  h->SetFillColor(kRed);
  c->SaveAs(out_fname.c_str());
  c->Close();
}


void analize_events_from_genie_v2(std::string input_rootfilename, std::string output_dir_tag)
{
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  
  // std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // double hist_max_edep = 3000.0;
  // double hist_max_edep = 1000.0;
  // double hist_max_edep = 500.0;
  // double hist_max_edep = 500.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  // double hist_max_edep = 5000.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  double hist_max_edep = 3000.0;
  int hist_max_bins_edep = 100;

  /// Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  TH1F* h_lepton_angle_distribution = new TH1F(
    "lepton angle distribution",
    "Lepton angle distribution;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_edep = new TH1F(
    "lepton edep",
    "Lepton energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );

  /*
  TH1F* h_lepton_angle_distribution_z = new TH1F(
    "lepton angle distribution z",
    "Lepton angle distribution z;cos #theta;N events", 
    60, -1.5, 1.5
  );
  TH1F* h_lepton_angle_distribution_x = new TH1F(
    "lepton angle distribution x",
    "Lepton angle distribution x;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_all_charged_partciles_edep = new TH1F(
    "all charged particles edep",
    "All charged particles energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );


  TH1F* h_vtx_distr_x = new TH1F(
    "X-vtx-distr",
    "Distribution of vertex position by X;Vertex X position, cm;N events", 
    200, -105.0, 105.0
  );
  TH1F* h_vtx_distr_y = new TH1F(
    "Y-vtx-distr",
    "Distribution of vertex position by Y;Vertex Y position, cm;N events", 
    80, -40.0, 40.0
    // 200, -100.0, 100.0
  );
  TH1F* h_vtx_distr_z = new TH1F(
    "Z-vtx-distr",
    "Distribution of vertex position by Z;Vertex Z position, cm;N events", 
    200, -105.0, 105.0
  );

  TH2F* h_angle_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_energy_primary_lepton_diplot_2",
    "Angle-energy;cos #theta;E, MeV", 
    // 60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
    60, -1.5, 1.5, 100, 0, 1000.0
  );

  TH2F* h_angle_initek_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_Ek0_primary_lepton_diplot_2",
    "Angle-initial kinetic energy;cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0.0, hist_max_edep
  );

  TH2F* h_angle_energy_all_charged_particles_diplot_2 = new TH2F(
    "angle_energy_all_charged_particles_diplot_2",
    "Angle-energy (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  TH2F* h_angle_Ek0_all_charged_particles_diplot_2 = new TH2F(
    "angle_Ek0_all_charged_particles_diplot_2",
    "Angle-Ek0 (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );
  // */

  // double max_ene = 2000.0;
  double max_ene = 3000.0;
  int n_bins = 100;

  TH1F* h_energies_of_gammas_left_sfgd = new TH1F(
    "#gamma left sfgd kinetic energy distribution",
    "#gamma left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_ele_left_sfgd = new TH1F(
    "e- left sfgd kinetic energy distribution",
    "e- left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_prot_left_sfgd = new TH1F(
    "proton left sfgd kinetic energy distribution",
    "proton left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_neut_left_sfgd = new TH1F(
    "neutron left sfgd knetic energy distribution",
    "neutron left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_energies_of_tracks_left_sfgd = new TH1F(
    "track left sfgd kinetic energy distribution",
    "track left sfgd kinetic energy distribution;Ek, MeV;N events", 
    n_bins, 0.0, max_ene
  );

  TH1F* h_ele_left_sfgd_per_event = new TH1F(
    "e- left sfgd per event",
    "e- left sfgd per event;N electrons;N events", 
    50, 0, 50
  );

  TH1F* h_gamma_left_sfgd_per_event = new TH1F(
    "#gamma left sfgd per event",
    "#gamma left sfgd per event;N #gamma;N events", 
    100, 0, 100
  );

  TH1F* h_ele_left_sfgd_carried_ene = new TH1F(
    "e- left sfgd carried energy",
    "e- left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  TH1F* h_gamma_left_sfgd_carried_ene = new TH1F(
    "#gamma left sfgd carried energy",
    "#gamma left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  TH1F* h_all_tracks_left_sfgd_carried_ene = new TH1F(
    "all tracks left sfgd carried energy",
    "all tracks left sfgd carried energy;E, MeV;N events",
    n_bins, 0.0, max_ene
  );

  std::map<std::tuple<int,std::string>, TH1F*> hists_map = generate_hists_map(max_ene, n_bins);


  int main_particle_PDG = -1;

  int NTreeEntries = tinput->GetEntries();

  
  // for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // for (int eventId = 1; eventId < 2; ++eventId)
  // for (int eventId = 1; eventId < 10; ++eventId)
  for (int eventId = 1; eventId < 100; ++eventId)
  // for (int eventId = 1; eventId < 500; ++eventId)
  // for (int eventId = 1; eventId < 1000; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;

    if (eventId == 1588) continue; // error in map.at somewhere in tracks processing

    tinput->GetEntry(eventId);
    
    EventInfo evtInfo(nd280UpEvent);
    evtInfo.print();

    // h_lepton_angle_distribution
    // std::unique_ptr<TrackInfo> primary_lepton = evtInfo.primary_lepton;
    // TrackInfo * primary_lepton = evtInfo.primary_lepton;
    if (evtInfo.primary_lepton)
    {
      h_lepton_angle_distribution->Fill(evtInfo.primary_lepton->InitCosTheta);
      h_lepton_edep->Fill(evtInfo.primary_lepton->dE_over_pts_in_sfgd);
    }

    std::vector<TrackInfo> TracksLeftSFGD = evtInfo.getTracksLeftSFGD();
    int n_out_ele = 0;
    int n_out_gamma = 0;
    double ene_carried_by_left_ele = 0.0;
    double ene_carried_by_left_gamma = 0.0;
    double total_energy_carried_by_all_left_tracks = 0.0;

    for (auto trk: TracksLeftSFGD)
    {
      double Ek_out = trk.left_SFGD_with_Ek;
      if (trk.PDG == 22)
      {
        h_energies_of_gammas_left_sfgd->Fill(Ek_out);
        hists_map.at({22, trk.left_SFGD_plane})->Fill(Ek_out);
        n_out_gamma++;
        ene_carried_by_left_gamma += Ek_out;
      }
      else if (trk.PDG == 11)
      {
        h_energies_of_ele_left_sfgd->Fill(Ek_out);
        hists_map.at({11, trk.left_SFGD_plane})->Fill(Ek_out);
        n_out_ele++;
        ene_carried_by_left_ele += Ek_out;
      }
      else if (trk.PDG == -11)
      {
        // h_energies_of_ele_left_sfgd->Fill(Ek_out);
        hists_map.at({-11, trk.left_SFGD_plane})->Fill(Ek_out);
        // n_out_ele++;
        // ene_carried_by_left_ele += Ek_out;
      }
      else if (trk.PDG == 2212)
      {
        h_energies_of_prot_left_sfgd->Fill(Ek_out);
        hists_map.at({2212, trk.left_SFGD_plane})->Fill(Ek_out);
      }
      else if (trk.PDG == 2112)
      {
        h_energies_of_neut_left_sfgd->Fill(Ek_out);
        hists_map.at({2112, trk.left_SFGD_plane})->Fill(Ek_out);
      }
      h_energies_of_tracks_left_sfgd->Fill(Ek_out);
      hists_map.at({-1, trk.left_SFGD_plane})->Fill(Ek_out);
      total_energy_carried_by_all_left_tracks += Ek_out;
    }

    h_ele_left_sfgd_per_event->Fill(n_out_ele);
    h_gamma_left_sfgd_per_event->Fill(n_out_gamma);
    h_ele_left_sfgd_carried_ene->Fill(ene_carried_by_left_ele);
    h_gamma_left_sfgd_carried_ene->Fill(ene_carried_by_left_gamma);
    h_all_tracks_left_sfgd_carried_ene->Fill(total_energy_carried_by_all_left_tracks);
  }

  std::string out_fname_primary_lepton_angle_distr = output_dir_tag + "/primary_lepton_angle_distr.pdf";
  draw_1d_hist(h_lepton_angle_distribution, out_fname_primary_lepton_angle_distr, "primary lepton angle distribution");

  std::string out_fname_primary_lepton_dedx_distr = output_dir_tag + "/primary_lepton_dedx_distr.pdf";
  draw_1d_hist(h_lepton_edep, out_fname_primary_lepton_dedx_distr, "primary lepton angle distribution");

  std::string out_fname_gammas_left_sfgd = output_dir_tag + "/gammas_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_gammas_left_sfgd, out_fname_gammas_left_sfgd, "#gamma left SFGD Ek", false, true);

  std::string out_fname_ele_left_sfgd = output_dir_tag + "/e-_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_ele_left_sfgd, out_fname_ele_left_sfgd, "e- left SFGD Ek", false, true);

  std::string out_fname_prot_left_sfgd = output_dir_tag + "/prot_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_prot_left_sfgd, out_fname_prot_left_sfgd, "prot left SFGD Ek", false, true);

  std::string out_fname_trks_left_sfgd = output_dir_tag + "/trks_left_sfgd-Ek.pdf";
  draw_1d_hist(h_energies_of_tracks_left_sfgd, out_fname_trks_left_sfgd, "tracks left SFGD Ek", false, true);

  std::string out_fname_n_ele_left_sfgd = output_dir_tag + "/e-_left_sfgd-per_event.pdf";
  draw_1d_hist(h_ele_left_sfgd_per_event, out_fname_n_ele_left_sfgd, "e- left SFGD per event");

  std::string out_fname_n_gamma_left_sfgd = output_dir_tag + "/gamma_left_sfgd-per_event.pdf";
  draw_1d_hist(h_gamma_left_sfgd_per_event, out_fname_n_gamma_left_sfgd, "#gamma left SFGD per event");

  std::string out_fname_ele_left_sfgd_carrene = output_dir_tag + "/e-_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_ele_left_sfgd_carried_ene, out_fname_ele_left_sfgd_carrene, "e- left SFGD carried energy");

  std::string out_fname_gamma_left_sfgd_carrene = output_dir_tag + "/gamma_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_gamma_left_sfgd_carried_ene, out_fname_gamma_left_sfgd_carrene, "#gamma left SFGD carried energy");

  std::string out_fname_all_tracks_left_sfgd_carrene = output_dir_tag + "/all_tracks_left_sfgd-carried_energy.pdf";
  draw_1d_hist(h_all_tracks_left_sfgd_carried_ene, out_fname_all_tracks_left_sfgd_carrene, "all tracks left SFGD carried energy");


  
  THStack hs_gamma("hs_gamma","#gamma left SFGD Ek;Ekin, MeV;N events");
  THStack hs_ele("hs_ele","e- left SFGD Ek;Ekin, MeV;N events");
  THStack hs_prot("hs_prot","proton left SFGD Ek;Ekin, MeV;N events");
  THStack hs_neut("hs_neut","neutron left SFGD Ek;Ekin, MeV;N events");
  THStack hs_trks("hs_trks","tracks left SFGD Ek;Ekin, MeV;N events");

  TLegend *hs_gamma_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_ele_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_prot_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_neut_leg = new TLegend(0.6,0.6,0.9,0.9);
  TLegend *hs_trks_leg = new TLegend(0.6,0.6,0.9,0.9);

  for (auto &item: hists_map)
  {
    std::tuple<int, std::string> pdg_plane = item.first;
    int pdg = std::get<0>(pdg_plane);
    std::string plane = std::get<1>(pdg_plane);
    TH1F* h = item.second;
    int N_entries = h->GetEntries();
    std::stringstream leg_item;
    leg_item << "left at " << plane << "; " << N_entries << " events";
    h->SetMinimum(0.0);
    // h->SetMaximum(100);

    if (pdg == 22)
    {
      hs_gamma.Add(h);
      hs_gamma_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 11)
    {
      hs_ele.Add(h);
      hs_ele_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 2212)
    {
      hs_prot.Add(h);
      hs_prot_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == 2112)
    {
      hs_neut.Add(h);
      hs_neut_leg->AddEntry(h, leg_item.str().c_str());
    }
    else if (pdg == -1)
    {
      hs_trks.Add(h);
      hs_trks_leg->AddEntry(h, leg_item.str().c_str());
    }
  }

  std::string out_fname_hs_gamma = output_dir_tag + "/gamma_left_sfgd-plane_hs.pdf";
  TCanvas* c_gamma_left_sfgd_hs = new TCanvas("gamma_left_sfgd-plane", "gamma_left_sfgd-plane", 1080, 720);
  c_gamma_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_gamma.Draw();
  hs_gamma_leg->Draw();
  c_gamma_left_sfgd_hs->SaveAs(out_fname_hs_gamma.c_str());
  c_gamma_left_sfgd_hs->Close();

  std::string out_fname_hs_ele = output_dir_tag + "/e-_left_sfgd-plane_hs.pdf";
  TCanvas* c_ele_left_sfgd_hs = new TCanvas("e-_left_sfgd-plane", "e-_left_sfgd-plane", 1080, 720);
  c_ele_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_ele.Draw();
  hs_ele_leg->Draw();
  c_ele_left_sfgd_hs->SaveAs(out_fname_hs_ele.c_str());
  c_ele_left_sfgd_hs->Close();

  std::string out_fname_hs_prot = output_dir_tag + "/proton_left_sfgd-plane_hs.pdf";
  TCanvas* c_prot_left_sfgd_hs = new TCanvas("proton_left_sfgd-plane", "proton_left_sfgd-plane", 1080, 720);
  c_prot_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_prot.Draw();
  hs_prot_leg->Draw();
  c_prot_left_sfgd_hs->SaveAs(out_fname_hs_prot.c_str());
  c_prot_left_sfgd_hs->Close();

  std::string out_fname_hs_neut = output_dir_tag + "/neutron_left_sfgd-plane_hs.pdf";
  TCanvas* c_neut_left_sfgd_hs = new TCanvas("neutron_left_sfgd-plane", "neutron_left_sfgd-plane", 1080, 720);
  c_neut_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_neut.Draw();
  hs_neut_leg->Draw();
  c_neut_left_sfgd_hs->SaveAs(out_fname_hs_neut.c_str());
  c_neut_left_sfgd_hs->Close();

  std::string out_fname_hs_tracks = output_dir_tag + "/tracks_left_sfgd-plane_hs.pdf";
  TCanvas* c_trks_left_sfgd_hs = new TCanvas("tracks_left_sfgd-plane", "tracks_left_sfgd-plane", 1080, 720);
  c_trks_left_sfgd_hs->cd();
  gPad->SetLogy();
  hs_trks.Draw();
  hs_trks_leg->Draw();
  // c_trks_left_sfgd_hs->BuildLegend();
  c_trks_left_sfgd_hs->SaveAs(out_fname_hs_tracks.c_str());
  c_trks_left_sfgd_hs->Close();
}


void analize_events_from_genie(std::string input_rootfilename, std::string output_dir_tag)
{
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  
  // std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // double hist_max_edep = 3000.0;
  // double hist_max_edep = 1000.0;
  // double hist_max_edep = 500.0;
  // double hist_max_edep = 500.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  // double hist_max_edep = 5000.0*1.3;
  // int hist_max_bins_edep = 100*1.3;
  double hist_max_edep = 3000.0;
  int hist_max_bins_edep = 100;

  /// Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  TH1F* h_lepton_angle_distribution = new TH1F(
    "lepton angle distribution",
    "Lepton angle distribution;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_angle_distribution_z = new TH1F(
    "lepton angle distribution z",
    "Lepton angle distribution z;cos #theta;N events", 
    60, -1.5, 1.5
  );
  TH1F* h_lepton_angle_distribution_x = new TH1F(
    "lepton angle distribution x",
    "Lepton angle distribution x;cos #theta;N events", 
    60, -1.5, 1.5
  );

  TH1F* h_lepton_edep = new TH1F(
    "lepton edep",
    "Lepton energy deposit;E deposited, MeV;N events", 
    // hist_max_bins_edep, 0.0, hist_max_edep
    100, 0.0, 1000
  );

  TH1F* h_all_charged_partciles_edep = new TH1F(
    "all charged particles edep",
    "All charged particles energy deposit;E deposited, MeV;N events", 
    hist_max_bins_edep, 0.0, hist_max_edep
  );


  TH1F* h_vtx_distr_x = new TH1F(
    "X-vtx-distr",
    "Distribution of vertex position by X;Vertex X position, cm;N events", 
    200, -105.0, 105.0
  );
  TH1F* h_vtx_distr_y = new TH1F(
    "Y-vtx-distr",
    "Distribution of vertex position by Y;Vertex Y position, cm;N events", 
    80, -40.0, 40.0
    // 200, -100.0, 100.0
  );
  TH1F* h_vtx_distr_z = new TH1F(
    "Z-vtx-distr",
    "Distribution of vertex position by Z;Vertex Z position, cm;N events", 
    200, -105.0, 105.0
  );

  TH2F* h_angle_energy_primary_lepton_diplot = new TH2F(
    "angle_energy_primary_lepton_diplot",
    "Angle-energy;E, MeV, MeV;cos #theta", 
    hist_max_bins_edep, 0, hist_max_edep, 60, -1.5, 1.5
  );
  TH2F* h_angle_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_energy_primary_lepton_diplot_2",
    "Angle-energy;cos #theta;E, MeV", 
    // 60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
    60, -1.5, 1.5, 100, 0, 1000.0
  );

  TH2F* h_angle_initek_energy_primary_lepton_diplot_2 = new TH2F(
    "angle_Ek0_primary_lepton_diplot_2",
    "Angle-initial kinetic energy;cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0.0, hist_max_edep
  );

  TH2F* h_angle_energy_all_charged_particles_diplot = new TH2F(
    "angle_energy_all_charged_particles_diplot",
    "Angle-energy (all charged particles);E, MeV;cos #theta", 
    hist_max_bins_edep, 0, hist_max_edep, 60, -1.5, 1.5
  );
  TH2F* h_angle_energy_all_charged_particles_diplot_2 = new TH2F(
    "angle_energy_all_charged_particles_diplot_2",
    "Angle-energy (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  TH2F* h_angle_Ek0_all_charged_particles_diplot_2 = new TH2F(
    "angle_Ek0_all_charged_particles_diplot_2",
    "Angle-Ek0 (all charged particles);cos #theta;E, MeV", 
    60, -1.5, 1.5, hist_max_bins_edep, 0, hist_max_edep
  );

  

  int main_particle_PDG = -1;

  int NTreeEntries = tinput->GetEntries();
  // for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // {
  //   std::cout << "\nEvent " << eventId << std::endl;
  //   tinput->GetEntry(eventId);

  //   int NVertices = nd280UpEvent->GetNVertices();

  //   for (int vtxID = 0; vtxID < NVertices; ++vtxID)
  //   {
  //     TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
  //     // vtx->PrintVertex();

  //     TND280UpTrack *trk0 = vtx->GetInTrack(0);
  //     TND280UpTrack *trk1 = vtx->GetInTrack(1);

  //     // std::cout << " Vertex input Track 0: " << trk0->GetPDG() << " " << trk0->GetInitMom().Mag() << std::endl;
  //     // std::cout << " Vertex input Track 1: " << trk1->GetPDG() << " " << trk1->GetInitMom().Mag() << std::endl;

  //     TVector3 vtx_pos = vtx->GetPosition();

  //     // std::cout << " Vertex position: " << vtx_pos.X() << " " << vtx_pos.Y() << " " << vtx_pos.Z() << std::endl;

  //     h_vtx_distr_x->Fill(vtx_pos.X() / 10.0);
  //     h_vtx_distr_y->Fill(vtx_pos.Y() / 10.0);
  //     h_vtx_distr_z->Fill(vtx_pos.Z() / 10.0);
  //     // h_vtx_distr_y->Fill(vtx_pos.Z() / 10.0);
  //     // h_vtx_distr_z->Fill(vtx_pos.Y() / 10.0);
  //   }
  // }


  // for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // for (int eventId = 1; eventId < 2; ++eventId)
  for (int eventId = 1; eventId < 10; ++eventId)
  // for (int eventId = 1; eventId < 100; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;
    tinput->GetEntry(eventId);

    int NVertices = nd280UpEvent->GetNVertices();
    int NHits = nd280UpEvent->GetNHits();
    int NTracks = nd280UpEvent->GetNTracks();

    std::cout << "NVertices " << NVertices << std::endl;
    std::cout << "NHits     " << NHits << std::endl;
    std::cout << "NTracks   " << NTracks << std::endl;

    TND280UpTrack* t_primary_lepton_track = nullptr;
    // std::vector<TND280UpTrack*> t_primary_tracks;

    bool skip_event = false;
    for (int vtxID = 0; vtxID < NVertices; ++vtxID)
    {
      TND280UpVertex *vtx = nd280UpEvent->GetVertex(vtxID);
      // vtx->PrintVertex();

      TND280UpTrack *trk0 = vtx->GetInTrack(0);
      TND280UpTrack *trk1 = vtx->GetInTrack(1);

      std::cout << " Vertex input Track 0: " << trk0->GetPDG() << " " << trk0->GetInitMom().Mag() << std::endl;
      std::cout << " Vertex input Track 1: " << trk1->GetPDG() << " " << trk1->GetInitMom().Mag() << std::endl;

      TVector3 vtx_pos = vtx->GetPosition();

      std::cout << " Vertex position: " << vtx_pos.X() << " " << vtx_pos.Y() << " " << vtx_pos.Z() << std::endl;

      if (vtx_pos.X() < -960.0+20.0 || vtx_pos.X() > 960.0-20.0) skip_event = true;
      if (vtx_pos.Z() < -920.0+20.0 || vtx_pos.Z() > 920.0-20.0) skip_event = true;
      if (vtx_pos.Y() < -280.0+16.0+20.0 || vtx_pos.Y() > 280.0+16.0-20.0) skip_event = true;

      h_vtx_distr_x->Fill(vtx_pos.X() / 10.0);
      h_vtx_distr_y->Fill(vtx_pos.Y() / 10.0);
      h_vtx_distr_z->Fill(vtx_pos.Z() / 10.0);
      // h_vtx_distr_y->Fill(vtx_pos.Z() / 10.0);
      // h_vtx_distr_z->Fill(vtx_pos.Y() / 10.0);
    }

    if (skip_event) continue;

    double sum_of_Ek0_for_out_vertex_particles = 0.0;
    double sum_of_Ek0_for_all_particles = 0.0;
    double sum_of_mom_for_out_vertex_particles = 0.0;
    double sum_of_mom_for_all_particles = 0.0;
    double total_deposited_energy_by_charged_particles = 0.0;

    std::map<int, TND280UpTrack*> tracks_map;
    std::map<int, double> tracks_edep_over_pts;

    int n_started_and_stopped_in_sfgd = 0;
    int n_stopped_in_sfgd_from_out = 0;
    int n_started_and_not_stopped_in_sfgd = 0;
    int n_other = 0;

    for (Int_t trjID = 0; trjID < NTracks; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
      int ParentID = track->GetParentID();
      int PDG = track->GetPDG();
      int NPoints = track->GetNPoints();
      int TrackID = track->GetTrackID();
      std::string ProcessName = track->GetProcessName();
      double InitKinEnergy = track->GetInitKinEnergy();
      double InitCosTheta = track->GetInitCosTheta();
      TVector3 InitMom = track->GetInitMom();

      std::cout 
                // << "\n\n\n"
                << "== "
                << " TID:" << TrackID
                << " PID:" << ParentID
                << " PDG:" << PDG
                << " Proc:" << ProcessName
                << " Ek0:" << InitKinEnergy
                << " cosT0:" << InitCosTheta
                << " Npt:" << NPoints
      << std::endl;

      sum_of_Ek0_for_all_particles += InitKinEnergy;
      sum_of_mom_for_all_particles += InitMom.Mag();

      tracks_map.insert({TrackID, track});

      bool is_started_inside_sfgd = false;
      bool is_stopped_inside_sfgd = false;

      // if (PDG != 22 && PDG != 2112 && PDG < 1000000000)
      {
        double trk_edep_over_pts = 0.0;
        double trk_edep_over_pts_full = 0.0;
        for (int ipt = 0; ipt < NPoints; ++ipt)
        {
          TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);

          // std::string material = trackPoint->GetMaterialName();

          // double Edep = trackPoint->GetEdep();
          // total_deposited_energy_by_charged_particles += Edep;

          /////
          int ptId = trackPoint->GetPointID();
          double ptTime = trackPoint->GetTime();
          TVector3 ptMomentum = trackPoint->GetMomentum();
          double ptEdep = trackPoint->GetEdep();
          double ptStepLength = trackPoint->GetStepLength();
          double ptStepDeltaLyz = trackPoint->GetStepDeltaLyz();
          int ptStepStatus = trackPoint->GetStepStatus();
          std::string ptPhysVolName = trackPoint->GetPhysVolName();
          std::string ptLogVolName = trackPoint->GetLogVolName();
          TVector3 ptPrePosition = trackPoint->GetPrePosition();
          TVector3 ptPostPosition = trackPoint->GetPostPosition();
          bool ptIsOnBoundary = trackPoint->IsOnBoundary();
          std::string ptPreProcessName = trackPoint->GetPreProcessName();
          std::string ptPostProcessName = trackPoint->GetPostProcessName();
          std::string ptMaterialName = trackPoint->GetMaterialName();
          
          bool is_in_sfgd = isPtInsideSFGD(ptLogVolName);

          std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ";
          size_t pos = ptLogVolName.find(to_replace);
          if (pos != std::string::npos) ptLogVolName.replace(pos, to_replace.length(), "~");

          double Ek_at_step = compute_energy_from_momentum(PDG, ptMomentum.Mag());
          /////

          /////
          std::string det = trackPoint->GetLogVolName();
          if (ipt == 0)
          {
            if (isPtInsideSFGD(det)) is_started_inside_sfgd = true;
          }
          else if (ipt == NPoints-1)
          {
            if (isPtInsideSFGD(det)) is_stopped_inside_sfgd = true;
          }

          if (isPtInsideSFGD(det))
          {
            double Edep = trackPoint->GetEdep();
            total_deposited_energy_by_charged_particles += Edep;
            trk_edep_over_pts += Edep;
          }
          /////
          trk_edep_over_pts_full += ptEdep;

          // if (NPoints < 100)
          {
            std::cout << "\t" << ptId << " "
                      << "dE: " << std::setw(10) << ptEdep << " "
                      << "dL: " << std::setw(10) << ptStepLength << " "
                      << "|Mom|: " << std::setw(10) << ptMomentum.Mag() << " "
                      << "|Ek|: " << std::setw(10) << Ek_at_step << " "
                      << "inSFGD: " << is_in_sfgd << " "
                      // << "Mat: "<< ptMaterialName
                      << "V: "<< ptLogVolName
                      << std::endl;
          } 
        }
        tracks_edep_over_pts.insert({TrackID, trk_edep_over_pts});

        std::cout << "  dE_over_pt:" << trk_edep_over_pts << " MeV"
                  << "  dE_over_pt_full:"  << trk_edep_over_pts_full << " MeV"
                  << "\n   Started in SFGD: " << is_started_inside_sfgd << " "
                  << " Stopped in SFGD: " << is_stopped_inside_sfgd << " "
                  << std::endl;
      }

      

      if (is_started_inside_sfgd && is_stopped_inside_sfgd)
      {
        n_started_and_stopped_in_sfgd++;
      }
      else if (!is_started_inside_sfgd && is_stopped_inside_sfgd)
      {
        n_stopped_in_sfgd_from_out++;
      }
      else if (is_started_inside_sfgd && !is_stopped_inside_sfgd)
      {
        n_started_and_not_stopped_in_sfgd++;
      }
      else // (!is_started_inside_sfgd && !is_stopped_inside_sfgd)
      {
        n_other++;
      }

      // if (ParentID != 0) continue;

      if (ParentID == 0)
      {
        if (abs(PDG) == 11 || abs(PDG) == 13)
        {
          t_primary_lepton_track = track;
          main_particle_PDG = PDG;
        }
        sum_of_Ek0_for_out_vertex_particles += InitKinEnergy;
        sum_of_mom_for_out_vertex_particles += InitMom.Mag();
      }

      if (ParentID == 0)
      {
        std::cout 
                  // << "\n\n\n"
                  << " TID:" << TrackID
                  << " PID:" << ParentID
                  << " PDG:" << PDG
                  << " Proc:" << ProcessName
                  << " Ek0:" << InitKinEnergy
                  << " cosT0:" << InitCosTheta
                  << " Npt:" << NPoints
        << std::endl;
      }
      
    }

    /*
    if (total_deposited_energy_by_charged_particles > 500.0)
    {

      for (Int_t trjID = 0; trjID < NTracks; trjID++)
      {
        TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
        int ParentID = track->GetParentID();
        int PDG = track->GetPDG();
        int NPoints = track->GetNPoints();
        int TrackID = track->GetTrackID();
        std::string ProcessName = track->GetProcessName();
        double InitKinEnergy = track->GetInitKinEnergy();
        double InitCosTheta = track->GetInitCosTheta();
        TVector3 InitMom = track->GetInitMom();
        // double StepLength = trackPoint->GetStepLength();

        std::cout << " (" << std::setw(5) << TrackID << "," << std::setw(5) << ParentID << ") "
                  << " PDG: " << std::setw(11) << PDG << " "
                  << " Ek0:" << std::setw(10) << InitKinEnergy << " "
                  // << " dE/dx_pts:" << std::setw(10) << tracks_edep_over_pts[TrackID]
                  << " dE/dx_pts:" << std::setw(10) << energy_best_units(tracks_edep_over_pts[TrackID])
                  // << " R:" << std::setw(10) << 
                  << std::endl;
      }

      continue;
    }
    // */


    for (auto& item: tracks_map)
    {
      int TrackID = item.first;
      TND280UpTrack* track = item.second;
      int ParentId = track->GetParentID();

      int p_PDG = track->GetPDG();
      int p_NPoints = track->GetNPoints();
      int p_TrackID = track->GetTrackID();
      std::string p_ProcessName = track->GetProcessName();
      double p_InitKinEnergy = track->GetInitKinEnergy();
      double p_InitCosTheta = track->GetInitCosTheta();


      if (tracks_map.find(ParentId) != tracks_map.end())
      {
        TND280UpTrack* parent_track = tracks_map[ParentId];
        int GParentId = parent_track->GetParentID();

        int p_PDG = parent_track->GetPDG();
        int p_NPoints = parent_track->GetNPoints();
        int p_TrackID = parent_track->GetTrackID();
        std::string p_ProcessName = parent_track->GetProcessName();
        double p_InitKinEnergy = parent_track->GetInitKinEnergy();
        double p_InitCosTheta = parent_track->GetInitCosTheta();

        if (tracks_map.find(GParentId) != tracks_map.end())
        {
          TND280UpTrack* gparent_track = tracks_map[GParentId];

          int gp_ParentID = gparent_track->GetParentID();
          int gp_PDG = gparent_track->GetPDG();
          int gp_NPoints = gparent_track->GetNPoints();
          int gp_TrackID = gparent_track->GetTrackID();
          std::string gp_ProcessName = gparent_track->GetProcessName();
          double gp_InitKinEnergy = gparent_track->GetInitKinEnergy();
          double gp_InitCosTheta = gparent_track->GetInitCosTheta();
        }
      }
      
    }


    if (t_primary_lepton_track)
    {
      int ParentID = t_primary_lepton_track->GetParentID();
      int PDG = t_primary_lepton_track->GetPDG();
      int NPoints = t_primary_lepton_track->GetNPoints();
      int TrackID = t_primary_lepton_track->GetTrackID();
      std::string ProcessName = t_primary_lepton_track->GetProcessName();
      double InitKinEnergy = t_primary_lepton_track->GetInitKinEnergy();
      double InitCosTheta = t_primary_lepton_track->GetInitCosTheta();
      TVector3 InitMom = t_primary_lepton_track->GetInitMom();

      TVector3 init_direction(
        InitMom.X() / InitMom.Mag(),
        InitMom.Y() / InitMom.Mag(),
        InitMom.Z() / InitMom.Mag()
      );

      h_lepton_angle_distribution->Fill(InitCosTheta);

      // double xy_val = TMath::Power(init_direction.X(), 2.0) + TMath::Power(init_direction.Y(), 2.0); 
      // double xy_angle_cos = init_direction.X() / TMath::Sqrt(xy_val);

      h_lepton_angle_distribution_z->Fill(init_direction.Z());
      h_lepton_angle_distribution_x->Fill(init_direction.X());

      double lepton_edep = 0.0;
      for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
      {
        TND280UpTrackPoint* trackPoint = t_primary_lepton_track->GetPoint(ipt);

        std::string det = trackPoint->GetLogVolName();
        if (!isPtInsideSFGD(det)) continue;

        double Edep = trackPoint->GetEdep();
        // total_edep_of_all_tracks += Edep;
        lepton_edep += Edep;
      }

      h_lepton_edep->Fill(lepton_edep);
      h_all_charged_partciles_edep->Fill(total_deposited_energy_by_charged_particles);
      h_angle_energy_primary_lepton_diplot->Fill(lepton_edep, init_direction.Z());
      h_angle_energy_primary_lepton_diplot_2->Fill(init_direction.Z(), lepton_edep);
      h_angle_energy_all_charged_particles_diplot->Fill(total_deposited_energy_by_charged_particles, init_direction.Z());
      h_angle_energy_all_charged_particles_diplot_2->Fill(init_direction.Z(), total_deposited_energy_by_charged_particles);
      h_angle_initek_energy_primary_lepton_diplot_2->Fill(init_direction.Z(), InitMom.Mag());
      h_angle_Ek0_all_charged_particles_diplot_2->Fill(init_direction.Z(), sum_of_Ek0_for_all_particles);
    }
    

    std::cout << "Total Energy of outgoing particles with ParentID = 0: " << sum_of_Ek0_for_out_vertex_particles << std::endl;
    std::cout << "Total Mom of outgoing particles with ParentID = 0: " << sum_of_mom_for_out_vertex_particles << std::endl;
    std::cout << "Total Energy of all particles: " << sum_of_Ek0_for_all_particles << std::endl;
    std::cout << "Total Mom of all particles: " << sum_of_mom_for_all_particles << std::endl;

    std::cout << "\nn_started_and_stopped_in_sfgd: " << n_started_and_stopped_in_sfgd
              << "\nn_stopped_in_sfgd_from_out: " << n_stopped_in_sfgd_from_out
              << "\nn_started_and_not_stopped_in_sfgd: " << n_started_and_not_stopped_in_sfgd
              << "\nn_other: " << n_other
              << std::endl;
    
  }

  return;

  std::map<int, std::string> particle_pdg_name{
    { 11, "e-"},
    {-11, "e+"},
    { 13, "mu-"},
    {-13, "mu+"},
  };

  h_lepton_angle_distribution->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution;cos #theta;N events").c_str());
  h_lepton_angle_distribution_z->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution z;cos #theta;N events").c_str());
  h_lepton_angle_distribution_x->SetTitle((particle_pdg_name[main_particle_PDG] + " angle distribution x;cos #theta;N events").c_str());
  h_lepton_edep->SetTitle((particle_pdg_name[main_particle_PDG] + " energy deposit;E deposited, MeV;N events").c_str());

  std::string out_fname_lepton_angle_z = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle_z.pdf";
  TCanvas* c_lepton_angle_z = new TCanvas("lepton_angle_z", "lepton angle z", 1080, 720);
  c_lepton_angle_z->cd();
  h_lepton_angle_distribution_z->Draw();
  h_lepton_angle_distribution_z->SetFillColor(kRed);
  c_lepton_angle_z->SaveAs(out_fname_lepton_angle_z.c_str());
  c_lepton_angle_z->Close();

  std::string out_fname_lepton_angle_x = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle_x.pdf";
  TCanvas* c_lepton_angle_x = new TCanvas("lepton_angle_x", "lepton angle x", 1080, 720);
  c_lepton_angle_x->cd();
  h_lepton_angle_distribution_x->Draw();
  h_lepton_angle_distribution_x->SetFillColor(kRed);
  c_lepton_angle_x->SaveAs(out_fname_lepton_angle_x.c_str());
  c_lepton_angle_x->Close();

  std::string out_fname_lepton_angle = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_angle.pdf";
  TCanvas* c_lepton_angle = new TCanvas("lepton_angle", "lepton angle", 1080, 720);
  c_lepton_angle->cd();
  h_lepton_angle_distribution->Draw();
  h_lepton_angle_distribution->SetFillColor(kRed);
  c_lepton_angle->SaveAs(out_fname_lepton_angle.c_str());
  c_lepton_angle->Close();

  std::string out_fname_lepton_edep = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-lepton_edep.pdf";
  TCanvas* c_lepton_edep = new TCanvas("lepton_angle", "lepton angle", 1080, 720);
  c_lepton_edep->cd();
  h_lepton_edep->Draw();
  h_lepton_edep->SetFillColor(kRed);
  c_lepton_edep->SaveAs(out_fname_lepton_edep.c_str());
  c_lepton_edep->Close();

  std::string out_fname_all_charged_partciles_edep = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-all_charged_partciles_edep.pdf";
  TCanvas* c_all_charged_partciles_edep = new TCanvas("all charged particles edep", "all charged particles edep", 1080, 720);
  c_all_charged_partciles_edep->cd();
  h_all_charged_partciles_edep->Draw();
  h_all_charged_partciles_edep->SetFillColor(kRed);
  c_all_charged_partciles_edep->SaveAs(out_fname_all_charged_partciles_edep.c_str());
  c_all_charged_partciles_edep->Close();

  std::string out_fname_angle_energy_primary_lepton_diplot = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_primary_lepton.pdf";
  TCanvas* c_angle_energy_primary_lepton_diplot = new TCanvas("lepton_angle_energy_diplot", "lepton angle", 1080, 720);
  c_angle_energy_primary_lepton_diplot->cd();
  h_angle_energy_primary_lepton_diplot->Draw("COLZ");
  h_angle_energy_primary_lepton_diplot->SetFillColor(kRed);
  c_angle_energy_primary_lepton_diplot->SaveAs(out_fname_angle_energy_primary_lepton_diplot.c_str());
  c_angle_energy_primary_lepton_diplot->Close();

  std::string out_fname_angle_energy_primary_lepton_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_primary_lepton_2.pdf";
  TCanvas* c_angle_energy_primary_lepton_diplot_2 = new TCanvas("lepton_angle_energy_diplot_2", "lepton angle", 1080, 720);
  c_angle_energy_primary_lepton_diplot_2->cd();
  h_angle_energy_primary_lepton_diplot_2->Draw("COLZ");
  h_angle_energy_primary_lepton_diplot_2->SetFillColor(kRed);
  c_angle_energy_primary_lepton_diplot_2->SaveAs(out_fname_angle_energy_primary_lepton_diplot_2.c_str());
  c_angle_energy_primary_lepton_diplot_2->Close();

  std::string out_fname_angle_Ek0_primary_lepton_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_Ek0_primary_lepton_2.pdf";
  TCanvas* c_angle_Ek0_primary_lepton_diplot_2 = new TCanvas("lepton_angle_Ek0_diplot_2", "lepton angle", 1080, 720);
  c_angle_Ek0_primary_lepton_diplot_2->cd();
  h_angle_initek_energy_primary_lepton_diplot_2->Draw("COLZ");
  h_angle_initek_energy_primary_lepton_diplot_2->SetFillColor(kRed);
  c_angle_Ek0_primary_lepton_diplot_2->SaveAs(out_fname_angle_Ek0_primary_lepton_diplot_2.c_str());
  c_angle_Ek0_primary_lepton_diplot_2->Close();

  std::string out_fname_angle_energy_all_charged_particles_diplot = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_all_charged_particles.pdf";
  TCanvas* c_angle_energy_all_charged_particles_diplot = new TCanvas("angle_energy_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_energy_all_charged_particles_diplot->cd();
  h_angle_energy_all_charged_particles_diplot->Draw("COLZ");
  h_angle_energy_all_charged_particles_diplot->SetFillColor(kRed);
  c_angle_energy_all_charged_particles_diplot->SaveAs(out_fname_angle_energy_all_charged_particles_diplot.c_str());
  c_angle_energy_all_charged_particles_diplot->Close();

  std::string out_fname_angle_energy_all_charged_particles_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_energy_all_charged_particles_2.pdf";
  TCanvas* c_angle_energy_all_charged_particles_diplot_2 = new TCanvas("angle_energy_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_energy_all_charged_particles_diplot_2->cd();
  h_angle_energy_all_charged_particles_diplot_2->Draw("COLZ");
  h_angle_energy_all_charged_particles_diplot_2->SetFillColor(kRed);
  c_angle_energy_all_charged_particles_diplot_2->SaveAs(out_fname_angle_energy_all_charged_particles_diplot_2.c_str());
  c_angle_energy_all_charged_particles_diplot_2->Close();

  std::string out_fname_angle_Ek0_all_charged_particles_diplot_2 = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-angle_Ek0_all_charged_particles_2.pdf";
  TCanvas* c_angle_Ek0_all_charged_particles_diplot_2 = new TCanvas("angle_Ek0_all_charged_particles", "lepton angle", 1080, 720);
  c_angle_Ek0_all_charged_particles_diplot_2->cd();
  h_angle_Ek0_all_charged_particles_diplot_2->Draw("COLZ");
  h_angle_Ek0_all_charged_particles_diplot_2->SetFillColor(kRed);
  c_angle_Ek0_all_charged_particles_diplot_2->SaveAs(out_fname_angle_Ek0_all_charged_particles_diplot_2.c_str());
  c_angle_Ek0_all_charged_particles_diplot_2->Close();

  std::string out_fname_vtx_pos = output_dir_tag + "/PDG" + particle_pdg_name[main_particle_PDG] + "-vtx_pos.pdf";
  TCanvas* c_vtx_pos = new TCanvas("h_edep_slices_per_cube_vs_zL_2", "Vertex position", 1080, 720);
  c_vtx_pos->Divide(2,2);
  c_vtx_pos->cd(1);
  // gPad->SetLogy();
  h_vtx_distr_x->Draw();
  h_vtx_distr_x->SetFillColor(kRed);
  c_vtx_pos->cd(2);
  // gPad->SetLogy();
  h_vtx_distr_y->Draw();
  h_vtx_distr_y->SetFillColor(kRed);
  c_vtx_pos->cd(3);
  // gPad->SetLogy();
  h_vtx_distr_z->Draw();
  h_vtx_distr_z->SetFillColor(kRed);
  // c_vtx_pos->cd(4);
  // gPad->SetLogy();
  // h_edep_at_30cm_cube->Draw();
  // h_edep_at_30cm_cube->SetFillColor(kRed);
  c_vtx_pos->SaveAs(out_fname_vtx_pos.c_str());
  c_vtx_pos->Close();
}


void analize_events_from_particle_gun(std::string input_rootfilename, std::string output_dir_tag)
{
  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

  std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  // Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  EventsInfo ei;

  // double dEmax = 3.0;
  // double dEmax = 5.0;
  // ei.cube0_edep = new TH1F("cube0_dE", "cube0 dE", 100, 0.0, dEmax);
  // ei.cube1_edep = new TH1F("cube1_dE", "cube1 dE", 100, 0.0, dEmax);
  // ei.cube2_edep = new TH1F("cube2_dE", "cube2 dE", 100, 0.0, dEmax);
  // ei.cube3_edep = new TH1F("cube3_dE", "cube3 dE", 100, 0.0, dEmax);

  // int NeventsWOgamma = 0;

  // TH1F* total_edep = new TH1F("total_edep", "total edep", 200, 0.0, 400);
  TH1F* total_edep = new TH1F("total_edep", "total energy deposit of primary particle;dE, MeV;N events", 300, 0.0, 300); // for 200 MeV
  // TH1F* total_edep = new TH1F("total_edep", "total energy deposit of primary particle;dE, MeV;N events", 350, 0.0, 700); // for 600-700 MeV
  // TH1F* total_edep_with_gamma = new TH1F("total_edep_g", "total edep with #gamma", 200, 0.0, 400);
  // TH1F* h_total_edep_all_tracks = new TH1F("total_all_tracks", "total edep of all tracks", 200, 0.0, 400);
  TH1F* h_total_edep_all_tracks = new TH1F("total_all_tracks", "total energy deposit of all tracks;dE, MeV;N events", 200, 0.0, 400); // for 200 MeV
  // TH1F* h_total_edep_all_tracks = new TH1F("total_all_tracks", "total energy deposit of all tracks;dE, MeV;N events", 350, 0.0, 700); // for 600-700 MeV
  // TH1F* h_total_edep_of_gammas = new TH1F("total_edep_of_gammas", "total edep of gammas", 200, 0.0, 400);
  // TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "edep_range_2d_hist", 300, 0.0, 300.0, 100, 0.0, 5.0);
  // TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "edep_range_2d_hist", 50, 0.0, 50.0, 100, 0.0, 20.0);
  // TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "edep_range_2d_hist", 100, 0.0, 100.0, 100, 0.0, 15.0);
  TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "Enerdy deposit at range;Track length, cm;dE, MeV", 100, 0.0, 100.0, 100, 0.0, 15.0); // for e-/mu- 200 MeV
  // TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "Enerdy deposit at range;Track length, cm;dE, MeV", 200, 0.0, 200.0, 100, 0.0, 15.0); // for e-/mu- 600-700 MeV
  
  TH1F* h_edep_at_10cm = new TH1F("edep at 10 cm", "Energy deposit at 10th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_15cm = new TH1F("edep at 15 cm", "Energy deposit at 15th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_20cm = new TH1F("edep at 20 cm", "Energy deposit at 20th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_30cm = new TH1F("edep at 30 cm", "Energy deposit at 30th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_40cm = new TH1F("edep at 40 cm", "Energy deposit at 40th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_50cm = new TH1F("edep at 50 cm", "Energy deposit at 50th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV

  TH1F* h_edep_at_10cm_cube = new TH1F("edep at 10 cm cube", "Energy deposit at 10th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_15cm_cube = new TH1F("edep at 15 cm cube", "Energy deposit at 15th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_20cm_cube = new TH1F("edep at 20 cm cube", "Energy deposit at 20th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_30cm_cube = new TH1F("edep at 30 cm cube", "Energy deposit at 30th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_40cm_cube = new TH1F("edep at 40 cm cube", "Energy deposit at 40th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
  TH1F* h_edep_at_50cm_cube = new TH1F("edep at 50 cm cube", "Energy deposit at 50th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV

  // TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "XY plane;X, cm;Y, cm", 192, -96.0, 96.0, 56, -28.0, 28.0);
  // TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "XZ plane;X, cm;Z, cm", 192, -96.0, 96.0, 184, -92.0, 92.0);
  // TH2F* h2_event_display_edep_yz = new TH2F("YZ plane", "YZ plane;Y, cm;Z, cm", 56, -28.0, 28.0, 184, -92.0, 92.0);
  
  // TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "XY plane;X, cm;Y, cm", 192, -96.0, 96.0, 56, -28.0, 28.0);
  // TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "XY plane;Y, cm;X, cm", 56, -28.0, 28.0, 192, -96.0, 96.0);
  // // TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "XZ plane;X, cm;Z, cm", 192, -96.0, 96.0, 184, -92.0, 92.0);
  // TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "XZ plane;Z, cm;X, cm", 184, -92.0, 92.0, 192, -96.0, 96.0);
  // TH2F* h2_event_display_edep_yz = new TH2F("YZ plane", "YZ plane;Y, cm;Z, cm", 56, -28.0, 28.0, 184, -92.0, 92.0);
  // // TCanvas* c_event_display = new TCanvas("c_event_display", "Event display", 1080, 720);
  
  // TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "", 56, -28.0, 28.0, 192, -96.0, 96.0);
  // TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "", 184, -92.0, 92.0, 192, -96.0, 96.0);
  // TH2F* h2_event_display_edep_yz = new TH2F("YZ plane", "", 184, -92.0, 92.0, 56, -28.0, 28.0);

  TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "", 56, 0.0, 56.0, 192, 0.0, 192.0);
  TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "", 184, 0.0, 184.0, 192, 0.0, 192.0);
  TH2F* h2_event_display_edep_yz = new TH2F("YZ plane", "", 184, 0.0, 184.0, 56, 0.0, 56.0);
  
  // TCanvas* c_event_display = new TCanvas("c_event_display", "Event display", 1000, 1000);
  TCanvas* c_event_display = new TCanvas("c_event_display", "Event display", 1100, 1000);


  int main_particle_PDG = -1;
  double main_particle_InitKinEnergy = -1.0;

  int NTreeEntries = tinput->GetEntries();

  for (int eventId = 0; eventId < NTreeEntries; ++eventId)
  // for (int eventId = 1; eventId < 2; ++eventId)
  // for (int eventId = 1; eventId < 10; ++eventId)
  // for (int eventId = 1; eventId < 100; ++eventId)
  {
    std::cout << "\nEvent " << eventId << std::endl;
    tinput->GetEntry(eventId);

    int NHits = nd280UpEvent->GetNHits();
    int NTracks = nd280UpEvent->GetNTracks();

    std::cout << "NHits " << NHits << std::endl;
    std::cout << "NTracks " << NTracks << std::endl;
    std::cout << "NVertices " << nd280UpEvent->GetNVertices() << std::endl;

    TND280UpTrack* primary_track;

    double energies_of_secondary_photons = 0.0;
    double total_edep_of_all_tracks = 0.0;
    // double total_edep_of_gammas = 0.0;

    for (Int_t trjID = 0; trjID < NTracks; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
      int ParentID = track->GetParentID();
      int PDG = track->GetPDG();
      int NPoints = track->GetNPoints();
      int TrackID = track->GetTrackID();
      std::string ProcessName = track->GetProcessName();
      double InitKinEnergy = track->GetInitKinEnergy();

      std::cout 
                // << "\n\n\n"
                << " TID:" << TrackID
                << " PID:" << ParentID
                << " PDG:" << PDG
                << " Proc:" << ProcessName
                << " Ek0:" << InitKinEnergy
                << " Npt:" << NPoints
      << std::endl;

      if (ParentID == 0)
      {
        primary_track = track;
        // break;
      }

      if (ParentID == 1 && PDG == 22)
      {
        energies_of_secondary_photons += track->GetInitKinEnergy();

        // int NPoints = track->GetNPoints();
        // for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
        // {
        //   TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);

        //   double Edep = trackPoint->GetEdep();
        //   total_edep_of_gammas += Edep;
        // }
      }

      for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
      {
        TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);

        double Edep = trackPoint->GetEdep();
        total_edep_of_all_tracks += Edep;

      //   TVector3 PrePosition = trackPoint->GetPrePosition();
      //   TVector3 PostPosition = trackPoint->GetPostPosition();
      //   TVector3 AvgPos = (PrePosition + PostPosition);
      //   AvgPos *= 0.5;
      //   double zAvgPos = AvgPos.Z();

      //   double Time = trackPoint->GetTime();
      //   TVector3 Momentum = trackPoint->GetMomentum();
      //   double StepLength = trackPoint->GetStepLength();
      //   double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      //   string PhysVolName = trackPoint->GetPhysVolName();
      //   std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1";
      //   if (PhysVolName.find(to_replace) != std::string::npos)
      //     PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"~");
      //   to_replace = "RepY/RepX/RepZ";
      //   if (PhysVolName.find(to_replace) != std::string::npos)
      //     PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"RXYZ");
      //   std::string PreProcessName = trackPoint->GetPreProcessName();
      //   std::string PostProcessName = trackPoint->GetPostProcessName();
      //   std::string MaterialName = trackPoint->GetMaterialName();
      //   int PointID = trackPoint->GetPointID();
      //   bool IsOnBoundary = trackPoint->IsOnBoundary();
      //   double dLpt = (PostPosition - PrePosition).Mag();
      //   std::cout << std::setprecision(5) << std::fixed << std::showpoint
      //             << std::setw(5) << "  Tp:" << PointID
      //             << std::setw(11) << " |M|:" << Momentum.Mag()
      //             << std::setw(11) << " dE:" << Edep
      //             << std::setw(11) << " dL:" << StepLength
      //             << std::setw(11) << " dLpt:" << dLpt
      //             << " " << std::setw(20) << PreProcessName
      //               << " " << std::setw(20) << PostProcessName
      //               << " " << std::setw(20) << MaterialName
      //             << " Bnd:" << std::setw(2) << IsOnBoundary
      //             //  << " X:" << PrePosition.X()
      //             //   << " Y:" << PrePosition.Y()
      //              << " Z:" << std::setw(8) << PrePosition.Z()
      //             //  << " X:" << PostPosition.X()
      //             //   << " Y:" << PostPosition.Y()
      //               << " Z:" << std::setw(8) << PostPosition.Z()
      //               << " avgZ:" << std::setw(8) << zAvgPos
      //             << " " << PhysVolName
      //             << std::endl;
      }
    }

    h_total_edep_all_tracks->Fill(total_edep_of_all_tracks);

    /// process primary track
    int TrackID = primary_track->GetTrackID();
    int ParentID = 0;
    std::string ProcessName = primary_track->GetProcessName();
    int PDG = primary_track->GetPDG();
    double InitKinEnergy = primary_track->GetInitKinEnergy();

    main_particle_PDG = PDG;
    main_particle_InitKinEnergy = InitKinEnergy;

    int NPoints = primary_track->GetNPoints();

    std::cout << " TID:" << TrackID
              << " PID:" << ParentID
              << " PDG:" << PDG
              << " Proc:" << ProcessName
              << " Ek0:" << InitKinEnergy
              << " Npt:" << NPoints
    << std::endl;

    double dL_over_points = 0.0;
    double dL_over_points2 = 0.0;
    double dE_over_pts = 0.0;

    // int zLayer = 0;
    // double zLayer_start = 0.0;

    // std::array<double, 184> zLayersA;
    // zLayersA.fill(0.0);

    std::map<std::tuple<int, double, double>, double> zLayersEdep;
    std::map<std::tuple<int, double, double>, double> zCubeEdep;
    // std::map<std::tuple<double, double, double, double, double, double>, double> edepAtCubes;
    std::map<std::tuple<double, double, double, double, double, double>, std::tuple<int,double>> edepAtCubes;
    // std::map<std::tuple<int, double, double>, double> zLayersEdep_with_gamma;
    for (int i = 0; i < 184; i++)
    {
      double z0 = i * 10.0 - 920.0;
      double z1 = (i+1) * 10.0 - 920.0;
      double fill_value = 0.0;
      zLayersEdep.insert({{i, z0, z1}, fill_value});
      zCubeEdep.insert({{i, z0, z1}, fill_value});
      // zLayersEdep_with_gamma.insert({{i, z0, z1}, fill_value});
    }

    int cube_idx = 0;
    for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
    {
      TND280UpTrackPoint* trackPoint = primary_track->GetPoint(ipt);
      TVector3 PrePosition = trackPoint->GetPrePosition();
      TVector3 PostPosition = trackPoint->GetPostPosition();
      TVector3 AvgPos = (PrePosition + PostPosition);
      AvgPos *= 0.5;
      double zAvgPos = AvgPos.Z();
      // if (!isPtIntoSFGD(PrePosition)) continue;

      double Time = trackPoint->GetTime();
      TVector3 Momentum = trackPoint->GetMomentum();
      double Edep = trackPoint->GetEdep();
      double StepLength = trackPoint->GetStepLength();
      double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
      string PhysVolName = trackPoint->GetPhysVolName();
      std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1";
      if (PhysVolName.find(to_replace) != std::string::npos)
        PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"~");
      to_replace = "RepY/RepX/RepZ";
      if (PhysVolName.find(to_replace) != std::string::npos)
        PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"RXYZ");
      std::string PreProcessName = trackPoint->GetPreProcessName();
      std::string PostProcessName = trackPoint->GetPostProcessName();
      std::string MaterialName = trackPoint->GetMaterialName();
      int PointID = trackPoint->GetPointID();
      bool IsOnBoundary = trackPoint->IsOnBoundary();
      double dLpt = (PostPosition - PrePosition).Mag();
      // std::cout << std::setprecision(5) << std::fixed << std::showpoint
      //           << std::setw(5) << "  Tp:" << PointID
      //           << std::setw(11) << " |M|:" << Momentum.Mag()
      //           << std::setw(11) << " dE:" << Edep
      //           << std::setw(11) << " dL:" << StepLength
      //           << std::setw(11) << " dLpt:" << dLpt
      //           << std::setw(20) << " " << PreProcessName << " " << std::setw(20) << PostProcessName << " " << std::setw(20) << MaterialName
      //           << std::setw(2) << " Bnd:" << IsOnBoundary
      //           //  << " X:" << PrePosition.X()
      //           //   << " Y:" << PrePosition.Y()
      //             << std::setw(8) << " Z:" << PrePosition.Z()
      //           //  << " X:" << PostPosition.X()
      //           //   << " Y:" << PostPosition.Y()
      //             << std::setw(8) << " Z:" << PostPosition.Z()
      //             << std::setw(8) << " avgZ:" << zAvgPos
      //           << " " << PhysVolName
      //           << std::endl;

      dL_over_points += dLpt;
      dL_over_points2 += StepLength;
      dE_over_pts += Edep;

      double new_x = (AvgPos.X() + 960.0) / 10.0;
      double new_y = (AvgPos.Y() + 280.0) / 10.0;
      double new_z = (AvgPos.Z() + 920.0) / 10.0;
      // std::cout << "New coords: " << new_x << " " << new_y << " " << new_z << std::endl;

      // // h2_event_display_edep_xy->Fill(AvgPos.X() / 10.0, AvgPos.Y() / 10.0, Edep);
      // h2_event_display_edep_xy->Fill(AvgPos.Y() / 10.0, AvgPos.X() / 10.0, Edep);
      // h2_event_display_edep_xz->Fill(AvgPos.X() / 10.0, AvgPos.Z() / 10.0, Edep);
      // // h2_event_display_edep_yz->Fill(AvgPos.Y() / 10.0, AvgPos.Z() / 10.0, Edep);
      // h2_event_display_edep_yz->Fill(AvgPos.Z() / 10.0, AvgPos.Y() / 10.0, Edep);

      h2_event_display_edep_xy->Fill(new_y, new_x, Edep);
      h2_event_display_edep_xz->Fill(new_x, new_z, Edep);
      // h2_event_display_edep_yz->Fill(AvgPos.Y() / 10.0, AvgPos.Z() / 10.0, Edep);
      h2_event_display_edep_yz->Fill(new_z, new_y, Edep);
      
      for (int i = 0; i < 184; i++)
      {
        double z0 = i * 10.0 - 920.0;
        double z1 = (i+1) * 10.0 - 920.0;
        if (zAvgPos >= z0 && zAvgPos < z1)
        {
          zLayersEdep[{i, z0, z1}] += Edep;
          // zLayersEdep.at({z0, z1}) += Edep;
        }
      }
      
      double avg_pt_x_pos_cm = AvgPos.X() / 10.0;
      double avg_pt_y_pos_cm = AvgPos.Y() / 10.0;
      double avg_pt_z_pos_cm = AvgPos.Z() / 10.0;

      double x_pos_cm = trunc(avg_pt_x_pos_cm);
      double y_pos_cm = trunc(avg_pt_y_pos_cm);
      double z_pos_cm = trunc(avg_pt_z_pos_cm);

      std::tuple<double, double, double, double, double, double> cubePos{
        x_pos_cm, x_pos_cm+1.0,
        y_pos_cm, y_pos_cm+1.0,
        z_pos_cm, z_pos_cm+1.0,
      };

      if (Edep > 0)
      {
        if (edepAtCubes.find(cubePos) != edepAtCubes.end())
        {
          std::tuple<int, double> prev_cube_edep = edepAtCubes[cubePos];
          std::tuple<int, double> new_cube_edep{std::get<0>(prev_cube_edep), std::get<1>(prev_cube_edep) + Edep};
          edepAtCubes[cubePos] = new_cube_edep;
          // edepAtCubes[cubePos] += Edep;
        } else {
          std::tuple<int, double> new_cube_edep{cube_idx, Edep};
          edepAtCubes.insert({cubePos, new_cube_edep});
          // edepAtCubes.insert({cubePos, Edep});
          cube_idx++;
        }
      }      
    }

    /// fill canvas or clear it
    {
      std::string canvas_title = "PDG: " + std::to_string(main_particle_PDG)
                                 + ", initial Ekin " + std::to_string(main_particle_InitKinEnergy)
                                 + ", display of event " + std::to_string(eventId);
      // h2_event_display_edep_xy->SetTitle(("PDG " + std::to_string(main_particle_PDG) + ", Event Display XY, Event " + std::to_string(eventId) + ";X, cm;Y, cm").c_str());
      // h2_event_display_edep_xz->SetTitle(("PDG " + std::to_string(main_particle_PDG) + ", Event Display XZ, Event " + std::to_string(eventId) + ";X, cm;Z, cm").c_str());
      // h2_event_display_edep_yz->SetTitle(("PDG " + std::to_string(main_particle_PDG) + ", Event Display YZ, Event " + std::to_string(eventId) + ";Y, cm;Z, cm").c_str());

      // h2_event_display_edep_xz->SetTitle(("PDG " + std::to_string(main_particle_PDG) + ", Event Display XZ, Event " + std::to_string(eventId)).c_str());
      
      h2_event_display_edep_xy->SetTitle(";Y, cm;X, cm");
      h2_event_display_edep_yz->SetTitle(";Z, cm;Y, cm");
      h2_event_display_edep_xz->SetTitle("");

      h2_event_display_edep_xz->SetStats(0);
      h2_event_display_edep_xy->SetStats(0);
      h2_event_display_edep_yz->SetStats(0);

      // h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.07);
      // h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.07);
      // h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.07);

      h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.05);
      h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.05);
      h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.05);

      h2_event_display_edep_yz->GetXaxis()->SetLabelSize(0.07);
      h2_event_display_edep_yz->GetYaxis()->SetLabelSize(0.07);
      h2_event_display_edep_yz->GetZaxis()->SetLabelSize(0.07);

      h2_event_display_edep_xz->GetXaxis()->SetLabelSize(0.0);
      h2_event_display_edep_xz->GetYaxis()->SetLabelSize(0.0);
      h2_event_display_edep_xz->GetZaxis()->SetLabelSize(0.02);

      // h2_event_display_edep_xz->SetTitleSize(0.07);
      h2_event_display_edep_yz->SetTitleSize(0.1, "X");
      h2_event_display_edep_yz->SetTitleSize(0.1, "Y");
      h2_event_display_edep_yz->SetTitleSize(0.1, "Z");
      // h2_event_display_edep_yz->SetTitleSize(0.07);
      // h2_event_display_edep_xy->SetTitleSize(0.07);
      h2_event_display_edep_xy->SetTitleSize(0.07, "X");
      h2_event_display_edep_xy->SetTitleSize(0.07, "Y");

      // h2_event_display_edep_xz->SetTitleFont(200);
      // h2_event_display_edep_yz->SetTitleFont(200);
      // h2_event_display_edep_xy->SetTitleFont(200);

      // gStyle->SetTitleFontSize(0.3);
      
      // c_event_display->SetTitle(canvas_title.c_str());

      // TLatex * tex = new TLatex(0.5,0.8,canvas_title.c_str());
      // tex->SetNDC();
      // tex->SetTextSize(0.2);
      // tex->Draw();
      // TLatex *tex = new TLatex();
      // tex->DrawLatexNDC(.4,.95,canvas_title.c_str());

      // c_event_display->cd();
      // TPad *ttl_pad = new TPad("ttl_pad", "ttl_pad", 0.0, 0.9, 1.0, 1.0);
      // TPad *display_pad = new TPad("display_pad", "display_pad", 0.0, 0.0, 1.0, 0.9);

      // TLatex * ttl_tex = new TLatex(0.5,0.92,canvas_title.c_str());
      // ttl_tex->SetNDC();
      // ttl_tex->SetTextSize(0.2);

      // c_event_display->cd();
      // ttl_pad->cd();
      // ttl_pad->Draw();
      // ttl_tex->Draw();

      // // display_pad->Draw();
      // // display_pad->Divide(2,2);
      // // gPad->SetTitle(canvas_title.c_str());
      // c_event_display->Divide(2,2);
      // c_event_display->cd(1);
      // // display_pad->cd(1);      
      // h2_event_display_edep_xy->Draw("COLZ");
      // c_event_display->cd(2);
      // // display_pad->cd(2);      
      // h2_event_display_edep_xz->Draw("COLZ");
      // c_event_display->cd(3);
      // // display_pad->cd(3);      
      // h2_event_display_edep_yz->Draw("COLZ");
      // // c_event_display->cd(4);
      // // c_event_display->Close();

      // ttl_pad->Draw();
      // display_pad->Draw();
      
      c_event_display->cd();
      // // TPad *display_pad = new TPad("display_pad", "display_pad", 0.01, 0.01, 0.99, 0.9);
      // // TPad *display_pad_xz = new TPad("display_pad", "display_pad", 0.01, 0.22, 0.99, 0.89);
      // TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", 0.25, 0.22, 0.99, 0.89);
      // // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.22, 0.23, 0.89);
      // // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.22, 0.24, 0.89);
      // // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.22, 0.249, 0.89);
      // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.18, 0.249, 0.89);
      // // TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", 0.25, 0.01, 0.99, 0.21);
      // TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", 0.25, 0.01, 0.99, 0.219);

      // TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", 0.25, 0.22, 0.99, 0.89);
      // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.18, 0.249, 0.89);
      // TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", 0.25, 0.01, 0.99, 0.219);

      // double yz_xz_start_xy_stop_x = 0.25;
      double yz_xz_start_xy_stop_x = 0.3;
      double xy_xz_start_yz_stop_y = 0.22;

      TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
      // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.18, yz_xz_start_xy_stop_x, 0.89);
      // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.15, yz_xz_start_xy_stop_x, 0.89);
      // TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.02, 0.13, yz_xz_start_xy_stop_x, 0.89);
      TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
      TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);
      // TPad *title_pad = new TPad("title_pad", "title_pad", 0.01, 0.9, 1.0, 1.0);

      display_pad_xz->Draw(); // top z -- lowest
      display_pad_xy->Draw(); // top z -- middle
      display_pad_yz->Draw(); // top z -- highest
      // title_pad->Draw();

      TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
      pt->AddText(canvas_title.c_str());
      pt->SetTextSize(0.03);
      pt->Draw();

      // display_pad_xy->SetFillColor(kCyan);
      // display_pad_xz->SetFillColor(kYellow-10);
      // display_pad_yz->SetFillColor(kGreen-10);
      // title_pad->SetFillColor(kRed-10);

      display_pad_xz->SetLeftMargin(0.03);
      // display_pad_xz->SetRightMargin(0);
      display_pad_xz->SetTopMargin(0);
      // display_pad_xz->SetBottomMargin(0.03);
      display_pad_xz->SetBottomMargin(0.0);

      display_pad_xy->SetLeftMargin(0.2);
      // display_pad_xy->SetRightMargin(0.2);
      display_pad_xy->SetRightMargin(0.15);
      display_pad_xy->SetTopMargin(0);
      // display_pad_xy->SetBottomMargin(0.03);
      // display_pad_xy->SetBottomMargin(0.08);
      display_pad_xy->SetBottomMargin(0.12);
      // display_pad_xy->SetBottomMargin(0.15);
      

      display_pad_yz->SetLeftMargin(0.03);
      // display_pad_yz->SetRightMargin(0);
      display_pad_yz->SetTopMargin(0);
      // display_pad_yz->SetBottomMargin(0.1);
      // display_pad_yz->SetBottomMargin(0.3);
      display_pad_yz->SetBottomMargin(0.2);

      display_pad_xz->cd();
      h2_event_display_edep_xz->Draw("COLZ");
      
      display_pad_xy->cd();
      h2_event_display_edep_xy->Draw("COLZ");

      display_pad_yz->cd();
      h2_event_display_edep_yz->Draw("COLZ");

      // title_pad->cd();
      // title_pad->PaintTextNDC(0.3, 0.3, canvas_title.c_str());

      

      // if (1)
      {
        std::string out_fname_event_display = output_dir_tag
          + "/event_display/PDG" + std::to_string(main_particle_PDG)
          + "-event_display" + std::to_string(eventId) + ".pdf";
        c_event_display->SaveAs(out_fname_event_display.c_str());
      }
      
      
      /// clear canvas and histograms
      h2_event_display_edep_xy->Reset("ICESM");
      h2_event_display_edep_xz->Reset("ICESM");
      h2_event_display_edep_yz->Reset("ICESM");
      c_event_display->Clear();
      // display_pad->Clear();
      // ttl_pad->Clear();
      // delete ttl_tex;
      // delete display_pad;
      // delete ttl_pad;
    }

    std::cout << "  == dL_over_pts:" << dL_over_points
              << " dL_over_pts2:" << dL_over_points2
              << " R:" << primary_track-> GetRange()
              << " Ltgt1:" << primary_track->GetLengthTarget1()
              << " dL_SD:" << primary_track->GetSDLength() // not used in simulation
              << std::endl;
    std::cout << "  == dE_over_pts:" << dE_over_pts
              << " dEtgt1:" << primary_track->GetEdepTarget1()
              << " dE_SD:" << primary_track->GetSDTotalEnergyDeposit() // do not consider
              << std::endl;

    double totEdep = 0.0;
    for (int i = 0; i < 184; i++)
    {
      double z0 = i * 10.0 - 920.0;
      double z1 = (i+1) * 10.0 - 920.0;
      std::cout << " >>> [" << i << " " << z0 << " " << z1 << "] " << zLayersEdep.at({i, z0, z1}) << std::endl;
      totEdep += zLayersEdep.at({i, z0, z1});
      // total_edep->Fill(totEdep);
      // total_edep_with_gamma
      if (zLayersEdep.at({i, z0, z1}) > 0.0)
      {
        h_edep_range_2d_hist->Fill(z0 / 10.0+92.0, zLayersEdep.at({i, z0, z1}));
      }

      double edep = zLayersEdep.at({i, z0, z1});
      if (edep > 0.0)
      {
        switch (i)
        {
          case 10-1: h_edep_at_10cm->Fill(edep); break;
          case 15-1: h_edep_at_15cm->Fill(edep); break;
          case 20-1: h_edep_at_20cm->Fill(edep); break;
          case 30-1: h_edep_at_30cm->Fill(edep); break;
          case 40-1: h_edep_at_40cm->Fill(edep); break;
          case 50-1: h_edep_at_50cm->Fill(edep); break;
          default: break;
        }
      }
    }
  
    // std::map<int, double> edepAtCubesPerTrack;
    for (auto& it: edepAtCubes)
    {
      std::tuple<double, double, double, double, double, double> cubePos = it.first;
      // double edep = it.second;
      std::tuple<int, double> edep = it.second;
      std::cout << std::get<0>(edep) << " "
                << "[ "
                  << "[" << std::get<0>(cubePos) << ", " << std::get<1>(cubePos) << "] "
                  << "[" << std::get<2>(cubePos) << ", " << std::get<3>(cubePos) << "] "
                  << "[" << std::get<4>(cubePos) << ", " << std::get<5>(cubePos) << "] "
                << "] "
                // << edep
                << std::get<1>(edep)
                << std::endl;

      // double z0 = std::get<4>(cubePos);
      // double z1 = std::get<5>(cubePos);
      // edepAtCubesPerTrack.insert(edep);

      double dE = std::get<1>(edep);
      if (dE > 0.0)
      {
        switch (std::get<0>(edep))
        {
          case 10-1: h_edep_at_10cm_cube->Fill(dE); break;
          case 15-1: h_edep_at_15cm_cube->Fill(dE); break;
          case 20-1: h_edep_at_20cm_cube->Fill(dE); break;
          case 30-1: h_edep_at_30cm_cube->Fill(dE); break;
          case 40-1: h_edep_at_40cm_cube->Fill(dE); break;
          case 50-1: h_edep_at_50cm_cube->Fill(dE); break;
          default: break;
        }
      }
    }

    total_edep->Fill(totEdep);
    // total_edep_with_gamma->Fill(totEdep+energies_of_secondary_photons);
    // total_edep_with_gamma->Fill(totEdep+total_edep_of_gammas);
    // h_total_edep_of_gammas->Fill(totEdep+total_edep_of_gammas);

    // std::cout << " >>> " << totEdep << std::endl;
    std::cout << " >>> " << totEdep << " " << energies_of_secondary_photons << " " << total_edep_of_all_tracks << std::endl;
  }

  std::string out_fname_total_edep = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep.pdf";
  TCanvas* c_total_edep = new TCanvas("total_edep", "Total Energy deposit;dE/dx, MeV;N events", 1080, 720);
  c_total_edep->cd();
  total_edep->Draw();
  total_edep->SetFillColor(kRed);
  c_total_edep->SaveAs(out_fname_total_edep.c_str());
  c_total_edep->Close();

  // std::string out_fname_total_edep_wgamma = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep_wgamma.pdf";
  // TCanvas* c_total_edep_wgamma = new TCanvas("total_edep_wgamma", "Total Energy deposit (with gamma);dE/dx, MeV;N events", 1080, 720);
  // c_total_edep_wgamma->cd();
  // total_edep_with_gamma->Draw();
  // total_edep_with_gamma->SetFillColor(kRed);
  // c_total_edep_wgamma->SaveAs(out_fname_total_edep_wgamma.c_str());
  // c_total_edep_wgamma->Close();

  std::string out_fname_total_all_trks = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep_all_tracks.pdf";
  TCanvas* c_total_edep_all_tracks = new TCanvas("h_total_edep_all_tracks", "Total Energy deposit by all tracks;dE/dx, MeV;N events", 1080, 720);
  c_total_edep_all_tracks->cd();
  h_total_edep_all_tracks->Draw();
  h_total_edep_all_tracks->SetFillColor(kRed);
  c_total_edep_all_tracks->SaveAs(out_fname_total_all_trks.c_str());
  c_total_edep_all_tracks->Close();

  std::string out_fname_edep_range_2d = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_to_range_2d.pdf";
  TCanvas* c_edep_range_2d_hist = new TCanvas("h_edep_range_2d_hist", "Energy deposit to range;Range, cm;Edep, MeV", 1080, 720);
  c_edep_range_2d_hist->cd();
  h_edep_range_2d_hist->Draw("COLZ");
  c_edep_range_2d_hist->SaveAs(out_fname_edep_range_2d.c_str());
  c_edep_range_2d_hist->Close();

  std::string out_fname_edep_slices = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices.pdf";
  TCanvas* c_edep_slices = new TCanvas("h_edep_slices", "Energy deposit slices", 1080, 720);
  c_edep_slices->Divide(2,3);
  c_edep_slices->cd(1);
  gPad->SetLogy();
  h_edep_at_10cm->Draw();
  h_edep_at_10cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->cd(2);
  gPad->SetLogy();
  h_edep_at_15cm->Draw();
  h_edep_at_15cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->cd(3);
  gPad->SetLogy();
  h_edep_at_20cm->Draw();
  h_edep_at_20cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->cd(4);
  gPad->SetLogy();
  h_edep_at_30cm->Draw();
  h_edep_at_30cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->cd(5);
  gPad->SetLogy();
  h_edep_at_40cm->Draw();
  h_edep_at_40cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->cd(6);
  gPad->SetLogy();
  h_edep_at_50cm->Draw();
  h_edep_at_50cm->SetFillColor(kRed);
  // c_edep_slices->SetLogy();
  c_edep_slices->SaveAs(out_fname_edep_slices.c_str());
  c_edep_slices->Close();


  std::string out_fname_edep_slices_per_cube = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube.pdf";
  TCanvas* c_edep_slices_per_cube = new TCanvas("h_edep_slices_per_cube", "Energy deposit slices (per cube)", 1080, 720);
  c_edep_slices_per_cube->Divide(2,3);
  c_edep_slices_per_cube->cd(1);
  gPad->SetLogy();
  h_edep_at_10cm_cube->Draw();
  h_edep_at_10cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->cd(2);
  gPad->SetLogy();
  h_edep_at_15cm_cube->Draw();
  h_edep_at_15cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->cd(3);
  gPad->SetLogy();
  h_edep_at_20cm_cube->Draw();
  h_edep_at_20cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->cd(4);
  gPad->SetLogy();
  h_edep_at_30cm_cube->Draw();
  h_edep_at_30cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->cd(5);
  gPad->SetLogy();
  h_edep_at_40cm_cube->Draw();
  h_edep_at_40cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->cd(6);
  gPad->SetLogy();
  h_edep_at_50cm_cube->Draw();
  h_edep_at_50cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube->SaveAs(out_fname_edep_slices_per_cube.c_str());
  c_edep_slices_per_cube->Close();


  std::string out_fname_edep_slices_per_cube_vs_zL_1 = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube_vs_zL-1.pdf";
  TCanvas* c_edep_slices_per_cube_vs_zL_1 = new TCanvas("h_edep_slices_per_cube_vs_zL_1", "Energy deposit slices (per cube vs per zLayer)", 1080, 720);
  c_edep_slices_per_cube_vs_zL_1->Divide(2,3);
  c_edep_slices_per_cube_vs_zL_1->cd(1);
  gPad->SetLogy();
  h_edep_at_10cm->Draw();
  h_edep_at_10cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->cd(3);
  gPad->SetLogy();
  h_edep_at_15cm->Draw();
  h_edep_at_15cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->cd(5);
  gPad->SetLogy();
  h_edep_at_20cm->Draw();
  h_edep_at_20cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->cd(2);
  gPad->SetLogy();
  h_edep_at_10cm_cube->Draw();
  h_edep_at_10cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->cd(4);
  gPad->SetLogy();
  h_edep_at_15cm_cube->Draw();
  h_edep_at_15cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->cd(6);
  gPad->SetLogy();
  h_edep_at_20cm_cube->Draw();
  h_edep_at_20cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_1->SaveAs(out_fname_edep_slices_per_cube_vs_zL_1.c_str());
  c_edep_slices_per_cube_vs_zL_1->Close();

  std::string out_fname_edep_slices_per_cube_vs_zL_2 = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube_vs_zL-2.pdf";
  TCanvas* c_edep_slices_per_cube_vs_zL_2 = new TCanvas("h_edep_slices_per_cube_vs_zL_2", "Energy deposit slices (per cube vs per zLayer)", 1080, 720);
  c_edep_slices_per_cube_vs_zL_2->Divide(2,3);
  c_edep_slices_per_cube_vs_zL_2->cd(1);
  gPad->SetLogy();
  h_edep_at_30cm->Draw();
  h_edep_at_30cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->cd(3);
  gPad->SetLogy();
  h_edep_at_40cm->Draw();
  h_edep_at_40cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->cd(5);
  gPad->SetLogy();
  h_edep_at_50cm->Draw();
  h_edep_at_50cm->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->cd(2);
  gPad->SetLogy();
  h_edep_at_30cm_cube->Draw();
  h_edep_at_30cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->cd(4);
  gPad->SetLogy();
  h_edep_at_40cm_cube->Draw();
  h_edep_at_40cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->cd(6);
  gPad->SetLogy();
  h_edep_at_50cm_cube->Draw();
  h_edep_at_50cm_cube->SetFillColor(kRed);
  c_edep_slices_per_cube_vs_zL_2->SaveAs(out_fname_edep_slices_per_cube_vs_zL_2.c_str());
  c_edep_slices_per_cube_vs_zL_2->Close();

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
  if (res != 0) // if error (directory exists or so)
  {
    // return -1;
  }
  std::stringstream cmd2; cmd2 << "mkdir " << output_dir_tag << "/event_display";
  int res2 = std::system(cmd2.str().c_str());
  if (res2 != 0) // if error (directory exists or so)
  {
    // return -1;
  }

  analize_events_from_particle_gun(input_rootfilename, output_dir_tag); 
  // analize_events_from_genie(input_rootfilename, output_dir_tag); 
  // analize_events_from_genie_v2(input_rootfilename, output_dir_tag); 
  
  return 0;
}