/**
 * @file EventInfo.hh
 *
 * @brief Struct for event information
 *
 * @creation_date     { 20.03.2024 }
 * // @modification_date ( 20.03.2024 )
 * @modification_date ( 22.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <TVector3.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMarker.h>

#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"
#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"

#include "utils.hh"
#include "PointInfo.hh"
#include "TrackInfo.hh"

#include "EventDisplay.hh" // EventDataDrawing


struct EventInfo
{
  bool is_irrelevant;

  int ID;
  int NVertices;
  int NHits;
  int NTracks;

  // TrackInfo *primary_lepton;
  std::unique_ptr<TrackInfo> primary_lepton;
  // TrackInfo primary_lepton;
  double neutrino_energy;
  int neutrino_pdg;
  [[maybe_unused]] short n_vtx_protons;
  [[maybe_unused]] short n_vtx_neutrons;
  [[maybe_unused]] short n_vtx_nuclei;

  std::map<int, TrackInfo> tracks; // {trk_id, trk}

  /// event display
  TH2F* h2_event_display_edep_xy_by_track_points;
  TH2F* h2_event_display_edep_xz_by_track_points;
  TH2F* h2_event_display_edep_yz_by_track_points;
  TCanvas* c_event_display_by_track_points;

  EventInfo(TND280UpEvent *nd280UpEvent, int eventId = -1)
  {
    is_irrelevant = false;

    h2_event_display_edep_xy_by_track_points = nullptr;
    h2_event_display_edep_xz_by_track_points = nullptr;
    h2_event_display_edep_yz_by_track_points = nullptr;
    c_event_display_by_track_points = nullptr;
    neutrino_energy = -999.0;
    neutrino_pdg = -999;

    ID = eventId;
    NVertices = nd280UpEvent->GetNVertices();

    if (NVertices > 1) is_irrelevant = true;

    NHits = nd280UpEvent->GetNHits();
    NTracks = nd280UpEvent->GetNTracks();

    /// look at vertex
    if (NVertices) {
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
    
      if (TMath::Abs(trk0->GetPDG()) == 12 || TMath::Abs(trk0->GetPDG()) == 14)
      {
        neutrino_energy = trk0->GetInitMom().Mag();
        neutrino_pdg = trk0->GetPDG();
      }
      else if (TMath::Abs(trk1->GetPDG()) == 12 || TMath::Abs(trk1->GetPDG()) == 14)
      {
        neutrino_energy = trk1->GetInitMom().Mag();
        neutrino_pdg = trk1->GetPDG();
      }
    }
       }

    // primary_lepton = nullptr;

    /// first loop over tracks
    std::map<int, TVector3> id_start_pos; // {id, TVector3(x,y,z)}
    std::map<int, int> id_par_id; // {id, parent_id}
    std::map<int, int> is_sfgd_contained;
    for (Int_t trjID = 0; trjID < NTracks; trjID++)
    {
      TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
      TrackInfo trk(track);

      if (!trk.startedInSFGD && !trk.stoppedInSFGD)
      {
        is_sfgd_contained.insert({trk.ID, 0});
      } else {
        is_sfgd_contained.insert({trk.ID, 1});
      }

      tracks.insert({trk.ID, trk});

      id_par_id.insert({trk.ID, trk.ParentID});
      id_start_pos.insert({trk.ID, trk.StartPos});

      if (trk.ParentID == 0 && (TMath::Abs(trk.PDG) == 11 || TMath::Abs(trk.PDG) == 13) )
      {
        // primary_lepton = &trk;
        primary_lepton = std::make_unique<TrackInfo>(trk);
        // std::cout << " ---- " << trk.ID << " " << trk.ParentID << " " << trk.PDG << std::endl;
      }

      // if (trk.ParentID == 0)
      // {
      //   primary_tracks_v.push_back(trk);
      //   primary_tracks_ids_v.push_back(trk.ID);
      // } else {
      //   remained_tracks.push_back(trk);
      // }
    }

    /// fill parents and childs for tracks (second loop)
    std::map<int, std::vector<int>> parents;
    std::map<int, std::vector<int>> childs;
    for (auto const& [tid, pid] : id_par_id)
    {
      if (is_sfgd_contained[tid] == 0) continue; // skip tracks which not started and stopped in sfgd

      std::vector<int> par;
      par.push_back(pid);
      parents.insert({tid, par});

      int pid_ = pid;
      while (pid_ != 0)
      {
        int t_id = id_par_id.at(pid_);
        parents[tid].push_back(t_id);
        pid_ = t_id;
      }

      if (childs.find(pid) != childs.end())
      {
        childs[pid].push_back(tid);
      } else if (pid != 0){
        std::vector<int> chld;
        chld.push_back(tid);
        childs[pid] = chld;
      }
    }

    /// set parents and childs for tracks (third loop)
    std::map<int, TrackInfo> new_tracks_map;
    for (auto item: tracks)
    {
      TrackInfo trk = item.second;
      int trk_parentid = trk.ParentID;
      if (trk_parentid == 0)
      {
        trk.SetGParentID(-1);

        trk.SetChilds(childs[trk.ID], tracks);
        trk.SetParents(parents[trk.ID]);
      }
      // else if (tracks.find(trk_parentid) == tracks.end())
      // {
      //   trk.SetGParentID(-2);
      // }
      else
      {
        TrackInfo parent_trk = tracks.at(trk_parentid);
        trk.SetGParentID(parent_trk.ParentID);
        // parent_trk.AddDaughterTrack(trk.ID);

        // trk.parent_tracks_ids = parents[trk.ID];
        // trk.daughter_tracks_ids = childs[trk.ID];

        trk.SetChilds(childs[trk.ID], tracks);
        trk.SetParents(parents[trk.ID]);

        int nearest_pt_id_of_parent_trk = parent_trk.GetNearestPtID(trk.StartPos);
        trk.NearestPtIDofParent = nearest_pt_id_of_parent_trk;
        parent_trk.MapPtToChild(nearest_pt_id_of_parent_trk, trk.ID);

        // parent_trk.points[nearest_pt_id_of_parent_trk].created_tracks_ids.push_back(trk.ID);
      }
      // trk.print();
      new_tracks_map.insert({trk.ID, trk});
    }

    tracks.clear();
    tracks = new_tracks_map;
    new_tracks_map.clear();

    for (auto item: tracks)
    {
      TrackInfo trk = item.second;
      // trk.processChild_test(tracks);
      trk.collectEdepByChilds(tracks);
      new_tracks_map.insert({trk.ID, trk});
    }
    tracks.clear();
    tracks = new_tracks_map;
    new_tracks_map.clear();
  }

  

  void print();

  std::vector<TrackInfo> getTracksLeftSFGD();


  /// display
  void _fill_display_with_hits_as_2dhist(int trackID = -999) {}

  std::string _fill_display_with_track_points_as_2dhist(
      int trackID,
      std::string &canvas_title,
      std::string &out_fname_prefix
    )
  {
    std::stringstream canvas_title_ss;
    std::stringstream out_fname_prefix_ss;

    if (trackID != -999 && tracks.find(trackID) != tracks.end())
    {
      TrackInfo trk = tracks.at(trackID);

      canvas_title_ss << "Event " << ID << " display: "
                      << "Track: " << trackID << ", "
                      << "PDG: " << trk.PDG << ","
                      << "Ek0: " << trk.InitKinEnergy;

      out_fname_prefix_ss << "Display_Evt" << ID
                          << "_Trk" << trackID
                          << "_PDG" << trk.PDG
                          << "_Ek0" << std::setprecision(5) << trk.InitKinEnergy;

      for (PointInfo pt: trk.points)
      {
        TVector3 AvgPos_SFGDCS = toSFGDCoordinateSystem(pt.AvgPosition);
        h2_event_display_edep_xy_by_track_points->Fill(AvgPos_SFGDCS.Y(), AvgPos_SFGDCS.X(), pt.Edep);
        h2_event_display_edep_xz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.X(), pt.Edep);
        h2_event_display_edep_yz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.Y(), pt.Edep);
      }

      // return canvas_title_ss.str();

      canvas_title = canvas_title_ss.str();
      out_fname_prefix = out_fname_prefix_ss.str();
      return canvas_title_ss.str();
    }

    canvas_title_ss << "Event " << ID << " display: Nu_PDG " << neutrino_pdg << ", Nu_ene " << std::setprecision(5) << neutrino_energy;
    out_fname_prefix_ss << "Display_Evt" << ID;

    for (auto item: tracks)
    {
      TrackInfo trk = item.second;
      
      // if (trackID != -999 && trk.ID != trackID) continue;
      
      for (PointInfo pt: trk.points)
      {
        TVector3 AvgPos_SFGDCS = toSFGDCoordinateSystem(pt.AvgPosition);
        h2_event_display_edep_xy_by_track_points->Fill(AvgPos_SFGDCS.Y(), AvgPos_SFGDCS.X(), pt.Edep);
        h2_event_display_edep_xz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.X(), pt.Edep);
        h2_event_display_edep_yz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.Y(), pt.Edep);
      }
    }
  
    // return canvas_title_ss.str();

    canvas_title = canvas_title_ss.str();
    out_fname_prefix = out_fname_prefix_ss.str();
    return canvas_title_ss.str();
  }

  void create_display(int trackID, bool by_track_points, bool by_hits, bool as_svg,
                      std::string output_dir_tag)
  {
    if (by_track_points)
    {
      h2_event_display_edep_xy_by_track_points = new TH2F(
        "XY plane", "", 56, 0.0, 56.0, 192, 0.0, 192.0);
      h2_event_display_edep_xz_by_track_points = new TH2F(
        "XZ plane", "", 184, 0.0, 184.0, 192, 0.0, 192.0);
      h2_event_display_edep_yz_by_track_points = new TH2F(
        "YZ plane", "", 184, 0.0, 184.0, 56, 0.0, 56.0);
      
      TCanvas* c_event_display_by_track_points = new TCanvas(
        "c_event_display_by_track_points", "c_event_display_by_track_points", 1100, 1000);

      // std::string out_fname_event_display = output_dir_tag
      //     + "/event_display/PDG" + std::to_string(main_particle_PDG)
      //     + "-event_display" + std::to_string(eventId) + ".pdf";
      std::string out_fname_prefix = "";
      std::string canvas_title = "";
      std::string _ = _fill_display_with_track_points_as_2dhist(
        trackID, canvas_title, out_fname_prefix);
      
      std::string out_fname_event_display = output_dir_tag
        + "/event_display/" + out_fname_prefix + ".pdf";

      std::cout << "canvas_title: " << canvas_title << std::endl;
      std::cout << "out_fname_prefix: " << out_fname_prefix << std::endl;

      createEventDisplay(
        c_event_display_by_track_points,
        canvas_title,
        h2_event_display_edep_xy_by_track_points,
        h2_event_display_edep_yz_by_track_points,
        h2_event_display_edep_xz_by_track_points,
        true,
        out_fname_event_display
      );

      delete h2_event_display_edep_xy_by_track_points;
      delete h2_event_display_edep_yz_by_track_points;
      delete h2_event_display_edep_xz_by_track_points;
      delete c_event_display_by_track_points;

    }
    
    if (by_hits) // in order to remove warning of unused variable
    {

    }
    
    if (as_svg) // in order to remove warning of unused variable
    {

    }

  } // event display. Maybe return canvas


  // void create_display_as_2dhist(
  //   int trackID,
  //   bool by_track_points,
  //   bool by_hits,
  //   bool as_svg,
  //   std::string output_dir_tag) {}

  void create_display_as_2dgraph(
    int trackID,
    bool by_track_points,
    bool by_hits,
    bool as_svg,
    std::string output_dir_tag)
  {
    if (by_track_points)
    {
      // g2_event_display_edep_xy_by_track_points = new TH2F(
      //   "XY plane", "", 56, 0.0, 56.0, 192, 0.0, 192.0);
      // g2_event_display_edep_xz_by_track_points = new TH2F(
      //   "XZ plane", "", 184, 0.0, 184.0, 192, 0.0, 192.0);
      // g2_event_display_edep_yz_by_track_points = new TH2F(
      //   "YZ plane", "", 184, 0.0, 184.0, 56, 0.0, 56.0);
      
      TCanvas* c_event_display_by_track_points = new TCanvas(
        "c_event_display_by_track_points", "c_event_display_by_track_points", 1100, 1000);

      TGraph * g2_event_display_edep_xy_by_track_points = new TGraph();

      g2_event_display_edep_xy_by_track_points->GetXaxis()->SetRangeUser(0.0, 192.0); // Range for X axis: 0 to 100
      g2_event_display_edep_xy_by_track_points->GetYaxis()->SetRangeUser(0.0, 56.0);  // Range for Y axis: 0 to 56

      c_event_display_by_track_points->cd();
      std::vector<TMarker*> markers_xy;
      std::vector<TMarker*> markers_xz;
      std::vector<TMarker*> markers_yz;
      for (auto item: tracks)
      {
        TrackInfo trk = item.second;
        
        // if (trackID != -999 && trk.ID != trackID) continue;
        
        for (PointInfo pt: trk.points)
        {
          TVector3 AvgPos_SFGDCS = toSFGDCoordinateSystem(pt.AvgPosition);
          // h2_event_display_edep_xy_by_track_points->Fill(AvgPos_SFGDCS.Y(), AvgPos_SFGDCS.X(), pt.Edep);
          // h2_event_display_edep_xz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.X(), pt.Edep);
          // h2_event_display_edep_yz_by_track_points->Fill(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.Y(), pt.Edep);

          TMarker *m_xy = new TMarker(AvgPos_SFGDCS.Y(), AvgPos_SFGDCS.X(), 1);
          markers_xy.push_back(m_xy);
          m_xy->SetMarkerSize(1);
          m_xy->SetMarkerColor(31);
          // m_xy->Draw();

          TMarker *m_xz = new TMarker(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.X(), 1);
          markers_xy.push_back(m_xz);
          m_xz->SetMarkerSize(1);
          m_xz->SetMarkerColor(31);
          // m_xz->Draw();

          TMarker *m_yz = new TMarker(AvgPos_SFGDCS.Z(), AvgPos_SFGDCS.Y(), 1);
          markers_xy.push_back(m_yz);
          m_yz->SetMarkerSize(1);
          m_yz->SetMarkerColor(31);
          // m_yz->Draw();
        }
      }

      for (auto m: markers_xy) delete m;
      for (auto m: markers_xz) delete m;
      for (auto m: markers_yz) delete m;

      delete c_event_display_by_track_points;
      delete g2_event_display_edep_xy_by_track_points;
    }

    if (by_hits) // in order to remove warning of unused variable
    {

    }

    if (as_svg) // in order to remove warning of unused variable
    {

    }
  }

};













class TracksTable
{
  public:
    /// int in map -- TrackID
    std::map<int, Track*> track_ptr;
    std::map<int, int> track_PDG;
    std::map<int, int> track_parent_id;

    TracksTable();
    ~TracksTable();

    void AddTrack(Track* track);
    void RemoveTrack(int TrackID);
    Track* GetTrackByID(int TrackID);
  private:
};


enum class EventType
{
  NUE = 0,
  NUMU,
  ANUE,
  ANUMU,
  ELE,
  POS,
  MU,
  AMU,
  PROT,
  OTHER,
  UNDEFINED,
};

class Event
{
  public:
    int ID;
    int NVertices;
    int NHits;
    int NTracks;
    EventType Type;

    // std::unique_ptr<TrackInfo> primary_lepton;
    // std::unique_ptr<Track> primary_lepton;
    // std::unique_ptr<Track> highest_momentum_neg_track;
    // std::unique_ptr<Track> highest_momentum_pos_track;

    Track * primary_lepton;
    Track * highest_momentum_neg_track;
    Track * highest_momentum_pos_track;

    double neutrino_energy;
    int neutrino_pdg;
    int neutrino_target_pdg;
    short n_vtx_protons;
    short n_vtx_neutrons;
    short n_vtx_nuclei;
    short n_vtx_in_tracks;
    short n_vtx_out_tracks;
    short n_vtx_out_protons;
    short n_vtx_out_neutrons;
    short n_vtx_out_pos_pions;
    short n_vtx_out_neg_pions;
    short n_vtx_out_neu_pions;

    std::map<int, Track*> tracks_m; // {trk_id, trk}

    EventDataDrawing * EventDisplay;


    Event(TND280UpEvent *nd280UpEvent, int eventId);
    ~Event();

    Track * GetTrack(int TrackID);

    void FillHighestMomentumTracks(std::map<int, double>& momentum_of_tracks);
    void FillTracksTree(std::map<int, std::tuple<int, double, double, double, double>>& trk_mom_pt);

    void print();
  private:
};

#endif


// for (Int_t j=2; j<n-1; j++) {
//       TMarker *m = new TMarker(nx[j], 0.5*ny[j], 22);
//       m->SetMarkerSize(2);
//       m->SetMarkerColor(31+j);
//       m->Draw();
//    }