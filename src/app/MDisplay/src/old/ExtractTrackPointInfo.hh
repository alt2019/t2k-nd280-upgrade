/**
 * @file ExtractTrackPointInfo.hh
 *
 * @brief Extrcting point of track information
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date ( 20.03.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef EXTRACT_TRACK_POINTS_INFORMATION_H
#define EXTRACT_TRACK_POINTS_INFORMATION_H

#include <string_view>

#include "color_definitions.hh"
#include "particlesDefinition.hh"


std::string sstep_status(int istep_status)
{
	std::string res; 
	switch (istep_status){
		case 0: res = "WorldBoundary"        ; break;
		case 1: res = "GeomBoundary"         ; break;
		case 2: res = "AtRestDoItProc"       ; break;
		case 3: res = "AlongStepDoItProc"    ; break;
		case 4: res = "PostStepDoItProc"     ; break;
		case 5: res = "UserDefinedLimit"     ; break;
		case 6: res = "ExclusivelyForcedProc"; break;
		case 7: res = "Undefined"            ; break;
	}

	return res;
}


struct SimplifiedPointInfo
{
	int PtID;
	double PtEdep;
	double PtEkin;
	double PtAbsMom;
	double PtTime;
	string PtPhysVolName;
	string ProcessName;
	string PtMaterialName;
	TVector3 PtPrePos;
	TVector3 PtMomDirection;
};



enum PointEnvironment
{
	// PT_INTO_SFGD,
	PT_INTO_CUBE,
	PT_INTO_FIBER_X,
	PT_INTO_FIBER_Y,
	PT_INTO_FIBER_Z,
	PT_INTO_HOLE,
	PT_INTO_CORE,
	PT_INTO_REFLECTOR
};


struct PointInformation
{
	TND280UpTrackPoint *nd280UpTrackPoint;
	int PtID;
	string PtPhysVolName;
	string ProcessName;
	string PreProcessName;
	string PostProcessName;
	int PtStepStatus;
	string PtMaterialName;
	double PtTime;
	double PtAbsMom;
	double PtEdep;
	double PtEkin;
	double PtStepLength;
	double PtDist;
	TVector3 PtMom;
	TVector3 PtPrePos;
	TVector3 PtPostPos;
	TVector3 PtMomDirection;
	bool IsOnBoundary;
	double _prevPtEkin;
	double _nextPtEkin;
	double __real_pt_edep = PtEkin - _nextPtEkin;

	PointInformation(){}

	PointInformation(TND280UpTrack *track, int ipt, int PDG){
		double mass = TABLE_PARTICLE_PDG_MASS[PDG];

		double edep;
		double next_edep;
		double _mom = 0.0;
		if (ipt != track->GetNPoints() - 1) {
			// TND280UpTrackPoint *nd280UpTrackPoint = dynamic_cast<TND280UpTrackPoint*>(track->GetPoint(ipt + 1));
			nd280UpTrackPoint = dynamic_cast<TND280UpTrackPoint*>(track->GetPoint(ipt + 1));
			edep = nd280UpTrackPoint->GetEdep();
			_mom = (nd280UpTrackPoint->GetMomentum()).Mag();
			next_edep = compute_energy_from_momentum(mass, _mom);
		} else { edep = 0.0; next_edep = 0.0;}
		TND280UpTrackPoint *nd280UpTrackPoint = dynamic_cast<TND280UpTrackPoint*>(track->GetPoint(ipt));

		// edep = (ipt == 0) ? edep = 0.0 : edep = nd280UpTrackPoint->GetEdep();
		edep = (ipt == 0) ? 0.0 : nd280UpTrackPoint->GetEdep();

		PtID = nd280UpTrackPoint->GetPointID();
		PtTime = nd280UpTrackPoint->GetTime();
		PtMom = nd280UpTrackPoint->GetMomentum();
		PtEdep = edep;
		PtPhysVolName = nd280UpTrackPoint->GetPhysVolName();
		PtPrePos = nd280UpTrackPoint->GetPrePosition();
		PtPostPos = nd280UpTrackPoint->GetPostPosition();
		IsOnBoundary = nd280UpTrackPoint->IsOnBoundary();
		PtAbsMom = PtMom.Mag();
		// ProcessName = nd280UpTrackPoint->GetProcessName();
		ProcessName = "old unused";
		PreProcessName = nd280UpTrackPoint->GetPreProcessName();
		PostProcessName = nd280UpTrackPoint->GetPostProcessName();
		PtStepStatus = nd280UpTrackPoint->GetStepStatus();
		PtStepLength = nd280UpTrackPoint->GetStepLength(); 
		PtDist = (PtPrePos - PtPostPos).Mag();
		PtEkin = compute_energy_from_momentum(mass, PtAbsMom);
		PtMaterialName = nd280UpTrackPoint->GetMaterialName();

		// momentum direction
		// PtMomDirection.SetXYZ(
		// 	PtMom[0] / PtAbsMom, 
		// 	PtMom[1] / PtAbsMom, 
		// 	PtMom[2] / PtAbsMom);
		PtMomDirection = PtMom.Unit();

		_nextPtEkin = next_edep;
	}

	SimplifiedPointInfo getSimplifiedInfo()
	{
		SimplifiedPointInfo pt;
		pt.PtID = PtID;
		pt.PtEdep = PtEdep;
		pt.PtEkin = PtEkin;
		pt.PtAbsMom = PtAbsMom;
		pt.PtTime = PtTime;
		pt.PtPhysVolName = PtPhysVolName;
		pt.ProcessName = ProcessName;
		pt.PtMaterialName = PtMaterialName;
		pt.PtPrePos = PtPrePos;
		pt.PtMomDirection = PtMomDirection;
		return pt;
	}

	std::string asSvg([[maybe_unused]] int pdg, int trackId)
	{
		std::string svg_point = R"(<circle cx="10" cy="10" r="2" fill="black" class="trackpoint" data-edep=")"\
								+ std::to_string(PtEdep) + "\" " \
								+ "data-time=\"" + std::to_string(PtTime) + "\" "\
								+ "id=\"" + std::to_string(PtID) + "\" "\
								+ "data-trackid=\"" + std::to_string(trackId) + "\" "\
								+ R"(/>)" "\n";
		return svg_point;
	}

	std::string shortInfo(bool theSameAsPreviousPt = false)
	{
		// std::string info = "T" + std::to_string(PtTime) + "|Ek" + std::to_string(PtEkin) + "|dE" + std::to_string(PtEdep);
		// return info;

		if (theSameAsPreviousPt)
		{
			std::stringstream info;
			info << std::setprecision(4) << "T" << PtTime << std::setprecision(3) << "|Ek+|dE+|dir+";
			return info.str();
		}

		std::stringstream info;
		info << std::setprecision(4) << "T" << PtTime << std::setprecision(3) << "|Ek" << PtEkin << "|dE" << PtEdep
			 << "|dir" << PtMomDirection[0] << "_" << PtMomDirection[1] << "_" << PtMomDirection[2];
		return info.str();
	}

	std::string shortInfo(PointInformation previousPoint)
	{

		std::stringstream info;
		info << std::setprecision(4) << "T" << PtTime;
		if (abs(PtEkin - previousPoint.PtEkin) < 1.0e-4) info << "|Ek+";
		else                                             info << std::setprecision(3) << "|Ek" << PtEkin;
		if (abs(PtEdep - previousPoint.PtEdep) < 1.0e-4) info << "|dE+";
		else                                             info << std::setprecision(3) << "|dE" << PtEdep;
		if (abs(PtMom.Mag() - previousPoint.PtMom.Mag()) < 1.0e-4) info << "|dir+";
		else info << "|dir" << PtMomDirection[0] << "_" << PtMomDirection[1] << "_" << PtMomDirection[2];
		
		return info.str();
	}

	std::string shortInfo(PointInformation previousPoint, int trackPDG)
	{

		std::stringstream info;
		info << std::setprecision(4) << "T" << PtTime;
		if (abs(PtEkin - previousPoint.PtEkin) < 1.0e-4 && trackPDG == 22) info << "";
		else if (abs(PtEkin - previousPoint.PtEkin) < 1.0e-4) info << "|Ek+";
		else                                             info << std::setprecision(3) << "|Ek" << PtEkin;
		
		if (abs(PtEdep - previousPoint.PtEdep) < 1.0e-4 && trackPDG == 22) info << "";
		else if (abs(PtEdep - previousPoint.PtEdep) < 1.0e-4) info << "|dE+";
		else                                             info << std::setprecision(3) << "|dE" << PtEdep;
		
		if (abs(PtMom.Mag() - previousPoint.PtMom.Mag()) < 1.0e-4 && trackPDG == 22) info << "";
		else if (abs(PtMom.Mag() - previousPoint.PtMom.Mag()) < 1.0e-4) info << "|dir+";
		else info << std::setprecision(3) << "|dir" << PtMomDirection[0] << "_" << PtMomDirection[1] << "_" << PtMomDirection[2];
		
		return info.str();
	}

	void print(int verbose = 3)
	{
		if (verbose == 3) {
			cout << "    " 
				 << RED  << " PtID: " << RESET << setw(4) << PtID
				 << BLUE << " |dE_pt|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEdep).value << energy_with_best_units(PtEdep).unit
				 << BLUE << " |P_pt|: " << RESET 
				 	<< setw(12) << mom_amp_with_best_units(PtAbsMom).value << mom_amp_with_best_units(PtAbsMom).unit
				 << BLUE << " |dE_real|: " << RESET 
				 	<< setw(12) << energy_with_best_units(__real_pt_edep).value << energy_with_best_units(__real_pt_edep).unit
				 << BLUE << " |T|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEkin).value << energy_with_best_units(PtEkin).unit
				 << "\n\t"
				 << BLUE << " PtPrePosX: " << RESET << setw(10) << PtPrePos[0] << print_green(" mm")
				 << BLUE << " Y: " << RESET << setw(10) << PtPrePos[1] << print_green(" mm")
				 << BLUE << " Z: " << RESET << setw(10) << PtPrePos[2] << print_green(" mm")
				 << "\n\t"
				 << BLUE << " PtPostPosX: " << RESET << setw(10) << PtPostPos[0] << print_green(" mm")
				 << BLUE << " Y: " << RESET << setw(10) << PtPostPos[1] << print_green(" mm")
				 << BLUE << " Z: " << RESET << setw(10) << PtPostPos[2] << print_green(" mm")
				 << "\n\t\t"
				 << BLUE << " StepL: " << RESET << setw(12) << PtStepLength << print_yellow(" mm")
				 // << BLUE << " Dist: " << RESET << setw(12) << PtDist << print_yellow(" mm") // ~equal to PtStepLength
				 // << BLUE << " PtProcName: " << RESET << setw(18) << ProcessName
				 << BLUE << " PreProcessName: " << RESET << setw(18) << PreProcessName
				 << BLUE << " PostProcessName: " << RESET << setw(18) << PostProcessName
				 << " " << setw(15) << print_substr_det_name(PtPhysVolName)
				 << " " << setw(22) << sstep_status(PtStepStatus) 
				 << " " << setw(16) << PtMaterialName
				 << "\n 		Md: " 
				 	<< " X: " << setw(8) << PtMomDirection[0]
				 	<< " Y: " << setw(8) << PtMomDirection[1]
				 	<< " Z: " << setw(8) << PtMomDirection[2]
				 	<< " theta: " << setw(8) << PtMom.Theta()
				 	<< " phi: " << setw(8) << PtMom.Phi()
				 << endl;
		} else if (verbose == 2) {
			cout << "    " 
			 	 << RED  << " PtID: " << RESET << setw(4) << PtID
				 << BLUE << " |dE_pt|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEdep).value << energy_with_best_units(PtEdep).unit
				 << BLUE << " |P_pt|: " << RESET 
				 	<< setw(12) << mom_amp_with_best_units(PtAbsMom).value << mom_amp_with_best_units(PtAbsMom).unit
				 << BLUE << " |dE_real|: " << RESET 
				 	<< setw(12) << energy_with_best_units(__real_pt_edep).value << energy_with_best_units(__real_pt_edep).unit
				 << BLUE << " |T|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEkin).value << energy_with_best_units(PtEkin).unit
				 << "\n\t"
				 << BLUE << " PtPrePosX: " << RESET << setw(10) << PtPrePos[0] << print_green(" mm")
				 << BLUE << " Y: " << RESET << setw(10) << PtPrePos[1] << print_green(" mm")
				 << BLUE << " Z: " << RESET << setw(10) << PtPrePos[2] << print_green(" mm")
				 << "\n\t"
				 << BLUE << " PtPostPosX: " << RESET << setw(10) << PtPostPos[0] << print_green(" mm")
				 << BLUE << " Y: " << RESET << setw(10) << PtPostPos[1] << print_green(" mm")
				 << BLUE << " Z: " << RESET << setw(10) << PtPostPos[2] << print_green(" mm")
				 << "\n\t\t"
				 << BLUE << " StepL: " << RESET << setw(12) << PtStepLength << print_yellow(" mm")
				 // << BLUE << " PtProcName: " << RESET << setw(18) << ProcessName
				 << BLUE << " PreProcessName: " << RESET << setw(18) << PreProcessName
				 << BLUE << " PostProcessName: " << RESET << setw(18) << PostProcessName
				 << " " << setw(15) << print_substr_det_name(PtPhysVolName)
				 << " " << setw(22) << sstep_status(PtStepStatus) 
				 << " " << setw(16) << PtMaterialName
				 << endl;
		} else if (verbose == 1) {
			cout << "    " 
			 	 << RED  << " PtID: " << RESET << setw(4) << PtID
				 << BLUE << " |dE_pt|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEdep).value << energy_with_best_units(PtEdep).unit
				 << BLUE << " |P_pt|: " << RESET 
				 	<< setw(12) << mom_amp_with_best_units(PtAbsMom).value << mom_amp_with_best_units(PtAbsMom).unit
				 << BLUE << " |dE_real|: " << RESET 
				 	<< setw(12) << energy_with_best_units(__real_pt_edep).value << energy_with_best_units(__real_pt_edep).unit
				 << BLUE << " |T|: " << RESET 
				 	<< setw(12) << energy_with_best_units(PtEkin).value << energy_with_best_units(PtEkin).unit
				 << "\n\t"
				 // << BLUE << " PtProcName: " << RESET << setw(18) << ProcessName
				 << BLUE << " PreProcessName: " << RESET << setw(18) << PreProcessName
				 << BLUE << " PostProcessName: " << RESET << setw(18) << PostProcessName
				 << " " << setw(15) << print_substr_det_name(PtPhysVolName)
				 << " " << setw(22) << sstep_status(PtStepStatus) 
				 << " " << setw(16) << PtMaterialName
				 << endl;
		}
		
	}

	PointEnvironment getEnvironment()
	{
		if      (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ"                         ) return PT_INTO_CUBE;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberX"   ) return PT_INTO_FIBER_X;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberY"   ) return PT_INTO_FIBER_Y;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberZ"   ) return PT_INTO_FIBER_Z;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole"          ) return PT_INTO_HOLE;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion/Core") return PT_INTO_CORE;
		else if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion"     ) return PT_INTO_REFLECTOR;
	}

	bool isIntoSuperFGD()
	{
		/// depending on point position ///
		if (PtPrePos[0] > 960.0 || PtPrePos[0] < -960.0) return false;
		if (PtPrePos[1] > 280.0 || PtPrePos[1] < -280.0) return false;
		if (PtPrePos[2] > 920.0 || PtPrePos[2] < -920.0) return false;

		/// depending on point volume /// NOT YET IMPLEMENTED ///
		return true;
	}

	bool isIntoCube()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ") return true;
		return false;
	}

	bool isIntoFiberX()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberX") return true;
		return false;
	}

	bool isIntoFiberY()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberY") return true;
		return false;
	}

	bool isIntoFiberZ()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole/FiberZ") return true;
		return false;
	}

	bool isIntoFiber()
	{
		if (isIntoFiberX()) return true;
		if (isIntoFiberY()) return true;
		if (isIntoFiberZ()) return true;
		return false;
	}

	bool isIntoHole()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Hole") return true;
		return false;
	}

	bool isIntoCoreScintillator()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion/Core") return true;
		return false;
	}

	bool isIntoReflector()
	{
		if (PtPhysVolName == "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion") return true;
		// if (std::string_view(PtPhysVolName).ends_with("Extrusion")) return true; // since c++20
		return false;
	}

	std::tuple<int, int, int> getCubePosition()
	{
		if (!isIntoSuperFGD()) return {};
	}

};


#endif