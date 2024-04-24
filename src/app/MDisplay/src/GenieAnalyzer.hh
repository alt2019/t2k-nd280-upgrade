/**
 * @file GenieAnalyzer.hh
 *
 * @brief Analysis of nu events simulated from GENIE
 *
 * @creation_date     { 20.03.2024 }
 * // @modification_date ( 22.03.2024 )
 * @modification_date ( 23.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef GENIE_ANALYZER_H
#define GENIE_ANALYZER_H

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

#include "SVG.hh"
#include "utils.hh"

// #include "old/particlesDefinition.hh"
#include "ParticlesDefinition.hh"

#include "GraphicsHolder.hh"

#include "PointInfo.hh"
#include "TrackInfo.hh"
#include "EventInfo.hh"

void analize_events_from_genie(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num = -1,
    int stop_event_num = -1);

void analize_events_from_genie_extended(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num = -1,
    int stop_event_num = -1);

void analize_events_from_genie_new(
    std::string input_rootfilename,
    std::string output_dir_tag,
    int start_event_num = -1,
    int stop_event_num = -1);

#endif