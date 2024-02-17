/**
 * @file SimulationAnalizer.hh
 *
 * @brief Analize simulation
 *
 * @creation_date     { 25.05.2021 }
 * @modification_date ( 25.05.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef SIMULATION_ANALIZER_H
#define SIMULATION_ANALIZER_H

#include <iostream>
#include <list>

#include <TString.h>
#include <TTree.h>
#include <TFile.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TEllipse.h>
#include <TBox.h>

// #include <TSeq.hxx>
// #include <TSeqCollection.h>
#include <ROOT/TSeq.hxx> // ROOT::TSeqUL
#include <TROOT.h> // ROOT::EnableImplicitMT

#include <omp.h> // omp exists, but currently not used

#include "TND280UpEvent.hh"
#include "ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
#include "AnalizeEvents.hh"


using namespace std;

// struct tm {
//    int tm_sec;   // seconds of minutes from 0 to 61
//    int tm_min;   // minutes of hour from 0 to 59
//    int tm_hour;  // hours of day from 0 to 24
//    int tm_mday;  // day of month from 1 to 31
//    int tm_mon;   // month of year from 0 to 11
//    int tm_year;  // year since 1900
//    int tm_wday;  // days since sunday
//    int tm_yday;  // days since January 1st
//    int tm_isdst; // hours of daylight savings time
// }


/*
string getCurrentTime()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	int YYYY = 1900 + ltm->tm_year;
	int MM = 1 + ltm->tm_mon;
	int DD = ltm->tm_mday;

	int H = ltm->tm_hour;
	int M = ltm->tm_min;
	int S = ltm->tm_sec;

	string curr_date = to_string(DD) + "." + to_string(MM) + "." + to_string(YYYY);
	string curr_time = to_string(H) + "." + to_string(M) + "." + to_string(S);
	curr_time = curr_date + "-" + curr_time;

	return curr_time;
}
// */


void draw_cube_with_injection_position_histohram(double insertion_pt_x /* mm */, double insertion_pt_y /* mm */)
{
	auto cube_hist = new TH2F("cube", "Injection position (red point)", 2, -0.5, 10.5, 2, -0.5, 10.5);
	cube_hist->GetXaxis()->SetTitle("x, mm");
	cube_hist->GetYaxis()->SetTitle("y, mm");
	cube_hist->SetStats(0);

	auto cube_with_reflector = new TBox(0.0, 0.0, 10.0, 10.0);
	cube_with_reflector->SetFillColor(kBlack);
	auto cube_scintilator = new TBox(0.1, 0.1, 9.9, 9.9);
	cube_scintilator->SetFillColor(42);
	// auto inner_hole_y = new TBox(6.25, 0.0, 7.75, 10.0);
	// auto inner_hole_y = new TWbox(6.25, 0.1, 7.75, 9.9, 40, 1, -1);
	auto inner_hole_y = new TBox(6.25, 0.1, 7.75, 9.9);
	// inner_hole_y->SetFillColor(40);
	// inner_hole_y->SetFillColor(41);
	inner_hole_y->SetFillColorAlpha(kWhite, 0.3);
	// auto inner_hole_x = new TWbox(0.1, 6.25, 9.9, 7.75, 40, 1, -1);
	auto inner_hole_x = new TBox(0.1, 6.25, 9.9, 7.75);
	// auto inner_hole_x = new TBox(0.0, 6.25, 10.0, 7.75);
	inner_hole_x->SetFillColorAlpha(kWhite, 0.3);
	
	auto inner_fiber_y = new TBox(6.5, 0.1, 7.5, 9.9);
	inner_fiber_y->SetFillColorAlpha(kGreen, 0.3);
	auto inner_fiber_x = new TBox(0.1, 6.5, 9.9, 7.5);
	inner_fiber_x->SetFillColorAlpha(kGreen, 0.3);

	TEllipse *hole = new TEllipse(3.0, 3.0, .75, .75);
	hole->SetFillColor(kWhite);
	TEllipse *fiberz = new TEllipse(3.0, 3.0, .5, .5);
	fiberz->SetFillColor(kGreen);
	TEllipse *point = new TEllipse(insertion_pt_x * 10, insertion_pt_y * 10, .15, .15);
	point->SetFillColor(kRed);
	point->SetLineColor(kRed);

	cube_hist->Draw();
	cube_with_reflector->Draw("same");
	cube_scintilator->Draw("same");
	hole->Draw("same");
	fiberz->Draw("same");
	inner_hole_y->Draw("same");
	inner_hole_x->Draw("same");
	inner_fiber_y->Draw("same");
	inner_fiber_x->Draw("same");
	point->Draw("same");
}



void draw_energy_histograms(TH1* range_hist, TH1* energy_deposit_hist, TH2* de2dx_hist,
							float start_position_x, float start_position_y,
							std::string particle_name, std::string momentum,
							std::string kinetic_energy, std::string magnetic_filed,
							std::string start_position, std::string tag,
							std::string output_directory)
{
	//!// canvas for energy with maltiple charts //!//
	// /*
	/// define canvas ///
	string characteristics_canvas_name = particle_name + "chacracteristics";
	TCanvas *ccharacteristics = new TCanvas("ccharacteristics", characteristics_canvas_name.c_str(), 200, 10, 6500, 3500); 
	
	/// fill canvas ///
	ccharacteristics->Divide(1, 2); // divide canvas into 2 horizontal parts
	ccharacteristics->cd(1); // move to the top side
	TPad *ctp1 = new TPad("ctp1", "ctp1", 0., 0., 1., 1.); // inner top pad for futher division
	ctp1->Draw();
	ctp1->Divide(2, 1); // divide inner top pad into 2 vertical parts
	TPad *ctp11 = (TPad*)ctp1->cd(1); // top left part
	ctp11->Draw();
	range_hist->Draw();

	TPad *ctp12 = (TPad*)ctp1->cd(2); // top right part
	ctp12->Draw();
	energy_deposit_hist->Draw();

	ccharacteristics->cd(2); // move to the bottom side
	TPad *ctp2 = new TPad("ctp2","ctp2",0.,0.,1.,1.); // inner bottom pad for futher division
	ctp2->Divide(2, 1); // divide inner bottom pad into 2 vertical parts
	ctp2->Draw();
	TPad *ctp21 = (TPad*)ctp2->cd(1); // bottom left parf
	ctp21->Draw();
	de2dx_hist->Draw("COLZ");

	//~~// do not draw the fourth part
	TPad *ctp22 = (TPad*)ctp2->cd(2); // bottom right parf
	ctp22->Draw();
	draw_cube_with_injection_position_histohram(start_position_x, start_position_y);

	/// save and clear canvas ///
	string characteristics_file_name = output_directory + "/" + tag + particle_name + kinetic_energy + "_" + momentum + "MeV2c_" \
		+ magnetic_filed + "stp" + start_position + "-characteristics"; // save name
	ccharacteristics->SaveAs((characteristics_file_name + ".pdf").c_str(), "pdf"); // save canvas
	ccharacteristics->SaveAs((characteristics_file_name + ".svg").c_str(), "svg"); // save canvas
	ccharacteristics->Clear(); // clear canvas
	delete ccharacteristics; // deallocate memory of canvas
	// */
}


TH1* Hist1D(string hist_name,
			string hist_title, 
			int NXbins, double Xmin, double Xmax, 
			const char * XTitle, const char * YTitle,
			Color_t color = kBlack,
			bool setStats = 0)
{
	TH1* hist = new TH1F(hist_name.c_str(), hist_title.c_str(), NXbins, Xmin, Xmax);
	hist->GetXaxis()->SetTitle(XTitle);
	hist->GetYaxis()->SetTitle(YTitle);
	hist->SetMarkerStyle(20);
	hist->SetLineColor(color);
	if (setStats) hist->SetStats(1);
	else hist->SetStats(0);

	return hist;
};


TH2* Hist2D(string hist_name,
			string hist_title, 
			int NXbins, double Xmin, double Xmax, 
			int NYbins, double Ymin, double Ymax,
			const char * XTitle, const char * YTitle,
			Color_t color = kBlack,
			bool setStats = 0)
{
	TH2* hist = new TH2F(hist_name.c_str(), hist_title.c_str(), NXbins, Xmin, Xmax, NYbins, Ymin, Ymax);
	hist->GetXaxis()->SetTitle(XTitle);
	hist->GetYaxis()->SetTitle(YTitle);
	hist->SetMarkerStyle(30);
	hist->SetLineColor(color);
	if (setStats) hist->SetStats(1);
	else hist->SetStats(0);

	return hist;
};



void eraseSubStr(string & mainStr, const string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}


struct SimulationAnalizer
{
	TFile* rootSimulationFilePtr;
	TTree* rootSimulationTreePtr;
	TND280UpEvent* nd280upgradeEventPtr;
	int NSimulatedEvents;

	string simulatedParticleInjectionPositionStr;
	string simulatedParticleMomentumAmplitudeStr;
	string simulatedParticleInitialKineticEnergyStr;
	string worldMagneticFieldStr;
	string injectedParticle;

	double simulatedParticleInitialKineticEnergy;
	float xStartPosition;
	float yStartPosition;
	string simulatedParticleInjectionPositionAsTitle;
	string simulatedParticleMomentumAmplitudeAsTitle;
	string simulatedParticleInitialKineticEnergyAsTitle;

	string outputAnalisysDirectoryRelativePath;
	string svgDirectoryRelativePath;
	string readmeRelativePath;

	string pathOfFileToAnalize;


	SimulationAnalizer(TString prefix, TString filename){
		pathOfFileToAnalize = static_cast<string>(prefix) + static_cast<string>(filename);
		// ------------------------------------------------------------------------ //
		// ------------------- Opening file, reading event tree ------------------- //
		// ------------------------------------------------------------------------ //
		// { read file and set tag variables block
			// { start reading .root file
				auto [file, tree, nd280UpEvent] = getEventsTreeFromRootFile(prefix, filename);
				rootSimulationFilePtr = file;
				rootSimulationTreePtr = tree;
				nd280upgradeEventPtr = nd280UpEvent;

				int NTreeEntries = tree->GetEntries(); // get number of events
				NSimulatedEvents = NTreeEntries;
				cout << cprint(" NTreeEntries: ", BLUE) << NTreeEntries << endl;
			// } stop reading .root file

			// { set variables block
				/// extract info from filename ///
				SimulationInformationFromFile info{static_cast<std::string>(filename)};
				info.print();

				/// set tag variables ///
				setTagVariables(info);
			// } end of set variables block
		// } end of read file and set tag variables block
		
		// ------------------------------------------------------------------------ //
		// ---------------------- Generating output directory --------------------- //
		// ------------------------------------------------------------------------ //
		createOutputDirectory();
		writeREADMEofAnalisys();
	};

	SimulationAnalizer(string prefix, string filename){
		pathOfFileToAnalize = prefix + filename;
		// ------------------------------------------------------------------------ //
		// ------------------- Opening file, reading event tree ------------------- //
		// ------------------------------------------------------------------------ //
		// { read file and set tag variables block
			// { start reading .root file
				auto [file, tree, nd280UpEvent] = getEventsTreeFromRootFile(static_cast<TString>(prefix), static_cast<TString>(filename));
				rootSimulationFilePtr = file;
				rootSimulationTreePtr = tree;
				nd280upgradeEventPtr = nd280UpEvent;

				int NTreeEntries = tree->GetEntries(); // get number of events
				cout << cprint(" NTreeEntries: ", BLUE) << NTreeEntries << endl;
			// } stop reading .root file

			// { set variables block
				/// extract info from filename ///
				SimulationInformationFromFile info{static_cast<string>(filename)};
				info.print();

				/// set tag variables ///
				setTagVariables(info);
			// } end of set variables block
		// } end of read file and set tag variables block
		createOutputDirectory();
		writeREADMEofAnalisys();
	};

	void setTagVariables(SimulationInformationFromFile& info)
	{
		simulatedParticleInjectionPositionStr = info.getPositionAsString();
		xStartPosition = info.x_start_pos;
		yStartPosition = info.y_start_pos;
		WriteToLog(LT_INFO, "start position: ", simulatedParticleInjectionPositionStr);
		worldMagneticFieldStr = info.getMagneticFieldAsString();
		WriteToLog(LT_INFO, "magnetic field: ", worldMagneticFieldStr);
		simulatedParticleMomentumAmplitudeStr  = info.getMomentumAmplitudeAsString();
		WriteToLog(LT_INFO, "momentum amplitude: ", simulatedParticleMomentumAmplitudeStr);
		simulatedParticleInitialKineticEnergyStr  = info.getInitialKineticEnergyAsString();
		simulatedParticleInitialKineticEnergy = info.initial_kin_energy;
		WriteToLog(LT_INFO, "initial kinetic energy: ", simulatedParticleInitialKineticEnergyStr);
		injectedParticle = info.particle_name;
		WriteToLog(LT_INFO, "injected particle: ", injectedParticle);

		simulatedParticleInjectionPositionAsTitle = info.getPositionAsStringName();
		simulatedParticleMomentumAmplitudeAsTitle = info.getMomentumAmplitudeAsStringValue();
		simulatedParticleInitialKineticEnergyAsTitle = info.getInitialKineticEnergyAsStringValue();
	}

	void createOutputDirectory()
	{
		string curr_time = getCurrentTime();

		system("mkdir output");
		outputAnalisysDirectoryRelativePath = "output/SA-" + curr_time + "/";
		system(("mkdir " + outputAnalisysDirectoryRelativePath).c_str());
		svgDirectoryRelativePath = outputAnalisysDirectoryRelativePath + "svg/";
		system(("mkdir " + svgDirectoryRelativePath).c_str());
		readmeRelativePath = outputAnalisysDirectoryRelativePath + "README.log";
	}

	void writeREADMEofAnalisys()
	{
		// string readme = R"readme(*******************************)readme";
		string readme = "******************************************************************\n";
		readme += "Analisys of '" + pathOfFileToAnalize + "'\n";
		readme += "Particle                       : " + injectedParticle + "\n";
		readme += "Magnetic field                 : " + worldMagneticFieldStr + "\n";
		readme += "Particle start position        : " + simulatedParticleInjectionPositionStr + "\n";
		readme += "Particle momentum amplitude    : " + simulatedParticleMomentumAmplitudeStr + "\n";
		readme += "PArticle initial kinetic energy: " + simulatedParticleInitialKineticEnergyStr + "\n";

		ofstream file;
		file.open (readmeRelativePath.c_str());
		file << readme;
		file.close();
	}

	void updateREADMEofAnalisys(string content)
	{
		ofstream file;
		file.open (readmeRelativePath.c_str(), ios_base::app);
		file << content;
		file.close();
	}

	void clear()
	{
		// ------------------------------------------------------------------------ //
		// ----------------- Deallocate allocated memory for tree ----------------- //
		// ------------------------------------------------------------------------ //
		// {
			delete rootSimulationTreePtr;
			rootSimulationFilePtr->Close();
			delete nd280upgradeEventPtr;
			delete rootSimulationFilePtr;
			rootSimulationFilePtr = ((TFile *)0);
		// }
	}

	void createSVGfilesInEvents(int start_event = 0, int stop_event = 1000)
	{
		/// cheak if correct numbers
		start_event = (start_event < 0) ? 0 : start_event;
		stop_event = ((stop_event < 0) || (stop_event > NSimulatedEvents)) ? NSimulatedEvents : stop_event;

		int _ = start_event;
		start_event = min(_, stop_event);
		stop_event = max(_, stop_event);

		// stop_event = (stop_event > NSimulatedEvents) ? NSimulatedEvents : stop_event;


		// int nthreads = 8;
		// ROOT::EnableImplicitMT(nthreads);

		/// events processing ///
		string jsFilesArray = "";
		// #pragma omp simd
		for (int ievent = start_event; ievent < stop_event; ++ievent)
		// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
		// for (auto ievent: ROOT::TSeq(stop_event - start_event)) // parallel processing
		{
			// rootSimulationTreePtr->GetEntry(ievent);
			EventAnalizer event{rootSimulationTreePtr, nd280upgradeEventPtr, ievent};
			event.print();
			string filename = event.createSVGofTracksTree(svgDirectoryRelativePath, "test-", ievent);
			eraseSubStr(filename, outputAnalisysDirectoryRelativePath);
			// jsFilesArray += "\t" + filename + ",\n";
		}

		jsFilesArray = "svgFilesArray = [\n" + jsFilesArray + "];\n";

		string jsFile = outputAnalisysDirectoryRelativePath + "svgFilesList.js";

		ofstream file;
		file.open (jsFile.c_str());
		file << jsFilesArray;
		file.close();

		string readmeInfo = "----------------------------------------------------------------\n";
		readmeInfo += "Created svg files for events from " + to_string(start_event) + " to " + to_string(stop_event) + "\n";
		readmeInfo += "Paths to mentioned files are written in JavaScript array in file '" + jsFile + "'\n";
		updateREADMEofAnalisys(readmeInfo);
	}

	void printEvents(int start_event = 0, int stop_event = 1000, bool printVertices = false)
	{
		/// cheak if correct numbers
		start_event = (start_event < 0) ? 0 : start_event;
		stop_event = ((stop_event < 0) || (stop_event > NSimulatedEvents)) ? NSimulatedEvents : stop_event;

		int _ = start_event;
		start_event = min(_, stop_event);
		stop_event = max(_, stop_event);

		// stop_event = (stop_event > NSimulatedEvents) ? NSimulatedEvents : stop_event;
		
		/// events processing ///
		for (int ievent = start_event; ievent < stop_event; ++ievent)
		// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
		{
			EventAnalizer event{rootSimulationTreePtr, nd280upgradeEventPtr, ievent};
			event.print();
			if (printVertices) event.printVertices();
			event.printPrimaryTracks();
		}
	}

	void computeEnergeticProperties(int start_event = 0, int stop_event = 1000)
	{

		// ------------------------------------------------------------------------ //
		// ------------------------ Declaring histograms -------------------------- //
		// ------------------------------------------------------------------------ //

		/// energy histograms ///
		// {
			// double x_limit = 5.0;
			// double x_limit = 10.0;
			// double x_limit = 20.0;
			// double x_limit = 25.0;
			// double x_limit = 30.0;
			// double x_limit = 60.0;
			// double x_limit = 40.0;
			// double x_limit = 70.0;
			// double x_limit = 80.0;
			// double x_limit = 90.0;
			// double x_limit = 110.0;
			// double x_limit = 130.0;
			// double x_limit = 140.0;
			// double x_limit = 180.0;
			// double x_limit = 200.0;
			// double x_limit = 220.0;
			double x_limit = 250.0;
			// double x_limit = 270.0;

			// double dE2dx_limit = 0.5;
			double dE2dx_limit = 3.0;
			// double dE2dx_limit = 5.0;
			// double dE2dx_limit = 10.0;
			// double dE2dx_limit = 30.0;
			// double dE2dx_limit = 40.0;

			// int nXbins_xlimit = int(x_limit) * 5;
			// int nXbins_dE2dxlimit = int(dE2dx_limit) * 5;
			// int nXbins_dE2dxlimit = int(dE2dx_limit);
			int nXbins_dE2dxlimit = int(dE2dx_limit) * 10;
			// int nXbins_dE2dxlimit = int(dE2dx_limit) * 100;
			int nXbins_xlimit = 75;
			// int nXbins_dE2dxlimit = int(dE2dx_limit);

			// switch (int(info.initial_kin_energy)) {
			// 	case 5: x_limit = 1.0; dE2dx_limit = 170.0; nXbins_xlimit = 50; nXbins_dE2dxlimit = nXbins_xlimit; break;
			// 	case 10: x_limit = 1.0; dE2dx_limit = 100.0; nXbins_xlimit = 50; nXbins_dE2dxlimit = nXbins_xlimit; break;
			// 	case 20: x_limit = 1.0; dE2dx_limit = 70.0; nXbins_xlimit = 50; nXbins_dE2dxlimit = nXbins_xlimit; break;
			// 	case 40: x_limit = 5.0; dE2dx_limit = 40.0; nXbins_xlimit = 50; break;
			// 	case 60: x_limit = 10.0; dE2dx_limit = 30.0; nXbins_xlimit = 100; break;
			// 	case 80: x_limit = 10.0; dE2dx_limit = 30.0; nXbins_xlimit = 100; break;
			// 	case 100: x_limit = 15.0; dE2dx_limit = 20.0; nXbins_xlimit = 150; break;
			// }

			string spec_e = simulatedParticleInitialKineticEnergyAsTitle;
			string spec_p = simulatedParticleMomentumAmplitudeAsTitle;
			string spec_mf = worldMagneticFieldStr;
			string particle_name = injectedParticle;

	// string simulatedParticleInjectionPositionStr;

			string h_edep2range_name = spec_e + " MeV (" + spec_p + " MeV/c) #" + particle_name + " edep2range";
			auto h_edep2range = Hist2D(
				"edep2range distribution", h_edep2range_name, 
				// int(x_limit) * 3, 0.0, x_limit,
				// int(dE2dx_limit * 10), 0.0, dE2dx_limit,
				nXbins_xlimit, 0.0, x_limit,
				nXbins_dE2dxlimit, 0.0, dE2dx_limit,
				"Range, cm", "dE/dx, MeV/cm", kRed, true);
			h_edep2range->SetStats(0);

			string h_range_name = spec_e + " MeV (" + spec_p + " MeV/c) #" + particle_name + " range";
			auto h_range = Hist1D("range distribution", h_range_name, 
				// int(x_limit * 3.0) * 3, 0.0, x_limit, "Range, cm", "N events", kRed, true);
				nXbins_xlimit, 0.0, x_limit, "Range, cm", "N events", kRed, true);
			h_range->SetFillColor(kRed);

			string h_edep_name = spec_e + " MeV (" + spec_p + " MeV/c) #" + particle_name + " edep";
			auto h_edep = Hist1D("edep distribution", h_edep_name, 
				// int(info.initial_kin_energy * 3.0) * 3, 0.0, info.initial_kin_energy * 1.5, "edep, MeV", "N events", kRed, true);
				75, 0.0, simulatedParticleInitialKineticEnergy  * 1.5, "edep, MeV", "N events", kRed, true);
			h_edep->SetFillColor(kRed);
		// }
	
		for (int ievent = start_event; ievent < stop_event; ++ievent)
		// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
		// for (auto ievent: ROOT::TSeq(stop_event - start_event)) // parallel processing
		{
			EventAnalizer event{rootSimulationTreePtr, nd280upgradeEventPtr, ievent};
			event.print();
			event.fillHistograms(
				h_range, // range histogram
				h_edep, // energy deposit histogram
				h_edep2range // dE/dx histogram
			);
		}

		// h_range->GetXaxis()->SetRangeUser(0.0, 10.0);
		// h_edep2range->GetXaxis()->SetRangeUser(0.0, 10.0);

		string tag = "";
		draw_energy_histograms(h_range, h_edep, h_edep2range,
							   xStartPosition, yStartPosition, particle_name,
							   spec_p, spec_e, spec_mf,
							   simulatedParticleInjectionPositionAsTitle, tag,
							   outputAnalisysDirectoryRelativePath);
	}

	void computeAverageProperties(int start_event = 0, int stop_event = 1000)
	{
		double avg_trk_len = 0.0, avg_trk_edep = 0.0, avg_trk_dE2dx = 0.0, trkInitialEkin;
		for (int ievent = start_event; ievent < stop_event; ++ievent)
		// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
		// for (auto ievent: ROOT::TSeq(stop_event - start_event)) // parallel processing
		{
			EventAnalizer event{rootSimulationTreePtr, nd280upgradeEventPtr, ievent};
			// event.print();
			auto [trk_len, trk_edep, trk_dE2dx, initialEkin] = event.getEnergeticProperties(11);
			avg_trk_len += trk_len;
			avg_trk_edep += trk_edep;
			avg_trk_dE2dx += trk_dE2dx;
			trkInitialEkin = initialEkin;
		}

		avg_trk_len /= (stop_event - start_event);
		avg_trk_edep /= (stop_event - start_event);
		avg_trk_dE2dx /= (stop_event - start_event);

		std::stringstream simInfo;
		simInfo << injectedParticle << "	" << trkInitialEkin << "	" << simulatedParticleInjectionPositionStr << "\n";
		simInfo << "R~: " << avg_trk_len << "\tE~: " << avg_trk_edep << "\tdE/dx~: " << avg_trk_dE2dx << "\n";

		std::string simInfoStr = simInfo.str();

		std::string filepath = outputAnalisysDirectoryRelativePath + 
				    		   "/" + injectedParticle + 
				    		   simulatedParticleInitialKineticEnergyStr + ".txt";
		std::cout << filepath << std::endl;

		ofstream file;
		file.open (filepath.c_str());
		file << simInfoStr;
		file.close();
	}

	void createHistogramsForPrimaryParticlesInNeutrinoInteractions()
	{

	}

	void getEnergiesOfPrimaryParticles(int start_event = 0, int stop_event = 1000)
	{
		std::stringstream simInfo;
		for (int ievent = start_event; ievent < stop_event; ++ievent)
		// for (auto ievent: ROOT::TSeqUL(stop_event - start_event)) // parallel processing
		// for (auto ievent: ROOT::TSeq(stop_event - start_event)) // parallel processing
		{
			EventAnalizer event{rootSimulationTreePtr, nd280upgradeEventPtr, ievent};
			event.print();

			std::vector<tuple<int, double>> energiesOfPrimaryParticles = event.getEnergiesOfPrimaryParticles();

			for (auto& item: energiesOfPrimaryParticles)
			{
				auto [pdg, initEkin] = item;
				simInfo << pdg << ":" << initEkin << ";";
			}
			simInfo << "\n";
		}

		std::string filepath = outputAnalisysDirectoryRelativePath + 
				    		   "/" + injectedParticle + 
				    		   simulatedParticleInitialKineticEnergyStr + "-energiesOfPrimPart.txt";

		std::string simInfoStr = simInfo.str();

		ofstream file;
		file.open (filepath.c_str());
		file << simInfoStr;
		file.close();
	}
};


#endif