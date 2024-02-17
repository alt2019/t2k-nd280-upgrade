

#include <iostream>
#include <list>

#include <TString.h>
#include <TTree.h>
#include <TFile.h>

#include <omp.h> // omp exists, but currently not used

#include "TND280UpEvent.hh"

// #include "color_definitions.hh"
#include "ExtractSimulationInformation.hh" // SimulationInformationFromFile, getEventsTreeFromRootFile
// #include "ExtractHitInfo.hh"
// #include "ExtractTrackInfo.hh"
// #include "ExtractTrackPointInfo.hh"
// #include "XML.hh"
#include "AnalizeEvents.hh"

#include "SimulationAnalizer.hh"

// #if defined __has_include
// #  if __has_include (<stdatomic.h>)
// #    include <stdatomic.h>
// #  endif
// #endif


void DR(TString prefix, TString filename) {
	// int nthreads = 4;
	// ROOT::EnableImplicitMT(nthreads);

	// ------------------------------------------------------------------------ //
	// ------------------- Opening file, reading event tree ------------------- //
	// ------------------------------------------------------------------------ //
	// { read file and set tag variables block
		// { start reading .root file
			auto [file, tree, nd280UpEvent] = getEventsTreeFromRootFile(prefix, filename);

			int NTreeEntries = tree->GetEntries(); // get number of events
			std::cout << cprint(" NTreeEntries: ", BLUE) << NTreeEntries << std::endl;
		// } stop reading .root file

		// { set variables block
			/// extract info from filename ///
			SimulationInformationFromFile info{static_cast<std::string>(filename)};
			info.print();

			/// set tag variables ///
			std::string start_pos = info.getPositionAsString();
			WriteToLog(LT_INFO, "start position: ", start_pos);
			std::string spec_mf = info.getMagneticFieldAsString();
			WriteToLog(LT_INFO, "magnetic field: ", spec_mf);
			std::string spec_p  = info.getMomentumAmplitudeAsString();
			WriteToLog(LT_INFO, "momentum amplitude: ", spec_p);
			std::string spec_e  = info.getInitialKineticEnergyAsString();
			WriteToLog(LT_INFO, "initial kinetic energy: ", spec_e);
		// } end of set variables block
	// } end of read file and set tag variables block
	
	// ------------------------------------------------------------------------ //
	// ---------------------- Print events infornmation ----------------------- //
	// ------------------------------------------------------------------------ //
	std::list<int> particles_to_skip = {22};
	printEventsInfo(tree, nd280UpEvent,
					0,            // start event
					1000,         // final event + 1
					0,            // print_tracks
					0,            // print_each_track_points
					particles_to_skip,  // list of particles to skip
					0,            // print_primary_track
					0,            // print_primary_track_points
					0             // print_hits
	);

	// ------------------------------------------------------------------------ //
	// --------------------------- Draw histograms ---------------------------- //
	// ------------------------------------------------------------------------ //
	
	// ------------------------------------------------------------------------ //
	// ---------------------- Create html visualization ----------------------- //
	// ------------------------------------------------------------------------ //
	
	// ------------------------------------------------------------------------ //
	// ----------------- Deallocate allocated memory for tree ----------------- //
	// ------------------------------------------------------------------------ //
	// {
		file->Close();
		delete nd280UpEvent;
		delete file;
		file = ((TFile *)0);
	// }
}


void DR1(TString prefix, TString filename) {
	SimulationAnalizer SA{prefix, filename};

	SA.createSVGfilesInEvents(0, 100);
	// SA.printEvents(0, 10,
	// 			   1 /* print vertices*/
	// );
	SA.computeEnergeticProperties(0, 1000);
	// SA.computeAverageProperties(0, 1000);
	// SA.getEnergiesOfPrimaryParticles(0, 1000);

	std::cout << "!!! Output directory: " << SA.svgDirectoryRelativePath << std::endl;

	SA.clear();
}



int main(int argc, char ** argv)
{
	std::cout << "Hi" << std::endl;
	WriteToLog(LT_INFO, " Current standard version: ", to_string(__cplusplus));
	WriteToLog(LT_INFO, " Current File: ", (string)(__FILE__));
	// std::cout << " File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
	if (argc >= 2)
	{
		std::cout << argv[1] << " " << argv[2] << std::endl;
		// DR(argv[1], argv[2]);
		DR1(argv[1], argv[2]);
	}
	// return 0;
	return EXIT_SUCCESS;
}