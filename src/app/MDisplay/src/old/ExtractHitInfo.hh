/**
 * @file ExtractHitInfo.hh
 *
 * @brief Extrcting detector hits information
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date ( 20.03.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef EXTRACT_HIT_INFORMATION_H
#define EXTRACT_HIT_INFORMATION_H

#include "color_definitions.hh" // BLUE, RESET
#include "particlesDefinition.hh" // energy_with_best_units

using namespace std;

struct HitInformation{
	int PDG;
	double StartX;
	double StartY;
	double StartZ;
	double StartT;
	double StopX;
	double StopY;
	double StopZ;
	double StopT;
	double Edep;
	string DetName;
	vector<int> fContributors;

	HitInformation(TND280UpEvent *event, int ihit) {
		TND280UpHit *nd280UpHit = dynamic_cast<TND280UpHit*>(event->GetHit(ihit));

		PDG = nd280UpHit->GetPDG();
		StartX = nd280UpHit->GetStartX();
		StartY = nd280UpHit->GetStartY();
		StartZ = nd280UpHit->GetStartZ();
		StartT = nd280UpHit->GetStartT();
		StopX = nd280UpHit->GetStopX();
		StopY = nd280UpHit->GetStopY();
		StopZ = nd280UpHit->GetStopZ();
		StopT = nd280UpHit->GetStopT();
		Edep = nd280UpHit->GetEnergyDeposit();
		DetName = nd280UpHit->GetDetName(); 
		fContributors = nd280UpHit->fContributors;
	}

	void print()
	{
		cout << "hit position (mm): " 
			 << " X: " << setw(12) << StartX << "," 
			 << " Y: " << setw(12) << StartY << ","
			 << " Z: " << setw(12) << StartZ << ","
			 << " T: " << setw(12) << StartT
			 << " PDG: " << setw(10) << PDG 
			 << BLUE << " Edep: " << RESET 
			 	<< setw(12) << energy_with_best_units(Edep).value << energy_with_best_units(Edep).unit
			 << "  " << DetName;
		cout << "	";
		for (auto& item: fContributors)
		{
			std::cout << item << " ";
		}
		cout << endl;
	}

	bool isIntoSuperFGD()
	{
		/// depending on point position ///
		if (StartX > 960.0 || StartX < -960.0) return false;
		if (StartY > 280.0 || StartY < -280.0) return false;
		if (StartZ > 920.0 || StartZ < -920.0) return false;
		return true;
	}
};

#endif