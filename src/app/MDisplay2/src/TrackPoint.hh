/**
 * @file TrackPoint.hh
 *
 * @brief Struct for track point information
 *
 * @creation_date     { 26.03.2024 }
 * @modification_date ( 26.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef TRACKPOINT_H
#define TRACKPOINT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <TVector3.h>

#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"

#include "ParticlesDefinition.hh"

#include "utils.hh"

typedef std::vector<std::tuple<int, int,
                               double, double, double, double,
                               std::string,
                               double, double, double,
                               int>> CreatedTrackAtPtDs;

class Track;

// class PreTrack
// {
//   public:
//     int ID;
//     int ParentID;
//     int PDG;
//     double InitKinEnergy;
//     // double dEoverPts;
// };

class TrackPoint
{
  public:
    int ID;
    int idx;
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
    int PreProcessType;
    int PostProcessType;

    bool IsFirstPtOfTrack;
    bool IsLastPtOfTrack;
    bool IsFirstPtOfTrack_InSFGD;
    bool IsLastPtOfTrack_InSFGD;

    // double dEk_with_prev_pt;
    // double dEk_dE_diff; // with previous track point

    double IncreasingEdep;
    bool IsIntoSFGD;
    bool IsIntoSFGDFV;
    int OwnerTrackId;

    std::vector<int> created_tracks_ids;
    // std::vector<std::pair<int, double>> created_tracks;
    // std::vector<std::tuple<int, double, double, double, double>> created_tracks;
    CreatedTrackAtPtDs created_tracks;
    std::vector<Track*> created_tracks_ptrs;

    TrackPoint(TND280UpTrackPoint* trackPoint, TND280UpTrack* ownerTrack);
    ~TrackPoint();

    void SetKinEnergyFromMomentum(int OwnerTrackPDG) { Ek = compute_energy_from_momentum(OwnerTrackPDG, Momentum.Mag()); }

    std::string getLocationMaterialStr();

    std::string MaterialToShort(std::string mat);
    std::string ProcessNameToShort(std::string proc);

    void print();

  private:
    bool _isIntoSFGD();
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
