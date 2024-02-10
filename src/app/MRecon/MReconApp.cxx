
#include <iostream>
#include <string>

#include <TString.h>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"



int main(int argc, char **argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  MReconApp <input ROOT file name (from GEANT4 simulation)> <output ROOT file (MRecon)>"  << std::endl;

    if (argc < 3)
    {
        std::cout << "Incorrect number of arguments" << std::endl;
        return 1;
    }

    TString outfilename = "reco_output.root";
    std::string input_rootfilename = argv[1];
    std::string output_rootfilename = argv[2];

    // TString outfilename = "/nfs/neutrinos/cjesus/work/MC_output.root";
    // string rootfilename = argv[1];
    // const int evtfirst = atoi(argv[2]);
    // const int nevents = atoi(argv[3]);
    // const int detectorID = atoi(argv[5]);
    // outfilename = argv[13];

    return 0;

    /*
    if (argc < 2)
    {
        std::cout << "Missing option: trackrecon, lightyield, neutron, stopproton, protomc, sfgrrecon" << std::endl;
        return 1;
    }

    char **subArgv = new char *[argc - 1];
    subArgv[0] = argv[0];
    // Skip 2nd argument which is the main option
    for (int i = 2; i < argc; i++)
    {
        subArgv[i - 1] = argv[i];
    }

    switch (str2int(argv[1]))
    {
    case str2int("trackrecon"):
        TrackReconstruction(argc - 1, subArgv);
        break;
    case str2int("lightyield"):
        LightYield(argc - 1, subArgv);
        break;
    case str2int("neutron"):
        NeutronAnalysis(argc - 1, subArgv);
        break;
    case str2int("stopproton"):
        StoppingProton(argc - 1, subArgv);
        break;
    case str2int("sfgrrecon"):
        SFGD_Reconstruction(argc - 1, subArgv);
        break;
    default:
        std::cerr << "Usage: ReconstructionApp <option> <arguments...>\nOption: trackrecon, lightyield, neutron, stopproton, protomc, sfgrrecon" << std::endl;
        return 0;
    }

    return 1;

    if (argc!=14){   // batch mode
        cout << "You need to provide the following arguments:" << endl;
        cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
        cout << " 2) first event number to run" << endl;
        cout << " 3) total number of events to run" << endl;
        cout << " 4) the tag for the output file name" << endl;
        cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
        cout << " 6) set debug option" << endl;
        cout << " 7) set debug plot option" << endl;
        cout << " 8) use view XY (0 or 1)" << endl;
        cout << " 9) use view XZ (0 or 1)" << endl;
        cout << " 10) use view YZ (0 or 1)" << endl;
        cout << " 11) Minimum track distance (if <=0 use default)" << endl;
        cout << " 12) Look only at tracks inside the Truth FV (use MC hits)" << endl;
        cout << " 13) Output file name. In iteractive run it can be first event, but for parallel need separate param" << endl;
        exit(1);
    }
    // */
}
