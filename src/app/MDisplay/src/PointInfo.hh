/**
 * @file PointInfo.hh
 *
 * @brief Struct for track point information
 *
 * @creation_date     { 20.03.2024 }
 * @modification_date ( 20.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef POINTINFO_H
#define POINTINFO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <TVector3.h>

#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"

// #include "old/particlesDefinition.hh"
#include "ParticlesDefinition.hh"

#include "utils.hh"

struct PointInfo
{
  int ID;
  int ChildTrackID;
  double Time;
  TVector3 Momentum;
  double Ek;
  double Edep;
  double StepLength;
  double StepDeltaLyz;
  double dEk_with_prev_pt;
  double dEk_dE_diff;
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
  int OwnerTrackId;
  std::vector<int> created_tracks_ids;

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
    
    OwnerTrackId = ownerTrack->GetTrackID();

    AvgPosition = (PrePosition + PostPosition) * 0.5;
    
    IsIntoSFGD = isPtInsideSFGD(LogVolName);

    // std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ";
    // size_t pos = LogVolName.find(to_replace);
    // LogVolNameShort = LogVolName;
    // if (pos != std::string::npos) LogVolNameShort.replace(pos, to_replace.length(), "~");
    LogVolNameShort = getLocationMaterialStr();

    Ek = -1.0;
    dEk_with_prev_pt = 0.0;
    ChildTrackID = -1;
  }

  void SetKinEnergyFromMomentum(int OwnerTrackPDG) { Ek = compute_energy_from_momentum(OwnerTrackPDG, Momentum.Mag()); }

  int getLocationMaterial();
  std::string getLocationMaterialStr();

  void print()
  {
    if (dEk_dE_diff > 1.0e-7 && dEk_dE_diff <= 1.0e-5) std::cout << YELLOW;
    else if (dEk_dE_diff > 1.0e-7) std::cout << GREEN;
    std::cout << "\t"
      << ID << " "
      << "dE: " << std::setw(12) << Edep << " "
      << "dL: " << std::setw(12) << StepLength << " "
      << "|Mom|: " << std::setw(12) << Momentum.Mag() << " "
      << "|Ek|: " << std::setw(12) << Ek << " "
      << "dEk: " << std::setw(12) << dEk_with_prev_pt << " "
      << "dEk: " << std::setw(12) << dEk_dE_diff << " "
      << "inSFGD: " << IsIntoSFGD << " "
      << " [X:" << AvgPosition.X() << " Y:" << AvgPosition.Y() << " Z:" << AvgPosition.Z() << "] "
      // << "Mat: "<< MaterialName
      << "V: "<< LogVolNameShort
      // << " P: " << PreProcessName
      // << " " << PostProcessName
      << std::endl;
    if (dEk_dE_diff > 1.0e-7) std::cout << RESET;
  }

};



class Track;

class TrackPoint
{
  public:
    int ID;
    // int ChildTrackID;
    double Time;
    TVector3 Momentum;
    double Ek;
    double Edep;
    double IncreasingEdep;
    double StepLength;
    double StepDeltaLyz;
    double dEk_with_prev_pt;
    double dEk_dE_diff; // with previous track point
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
    int PreProcessType;
    int PostProcessType;
    bool IsIntoSFGD;
    int OwnerTrackId;
    std::vector<int> created_tracks_ids;
    // std::vector<std::pair<int, double>> created_tracks;
    // std::vector<std::tuple<int, double, double, double, double>> created_tracks;
    std::vector<std::tuple<int, int, double, double, double, double, std::string, double, double, double, int>> created_tracks;
    std::vector<Track*> created_tracks_ptrs;

    TrackPoint(TND280UpTrackPoint* trackPoint, TND280UpTrack* ownerTrack);
    ~TrackPoint();

    void print();
    std::string getLocationMaterialStr();

    void SetKinEnergyFromMomentum(int OwnerTrackPDG) { Ek = compute_energy_from_momentum(OwnerTrackPDG, Momentum.Mag()); }
  private:
};

/* 
/// ProcessType
  fNotDefined 	
  fTransportation 	
  fElectromagnetic 	
  fOptical 	
  fHadronic 	
  fPhotolepton_hadron 	
  fDecay 	
  fGeneral 	
  fParameterisation 	
  fUserDefined 	
  fParallel 
// */

#endif