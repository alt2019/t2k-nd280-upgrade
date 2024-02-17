/**
 * @file AnalizeEvents.hh
 *
 * @brief Analize events
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date ( 25.05.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef ANALIZE_EVENTS_H
#define ANALIZE_EVENTS_H

#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <optional> // std::optional, std::nullopt
#include <ctime> // std::ctime

#include <TH1F.h>
#include <TH2F.h>

#include "TND280UpTrack.hh"

#include "color_definitions.hh"
// #include "ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
#include "ExtractHitInfo.hh"
#include "ExtractTrackPointInfo.hh"
#include "ExtractTrackInfo.hh"
#include "ExtractVertexInfo.hh"
#include "CreateInteractiveVisualization.hh"
#include "XML.hh"

#include "omp.h"





double get_max_of_map(std::map<std::pair<int, int>, double>& cube_edep_map)
{
	std::map<std::pair<int, int>, double>::iterator best_max
        = std::max_element(cube_edep_map.begin(), cube_edep_map.end(), [] (
        	const std::pair<std::pair<int, int>, double>& a,
        	const std::pair<std::pair<int, int>, double>& b)->bool{ return a.second < b.second; } );
    double max_val = best_max->second;
    return max_val;
}

// -------------------------------------------------------------------

#define OPTIONAL_PARAM NULL

using namespace std;

struct EventAnalizer
{
	typedef std::map<std::pair<int, int>, double> ScanMap;

	int EventID;
	int NTracks;
	int NVertices;
	int NHits;

	TND280UpEvent* nd280UpEventPtr;

	map<int, int> eventId2TrackIdMap;
	std::map<int, std::vector<int>> tracksParentsTree;
	std::map<int, std::vector<int>> tracksChildsTree;

	vector<SimplifiedTrackInfo*> tracks;

	EventAnalizer(TTree* tree, TND280UpEvent* nd280UpEvent, int eventInd)
	{
		tree->GetEntry(eventInd);

		nd280UpEventPtr = nd280UpEvent;

		EventID = nd280UpEventPtr->GetEventID();
		NTracks = nd280UpEventPtr->GetNTracks();
		NVertices = nd280UpEventPtr->GetNVertices();
		NHits = nd280UpEventPtr->GetNHits();

		createTracksTree();
	}

	void getTracks()
	{

	}

	void getHits()
	{

	}

	void getVertices()
	{

	}

	void createTracksTree()
	{
		std::map<int, std::vector<int>> tracksTree;
		std::map<int, int> idParentIdTracksMap;

		for (int itrack = 0; itrack < NTracks; ++itrack)
		{ // start loop over tracks
			TrackInformation track{nd280UpEventPtr, itrack, false}; // get track

			auto _ = tracksTree.find(track.ParentID);
			if (_ != tracksTree.end())
			{
				(_->second).push_back(track.TrackID);
			} else {
				tracksTree.insert({track.ParentID, {track.TrackID}});
			}

			idParentIdTracksMap.insert({track.TrackID, track.ParentID});
			eventId2TrackIdMap.insert({track.TrackID, itrack});
		} // start loop over tracks

		for (auto& item: idParentIdTracksMap)
		{
			/// fill parents
			std::vector<int> ids = {item.second};
			int id = item.second;
			while (1)
			{
				auto _ = idParentIdTracksMap.find(id);
				if (_ != idParentIdTracksMap.end())
				{
					id = _->second;
					ids.push_back(id);
				} else {
					break;
				}
			}
			tracksParentsTree.insert({item.first, ids});

			/// fill childs
			auto __ = tracksChildsTree.find(item.second);
			if (__ != tracksChildsTree.end())
			{
				__->second.push_back(item.first);
			} else {
				tracksChildsTree.insert({item.second, {item.first}});
			}
		}
	}

	/// printing ///
	void print()
	{
		cout << cprint("EventID: "  , RED ) << setw(4) << EventID   << " " 
			 << cprint("NTracks: "  , BLUE) << setw(4) << NTracks   << " " 
			 << cprint("NVertices: ", BLUE) << setw(4) << NVertices << " " 
			 << cprint("NHits: "    , BLUE) << setw(4) << NHits     << " " 
			 << endl;
	}

	void printTracks(const vector<int>& skip_particles_pdg = {})
	{
		for (int itrack = 0; itrack < NTracks; ++itrack)
		{ // start loop over tracks
			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, false}; // get track
			if (std::find(skip_particles_pdg.begin(), skip_particles_pdg.end(), track.PDG) != skip_particles_pdg.end()) continue;
			track.print(1);
		}
	}

	void printPrimaryTracks()
	{
		for (int itrack = 0; itrack < NTracks; ++itrack)
		{ // start loop over tracks
			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true}; // get track
			if (track.ParentID == 0) track.print(1);
			else continue;
		}
	}

	void printTrackWithParents(int TrackID)
	{
		TrackInformation track{nd280UpEventPtr, eventId2TrackIdMap[TrackID], tracksParentsTree, tracksChildsTree, true}; // get track
		track.print(1);

		/// recursive variant ///
		int parentId = track.ParentID;
		while (1)
		{
			TrackInformation parentTrack{nd280UpEventPtr, eventId2TrackIdMap[parentId], tracksParentsTree, tracksChildsTree, true}; // get track
			if (parentId == parentTrack.ParentID) break;
			parentTrack.print(1);
			parentId = parentTrack.ParentID;
		}

		/// loop variant ///
		for (auto& parentTrackId: track.parentIDsPath)
		{
			TrackInformation parentTrack{nd280UpEventPtr, eventId2TrackIdMap[parentTrackId], tracksParentsTree, tracksChildsTree, true};
			parentTrack.print();
		}
	}

	void printHits(const vector<int>& skip_particles_pdg = {})
	// void printHits(const optional<vector<int>>& skip_particles_pdg = nullopt)
	{
		cout << cprint("\tHits stored in event:", RED) << endl;

		for (int ihit = 0; ihit < NHits; ++ihit)
		{ // start loop over hits
			HitInformation hit{nd280UpEventPtr, ihit};
			if (std::find(skip_particles_pdg.begin(), skip_particles_pdg.end(), hit.PDG) != skip_particles_pdg.end()) continue;
			hit.print();
		} // stop loop over hits
	}

	void printVertices()
	{
		cout << cprint("\tVertices stored in event:", RED) << endl;
        for (int ivertex = 0; ivertex < NVertices; ++ivertex)
        { // start loop over vertices
        	VertexInformation vertex{nd280UpEventPtr, ivertex};
        	if (vertex.isIntoSuperFGD()) vertex.print();
        	else cout << "Vertex not into SFGD!!!" << endl;
        } // stop loop over vertices
	}

	/// visualization ///
	void createDetectorResponse()
	{

	}

	string createSVGofTracksTree(string dirname, string tag, int eventID, double energyCutMin = 1.0e-6 /* 1 eV */)
	{
		std::map<int, int> tracksCount;
		string svg_group_of_tracks_xz = "";
		string svg_group_of_tracks_yz = "";
		string svg_group_of_tracks_xy = "";
		ScanMap scanTracksXZ, scanTracksYZ, scanTracksXY;
		for (int itrack = 0; itrack < NTracks; ++itrack)
		{ // start loop over tracks
			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true}; // get track

			// if (track.Edep < energyCutMin && track.PDG != 22) continue;

			track.print(1);

			if (tracksCount.find(track.PDG) != tracksCount.end())
			{
				tracksCount[track.PDG] += 1;
			} else {
				tracksCount.insert({track.PDG, 1});
			}

			auto [svg_trackpath_xz, svg_trackpath_yz, svg_trackpath_xy, svg_trackpath_start_pt] = track.asSvgIntoSuperFGD();
			svg_group_of_tracks_xz += svg_trackpath_xz;
			svg_group_of_tracks_yz += svg_trackpath_yz;
			svg_group_of_tracks_xy += svg_trackpath_xy;

			for (int ipt = 0; ipt < track.NPoints; ++ipt)
			{ // start loop over points
				PointInformation point(track.nd280UpTrack, ipt, track.PDG);

				if (!point.isIntoSuperFGD()) continue;
				if (point.PtEdep < energyCutMin) continue;

				__insert_point_coords_into_cubes_map(scanTracksXZ, point, 0, 2);
				__insert_point_coords_into_cubes_map(scanTracksYZ, point, 1, 2);
				__insert_point_coords_into_cubes_map(scanTracksXY, point, 0, 1);
			} // stop loop over points
		} // stop loop over tracks

		/// convert tracksCount map to string
		std::string tracksCountStr = "";
		for (auto& item: tracksCount)
		{
			tracksCountStr += to_string(item.first) + ":" + to_string(item.second) + ",";
		}
		tracksCountStr.pop_back();

		


		XMLObject * svg_group_of_tracks_xz_xml = new XMLObject("g", svg_group_of_tracks_xz, {{"id", "group_of-tracks-xz"}});
		XMLObject * svg_group_of_tracks_yz_xml = new XMLObject("g", svg_group_of_tracks_yz, {{"id", "group_of-tracks-yz"}});
		XMLObject * svg_group_of_tracks_xy_xml = new XMLObject("g", svg_group_of_tracks_xy, {{"id", "group_of-tracks-xy"}});

		/// set energy cuts on tracks
		__set_energy_cut_on_cube_edep_map(scanTracksXZ, energyCutMin /* less that 1eV is not registered */);
	    __set_energy_cut_on_cube_edep_map(scanTracksYZ, energyCutMin /* less that 1eV is not registered */);
	    __set_energy_cut_on_cube_edep_map(scanTracksXY, energyCutMin /* less that 1eV is not registered */);

		// string svg_group_of_tracks_xz = __convertCubeEdepMapToSvgStrGroup(
		// 	"group-of-tracks-xz", scanTracksXZ, "xz");
		// string svg_group_of_tracks_yz = __convertCubeEdepMapToSvgStrGroup(
		// 	"group-of-tracks-yz", scanTracksYZ, "yz");
		// string svg_group_of_tracks_xy = __convertCubeEdepMapToSvgStrGroup(
		// 	"group-of-tracks-xy", scanTracksXY, "xy");

		ScanMap scanHitsXZ, scanHitsYZ, scanHitsXY;
		for (int ihit = 0; ihit < NHits; ++ihit)
		{ // start loop over hits
			HitInformation hit{nd280UpEventPtr, ihit};
			if (!hit.isIntoSuperFGD()) continue;
			__insert_point_coords_into_cubes_map(scanHitsXZ, hit, 0, 2);
			__insert_point_coords_into_cubes_map(scanHitsYZ, hit, 1, 2);
			__insert_point_coords_into_cubes_map(scanHitsXY, hit, 0, 1);
		} // stop loop over hits

		/// set energy cuts on hits
		__set_energy_cut_on_cube_edep_map(scanHitsXZ, energyCutMin /* less that 1eV is not registered */);
	    __set_energy_cut_on_cube_edep_map(scanHitsYZ, energyCutMin /* less that 1eV is not registered */);
	    __set_energy_cut_on_cube_edep_map(scanHitsXY, energyCutMin /* less that 1eV is not registered */);

	    string svg_group_of_simulated_detector_response_xz = __convertCubeEdepMapToSvgStrGroup(
	    	"sim-det-resp-activated-cubes-xz", scanHitsXZ, "xz");
		string svg_group_of_simulated_detector_response_yz = __convertCubeEdepMapToSvgStrGroup(
			"sim-det-resp-activated-cubes-yz", scanHitsYZ, "yz");
		string svg_group_of_simulated_detector_response_xy = __convertCubeEdepMapToSvgStrGroup(
			"sim-det-resp-activated-cubes-xy", scanHitsXY, "xy");

		/// get svg detector
		auto [xz_reflector_elements, yz_reflector_elements, xy_reflector_elements] = createDetectorAsXMLObject();

		/// main svg
		XMLObject *SVG = new XMLObject("svg", "", {
			{"version", "1.1"}, {"xmlns", "http://www.w3.org/2000/svg"},
			{"data-eventid", to_string(eventID)},
			{"data-trackscount", tracksCountStr},
			{"id", "main-svg-wrapper"}
		});

		XMLObject *defs = new XMLObject("defs", R"(<marker id='head' orient="auto"
		    markerWidth='3' markerHeight='4'
		    refX='0.1' refY='2'>
		    <!-- triangle pointing right (+x) -->
		    <path d='M0,0 V4 L2,2 Z' fill="#AAA"/>
		  </marker>
		  <marker id="arrowhead" markerWidth="10" markerHeight="7" 
		    refX="0" refY="3.5" orient="auto" fill="blue">
		      <polygon points="0 0, 10 3.5, 0 7" />
		  </marker>)", SVG);

		/// create axis arrows
		XMLObject *axisArrows = new XMLObject("g", "", {{"id", "axis-arrows"}}, SVG);

		[[maybe_unused]] XMLObject *xAxisArrow = new XMLObject("path", "", {
			{"id", "x-axis-line"},
			{"marker-end", "url(#head)"},
			{"stroke-width", "10"},
			{"fill", "none"},
			{"stroke", "#AAA"},
			{"d", "M100,1960 L1910,1960"}}, axisArrows);
		[[maybe_unused]] XMLObject *xAxisLabel = new XMLObject("text", "X", {
			{"class", "axis-label"},
			{"x", "1950"},
			{"y", "1980"},
			{"fill", "#AAA"}}, axisArrows);

		[[maybe_unused]] XMLObject *zAxisArrow = new XMLObject("path", "", {
			{"id", "z-axis-line"},
			{"marker-end", "url(#head)"},
			{"stroke-width", "10"},
			{"fill", "none"},
			{"stroke", "#AAA"},
			{"d", "M2035,1950 L2035,200"}}, axisArrows);
		[[maybe_unused]] XMLObject *zAxisLabel = new XMLObject("text", "Z", {
			{"class", "axis-label"},
			{"x", "2015"},
			{"y", "150"},
			{"fill", "#AAA"}}, axisArrows);

		[[maybe_unused]] XMLObject *y1AxisArrow = new XMLObject("path", "", {
			{"id", "y1-axis-line"},
			{"marker-end", "url(#head)"},
			{"stroke-width", "10"},
			{"fill", "none"},
			{"stroke", "#AAA"},
			{"d", "M80,1960 L80,2400"}}, axisArrows);
		[[maybe_unused]] XMLObject *y1AxisLabel = new XMLObject("text", "Y", {
			{"class", "axis-label"},
			{"x", "60"},
			{"y", "2500"},
			{"fill", "#AAA"}}, axisArrows);

		[[maybe_unused]] XMLObject *y2AxisArrow = new XMLObject("path", "", {
			{"id", "y2-axis-line"},
			{"marker-end", "url(#head)"},
			{"stroke-width", "10"},
			{"fill", "none"},
			{"stroke", "#AAA"},
			{"d", "M2035,1960 L2500,1960"}}, axisArrows);
		[[maybe_unused]] XMLObject *y2AxisLabel = new XMLObject("text", "Y", {
			{"class", "axis-label"},
			{"x", "2550"},
			{"y", "1980"},
			{"fill", "#AAA"}}, axisArrows);

		/// create magnetic field pointers
		XMLObject *magneticFieldPointers = new XMLObject("g", "", {{"id", "mf-pointers"}}, SVG);
		createMagneticFieldPointer("xz", 150, 1920, 260, 1920, "blue", magneticFieldPointers);
		createMagneticFieldPointer("xy", 150, 2500, 260, 2500, "blue", magneticFieldPointers);
		createMagneticFieldPointer("yz", 2100, 1850, 2100, 1850, "blue", magneticFieldPointers);


		// XMLObject *svgJS = new XMLObject("script", "", {{"href", "../svg.js"}}, SVG);

		/// xz plane
		XMLObject *plane_xz_svg_group = new XMLObject("g", "", {{"id", "plane-xz"}}, SVG);
		XMLObject *svg_xz = new XMLObject("svg", "", {
			{"version", "1.1"}, {"xmlns", "http://www.w3.org/2000/svg"}, {"id", "svg-plane-xz"},
			{"width", "1920"}, {"height", "1840"}, {"viewBox", "-960, -920, 1920, 1840"}
		}, plane_xz_svg_group);
		svg_xz->appendChild(xz_reflector_elements);
		svg_xz->appendChild(svg_group_of_tracks_xz_xml);
		svg_xz->setContent(svg_group_of_simulated_detector_response_xz);
		// svg_xz->addContent(svg_group_of_tracks_xz);

		/// yz plane
		XMLObject *plane_yz_svg_group = new XMLObject("g", "", {{"id", "plane-yz"}}, SVG);
		XMLObject *svg_yz = new XMLObject("svg", "", {
			{"version", "1.1"}, {"xmlns", "http://www.w3.org/2000/svg"}, {"id", "svg-plane-yz"},
			{"width", "560"}, {"height", "1840"}, {"viewBox", "-280, -920, 560, 1840"}
		}, plane_yz_svg_group);
		svg_yz->appendChild(yz_reflector_elements);
		svg_yz->appendChild(svg_group_of_tracks_yz_xml);
		svg_yz->setContent(svg_group_of_simulated_detector_response_yz);
		// svg_yz->addContent(svg_group_of_tracks_yz);

		/// xy plane
		XMLObject *plane_xy_svg_group = new XMLObject("g", "", {{"id", "plane-xy"}}, SVG);
		XMLObject *svg_xy = new XMLObject("svg", "", {
			{"version", "1.1"}, {"xmlns", "http://www.w3.org/2000/svg"}, {"id", "svg-plane-xy"},
			{"width", "1920"}, {"height", "560"}, {"viewBox", "-960, -280, 1920, 560"}
		}, plane_xy_svg_group);
		svg_xy->appendChild(xy_reflector_elements);
		svg_xy->appendChild(svg_group_of_tracks_xy_xml);
		svg_xy->setContent(svg_group_of_simulated_detector_response_xy);
		// svg_xy->addContent(svg_group_of_tracks_xy);

		/// get svg content
		string svg_content = SVG->to_str();

		/// clear pointer
		SVG->clear();

		svg_content = "<?xml-stylesheet type=\"text/css\" href=\"../svg.css\" ?>\n" + svg_content;

		/// write to file
		// system("mkdir output");
		// std::time_t result = std::time(nullptr);
		// string curr_time = std::ctime(&result);
		// std::replace(curr_time.begin(), curr_time.end(), '\n', ' ');
		// std::replace(curr_time.begin(), curr_time.end(), ':', '.');
		// std::replace(curr_time.begin(), curr_time.end(), ' ', '-');
		// string dirname = "output/" + curr_time;
		// system(("mkdir " + dirname).c_str());

		// string filename = dirname + "/" + tag + "Event-" + to_string(EventID) + ".svg";
		string filename = dirname + tag + "Event-" + to_string(EventID) + ".svg";
		ofstream file;
		file.open (filename);
		file << svg_content;
		file.close();

		return filename;
	}

	void __insert_point_coords_into_cubes_map(ScanMap& cube_edep_map, PointInformation& point, int coord1, int coord2)
	{
		int cube_coord1 = std::floor(point.PtPrePos[coord1] / 10.0);
		int cube_coord2 = std::floor(point.PtPrePos[coord2] / 10.0);
		auto it_xz = cube_edep_map.find({cube_coord1, cube_coord2});
		if (it_xz != cube_edep_map.end()) {
			it_xz->second += point.PtEdep;
		} else {
			cube_edep_map.insert({ {cube_coord1, cube_coord2}, point.PtEdep });
		}
	}


	void __insert_point_coords_into_cubes_map(ScanMap& cube_edep_map, HitInformation& hit, int coord1, int coord2)
	{
		double x_coord, y_coord;
		if      (coord1 == 0) x_coord = hit.StartX;
		else if (coord1 == 1) x_coord = hit.StartY;
		else if (coord1 == 2) x_coord = hit.StartZ;
		if      (coord2 == 0) y_coord = hit.StartX;
		else if (coord2 == 1) y_coord = hit.StartY;
		else if (coord2 == 2) y_coord = hit.StartZ;

		int cube_coord1 = std::floor(x_coord / 10.0);
		int cube_coord2 = std::floor(y_coord / 10.0);

		auto it_xz = cube_edep_map.find({cube_coord1, cube_coord2});
		if (it_xz != cube_edep_map.end()) {
			it_xz->second += hit.Edep;
		} else {
			cube_edep_map.insert({ {cube_coord1, cube_coord2}, hit.Edep });
		}
	}

	void __set_energy_cut_on_cube_edep_map(ScanMap& cube_edep_map, double min_energy)
	{
		for (auto itr = cube_edep_map.cbegin() ; itr != cube_edep_map.cend() ; )
			itr = (itr->second < min_energy) ? cube_edep_map.erase(itr) : std::next(itr);
	}


	string __convertCubeEdepMapToSvgStrGroup(string groupName, ScanMap& CubeEdepMap, string plane)
	{
		XMLObject * activated_cubes_svg_group = new XMLObject("g", "", {{"id", groupName}});
		double max_val = get_max_of_map(CubeEdepMap);
		for(auto it = CubeEdepMap.begin(); it != CubeEdepMap.end(); ++it)
		{
			int cubeXcoord = (it->first).first;
			int cubeYcoord = (it->first).second;
			double cubeEnergy = it->second;
			string title = strInitKinEnergyWithBestValues(cubeEnergy) + " at cube (" + to_string(cubeXcoord) + " " + to_string(cubeYcoord) + ")";
			title = "<title>" + title + "</title>";
			[[maybe_unused]] XMLObject * _ = new XMLObject("rect", title, {
				{"class", "activated-cube " + plane},
				{"x", to_string(cubeXcoord * 10)},
				{"y", to_string(cubeYcoord * 10)},
				{"width", "10"}, {"height", "10"},
				{"fill", double2color_v2((cubeEnergy) / max_val)},
				{"data-energy", to_string(cubeEnergy)}
			}, activated_cubes_svg_group);
		}

		string activated_cubes_svg_group_str = activated_cubes_svg_group->to_str();
		activated_cubes_svg_group->clear();
		return activated_cubes_svg_group_str;
	}

	void computeEnergeticProperties()
	{

	}

	void fillHistograms(TH1 * range_histogram, TH1 * energy_deposit_histogram, TH2 * dE_dx_histogram)
	{
		// average_edep += primary_track.SumEdepAllPts / primary_track.LengthPtStep * 10;
		
		double total_edep = 0.0, primary_trk_len = 0.0;

		for (int itrack = 0; itrack < NTracks; ++itrack)
		{
			// TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, false};
			// if (track.ParentID == 0)
			// {
			// 	TrackInformation primary_track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true};
			// 	double primary_trk_len = primary_track.LengthPtStep / 10;
			// 	double primary_trk_edep = primary_track.SumEdepAllPts;

			// 	range_histogram->Fill(primary_trk_len);
			// 	energy_deposit_histogram->Fill(primary_trk_edep);
			// 	dE_dx_histogram->Fill(primary_trk_len, primary_trk_edep / primary_trk_len);
			// }
			// else continue;

			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, false};
			// if (track.PDG != 22)
			if (1)
			{
				TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true};
				total_edep += track.SumEdepAllPts;
			}
			if (track.ParentID == 0)
			{
				TrackInformation primary_track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true};
				primary_trk_len = primary_track.LengthPtStep / 10;
				double primary_trk_edep = primary_track.SumEdepAllPts;

				range_histogram->Fill(primary_trk_len);
				dE_dx_histogram->Fill(primary_trk_len, primary_trk_edep / primary_trk_len);
			}
		}

		energy_deposit_histogram->Fill(total_edep);
		// dE_dx_histogram->Fill(primary_trk_len, total_edep / primary_trk_len);

		// int a = 0;
		// a = ++ ++ ++ ++ a;
	}

	std::tuple<double, double, double, double> getEnergeticProperties(int PDG = 11)
	{
		double trk_len = 0.0, trk_edep = 0.0, trk_dE2dx = 0.0, initialEkin = 0.0;
		for (int itrack = 0; itrack < NTracks; ++itrack)
		{
			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, false};
			if (track.ParentID == 0 /*&& track.PDG == PDG */)
			{
				TrackInformation primary_track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true};
				trk_len = primary_track.LengthPtStep / 10;
				trk_edep = primary_track.SumEdepAllPts;
				initialEkin = primary_track.InitKinEnergy;

				// trk_dE2dx = trk_edep / trk_len;
				// return {trk_len, trk_edep, trk_dE2dx, initialEkin};
			}
			else continue;
		}

		trk_dE2dx = trk_edep / trk_len;
		return {trk_len, trk_edep, trk_dE2dx, initialEkin};
	}

	std::vector<std::tuple<int, double>> getEnergiesOfPrimaryParticles()
	{
		std::vector<std::tuple<int, double>> energies_of_primary_particles;
		for (int itrack = 0; itrack < NTracks; ++itrack)
		{
			TrackInformation track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, false};
			if (track.ParentID == 0 /*&& track.PDG == PDG */)
			{
				TrackInformation primary_track{nd280UpEventPtr, itrack, tracksParentsTree, tracksChildsTree, true};
				double initialEkin = primary_track.InitKinEnergy;
				energies_of_primary_particles.push_back({track.PDG, initialEkin});

				// trk_dE2dx = trk_edep / trk_len;
				// return {trk_len, trk_edep, trk_dE2dx, initialEkin};
			}
			else continue;
		}

		return energies_of_primary_particles;
	}
};


//---------------------------------------------------------------------







void printEventsInfo(TTree* tree, TND280UpEvent* nd280UpEvent,
					 int start_event, int stop_event,
					 int print_tracks = 0, int print_each_track_points = 0,
					 std::list<int> const & skip_particles_pdg = std::list<int>(),
					 int print_primary_track = 0, int print_primary_track_points = 0,
					 int print_hits = 0)
{
	// int nthreads = 4;
	// ROOT::EnableImplicitMT(nthreads);

	std::string pdg_codes = "";
	int number_of_needed_cases = 0;

	system("mkdir output");
	std::time_t result = std::time(nullptr);
	string curr_time = std::ctime(&result);
	std::replace(curr_time.begin(), curr_time.end(), '\n', ' ');
	std::replace(curr_time.begin(), curr_time.end(), ':', '.');
	std::replace(curr_time.begin(), curr_time.end(), ' ', '-');
	string dirname = "output/" + curr_time;
	system(("mkdir " + dirname).c_str());

	#pragma omp parallel for
	for (int ievent = start_event; ievent < stop_event; ++ievent)
	// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
	{
		tree->GetEntry(ievent); // read event from tree
		// tree->GetEntry(start_event + ievent); // read event from tree in parallel
		
		cout << "\n-----------------------------------------------\n";
		EventAnalizer __event{tree, nd280UpEvent, ievent};
		__event.print();
		__event.printVertices();
		// __event.printTracks();
		__event.printPrimaryTracks();
		cout << "\t****************************************\n";
		__event.printTrackWithParents(10);
		cout << "\t****************************************\n";
		if (ievent == 0) __event.createSVGofTracksTree(dirname, "test-svg-", ievent);
		cout << "\n-----------------------------------------------\n";

		// get event info
		int EventID = nd280UpEvent->GetEventID();
		int NTracks = nd280UpEvent->GetNTracks();
		int NVertices = nd280UpEvent->GetNVertices();
		int NHits = nd280UpEvent->GetNHits();
		std::cout << cprint("EventID: ", RED) << setw(4) << EventID << " " 
				  << cprint("NTracks: ", BLUE) << setw(4) << NTracks << " " 
				  << cprint("NVertices: ", BLUE) << setw(4) << NVertices << " " 
				  << cprint("NHits: ", BLUE) << setw(4) << NHits << " " 
				  << std::endl;

		std::cout << print_tracks << std::endl;
		for (auto pdg : skip_particles_pdg)
        	std::cout << pdg << std::endl;

        [[maybe_unused]] bool skip_event = false;
        if (1)
        {/// start printing vertices
        	std::cout << cprint("\tVertices stored in event:", RED) << std::endl;
        	for (int ivertex = 0; ivertex < NVertices; ++ivertex)
        	{ // start loop over vertices
        		VertexInformation vertex{nd280UpEvent, ivertex};
        		if (vertex.isIntoSuperFGD()) vertex.print();
        		else skip_event = true;
        	} // stop loop over vertices
        }/// stop printing vertices
        // if (skip_event) continue;

		if (print_primary_track)
		{/// start printing tracks
			// std::cout << cprint("Tracks stored in event:", RED) << std::endl;
			std::cout << cprint("\tPrimary track:", RED) << std::endl;

			TrackInformation primary_track{nd280UpEvent, 0}; // get track
			primary_track.print(3);

			if (print_primary_track_points)
			{ // start loop over primary track points

			} // stop loop over primary track points
		}/// stop printing tracks

		if (print_tracks)
		{
			/// determine process type
			std::vector<int> primary_pdg_codes;
			std::map<int, int> primary_pdg_codes_map;

			std::cout << cprint("\tTracks stored in event:", RED) << std::endl;
			int start_track_index = print_primary_track ? 1 : 0;
			for (int itrack = start_track_index; itrack < NTracks; ++itrack)
			{ // start loop over tracks
				// std::cout << "-";
				TrackInformation track{nd280UpEvent, itrack, false}; // get track
				if (itrack > 1000) break;
				if (track.ParentID != 0) continue;
				// track.computeAdditionalInfo();

				primary_pdg_codes.push_back(track.PDG);
				if (primary_pdg_codes_map.find(track.PDG) != primary_pdg_codes_map.end())
					primary_pdg_codes_map[track.PDG] += 1;
				else primary_pdg_codes_map.insert({track.PDG, 1});

				if (std::find(skip_particles_pdg.begin(), skip_particles_pdg.end(), track.PDG) != skip_particles_pdg.end()) continue;
				track.print(3);

				if (print_each_track_points)
				{ // start loop over track points

				} // stop loop over track points
			} // start loop over tracks

			/// determine process type
			if (   primary_pdg_codes.size() == 2
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 11  ) != primary_pdg_codes.end()
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 2212) != primary_pdg_codes.end())
			{
				std::cout << CYAN << " Process type: " << RED << " Neutral Current of nu_e interaction with proton " << endl;
			} else if  ( 
				   primary_pdg_codes.size() == 2
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 12  ) != primary_pdg_codes.end()
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 2112) != primary_pdg_codes.end())
			{
				std::cout << CYAN << " Process type: " << RED << " Charged Current of nu_e interaction with neutron " << endl;
			} else if (
				std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 111) != primary_pdg_codes.end())
			{
				std::cout << CYAN << " Process type: " << RED << " Neutral Current of nu_e interaction with neutral pion production " << endl;
			} else if  ( 
				   primary_pdg_codes.size() == 2
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 11) != primary_pdg_codes.end()
				&& std::find(primary_pdg_codes.begin(), primary_pdg_codes.end(), 12) != primary_pdg_codes.end())
			{
				std::cout << CYAN << " Process type: " << RED << " !!! Charged Current of nu_e interaction with electron !!! " << endl;
				++number_of_needed_cases;
			}
			// else std::cout << CYAN << " Process type: " << RED << " Undefined " << endl;
			else std::cout << CYAN << " Process type: " << RED << " Neutrino-nucleus interactions " << endl;

			for (auto& pair: primary_pdg_codes_map)
			{
				pdg_codes += std::to_string(pair.first) + ": "  + std::to_string(pair.second) + "\t";
			}
			pdg_codes += "\n";
		}


		if (print_hits)
		{/// start printing tracks
			std::cout << cprint("\tHits stored in event:", RED) << std::endl;

			for (int ihit = 0; ihit < NHits; ++ihit)
			{ // start loop over hits
				HitInformation hit{nd280UpEvent, ihit};

				if (std::find(skip_particles_pdg.begin(), skip_particles_pdg.end(), hit.PDG) != skip_particles_pdg.end()) continue;
				hit.print();
			} // stop loop over hits
		}/// stop printing tracks
	}

	std::cout << "Number of needed cases: " << number_of_needed_cases << std::endl;
	ofstream outfile;
	outfile.open("tracks-pdg.txt");
	outfile << pdg_codes << number_of_needed_cases;
	outfile.close();
}


#endif