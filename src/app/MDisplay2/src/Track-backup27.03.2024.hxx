/**
 * @file Track.hh
 *
 * @brief Struct for track information
 *
 * @modification_date ( 26.03.2024 )
 * @creation_date     { 26.03.2024 }
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
#include "TrackPoint.hh"


class Track
{
  public:
    int ID;
    int ParentID;
    // int NearestPtIDofParent;
    // int GParentID;
    int PDG;
    int NPoints;
    // int Level;
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
    // bool left_and_returned_to_SFGD;
    // double dE_in_reflector;
    // double dE_in_fiber;
    // double dE_in_scintillator;
    double dE_over_pts_in_sfgd;
    // double dE_over_all_pts;
    double left_SFGD_with_Ek;
    double _left_SFGD_with_Ek; // control
    double sum_Ek0_of_childs;
    // double sum_Ek0_of_childs_dE;
    // double sum_Ek0_of_childs_dE_Ekleft;

    /// for tracks that produces as secondaries at some track point, but bot included into list of tracks
    double UntrackedEk0Loss;
    double UntrackedEk0Loss_wchilds;

    double Len_over_pts_in_sfgd;

    double edep_in_SFGD_wchilds;
    double left_SFGD_elepos_sumEk_childs;
    double left_SFGD_gamma_sumEk_childs;
    // double left_SFGD_neutron_sumEk_childs;
    double left_SFGD_other_sumEk_childs;

    double left_SFGD_with_Ek_wchilds;
    std::string left_SFGD_plane;
    // TrackType type;

    // double EdepSumAtPt;

    TVector3 LeftSFGDwithMometum;

    double ReturnedToSFGDwithEk;
    double dEkOutSFGD;
    double EnteredInSFGDwithEk;

    double dEkOutSFGD_wchilds;
    double UntrackedEk0Loss_gamma;

    int leftSFGDatPt;
    int returnedToSFGDatPt;

    Track * parent;
    std::vector<Track*> childs;
    // std::vector<int> points_related_to_childs_ids;
    std::vector<TrackPoint *> points_related_to_childs_ptrs;
    // std::map<Track*, TrackPoint*> point_child_m;
    std::map<TrackPoint*, std::vector<Track*>> point_child_m;

    std::map<int, TrackPoint*> points_m;

    /// control
    double total_edep_sum_by_childs_recursive;
    double total_edep_sum_by_childs_recursive_wutek;
    double total_edep_sum_by_childs_ekin;

    int HasUntrackedParticles;


    /// losses
    /*
     * Track_Ek = IonizationLoss
     *          + EkOutsideSFGDLoss                 // if track left outside SFGD (and maybe returned)
     *          + [SumEkOfChilds_PreLoss:recursive] // for each child track the same scheme
     *          + (UntrackedParticlesSumEkLoss:error in simulation?)
     *          + {GammaConversionLoss}             // if photon decays into e+e-
     *          + {ElePosAnnihilationLoss}          // if e+ or e- annihilate to 2 photons
     */
    double IonizationLoss; // ordinary dE per TrackPoint
    double UntrackedParticlesSumEkLoss; // sum Ek0 of particles which were
                                        // created by geant4 simulation at point,
                                        // but do not exist as tracks
    double EkOutsideSFGDLoss; // if track left SFGD several times
    double GammaConversionLoss; // gamma -> e+e- : extra Ek = 0.51*2 needed
    double ElePosAnnihilationLoss; // e+e- or e-e+ -> 2 photons
    double SumEkOfChilds_PreLoss;

    /// all the same as above but collected by all childs of that track
    double IonizationLoss_WithChilds;
    double UntrackedParticlesSumEkLoss_WithChilds;
    double EkOutsideSFGDLoss_WithChilds;
    double EkOutsideSFGDLoss_WithChilds_SplitByElePos;
    double EkOutsideSFGDLoss_WithChilds_SplitByGamma;
    double EkOutsideSFGDLoss_WithChilds_SplitByNeutron;
    double GammaConversionLoss_WithChilds;
    double ElePosAnnihilationLoss_WithChilds;


    Track(TND280UpTrack* track);
    ~Track();

    void SetParent(Track * t) { parent = t; };
    Track * GetParent() { return parent; }

    // void SetParent(Track * t) { parent = t; };

    void CollectEdepInSFGD(
      double& EdepInSFGD, double& ElePosLeftSFGD, double& GammaLeftSFGD,
      double& OthersLeftSFGD, double& UntrackedEkLoss, double &dEkOutSFGD_wchilds);

    void CollectEdepInSFGDbyChilds(double& EdepInSFGD);
    void CollectLeftSFGDeleposByChilds(double& ElePosLeftSFGD);
    void CollectLeftSFGDgammaByChilds(double& GammaLeftSFGD);
    void CollectLeftSFGDothersByChilds(double& OthersLeftSFGD);
    void CollectUntrackedEkInSFGDByChilds(double& UntrackedEkLoss);
    void CollectEdepOdReturnedTrksToSFGDByChilds(double& dEkOutSFGD_wchilds);
    void CollectUntrackedEkInSFGDByChildsForGamma(double& UntrackedEkLoss_gamma);

    void print(std::string indent = "", bool with_points = true, bool my_condition = false);

    void SetControl();
  private:
};



#endif