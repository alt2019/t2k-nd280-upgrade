/**
 * @file ExtractTrackPointInfo.hh
 *
 * @brief Extrcting point of track information
 *
 * @creation_date     { 21.03.2021 }
 * @modification_date ( 21.03.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef EXTRACT_VERTEXS_INFORMATION_H
#define EXTRACT_VERTEXS_INFORMATION_H

#include <iomanip>
#include <string_view>
#include <typeinfo>  // 'typeid'

#include <TList.h>

#include "color_definitions.hh"
#include "particlesDefinition.hh"
#include "ExtractTrackInfo.hh"


using namespace std;


struct VertexInformation
{
	TND280UpVertex *nd280UpVertex;
	// TList *ListOfInTracks;
	int NInTracks;  
	// TList *ListOfOutTracks;
	int NOutTracks;
	int VertexID;
	int ReacMode;
	string ReacModeString;
	TVector3 Position;
	string VertexName;
	double Time;

	VertexInformation(TND280UpEvent *event, int ivertex)
	{
		nd280UpVertex = dynamic_cast<TND280UpVertex*>(event->GetVertex(ivertex));
		NInTracks      = nd280UpVertex->GetNInTracks();
		NOutTracks     = nd280UpVertex->GetNOutTracks();
		VertexID       = nd280UpVertex->GetVertexID();
		ReacMode       = nd280UpVertex->GetReacMode();
		ReacModeString = nd280UpVertex->GetReacModeString();
		Position       = nd280UpVertex->GetPosition();
		VertexName     = nd280UpVertex->GetVertexName();
		Time           = nd280UpVertex->GetTime();
	};

	void print()
	{
		cout << "    " 
			 << RED  << " VtxID: " << RESET << setw(4) << VertexID
			 << BLUE << " Time: " << RESET << setw(8) << Time
			 << BLUE << " NInTrks: " << RESET << setw(4) << NInTracks
			 << BLUE << " NOutTrks: " << RESET << setw(4) << NOutTracks
			 << BLUE << " ReacMode: " << RESET << ReacModeString
			 << BLUE << " VtxName: " << RESET << VertexName
			 << BLUE << " Position: " << RESET 
			 		 << setw(12) << Position[0] << print_green(" mm")
			 		 << setw(12) << Position[1] << print_green(" mm")
			 		 << setw(12) << Position[2] << print_green(" mm")
			 // << BLUE << " IsIntoSFGD?: " << RESET << isIntoSuperFGD()
			 << endl;
		cout << "inTracks: \n";
		for (int itrk = 0; itrk < NInTracks; ++itrk)
		{
			cout << RED  << " TrkPDG: " << RESET << setw(4) << nd280UpVertex->GetInTrack(itrk)->GetPDG() << endl;
		}
		cout << "outTracks: \n";
		for (int itrk = 0; itrk < NOutTracks; ++itrk)
		{
			cout << RED  << " TrkPDG: " << RESET << setw(4) << nd280UpVertex->GetOutTrack(itrk)->GetPDG() << endl;
		}
		cout << endl;
	}

	bool isIntoSuperFGD()
	{
		/// depending on point position ///
		if (Position[0] > 960.0 || Position[0] < -960.0) return false;
		if (Position[1] > 280.0 || Position[1] < -280.0) return false;
		if (Position[2] > 920.0 || Position[2] < -920.0) return false;

		/// depending on point volume /// NOT YET IMPLEMENTED ///
		return true;
	}
};


#endif