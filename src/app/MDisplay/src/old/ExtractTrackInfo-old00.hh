/**
 * @file ExtractTrackInfo.hh
 *
 * @brief Extrcting tracks information
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date ( 21.03.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef EXTRACT_TRACK_INFORMATION_H
#define EXTRACT_TRACK_INFORMATION_H

#include "color_definitions.hh"
#include "particlesDefinition.hh"

#include "ExtractTrackPointInfo.hh"


struct SimplifiedTrackInfo
{
	int TrackID;
	int ParentID;
	int PDG;
	double LengthPtStep;
	double SumEdepAllPts;
	double InitKinEnergy;
	TVector3 Momentum;
	std::string ProcessName;
	std::vector<SimplifiedPointInfo> points;
	std::vector<SimplifiedPointInfo*> points_;
};


struct TrackInformation{
	int TrackID; 
	int ParentID; 
	int NPoints; 
	int PDG; 
	string ProcessName;
	double Edep;
	double Range;
	double Length;
	double LengthZ;
	double LengthPtStep;
	double InitKinEnergy; 
	TVector3 Momentum; 
	double AbsMomentum; 
	double SumEdepAllPts;
	double Edep2Range;
	double Edep2LengthPtStep;
	double dE2dx;

	double AirLengthPtStep;
	double ScintillatorLengthPtStep;
	double ReflectorLengthPtStep;
	double FiberLengthPtStep;

	double AirEdep2LengthPtStep;
	double ScintillatorEdep2LengthPtStep;
	double ReflectorEdep2LengthPtStep;
	double FiberEdep2LengthPtStep;

	TND280UpTrack *nd280UpTrack;

	std::vector<SimplifiedPointInfo> points;

	std::vector<int> parentIDsPath;
	std::vector<int> childIDsPath;


	TrackInformation(TND280UpEvent *event, int itrack, bool compute_additional_params = false)
	{
		// TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(event->GetTrack(itrack));
		nd280UpTrack = dynamic_cast<TND280UpTrack*>(event->GetTrack(itrack));

		TrackID = nd280UpTrack->GetTrackID();
		ParentID = nd280UpTrack->GetParentID();
		NPoints = nd280UpTrack->GetNPoints();
		PDG = nd280UpTrack->GetPDG(); 
		ProcessName = nd280UpTrack->GetProcessName();
		Edep = nd280UpTrack->GetSDTotalEnergyDeposit();
		Range = nd280UpTrack->GetRange();
		InitKinEnergy = nd280UpTrack->GetInitKinEnergy();
		Momentum = nd280UpTrack->GetInitMom();
		AbsMomentum = Momentum.Mag();

		if (compute_additional_params)
		{
			if (PDG == 12 || PDG == -12 || PDG == 14 || PDG == -14 || PDG == 16 || PDG == -16) compute_additional_params = false;
			if (Edep < 1.0e-6) compute_additional_params = false;

			if (compute_additional_params) computeAdditionalInfo();
		}
		
		// if (compute_additional_params){
		// float total_distance_length_ptstep_air = 0.0;
		// float total_distance_length_ptstep_fiber = 0.0;
		// float total_distance_length_ptstep_scint = 0.0;
		// float total_distance_length_ptstep_refl = 0.0;

		// float cube_sum = 0.0;

		// float air_sum = 0.0;
		// float fiber_core_sum = 0.0;
		// float fgd_scint_sum = 0.0;
		// float reflector_sum = 0.0;

		// // std::cout << std::endl;
		// for (int ipt = 0; ipt < NPoints; ipt++)
		// {
		// 	PointInformation point(nd280UpTrack, ipt, PDG);

		// 	// if (NPoints > 10 && ipt % 100 == 0) std::cout << ".";
		// 	// else std::cout << ".";
		// 	// if (ipt % 100 == 0) std::cout << ".";

		// 	if (!point.isIntoSuperFGD()) continue;

		// 	if (point.isIntoHole())             
		// 	{
		// 		air_sum += point.PtEdep;
		// 		total_distance_length_ptstep_air += point.PtStepLength;
		// 	}
		// 	else if (point.isIntoFiber())            
		// 	{
		// 		fiber_core_sum += point.PtEdep;
		// 		total_distance_length_ptstep_fiber += point.PtStepLength;
		// 	}
		// 	else if (point.isIntoCoreScintillator()) 
		// 	{
		// 		fgd_scint_sum += point.PtEdep;
		// 		total_distance_length_ptstep_scint += point.PtStepLength;
		// 	}
		// 	else if (point.isIntoReflector())        
		// 	{
		// 		reflector_sum += point.PtEdep;
		// 		total_distance_length_ptstep_refl += point.PtStepLength;
		// 	}
		// 	else if (point.isIntoCube())             
		// 	{
		// 		cube_sum += point.PtEdep;
		// 	}

		// 	points.push_back(point.getSimplifiedInfo());
		// }
		// // std::cout << std::endl;
		// // std::cout << "\t";

		// SumEdepAllPts = air_sum + fiber_core_sum + fgd_scint_sum + reflector_sum + cube_sum;

		// // auto [total_distance_length, total_distance_length_z, total_distance_length_ptstep] = computeLengthInSuperFGD();
		// float total_distance_length, total_distance_length_z, total_distance_length_ptstep;
		// std::tie(total_distance_length, total_distance_length_z, total_distance_length_ptstep) = computeLengthInSuperFGD();
		
		// Length = total_distance_length; // total track length with fluctuations
		// LengthZ = total_distance_length_z;
		// LengthPtStep = total_distance_length_ptstep;

		// Edep2Range = SumEdepAllPts / Range; /* MeV/mm */
		// Edep2LengthPtStep = SumEdepAllPts / LengthPtStep; /* MeV/mm */

		// AirLengthPtStep = total_distance_length_ptstep_air;
		// ScintillatorLengthPtStep = total_distance_length_ptstep_scint;
		// ReflectorLengthPtStep = total_distance_length_ptstep_refl;
		// FiberLengthPtStep = total_distance_length_ptstep_fiber;

		// // less that 1.0 mm
		// AirEdep2LengthPtStep = (total_distance_length_ptstep_air < 1.0) ? 0.0 : air_sum / total_distance_length_ptstep_air;
		// ScintillatorEdep2LengthPtStep = (total_distance_length_ptstep_scint < 1.0) ? 0.0 : fgd_scint_sum / total_distance_length_ptstep_scint;
		// ReflectorEdep2LengthPtStep = (total_distance_length_ptstep_refl < 1.0) ? 0.0 : reflector_sum / total_distance_length_ptstep_refl;
		// FiberEdep2LengthPtStep = (total_distance_length_ptstep_fiber < 1.0) ? 0.0 : fiber_core_sum / total_distance_length_ptstep_fiber;

		// dE2dx = Edep / Range; /* MeV/mm */
		// }
	}

	TrackInformation(TND280UpEvent *event, int itrack,
					 std::map<int, std::vector<int>>& parentsPathMap,
					 std::map<int, std::vector<int>>& childsPathMap,
					 bool compute_additional_params = false)
	{
		nd280UpTrack = dynamic_cast<TND280UpTrack*>(event->GetTrack(itrack));

		TrackID = nd280UpTrack->GetTrackID();
		ParentID = nd280UpTrack->GetParentID();
		NPoints = nd280UpTrack->GetNPoints();
		PDG = nd280UpTrack->GetPDG(); 
		ProcessName = nd280UpTrack->GetProcessName();
		Edep = nd280UpTrack->GetSDTotalEnergyDeposit();
		Range = nd280UpTrack->GetRange();
		InitKinEnergy = nd280UpTrack->GetInitKinEnergy();
		Momentum = nd280UpTrack->GetInitMom();
		AbsMomentum = Momentum.Mag();

		parentIDsPath = parentsPathMap[TrackID];
		childIDsPath = childsPathMap[TrackID];

		// computeAdditionalInfo = false;
		// computeAdditionalInfo = true;
		if (compute_additional_params)
		{
			computeAdditionalInfo();
		}
	}

	void computeAdditionalInfo()
	{
		/// edep variables
		float total_distance_length_ptstep_air = 0.0;
		float total_distance_length_ptstep_fiber = 0.0;
		float total_distance_length_ptstep_scint = 0.0;
		float total_distance_length_ptstep_refl = 0.0;

		float cube_sum = 0.0;

		float air_sum = 0.0;
		float fiber_core_sum = 0.0;
		float fgd_scint_sum = 0.0;
		float reflector_sum = 0.0;

		/// distance variables
		float distance_betw_pts = 0.0;
		float total_distance_length = 0.0;
		float distance_betw_pts_z = 0.0;
		float total_distance_length_z = 0.0;
		float total_distance_length_ptstep = 0.0;
		float pt_pos_pre_x = 0.0;
		float pt_pos_pre_y = 0.0;
		float pt_pos_pre_z = 0.0;

		// std::cout << std::endl;
		for (int ipt = 0; ipt < NPoints; ipt++)
		{
			PointInformation point(nd280UpTrack, ipt, PDG);

			// if (NPoints > 10 && ipt % 100 == 0) std::cout << ".";
			// else std::cout << ".";
			// if (ipt % 100 == 0) std::cout << ".";

			if (!point.isIntoSuperFGD()) continue;

			//// compute edep ////
			PointEnvironment env = point.getEnvironment();
			switch (env)
			{
				case PT_INTO_HOLE:
					air_sum += point.PtEdep;
					total_distance_length_ptstep_air += point.PtStepLength;
					break;
				case PT_INTO_FIBER_X:
				case PT_INTO_FIBER_Y:
				case PT_INTO_FIBER_Z:
					fiber_core_sum += point.PtEdep;
					total_distance_length_ptstep_fiber += point.PtStepLength;
					break;
				case PT_INTO_CORE:
					fgd_scint_sum += point.PtEdep;
					total_distance_length_ptstep_scint += point.PtStepLength;
					break;
				case PT_INTO_REFLECTOR:
					reflector_sum += point.PtEdep;
					total_distance_length_ptstep_refl += point.PtStepLength;
					break;
				case PT_INTO_CUBE:
					cube_sum += point.PtEdep;
					break;
			}

			//// compute edep ////
			/*
			if (point.isIntoHole())             
			{
				air_sum += point.PtEdep;
				total_distance_length_ptstep_air += point.PtStepLength;
			}
			else if (point.isIntoFiber())            
			{
				fiber_core_sum += point.PtEdep;
				total_distance_length_ptstep_fiber += point.PtStepLength;
			}
			else if (point.isIntoCoreScintillator()) 
			{
				fgd_scint_sum += point.PtEdep;
				total_distance_length_ptstep_scint += point.PtStepLength;
			}
			else if (point.isIntoReflector())        
			{
				reflector_sum += point.PtEdep;
				total_distance_length_ptstep_refl += point.PtStepLength;
			}
			else if (point.isIntoCube())
			{
				cube_sum += point.PtEdep;
			}
			// */

			//// compute distance ////
			distance_betw_pts = (point.PtPrePos - TVector3(pt_pos_pre_x, pt_pos_pre_y, pt_pos_pre_z)).Mag();
			total_distance_length += distance_betw_pts;
			distance_betw_pts_z = abs(point.PtPrePos[2] - pt_pos_pre_z);
			total_distance_length_z += distance_betw_pts_z;
			pt_pos_pre_x = point.PtPrePos[0];
			pt_pos_pre_y = point.PtPrePos[1];
			pt_pos_pre_z = point.PtPrePos[2];
			total_distance_length_ptstep += point.PtStepLength; // true track length

			//// simplified points information ////
			// points.push_back(point.getSimplifiedInfo());
		}
		// std::cout << std::endl;
		// std::cout << "\t";

		SumEdepAllPts = air_sum + fiber_core_sum + fgd_scint_sum + reflector_sum + cube_sum;

		/// second loop over all pts, not good
		// auto [total_distance_length, total_distance_length_z, total_distance_length_ptstep] = computeLengthInSuperFGD();
		// float total_distance_length, total_distance_length_z, total_distance_length_ptstep;
		// std::tie(total_distance_length, total_distance_length_z, total_distance_length_ptstep) = computeLengthInSuperFGD();
		
		Length = total_distance_length; // total track length with fluctuations
		LengthZ = total_distance_length_z;
		LengthPtStep = total_distance_length_ptstep;

		Edep2Range = SumEdepAllPts / Range; /* MeV/mm */
		Edep2LengthPtStep = SumEdepAllPts / LengthPtStep; /* MeV/mm */

		AirLengthPtStep = total_distance_length_ptstep_air;
		ScintillatorLengthPtStep = total_distance_length_ptstep_scint;
		ReflectorLengthPtStep = total_distance_length_ptstep_refl;
		FiberLengthPtStep = total_distance_length_ptstep_fiber;

		// less that 1.0 mm
		AirEdep2LengthPtStep = (total_distance_length_ptstep_air < 1.0) ? 0.0 : air_sum / total_distance_length_ptstep_air;
		ScintillatorEdep2LengthPtStep = (total_distance_length_ptstep_scint < 1.0) ? 0.0 : fgd_scint_sum / total_distance_length_ptstep_scint;
		ReflectorEdep2LengthPtStep = (total_distance_length_ptstep_refl < 1.0) ? 0.0 : reflector_sum / total_distance_length_ptstep_refl;
		FiberEdep2LengthPtStep = (total_distance_length_ptstep_fiber < 1.0) ? 0.0 : fiber_core_sum / total_distance_length_ptstep_fiber;

		dE2dx = Edep / Range; /* MeV/mm */
	}

	std::string getParentsAsStr()
	{
		std::stringstream res;
		for (auto& item: parentIDsPath)
		{
			res << item << ",";
			// std::cout << item << ",";
		}
		// std::cout << res.str() << std::endl;
		return res.str();
	}

	std::string getChildsAsStr()
	{
		std::stringstream res;
		for (auto& item: childIDsPath)
		{
			res << item << ",";
		}
		return res.str();
	}

	int getParentsSize()
	{
		return parentIDsPath.size();
	}

	SimplifiedTrackInfo getSimplifiedTrackInfo()
	{
		SimplifiedTrackInfo tr;
		tr.TrackID = TrackID;
		tr.ParentID = ParentID;
		tr.PDG = PDG;
		tr.LengthPtStep = LengthPtStep;
		tr.SumEdepAllPts = SumEdepAllPts;
		tr.InitKinEnergy = InitKinEnergy;
		tr.Momentum = Momentum;
		tr.ProcessName = ProcessName;
		tr.points = points;
		return tr;
	}

	std::tuple<float, float, float> computeLengthInSuperFGD()
	{
		float distance_betw_pts = 0.0;
		float total_distance_length = 0.0;
		float distance_betw_pts_z = 0.0;
		float total_distance_length_z = 0.0;
		float total_distance_length_ptstep = 0.0;
		float pt_pos_pre_x = 0.0;
		float pt_pos_pre_y = 0.0;
		float pt_pos_pre_z = 0.0;

		for (int ipt = 0; ipt < NPoints; ipt++)
		{
			PointInformation point(nd280UpTrack, ipt, PDG);
			if (!point.isIntoSuperFGD()) continue;
			distance_betw_pts = (point.PtPrePos - TVector3(pt_pos_pre_x, pt_pos_pre_y, pt_pos_pre_z)).Mag();
			total_distance_length += distance_betw_pts;
			distance_betw_pts_z = abs(point.PtPrePos[2] - pt_pos_pre_z);
			total_distance_length_z += distance_betw_pts_z;
			pt_pos_pre_x = point.PtPrePos[0];
			pt_pos_pre_y = point.PtPrePos[1];
			pt_pos_pre_z = point.PtPrePos[2];
			total_distance_length_ptstep += point.PtStepLength; // true track length
		}
		// return {total_distance_length, total_distance_length_z, total_distance_length_ptstep};
		return std::make_tuple(total_distance_length, total_distance_length_z, total_distance_length_ptstep);
	}


	std::string color()
	{
		std::string color;
		switch (PDG) {
			case 11:  color = "red"; break; // e-: color red
			case -11: color = "blue"; break; // e+: color blue
			case 12:  color = "#DDDDDD"; break; // nu_e
			case -12: color = "#DDDDDD"; break; // anti_nu_e
			case 22:  color = "green"; break; // gamma: color green
			case 2112: color = "#00F009"; break; // neutron
			case 2212: color = "#FAF009"; break; // proton
			case 14:  color = "#FA0000"; break; // nu_mu
			case -14: color = "#0000FA"; break; // anti_nu_mu
			case 13:  color = "#F00009"; break; // mu-
			case -13: color = "#9000F0"; break; // mu+
			default: color = "white"; break; // any other particles: color white
		}
		return color;
	}


	std::string strInitKinEnergyWithBestValues()
	{
		std::stringstream res;
		if      (InitKinEnergy >= 1000.0) (res << std::setprecision(5) << InitKinEnergy / 1000.0 << "GeV");
		else if (InitKinEnergy < 1000.0 && InitKinEnergy >= 1.0) (res << std::setprecision(5) << InitKinEnergy << "MeV");
		else if (InitKinEnergy < 1.0 && InitKinEnergy >= 1.0e-3) (res << std::setprecision(5) << InitKinEnergy * 1.0e3 << "KeV");
		else if (InitKinEnergy < 1.0e-3 && InitKinEnergy >= 1.0e-6) (res << std::setprecision(5) << InitKinEnergy * 1.0e6 << "eV");
		else if (InitKinEnergy < 1.0e-6 && InitKinEnergy >= 1.0e-9) (res << std::setprecision(5) << InitKinEnergy * 1.0e9 << "meV");
		else (res << "0eV");
		return res.str();
	}


	std::string strTrackLengthWithBestValues()
	{
		std::stringstream res;
		if      (LengthPtStep >= 1000.0) (res << std::setprecision(4) << LengthPtStep / 1000.0 << "m");
		else if (LengthPtStep < 1000.0 && LengthPtStep >= 10.0) (res << std::setprecision(4) << LengthPtStep / 10.0 << "cm");
		else if (LengthPtStep < 10.0 && LengthPtStep >= 1.0) (res << std::setprecision(4) << LengthPtStep << "mm");
		else if (LengthPtStep < 1.0 && LengthPtStep >= 1.0e-3) (res << std::setprecision(4) << LengthPtStep * 1.0e3 << "mkm");
		else if (LengthPtStep < 1.0e-3 && LengthPtStep >= 1.0e-6) (res << std::setprecision(4) << LengthPtStep * 1.0e6 << "nm");
		else if (LengthPtStep < 1.0e-6 && LengthPtStep >= 1.0e-9) (res << std::setprecision(4) << LengthPtStep * 1.0e9 << "pm");
		else (res << "0mm");
		return res.str();
	}


	std::tuple<std::string, std::string, std::string, std::string> asSvgIntoSuperFGD()
	{
		std::string path_xz, path_yz, path_xy;
		std::string start_point;
		for (int ipt = 0; ipt < NPoints; ipt++)
		{
			PointInformation point(nd280UpTrack, ipt, PDG);

			// if (ipt == 0)
			// {
			// 	if (!point.isIntoSuperFGD()) continue;
			// 	path_xz = "M" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			// 	path_yz = "M" + std::to_string(int(point.PtPostPos[1])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			// 	path_xy = "M" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[1])) + " ";
			// 	start_point = point.asSvg(PDG, TrackID);
			// } else {
			// 	if (!point.isIntoSuperFGD()) continue;
			// 	path_xz += "L" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			// 	path_yz += "L" + std::to_string(int(point.PtPostPos[1])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			// 	path_xy += "L" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[1])) + " ";
			// }

			if (!point.isIntoSuperFGD()) continue;
			if (path_xz.empty()) {
				path_xz = "M" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			} else {
				path_xz += "L" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			}

			if (path_yz.empty()) {
				path_yz = "M" + std::to_string(int(point.PtPostPos[1])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			} else {
				path_yz += "L" + std::to_string(int(point.PtPostPos[1])) + "," + std::to_string(int(point.PtPostPos[2])) + " ";
			}

			if (path_xy.empty()) {
				path_xy = "M" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[1])) + " ";
			} else {
				path_xy += "L" + std::to_string(int(point.PtPostPos[0])) + "," + std::to_string(int(point.PtPostPos[1])) + " ";
			}
		}

		// if (path_xz.empty()) return {"", "", "", ""};
		// if (path_yz.empty()) return {"", "", "", ""};
		// if (path_xy.empty()) return {"", "", "", ""};

		if (path_xz.empty()) return std::make_tuple("", "", "", "");
		if (path_yz.empty()) return std::make_tuple("", "", "", "");
		if (path_xy.empty()) return std::make_tuple("", "", "", "");

		path_xz.pop_back();
		path_yz.pop_back();
		path_xy.pop_back();

		std::stringstream formattedTrkLen;
		formattedTrkLen << std::setprecision(6) << LengthPtStep;
		std::stringstream formattedTrkInitKinEnergy;
		formattedTrkInitKinEnergy << std::setprecision(7) << InitKinEnergy;
		std::string svg_track_start = "<path id=\"" + std::to_string(TrackID)
									  // + "\" data-trackid=\"" + std::to_string(TrackID)
									  + "\" data-parentid=\"" + std::to_string(ParentID)
									  // + "\" data-pdg=\"" + std::to_string(PDG)
									  // + "\" class=\"trackpath particle" + std::to_string(PDG)
									  + "\" data-trklen=\"" + formattedTrkLen.str()
									  + "\" data-trkE0=\"" + formattedTrkInitKinEnergy.str()
									  + "\" class=\"trackpath pdg" + std::to_string(PDG)
									  + " id" + std::to_string(TrackID)
									  + "\" fill=\"none"
									  + "\" d=";
		std::string svg_track_stop = "stroke=\"" + color() + "\" stroke-width=\"2\">"\
									 + ("<title>"
									 	"TrkId:" + std::to_string(TrackID) +
									 	", ParId:" + std::to_string(ParentID) +
									 	", PDG:" + std::to_string(PDG) +
									 	", CreatorProc:" + ProcessName +
									 	// ", Ekin:" + formattedTrkInitKinEnergy.str() + "MeV" +
									 	", Ekin:" + strInitKinEnergyWithBestValues() +
									 	// ", TrkLen:" + formattedTrkLen.str() + "mm" +
									 	", TrkLen:" + strTrackLengthWithBestValues() +
									 	"</title></path>\n");

		path_xz = svg_track_start + "\"" + path_xz + "\" " + svg_track_stop;
		path_yz = svg_track_start + "\"" + path_yz + "\" " + svg_track_stop;
		path_xy = svg_track_start + "\"" + path_xy + "\" " + svg_track_stop;

		// return {path_xz, path_yz, path_xy, start_point};
		return std::make_tuple(path_xz, path_yz, path_xy, start_point);
	}

	void svgOfPhotonInterestingPoints()
	{
		std::string svg_group_of_points = "<g class=\"id" + std::to_string(TrackID) + "track points\">";
		for (int ipt = 0; ipt < NPoints; ipt++)
		{
			PointInformation point(nd280UpTrack, ipt, PDG);
			 if (point.PtEdep > 0.0)
			 {
			 	svg_group_of_points += "<circle>";
			 }
		}
	}


	void print(int verbose = 1)
	{
		if (verbose == 3){
			cout << ""
			 	<< " ParentID: " << setw(3) << ParentID << "|" 
			 	<< " TrkID: " << setw(3) << TrackID << "|" 
			 	<< " #pts: " << setw(5) << NPoints << "|"
			 	<< " PDG: " << setw(10) << PDG << "|" 
			 	<< " Momentum: " 
			 		<< setw(12) << Momentum[0] << ", " 
			 		<< setw(12) << Momentum[1] << ", " 
			 		<< setw(12) << Momentum[2] << "|" 
			 	<< " |P|: " << setw(12) << mom_amp_with_best_units(AbsMomentum).value << mom_amp_with_best_units(AbsMomentum).unit << "|"
			 	<< " T0: " << setw(12) << energy_with_best_units(InitKinEnergy).value << energy_with_best_units(InitKinEnergy).unit << "|"
			 	<< BLUE << " R: " 
			 		<< BOLDMAGENTA << setw(12) << Range/10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " L: " 
			 		<< BOLDMAGENTA << setw(12) << Length/10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " Lz: " 
			 		<< BOLDMAGENTA << setw(12) << LengthZ/10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " Lpts: " 
			 		<< BOLDMAGENTA << setw(12) << LengthPtStep/10 << " cm" 
			 		<< RESET << "|"
			 	<< " dE_i: " << setw(12) << energy_with_best_units(Edep).value << energy_with_best_units(Edep).unit << "|"
			 	<< BLUE << " +dE_p: " 
			 		<< BOLDMAGENTA << setw(12) << energy_with_best_units(SumEdepAllPts).value << energy_with_best_units(SumEdepAllPts).unit << " MeV" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/Range: " 
			 		<< CYAN << setw(12) << Edep2Range * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/LengthPtStep: " 
			 		<< CYAN << setw(12) << Edep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_h/dx: " 
			 		<< CYAN << setw(12) << dE2dx * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< "\n\t"
			 	<< BLUE << " dE_p_air/LengthPtStep: " 
			 		<< CYAN << setw(12) << AirEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_scint/LengthPtStep: " 
			 		<< CYAN << setw(12) << ScintillatorEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_refl/LengthPtStep: " 
			 		<< CYAN << setw(12) << ReflectorEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_fiber/LengthPtStep: " 
			 		<< CYAN << setw(12) << FiberEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< "\n\t"
			 	<< BLUE << " AirLengthPtStep: " 
			 		<< CYAN << setw(12) << AirLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " ScintillatorLengthPtStep: " 
			 		<< CYAN << setw(12) << ScintillatorLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " ReflectorLengthPtStep: " 
			 		<< CYAN << setw(12) << ReflectorLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " FiberLengthPtStep: " 
			 		<< CYAN << setw(12) << FiberLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< " ProcName: " << ProcessName 
			 	<< " Parents: " << getParentsAsStr()
			 	<< " Childs: " << getChildsAsStr()
			 	<< endl;
		} else if (verbose == 2) {
			cout << ""
			 	<< " ParentID: " << setw(3) << ParentID << "|" 
			 	<< " TrkID: " << setw(3) << TrackID << "|" 
			 	<< " #pts: " << setw(5) << NPoints << "|"
			 	<< " PDG: " << setw(10) << PDG << "|" 
			 	<< " Momentum: " 
			 		<< setw(12) << Momentum[0] << ", " 
			 		<< setw(12) << Momentum[1] << ", " 
			 		<< setw(12) << Momentum[2] << "|" 
			 	<< " |P|: " << setw(12) << mom_amp_with_best_units(AbsMomentum).value << mom_amp_with_best_units(AbsMomentum).unit << "|"
			 	<< " T0: " << setw(12) << energy_with_best_units(InitKinEnergy).value << energy_with_best_units(InitKinEnergy).unit << "|"
			 	<< BLUE << " R: " 
			 		<< BOLDMAGENTA << setw(12) << Range/10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " Lpts: " 
			 		<< BOLDMAGENTA << setw(12) << LengthPtStep/10 << " cm" 
			 		<< RESET << "|"
			 	<< " dE_i: " << setw(12) << energy_with_best_units(Edep).value << energy_with_best_units(Edep).unit << "|"
			 	<< BLUE << " +dE_p: " 
			 		<< BOLDMAGENTA << setw(12) << energy_with_best_units(SumEdepAllPts).value << energy_with_best_units(SumEdepAllPts).unit << " MeV" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/Range: " 
			 		<< CYAN << setw(12) << Edep2Range * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/LengthPtStep: " 
			 		<< CYAN << setw(12) << Edep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< "\n\t"
			 	<< BLUE << " dE_p_air/LengthPtStep: " 
			 		<< CYAN << setw(12) << AirEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_scint/LengthPtStep: " 
			 		<< CYAN << setw(12) << ScintillatorEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_refl/LengthPtStep: " 
			 		<< CYAN << setw(12) << ReflectorEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " dE_p_fiber/LengthPtStep: " 
			 		<< CYAN << setw(12) << FiberEdep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< "\n\t"
			 	<< BLUE << " AirLengthPtStep: " 
			 		<< CYAN << setw(12) << AirLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " ScintillatorLengthPtStep: " 
			 		<< CYAN << setw(12) << ScintillatorLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " ReflectorLengthPtStep: " 
			 		<< CYAN << setw(12) << ReflectorLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " FiberLengthPtStep: " 
			 		<< CYAN << setw(12) << FiberLengthPtStep / 10 << " cm" 
			 		<< RESET << "|"
			 	<< " ProcName: " << ProcessName 
			 	<< " Parents: " << getParentsAsStr()
			 	<< " Childs: " << getChildsAsStr()
			 	<< endl;
		} else if (verbose == 1) {
			cout << ""
			 	<< " ParentID: " << setw(3) << ParentID << "|" 
			 	<< " TrkID: " << setw(3) << TrackID << "|" 
			 	<< " #pts: " << setw(5) << NPoints << "|"
			 	<< " PDG: " << setw(10) << PDG << "|" 
			 	<< " Momentum: " 
			 		<< setw(12) << Momentum[0] << ", " 
			 		<< setw(12) << Momentum[1] << ", " 
			 		<< setw(12) << Momentum[2] << "|" 
			 	<< " |P|: " << setw(12) << mom_amp_with_best_units(AbsMomentum).value << mom_amp_with_best_units(AbsMomentum).unit << "|"
			 	<< " T0: " << setw(12) << energy_with_best_units(InitKinEnergy).value << energy_with_best_units(InitKinEnergy).unit << "|"
			 	<< BLUE << " R: " 
			 		<< BOLDMAGENTA << setw(12) << Range/10 << " cm" 
			 		<< RESET << "|"
			 	<< BLUE << " Lpts: " 
			 		<< BOLDMAGENTA << setw(12) << LengthPtStep/10 << " cm" 
			 		<< RESET << "|"
			 	<< " dE_i: " << setw(12) << energy_with_best_units(Edep).value << energy_with_best_units(Edep).unit << "|"
			 	<< BLUE << " +dE_p: " 
			 		<< BOLDMAGENTA << setw(12) << energy_with_best_units(SumEdepAllPts).value << energy_with_best_units(SumEdepAllPts).unit << " MeV" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/Range: " 
			 		<< CYAN << setw(12) << Edep2Range * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< BLUE << " +dE_p/LengthPtStep: " 
			 		<< CYAN << setw(12) << Edep2LengthPtStep * 10 << " MeV/cm" 
			 		<< RESET << "|"
			 	<< "\n\t"
			 	<< " ProcName: " << ProcessName 
			 	<< " Parents: " << getParentsAsStr()
			 	<< " Childs: " << getChildsAsStr()
			 	<< endl;
		}
	}
};

#endif