
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TString.h>
#include <TVector3.h>
#include <TCanvas.h>

#include <ND280UpApplyResponse.hh>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"


void draw_MPPCProj2D(TH2F* histogram, std::string projection)
{
  std::string outFilename = "MPPCProj2D_" + projection + ".pdf";

  TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
  histogram->Draw("colz");
  canvas->SaveAs(outFilename.c_str());
}

// void find_


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

    std::cout << "===============================================================" << std::endl;
    std::cout << "Received input ROOT file: " << input_rootfilename << std::endl;
    std::cout << "ROOT file " << output_rootfilename << " will be produced" << std::endl;
    std::cout << "===============================================================" << std::endl;

    TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
    std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

    TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
    h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
    h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
    h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

    // draw_MPPCProj2D(h2d_xy, "XY");
    // draw_MPPCProj2D(h2d_xz, "XZ");
    // draw_MPPCProj2D(h2d_yz, "YZ");

    Int_t binX = h2d_xy->GetXaxis()->GetNbins();
    Int_t binY = h2d_xy->GetYaxis()->GetNbins();
    Int_t binZ = h2d_xz->GetYaxis()->GetNbins();
    
    Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
    Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
    Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
    Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binY);
    Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
    Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binZ);
    
    std::cout << " x min " << X_min << " x max " << X_max << "  nbins " << binX << std::endl;
    std::cout << " y min " << Y_min << " y max " << Y_max << "  nbins " << binY << std::endl;
    std::cout << " z min " << Z_min << " z max " << Z_max << "  nbins " << binZ << std::endl;

    TTree *tinput = (TTree*) finput->Get("ND280upEvents");

    // Declare the input true event object
    TND280UpEvent *nd280UpEvent = new TND280UpEvent();
    tinput->SetBranchAddress("Event",&nd280UpEvent);
    
    int NTreeEntries = tinput->GetEntries();
    int evtlasttree = NTreeEntries-1;

    int evtfirst = 0;
    int Nentries = NTreeEntries - evtfirst;
    int EntryLast = evtfirst+Nentries-1;
    
    std::cout << std::endl;
    std::cout << "# of entries in the Tree: " << NTreeEntries << std::endl;
    std::cout << "last tree event ID: " << evtlasttree << std::endl;
    std::cout << "first event ID: " << evtfirst << std::endl;
    std::cout << std::endl;

    /// calculate detector responce in photoelectrons
    ND280UpApplyResponse ApplyResponse;
    ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
    ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
    ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);

    nd280upconv::TargetType_t DetType = nd280upconv::kSuperFGD;

    std::map<int, int> hit_pdg_map;
    std::map<int, int> truetrack_pdg_map;

    std::cout << std::endl;
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++)
    {
        std::cout << "\n\nProcessing event " << ievt << " of " << Nentries << std::endl;

        tinput->GetEntry(ievt);

        int NHits = nd280UpEvent->GetNHits();
        int NTracks = nd280UpEvent->GetNTracks();

        std::cout << "NHits " << NHits << std::endl;
        std::cout << "NTracks " << NTracks << std::endl;

        /// find events with e- -> gamma -> e+/e- topology

        std::vector<int> trkIDs;
        std::vector<int> trkPIDs;
        std::vector<int> trkPDGs;
        std::vector<std::string> trkProcNames;
        std::vector<double> trkE0ks;

        std::map<int, int> tid_pid;
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
        {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
            int TrackID = track->GetTrackID();
            int ParentID = track->GetParentID();
            tid_pid.insert({TrackID, ParentID});
        }

        std::map<int, int> tid_gpid;
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
        {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
            int TrackID = track->GetTrackID();
            int ParentID = track->GetParentID();
            int GParentID = tid_pid[ParentID];
            tid_gpid.insert({TrackID, GParentID});
        }

        // TND280UpTrack* maintrack;

        /// loop over true tracks
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
        {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

            int TrackID = track->GetTrackID();
            int ParentID = track->GetParentID();
            int GParentID = tid_gpid[TrackID];
            std::string ProcessName = track->GetProcessName();
            int PDG = track->GetPDG();
            double InitKinEnergy = track->GetInitKinEnergy();

            trkIDs.push_back(TrackID);
            trkPIDs.push_back(ParentID);
            trkPDGs.push_back(PDG);
            trkProcNames.push_back(ProcessName);
            trkE0ks.push_back(InitKinEnergy);

            // if (TrackID == 1) { maintrack = track; continue; }
            // if (ParentID == 1)
            // {

            // }

            // if (ParentID != 0) continue;
            // if (InitKinEnergy < 0.5 /* 500 KeV */) continue;
            // if (ProcessName == "compt") continue;
            // if (PDG == 22) continue;
            // if ((GParentID != 1) && (ParentID != 0)) continue;
            
            // if (GParentID != 1)
            // {
            //     if (ParentID != 1)
            //     {
            //         if (ParentID != 0) continue;
            //     }
            // }

            // if (GParentID == 1)
            // {
            //     int parentPDG = nd280UpEvent->GetTrack(ParentID)->GetPDG();
            //     if (parentPDG != 22) continue;
            //     if (abs(PDG) != 11) continue;
            // }

            if (!((GParentID == 1) && (ProcessName == "conv"))) continue;

            std::cout << "Track " << TrackID
                      << ": Pid[" << ParentID << "] GPid[" << GParentID << "]"
                      << " PDG[" << PDG << "] E0kin[" << InitKinEnergy << "] Proc[" << ProcessName << "] "
            << std::endl;

            // TVector3 GetInitMom() {return fInitMom;};
            // double GetInitCosTheta(){return fInitCosTheta;};
            // double GetCharge(){return fCharge;};
            // double GetRange(){return fRange;};
            // double GetSDTotalEnergyDeposit(){return fSDTotalEnergyDeposit;};
            // double GetSDLength(){return fSDLength;};
            // double GetLengthTarget1() {return fLengthTarget1;};
            // double GetEdepTarget1()  {return fEdepTarget1;};

            // listOfTrueTrackID.push_back(track->GetTrackID());
            // listOfParentID.push_back(track->GetTrackID());
            // cout << "ALL || PDG: " <<  track->GetPDG()
            //          << ", ID: " <<    track->GetTrackID() 
            //          << ", prntID: " << track->GetParentID() << endl;
            // if(track->GetPDG() == 13 && !track->GetParentID()) mu_found = true;
        }

        





        double total_hits_edep = 0.0;

        /// loop over hits
        // for (int ihit = 0; ihit < NHits; ++ihit)
        for (int ihit = 0; ihit < 0; ++ihit)
        {
            TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

            double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
            double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
            double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
            double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
            TVector3 lightPos(posX,posY,posZ); // needed for ApplyResponse
            double charge = 1.; // assumption on hit charge; needed for ApplyResponse

            double edep = nd280UpHit->GetEnergyDeposit();
            double steplength = nd280UpHit->GetTrackLength();
            std::string detname = nd280UpHit->GetDetName();

            total_hits_edep += edep;

            int PDG = nd280UpHit->GetPDG();
            int TrackID = nd280UpHit->GetTrackID();
            int ParentID = nd280UpHit->GetParentID();

            // ApplyResponse.SetTargetID(DetType);
            // ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);//ApplyResponse.CalcResponse(lightPos,1,0,1 ,0 /*time*/,-1 /*steplength*/,sat_edep*156.42,"kSuperFGD");
            // PE_expect += (ApplyResponse.GetHitPE().x() +  ApplyResponse.GetHitPE().y() +  ApplyResponse.GetHitPE().z());

            /// not set from geant4 simulation
            // double PEX = nd280UpHit->GetPEX();
            // double PEY = nd280UpHit->GetPEY();
            // double PEZ = nd280UpHit->GetPEZ();
            // double MPPCPosX = nd280UpHit->GetMPPCPosX();
            // double MPPCPosY = nd280UpHit->GetMPPCPosY();
            // double MPPCPosZ = nd280UpHit->GetMPPCPosZ();
            // double TimePEX = nd280UpHit->GetTimePEX();
            // double TimePEY = nd280UpHit->GetTimePEY();
            // double TimePEZ = nd280UpHit->GetTimePEZ();

            std::vector<int> fContributors = nd280UpHit->fContributors;
            std::stringstream sContributors;
            for (auto it: fContributors)
            {
                sContributors << it << ",";
            }

            std::cout << "Hit " << ihit
                      << ": T[" << time << "] X[" << posX << "] Y[" << posY << "] Z[" << posZ << "] "
                      << "dE[" << edep << "] dL[" << steplength << "] Det[" << detname << "] "
                      << "\n"
                      << " PDG[" << PDG << "] TrackID[" << TrackID << "] ParentID[" << ParentID << "] C[" << sContributors.str() << "] "
                    //   << "\n PEX[" << PEX << "] PEY[" << PEY << "] PEZ[" << PEZ << "] "
                    //   << "\n MPPCPosX[" << MPPCPosX << "] MPPCPosY[" << MPPCPosY << "] MPPCPosZ[" << MPPCPosZ << "] "
            << std::endl;
        }

        std::cout << "Total edep over hits: " << total_hits_edep << std::endl;

        

    }

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
