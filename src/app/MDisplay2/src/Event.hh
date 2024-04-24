
/**
 * @file Event.hh
 *
 * @brief Struct for event information
 *
 * @creation_date     { 26.03.2024 }
 * @modification_date ( 26.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef EVENT_H
#define EVENT_H

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
#include "Track.hh"

#include "EventDisplay.hh"

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
    bool CHECK_MEMORY;
    double vm;
    double rss;

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

    double vtx_out_particles_totene;
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
    short n_vtx_out_neu_apions;

    std::map<int, Track*> tracks_m; // {trk_id, trk}
    // std::map<std::tuple<int, int>, TrackPoint*> evt_points_m;

    EventDataDrawing * histCollection;
    EventDisplay * eventDisplay;

    Event(EventDataDrawing * histCollection, TND280UpEvent *nd280UpEvent, int eventId);
    ~Event();

    Track * GetTrack(int TrackID);

    void ProcessVertex(TND280UpEvent *nd280UpEvent);
    void FillHighestMomentumTracks(std::map<int, double>& momentum_of_tracks);
    void FillTracksTree(std::map<int, std::tuple<int, double, double, double, double>>& trk_mom_pt);
    // void FillTracksTree2();
    // void ProcessTracksTree2();
    void FillTracksTree3();
    void CreateDisplay(std::string out_fpath);
    // void SaveDataHistograms(std::string output_dir_tag);

    void print();
  private:
};

#endif