/**
 * @file TrackInfo.hh
 *
 * @brief Struct for track information
 *
 * @creation_date     { 20.03.2024 }
 * @modification_date ( 20.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <TVector3.h>

#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"

#include "utils.hh" // getDetectorPlaneOfPoint
#include "ColorDefinitions.hh"
#include "PointInfo.hh"


enum class TrackType
{
  Undefined = -1,
  DeltaElectron,
};

struct TrackInfo
{
  int ID;
  int ParentID;
  int NearestPtIDofParent;
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
  std::map<int, PointInfo> points_m;
  std::vector<PointInfo> points;
  std::vector<PointInfo> points_with_large_dEk;
  std::vector<int> parent_tracks_ids;
  std::vector<int> daughter_tracks_ids;
  int root_track_id; // id of track with parent_id == 0
  bool startedInSFGD;
  bool stoppedInSFGD;
  bool left_and_returned_to_SFGD;
  double dE_in_reflector;
  double dE_in_fiber;
  double dE_in_scintillator;
  double dE_over_pts_in_sfgd;
  double dE_over_all_pts;
  double left_SFGD_with_Ek;
  double sum_Ek0_of_childs;
  double sum_Ek0_of_childs_dE;
  double sum_Ek0_of_childs_dE_Ekleft;

  double edep_in_SFGD_wchilds;
  double left_SFGD_elepos_sumEk_childs;
  double left_SFGD_gamma_sumEk_childs;
  double left_SFGD_neutron_sumEk_childs;
  double left_SFGD_other_sumEk_childs;

  double left_SFGD_with_Ek_wchilds;
  std::string left_SFGD_plane;
  TrackType type;

  TrackInfo(TND280UpTrack* track);

  int GetNearestPtID(TVector3 pt_pos);

  void SetGParentID(int gparentid) { GParentID = gparentid; };
  void SetLevel(int level) { Level = level; };
  void AddDaughterTrack(int track_id) { daughter_tracks_ids.push_back(track_id); }
  void AddParentTrack(int track_id) { parent_tracks_ids.push_back(track_id); }
  void SetChilds(std::vector<int>& childs, std::map<int, TrackInfo>& tracks);
  void SetParents(std::vector<int>& parents);

  void processChild_test(std::map<int, TrackInfo>& tracks);
  void collectEdepByChilds(std::map<int, TrackInfo>& tracks);

  void MapPtToChild(int nearest_pt_id_of_parent_trk, int TrackID);

  void getEdepWithChilds(double &edep_in_SFGD, double &left_SFGD_with_Ek0, std::map<int, TrackInfo>& tracks)
  {
    /// edepWithChilds =
    ///  dE_over_pts_in_sfgd_of_this_track
    ///  + sum(dE_over_pts_in_sfgd_of_child_tracks)
    ///  + Ek_left_SFGD;

    double _edep_in_SFGD = 0.0;
    double _left_SFGD_with_Ek = 0.0;
    for (auto cid: daughter_tracks_ids)
    {
      double cid_edep_in_SFGD = 0.0;
      double cid_left_SFGD_with_Ek = 0.0;
      TrackInfo ctr = tracks.at(cid);
      ctr.getEdepWithChilds(cid_edep_in_SFGD, cid_left_SFGD_with_Ek, tracks);

      _edep_in_SFGD += cid_edep_in_SFGD;
      _left_SFGD_with_Ek += cid_left_SFGD_with_Ek;
    }

    if (left_SFGD_with_Ek != -1.0)
    {
      left_SFGD_with_Ek0 = _left_SFGD_with_Ek + left_SFGD_with_Ek;
    }
    // edep_in_SFGD = dE_over_pts_in_sfgd + _edep_in_SFGD;
    edep_in_SFGD += dE_over_pts_in_sfgd + _edep_in_SFGD + left_SFGD_with_Ek0;
  }

  void computeTotalEdepInSFGDbyChilds(
    double &edep_in_SFGD,
    double &sum_Ek0_of_gammas_left_SFGD,
    double &sum_Ek0_of_elepos_left_SFGD,
    double &sum_Ek0_of_neutrons_left_SFGD,
    double &sum_Ek0_of_other_left_SFGD,
    std::map<int, TrackInfo>& tracks);

  void computeTotalEdepInSFGDbyChilds_v2(
    double &edep_in_SFGD,
    double &sum_Ek0_of_gammas_left_SFGD,
    double &sum_Ek0_of_elepos_left_SFGD,
    double &sum_Ek0_of_neutrons_left_SFGD,
    double &sum_Ek0_of_other_left_SFGD,
    std::map<int, TrackInfo>& tracks);

  void print();
};


class Track
{
  public:
    int ID;
    int ParentID;
    int NearestPtIDofParent;
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
    // std::map<int, PointInfo> points_m;
    // std::vector<PointInfo> points;
    // std::vector<PointInfo> points_with_large_dEk;
    std::vector<int> parent_tracks_ids;
    std::vector<int> daughter_tracks_ids;
    int root_track_id; // id of track with parent_id == 0
    bool startedInSFGD;
    bool stoppedInSFGD;
    bool left_and_returned_to_SFGD;
    double dE_in_reflector;
    double dE_in_fiber;
    double dE_in_scintillator;
    double dE_over_pts_in_sfgd;
    double dE_over_all_pts;
    double left_SFGD_with_Ek;
    double sum_Ek0_of_childs;
    double sum_Ek0_of_childs_dE;
    double sum_Ek0_of_childs_dE_Ekleft;

    double Len_over_pts_in_sfgd;

    double edep_in_SFGD_wchilds;
    double left_SFGD_elepos_sumEk_childs;
    double left_SFGD_gamma_sumEk_childs;
    double left_SFGD_neutron_sumEk_childs;
    double left_SFGD_other_sumEk_childs;

    double left_SFGD_with_Ek_wchilds;
    std::string left_SFGD_plane;
    TrackType type;

    // double EdepSumAtPt;

    Track * parent;
    std::vector<Track*> childs;
    std::vector<int> points_related_to_childs_ids;
    std::vector<TrackPoint *> points_related_to_childs_ptrs;
    // std::map<Track*, TrackPoint*> point_child_m;
    std::map<TrackPoint*, std::vector<Track*>> point_child_m;

    std::map<int, TrackPoint*> points_m;

    Track(TND280UpTrack* track);
    ~Track();

    void SetParent(Track * t) { parent = t; };
    Track * GetParent() { return parent; }

    // void SetParent(Track * t) { parent = t; };

    void CollectEdepInSFGD(double& EdepInSFGD, double& ElePosLeftSFGD, double& GammaLeftSFGD, double& OthersLeftSFGD);

    void print(std::string indent = "", bool with_points = true);
  private:
};

#endif