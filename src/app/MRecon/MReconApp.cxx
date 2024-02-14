
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>

#include <TString.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include <ND280UpApplyResponse.hh>

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"

struct hits_info
{
  TH2F* h2d_xy;
  TH2F* h2d_xz;
  TH2F* h2d_yz;
  double total_edep;
};


void draw_MPPCProj2D(TH2F* histogram, std::string projection)
{
  std::string outFilename = "MPPCProj2D_" + projection + ".pdf";

  TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
  histogram->Draw("colz");
  canvas->SaveAs(outFilename.c_str());
}

hits_info process_hits(TND280UpEvent *nd280UpEvent, TH2F* h2d_xy_, TH2F* h2d_xz_, TH2F* h2d_yz_)
{
  // TH2F* h2d_xy = new TH2F("XY", "XY projection", 192, -96.0, 96.0, 56, -28.0, 28.0);
  // TH2F* h2d_xz = new TH2F("XZ", "XZ projection", 192, -96.0, 96.0, 184, -92.0, 92.0);
  // TH2F* h2d_yz = new TH2F("YZ", "YZ projection", 56, -28.0, 28.0, 184, -92.0, 92.0);

  int NHits = nd280UpEvent->GetNHits();
  
  double total_hits_edep = 0.0;

  // for (int ihit = 0; ihit < NHits; ++ihit)
  // {
  //   TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
  //   double edep = nd280UpHit->GetEnergyDeposit();
  //   total_hits_edep += edep;
  // }

  for (int ihit = 0; ihit < NHits; ++ihit)
  {
    TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

    double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
    double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X, mm
    double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y, mm
    double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z, mm
    
    TVector3 lightPos(posX,posY,posZ); // needed for ApplyResponse
    double charge = 1.; // assumption on hit charge; needed for ApplyResponse

    double edep = nd280UpHit->GetEnergyDeposit();
    double steplength = nd280UpHit->GetTrackLength();
    std::string detname = nd280UpHit->GetDetName();

    // std::cout << "Hit " << ihit << " [" << posX << " " << posY << " " << posZ << "] " << edep << std::endl;

    total_hits_edep += edep;

    int PDG = nd280UpHit->GetPDG();
    // int TrackID = nd280UpHit->GetTrackID();
    // int ParentID = nd280UpHit->GetParentID();

    std::vector<int> contributors = nd280UpHit->fContributors;

    h2d_xy_->Fill(posX / 10.0, posY / 10.0, edep); // pos in cm
    h2d_xz_->Fill(posX / 10.0, posZ / 10.0, edep);
    h2d_yz_->Fill(posY / 10.0, posZ / 10.0, edep);
    // h2d_xy_->Fill(posX, posY, edep / total_hits_edep);
    // h2d_xz_->Fill(posX, posZ, edep / total_hits_edep);
    // h2d_yz_->Fill(posY, posZ, edep / total_hits_edep);
    // h2d_xy_->Fill(posX / 10.0, posY / 10.0);
    // h2d_xz_->Fill(posX / 10.0, posZ / 10.0);
    // h2d_yz_->Fill(posY / 10.0, posZ / 10.0);
  }

  std::cout << "NHits: " << NHits << ", edep: " << total_hits_edep << std::endl;

  hits_info hi;
  hi.h2d_xy = h2d_xy_;
  hi.h2d_yz = h2d_yz_;
  hi.h2d_xz = h2d_xz_;
  hi.total_edep = total_hits_edep;

  return hi;
}

void process_true_tracks(TND280UpEvent *nd280UpEvent)
{
  TND280UpTrack* primary_track;

  for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

    // int TrackID = track->GetTrackID();
    // int ParentID = track->GetParentID();
    // int GParentID = tid_gpid[TrackID];
    // std::string ProcessName = track->GetProcessName();
    // int PDG = track->GetPDG();
    // double InitKinEnergy = track->GetInitKinEnergy();
    if (track->GetParentID() == 0) { primary_track = track; break; }
  }

  if (!primary_track) return;


}

TND280UpTrack* findPrimaryTrack(TND280UpEvent *nd280UpEvent)
{
  TND280UpTrack* primary_track = nullptr;

  for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++)
  {
    TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
    if (track->GetParentID() == 0) { primary_track = track; break; }
  }

  return primary_track;
}


int main(int argc, char **argv)
{
  std::cout << "Usage:" << std::endl;
  std::cout << "  MReconApp <input ROOT file name (from GEANT4 simulation)> <output ROOT file (MRecon)>"  << std::endl;

  if (argc != 3)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  TString outfilename = "reco_output.root";
  std::string input_rootfilename = argv[1];
  std::string output_rootfilename = argv[2];
  // int event_id_to_process = atoi(argv[3]);

  std::cout << "===============================================================" << std::endl;
  std::cout << "Received input ROOT file: " << input_rootfilename << std::endl;
  std::cout << "ROOT file " << output_rootfilename << " will be produced" << std::endl;
  std::cout << "===============================================================" << std::endl;

  std::size_t found = output_rootfilename.find_last_of("/");
  std::string out_file_path = output_rootfilename.substr(0,found);
  std::string out_file_tag = output_rootfilename.substr(found+1);
  // std::string out_file_pathtag_fin = output_rootfilename + "/" + out_file_tag;
  std::stringstream cmd; cmd << "mkdir " << output_rootfilename;
  int res = std::system(cmd.str().c_str());
  if (res != 0)
  {
    return -1;
  }
  // std::cout << " path: " << str.substr(0,found) << '\n';
  // std::cout << " file: " << str.substr(found+1) << '\n';

  TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
  std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

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

  auto c = new TCanvas("c","c");
  // c->SetCanvasSize(1500, 1500);
  // c->SetWindowSize(500, 500);
  c->SetCanvasSize(2500, 1500);

  TH1F* range_distrn = new TH1F("Range", "Ranges of e-;Range,cm;N events", 100, 0.0, 230.0);
  TH1F* edep_distrn = new TH1F("Edep", "Energy deposit of e-;E, MeV;N events", 100, 0.0, 800.0);
  TH1F* dEdx_distrn = new TH1F("dedx", "dE/dx of e-;dE/dx, MeV/cm;N events", 100, 0.0, 30.0);
  // TH2F* dEdx_distrn = new TH2F("dedx", "dE/dx of e-;Range;dE/dx, MeV/cm", 100, 0.0, 230.0, 20, 0.0, 10.0);
  range_distrn->SetFillColor(kRed);
  edep_distrn->SetFillColor(kRed);
  dEdx_distrn->SetFillColor(kRed);
  auto c_range = new TCanvas("c_range","c_range");

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++)
  // for(int ievt=evtfirst;ievt<=100;ievt++)
  {
    std::cout << "\n\nProcessing event " << ievt << " of " << Nentries << std::endl;

    tinput->GetEntry(ievt);

    int NHits = nd280UpEvent->GetNHits();
    int NTracks = nd280UpEvent->GetNTracks();

    std::cout << "NHits " << NHits << std::endl;
    std::cout << "NTracks " << NTracks << std::endl;

    TH2F* h2d_xy_p = new TH2F("XY", "XY projection;X,cm;Y,cm", 192, -96.0, 96.0, 56, -28.0, 28.0);
    TH2F* h2d_xz_p = new TH2F("XZ", "XZ projection;X,cm;Z,cm", 192, -96.0, 96.0, 184, -92.0, 92.0);
    TH2F* h2d_yz_p = new TH2F("YZ", "YZ projection;Y,cm;Z,cm", 56, -28.0, 28.0, 184, -92.0, 92.0);
    h2d_xy_p->SetStats(0);
    h2d_xz_p->SetStats(0);
    h2d_yz_p->SetStats(0);

    /// simulated edep
    hits_info hi = process_hits(nd280UpEvent, h2d_xy_p, h2d_xz_p, h2d_yz_p);

    /// true tracks
    TND280UpTrack* true_primary_track = findPrimaryTrack(nd280UpEvent);
    if (!true_primary_track) continue;
    // double tpt_range = true_primary_track->GetRange();
    // double tpt_range = true_primary_track->GetSDLength();
    double tpt_range = true_primary_track->GetLengthTarget1();
    int tpt_PDG = true_primary_track->GetPDG();

    std::string range_distrn_ttl = "Ranges of " + std::to_string(tpt_PDG) + ";Range,cm;N events";
    std::string edep_distrn_ttl = "Energy deposit of " + std::to_string(tpt_PDG) + ";E, MeV;N events";
    std::string dEdx_distrn_ttl = "dE/dx of " + std::to_string(tpt_PDG) + ";dE/dx, MeV/cm;N events";
    range_distrn->SetTitle(range_distrn_ttl.c_str());
    edep_distrn->SetTitle(edep_distrn_ttl.c_str());
    dEdx_distrn->SetTitle(dEdx_distrn_ttl.c_str());

    range_distrn->Fill(tpt_range / 10.0);
    edep_distrn->Fill(hi.total_edep);
    dEdx_distrn->Fill(hi.total_edep / tpt_range * 10.0);
    // dEdx_distrn->Fill(tpt_range, hi.total_edep / tpt_range * 10.0);

    std::stringstream out_fname;
    // out_fname << output_rootfilename << "-Evt" << ievt << "-pXY.pdf";
    // out_fname << output_rootfilename << "-Evt" << ievt << "-pXZ.pdf";
    // out_fname << output_rootfilename << "-Evt" << ievt << ".pdf";
    out_fname << output_rootfilename << "/Evt" << ievt << ".pdf";
    
    // c->cd();
    c->Divide(2,2);
    c->cd(1);
    // hi.h2d_xy->Draw("COLZ");
    h2d_xy_p->Draw("COLZ");
    c->cd(2);
    // hi.h2d_xz->Draw("COLZ");
    h2d_xz_p->Draw("COLZ");
    c->cd(3);
    // hi.h2d_yz->Draw("COLZ");
    h2d_yz_p->Draw("COLZ");

    c->cd(4);
    TPaveText *pt = new TPaveText(.05,.1,.95,.8);
    std::stringstream ln_0; ln_0 << "Particle " << tpt_PDG;
    pt->AddText(ln_0.str().c_str());
    std::stringstream ln_1; ln_1 << "Total collected energy deposit in hits " << hi.total_edep << " MeV";
    pt->AddText(ln_1.str().c_str());
    std::stringstream ln_2; ln_2 << "Range: " << tpt_range / 10.0 << " cm";
    pt->AddText(ln_2.str().c_str());
    pt->Draw();

    c->SaveAs(out_fname.str().c_str());
    c->Clear();

    delete h2d_xy_p;
    delete h2d_xz_p;
    delete h2d_yz_p;
    delete pt;

    /// reconstruction
  }

  // std::stringstream out_fname_range; out_fname_range << output_rootfilename << "-ranges.pdf";
  std::stringstream out_fname_range; out_fname_range << output_rootfilename << "/ranges.pdf";
  c_range->Divide(1,2);
  c_range->cd(1);
  range_distrn->Draw();
  c_range->cd(2);
  edep_distrn->Draw();
  // c_range->cd(3);
  // dEdx_distrn->Draw("COLZ");
  c_range->SaveAs(out_fname_range.str().c_str());
}






















int main_v0(int argc, char **argv)
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
