
// #include "utils.hh"
// #include "ParticleGunAnalyzer.hh"

// void analize_events_from_particle_gun_1(std::string input_rootfilename, std::string output_dir_tag)
// {
//   TFile *finput = new TFile(input_rootfilename.c_str(),"READ");
//   std::cout << "Reading the input ROOT file: " << input_rootfilename << std::endl;

//   std::string out_csv_path = output_dir_tag + "/main-edep-data.csv";

//   TTree *tinput = (TTree*) finput->Get("ND280upEvents");

//   // Declare the input true event object
//   TND280UpEvent *nd280UpEvent = new TND280UpEvent();
//   tinput->SetBranchAddress("Event",&nd280UpEvent);

//   // int NeventsWOgamma = 0;

//   TH1F* total_edep = new TH1F("total_edep", "total energy deposit of primary particle;dE, MeV;N events", 300, 0.0, 300); // for 200 MeV
//   TH1F* h_total_edep_all_tracks = new TH1F("total_all_tracks", "total energy deposit of all tracks;dE, MeV;N events", 200, 0.0, 400); // for 200 MeV
//   TH2F* h_edep_range_2d_hist = new TH2F("edep_range_2d_hist", "Enerdy deposit at range;Track length, cm;dE, MeV", 100, 0.0, 100.0, 100, 0.0, 15.0); // for e-/mu- 200 MeV
  
//   TH1F* h_edep_at_10cm = new TH1F("edep at 10 cm", "Energy deposit at 10th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_15cm = new TH1F("edep at 15 cm", "Energy deposit at 15th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_20cm = new TH1F("edep at 20 cm", "Energy deposit at 20th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_30cm = new TH1F("edep at 30 cm", "Energy deposit at 30th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_40cm = new TH1F("edep at 40 cm", "Energy deposit at 40th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_50cm = new TH1F("edep at 50 cm", "Energy deposit at 50th XYlayer;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV

//   TH1F* h_edep_at_10cm_cube = new TH1F("edep at 10 cm cube", "Energy deposit at 10th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_15cm_cube = new TH1F("edep at 15 cm cube", "Energy deposit at 15th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_20cm_cube = new TH1F("edep at 20 cm cube", "Energy deposit at 20th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_30cm_cube = new TH1F("edep at 30 cm cube", "Energy deposit at 30th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_40cm_cube = new TH1F("edep at 40 cm cube", "Energy deposit at 40th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV
//   TH1F* h_edep_at_50cm_cube = new TH1F("edep at 50 cm cube", "Energy deposit at 50th cube;dE, MeV;N events", 60, 0.0, 15.0); // for 200 MeV

//   TH2F* h2_event_display_edep_xy = new TH2F("XY plane", "", 56, 0.0, 56.0, 192, 0.0, 192.0);
//   TH2F* h2_event_display_edep_xz = new TH2F("XZ plane", "", 184, 0.0, 184.0, 192, 0.0, 192.0);
//   TH2F* h2_event_display_edep_yz = new TH2F("YZ plane", "", 184, 0.0, 184.0, 56, 0.0, 56.0);
  
//   TCanvas* c_event_display = new TCanvas("c_event_display", "Event display", 1100, 1000);


//   int main_particle_PDG = -1;
//   double main_particle_InitKinEnergy = -1.0;

//   int NTreeEntries = tinput->GetEntries();

//   for (int eventId = 0; eventId < NTreeEntries; ++eventId)
//   // for (int eventId = 1; eventId < 2; ++eventId)
//   // for (int eventId = 1; eventId < 10; ++eventId)
//   // for (int eventId = 1; eventId < 100; ++eventId)
//   {
//     std::cout << "\nEvent " << eventId << std::endl;
//     tinput->GetEntry(eventId);

//     int NHits = nd280UpEvent->GetNHits();
//     int NTracks = nd280UpEvent->GetNTracks();

//     std::cout << "NHits " << NHits << std::endl;
//     std::cout << "NTracks " << NTracks << std::endl;
//     std::cout << "NVertices " << nd280UpEvent->GetNVertices() << std::endl;

//     TND280UpTrack* primary_track;

//     double energies_of_secondary_photons = 0.0;
//     double total_edep_of_all_tracks = 0.0;
//     // double total_edep_of_gammas = 0.0;

//     for (Int_t trjID = 0; trjID < NTracks; trjID++)
//     {
//       TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
//       int ParentID = track->GetParentID();
//       int PDG = track->GetPDG();
//       int NPoints = track->GetNPoints();
//       int TrackID = track->GetTrackID();
//       std::string ProcessName = track->GetProcessName();
//       double InitKinEnergy = track->GetInitKinEnergy();

//       std::cout 
//                 // << "\n\n\n"
//                 << " TID:" << TrackID
//                 << " PID:" << ParentID
//                 << " PDG:" << PDG
//                 << " Proc:" << ProcessName
//                 << " Ek0:" << InitKinEnergy
//                 << " Npt:" << NPoints
//       << std::endl;

//       if (ParentID == 0)
//       {
//         primary_track = track;
//         // break;
//       }

//       if (ParentID == 1 && PDG == 22)
//       {
//         energies_of_secondary_photons += track->GetInitKinEnergy();
//       }

//       for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
//       {
//         TND280UpTrackPoint* trackPoint = track->GetPoint(ipt);

//         double Edep = trackPoint->GetEdep();
//         total_edep_of_all_tracks += Edep;

//         /* /// fill event display for all tracks
//         TVector3 PrePosition = trackPoint->GetPrePosition();
//         TVector3 PostPosition = trackPoint->GetPostPosition();
//         TVector3 AvgPos = (PrePosition + PostPosition);
//         AvgPos *= 0.5;

//         // double new_x = (AvgPos.X() + 960.0) / 10.0;
//         double new_x = (- (AvgPos.X() - 960.0)) / 10.0; // right coordinate system
//         double new_y = (AvgPos.Y() + 280.0) / 10.0;
//         double new_z = (AvgPos.Z() + 920.0) / 10.0;
//         // std::cout << "New coords: " << new_x << " " << new_y << " " << new_z << std::endl;

//         h2_event_display_edep_xy->Fill(new_y, new_x, Edep);
//         h2_event_display_edep_xz->Fill(new_z, new_x, Edep);
//         h2_event_display_edep_yz->Fill(new_z, new_y, Edep);
//         // */
//       }
//     }

//     h_total_edep_all_tracks->Fill(total_edep_of_all_tracks);

//     /// process primary track
//     int TrackID = primary_track->GetTrackID();
//     int ParentID = 0;
//     std::string ProcessName = primary_track->GetProcessName();
//     int PDG = primary_track->GetPDG();
//     double InitKinEnergy = primary_track->GetInitKinEnergy();

//     main_particle_PDG = PDG;
//     main_particle_InitKinEnergy = InitKinEnergy;

//     int NPoints = primary_track->GetNPoints();

//     std::cout << " TID:" << TrackID
//               << " PID:" << ParentID
//               << " PDG:" << PDG
//               << " Proc:" << ProcessName
//               << " Ek0:" << InitKinEnergy
//               << " Npt:" << NPoints
//     << std::endl;

//     double dL_over_points = 0.0;
//     double dL_over_points2 = 0.0;
//     double dE_over_pts = 0.0;

//     std::map<std::tuple<int, double, double>, double> zLayersEdep;
//     std::map<std::tuple<int, double, double>, double> zCubeEdep;
//     std::map<std::tuple<double, double, double, double, double, double>, std::tuple<int,double>> edepAtCubes;
//     // std::map<std::tuple<int, double, double>, double> zLayersEdep_with_gamma;
//     for (int i = 0; i < 184; i++)
//     {
//       double z0 = i * 10.0 - 920.0;
//       double z1 = (i+1) * 10.0 - 920.0;
//       double fill_value = 0.0;
//       zLayersEdep.insert({{i, z0, z1}, fill_value});
//       zCubeEdep.insert({{i, z0, z1}, fill_value});
//       // zLayersEdep_with_gamma.insert({{i, z0, z1}, fill_value});
//     }

//     int cube_idx = 0;
//     for (int ipt = 0; ipt < NPoints; ++ipt) // loop over track points
//     {
//       TND280UpTrackPoint* trackPoint = primary_track->GetPoint(ipt);
//       TVector3 PrePosition = trackPoint->GetPrePosition();
//       TVector3 PostPosition = trackPoint->GetPostPosition();
//       TVector3 AvgPos = (PrePosition + PostPosition);
//       AvgPos *= 0.5;
//       double zAvgPos = AvgPos.Z();
//       // if (!isPtIntoSFGD(PrePosition)) continue;

//       double Time = trackPoint->GetTime();
//       TVector3 Momentum = trackPoint->GetMomentum();
//       double Edep = trackPoint->GetEdep();
//       double StepLength = trackPoint->GetStepLength();
//       double StepDeltaLyz = trackPoint->GetStepDeltaLyz();
//       string PhysVolName = trackPoint->GetPhysVolName();
//       std::string to_replace = "/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1";
//       if (PhysVolName.find(to_replace) != std::string::npos)
//         PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"~");
//       to_replace = "RepY/RepX/RepZ";
//       if (PhysVolName.find(to_replace) != std::string::npos)
//         PhysVolName.replace(PhysVolName.find(to_replace),to_replace.length(),"RXYZ");
//       std::string PreProcessName = trackPoint->GetPreProcessName();
//       std::string PostProcessName = trackPoint->GetPostProcessName();
//       std::string MaterialName = trackPoint->GetMaterialName();
//       int PointID = trackPoint->GetPointID();
//       bool IsOnBoundary = trackPoint->IsOnBoundary();
//       double dLpt = (PostPosition - PrePosition).Mag();

//       if (0) // do not print
//       {
//         std::cout << std::setprecision(5) << std::fixed << std::showpoint
//                   << std::setw(5) << "  Tp:" << PointID
//                   << std::setw(11) << " |M|:" << Momentum.Mag()
//                   << std::setw(11) << " dE:" << Edep
//                   << std::setw(11) << " dL:" << StepLength
//                   << std::setw(11) << " dLpt:" << dLpt
//                     << std::setw(20) << " " << PreProcessName << " "
//                     << std::setw(20) << PostProcessName << " "
//                     << std::setw(20) << MaterialName
//                   << std::setw(2) << " Bnd:" << IsOnBoundary
//                   //  << " X:" << PrePosition.X()
//                   //   << " Y:" << PrePosition.Y()
//                     << std::setw(8) << " Z:" << PrePosition.Z()
//                   //  << " X:" << PostPosition.X()
//                   //   << " Y:" << PostPosition.Y()
//                     << std::setw(8) << " Z:" << PostPosition.Z()
//                     << std::setw(8) << " avgZ:" << zAvgPos
//                   << " " << PhysVolName
//                   << std::endl;
//       }

//       dL_over_points += dLpt;
//       dL_over_points2 += StepLength;
//       dE_over_pts += Edep;

//       double new_x = (AvgPos.X() + 960.0) / 10.0;
//       // double new_x = (- (AvgPos.X() - 960.0)) / 10.0; // right coordinate system
//       double new_y = (AvgPos.Y() + 280.0) / 10.0;
//       double new_z = (AvgPos.Z() + 920.0) / 10.0;
//       // std::cout << "New coords: " << new_x << " " << new_y << " " << new_z << std::endl;

//       h2_event_display_edep_xy->Fill(new_y, new_x, Edep);
//       // h2_event_display_edep_xz->Fill(new_x, new_z, Edep);
//       h2_event_display_edep_xz->Fill(new_z, new_x, Edep);
//       h2_event_display_edep_yz->Fill(new_z, new_y, Edep);
      
//       for (int i = 0; i < 184; i++)
//       {
//         double z0 = i * 10.0 - 920.0;
//         double z1 = (i+1) * 10.0 - 920.0;
//         if (zAvgPos >= z0 && zAvgPos < z1)
//         {
//           zLayersEdep[{i, z0, z1}] += Edep;
//         }
//       }
      
//       double avg_pt_x_pos_cm = AvgPos.X() / 10.0;
//       double avg_pt_y_pos_cm = AvgPos.Y() / 10.0;
//       double avg_pt_z_pos_cm = AvgPos.Z() / 10.0;

//       double x_pos_cm = trunc(avg_pt_x_pos_cm);
//       double y_pos_cm = trunc(avg_pt_y_pos_cm);
//       double z_pos_cm = trunc(avg_pt_z_pos_cm);

//       std::tuple<double, double, double, double, double, double> cubePos{
//         x_pos_cm, x_pos_cm+1.0,
//         y_pos_cm, y_pos_cm+1.0,
//         z_pos_cm, z_pos_cm+1.0,
//       };

//       if (Edep > 0)
//       {
//         if (edepAtCubes.find(cubePos) != edepAtCubes.end())
//         {
//           std::tuple<int, double> prev_cube_edep = edepAtCubes[cubePos];
//           std::tuple<int, double> new_cube_edep{std::get<0>(prev_cube_edep), std::get<1>(prev_cube_edep) + Edep};
//           edepAtCubes[cubePos] = new_cube_edep;
//           // edepAtCubes[cubePos] += Edep;
//         } else {
//           std::tuple<int, double> new_cube_edep{cube_idx, Edep};
//           edepAtCubes.insert({cubePos, new_cube_edep});
//           // edepAtCubes.insert({cubePos, Edep});
//           cube_idx++;
//         }
//       }      
//     }

//     /// fill canvas or clear it
//     std::string canvas_title = "PDG: " + std::to_string(main_particle_PDG)
//                                  + ", initial Ekin " + std::to_string(main_particle_InitKinEnergy)
//                                  + ", display of event " + std::to_string(eventId);
//     std::string out_fname_event_display = output_dir_tag
//           + "/event_display/PDG" + std::to_string(main_particle_PDG)
//           + "-event_display" + std::to_string(eventId) + ".pdf";
//     createEventDisplay(
//       c_event_display,
//       canvas_title,
//       h2_event_display_edep_xy,
//       h2_event_display_edep_yz,
//       h2_event_display_edep_xz,
//       true,
//       out_fname_event_display
//     );
//     /*{
//       std::string canvas_title = "PDG: " + std::to_string(main_particle_PDG)
//                                  + ", initial Ekin " + std::to_string(main_particle_InitKinEnergy)
//                                  + ", display of event " + std::to_string(eventId);
      
//       h2_event_display_edep_xy->SetTitle(";Y, cm;X, cm");
//       h2_event_display_edep_yz->SetTitle(";Z, cm;Y, cm");
//       h2_event_display_edep_xz->SetTitle("");

//       h2_event_display_edep_xz->SetStats(0);
//       h2_event_display_edep_xy->SetStats(0);
//       h2_event_display_edep_yz->SetStats(0);

//       // h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.07);
//       // h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.07);
//       // h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.07);

//       h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.05);
//       h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.05);
//       h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.05);

//       h2_event_display_edep_yz->GetXaxis()->SetLabelSize(0.07);
//       h2_event_display_edep_yz->GetYaxis()->SetLabelSize(0.07);
//       h2_event_display_edep_yz->GetZaxis()->SetLabelSize(0.07);

//       h2_event_display_edep_xz->GetXaxis()->SetLabelSize(0.0);
//       h2_event_display_edep_xz->GetYaxis()->SetLabelSize(0.0);
//       h2_event_display_edep_xz->GetZaxis()->SetLabelSize(0.02);

//       // h2_event_display_edep_xz->SetTitleSize(0.07);
//       h2_event_display_edep_yz->SetTitleSize(0.1, "X");
//       h2_event_display_edep_yz->SetTitleSize(0.1, "Y");
//       h2_event_display_edep_yz->SetTitleSize(0.1, "Z");
//       // h2_event_display_edep_yz->SetTitleSize(0.07);
//       // h2_event_display_edep_xy->SetTitleSize(0.07);
//       h2_event_display_edep_xy->SetTitleSize(0.07, "X");
//       h2_event_display_edep_xy->SetTitleSize(0.07, "Y");

//       // h2_event_display_edep_xz->SetTitleFont(200);
//       // h2_event_display_edep_yz->SetTitleFont(200);
//       // h2_event_display_edep_xy->SetTitleFont(200);

//       // gStyle->SetTitleFontSize(0.3);
      
//       c_event_display->cd();

//       double yz_xz_start_xy_stop_x = 0.3;
//       double xy_xz_start_yz_stop_y = 0.22;

//       TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
//       TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
//       TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);

//       display_pad_xz->Draw(); // top z -- lowest
//       display_pad_xy->Draw(); // top z -- middle
//       display_pad_yz->Draw(); // top z -- highest

//       TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
//       pt->AddText(canvas_title.c_str());
//       pt->SetTextSize(0.03);
//       pt->Draw();

//       // display_pad_xy->SetFillColor(kCyan);
//       // display_pad_xz->SetFillColor(kYellow-10);
//       // display_pad_yz->SetFillColor(kGreen-10);

//       display_pad_xz->SetLeftMargin(0.03);
//       // display_pad_xz->SetRightMargin(0);
//       display_pad_xz->SetTopMargin(0);
//       // display_pad_xz->SetBottomMargin(0.03);
//       display_pad_xz->SetBottomMargin(0.0);

//       display_pad_xy->SetLeftMargin(0.2);
//       // display_pad_xy->SetRightMargin(0.2);
//       display_pad_xy->SetRightMargin(0.15);
//       display_pad_xy->SetTopMargin(0);
//       // display_pad_xy->SetBottomMargin(0.03);
//       // display_pad_xy->SetBottomMargin(0.08);
//       display_pad_xy->SetBottomMargin(0.12);
//       // display_pad_xy->SetBottomMargin(0.15);
      

//       display_pad_yz->SetLeftMargin(0.03);
//       // display_pad_yz->SetRightMargin(0);
//       display_pad_yz->SetTopMargin(0);
//       // display_pad_yz->SetBottomMargin(0.1);
//       // display_pad_yz->SetBottomMargin(0.3);
//       display_pad_yz->SetBottomMargin(0.2);

//       display_pad_xz->cd();
//       h2_event_display_edep_xz->Draw("COLZ");
      
//       display_pad_xy->cd();
//       h2_event_display_edep_xy->Draw("COLZ");

//       display_pad_yz->cd();
//       h2_event_display_edep_yz->Draw("COLZ");

//       // if (1)
//       {
//         std::string out_fname_event_display = output_dir_tag
//           + "/event_display/PDG" + std::to_string(main_particle_PDG)
//           + "-event_display" + std::to_string(eventId) + ".pdf";
//         c_event_display->SaveAs(out_fname_event_display.c_str());
//       }
      
      
//       /// clear canvas and histograms
//       h2_event_display_edep_xy->Reset("ICESM");
//       h2_event_display_edep_xz->Reset("ICESM");
//       h2_event_display_edep_yz->Reset("ICESM");
//       c_event_display->Clear();
//       // display_pad->Clear();
//       // ttl_pad->Clear();
//       // delete ttl_tex;
//       // delete display_pad;
//       // delete ttl_pad;
//     } // */

//     std::cout << "  == dL_over_pts:" << dL_over_points
//               << " dL_over_pts2:" << dL_over_points2
//               << " R:" << primary_track-> GetRange()
//               << " Ltgt1:" << primary_track->GetLengthTarget1()
//               << " dL_SD:" << primary_track->GetSDLength() // not used in simulation
//               << std::endl;
//     std::cout << "  == dE_over_pts:" << dE_over_pts
//               << " dEtgt1:" << primary_track->GetEdepTarget1()
//               << " dE_SD:" << primary_track->GetSDTotalEnergyDeposit() // do not consider
//               << std::endl;

//     double totEdep = 0.0;
//     for (int i = 0; i < 184; i++)
//     {
//       double z0 = i * 10.0 - 920.0;
//       double z1 = (i+1) * 10.0 - 920.0;
//       std::cout << " >>> [" << i << " " << z0 << " " << z1 << "] " << zLayersEdep.at({i, z0, z1}) << std::endl;
//       totEdep += zLayersEdep.at({i, z0, z1});
//       // total_edep->Fill(totEdep);
//       // total_edep_with_gamma
//       if (zLayersEdep.at({i, z0, z1}) > 0.0)
//       {
//         h_edep_range_2d_hist->Fill(z0 / 10.0+92.0, zLayersEdep.at({i, z0, z1}));
//       }

//       double edep = zLayersEdep.at({i, z0, z1});
//       if (edep > 0.0)
//       {
//         switch (i)
//         {
//           case 10-1: h_edep_at_10cm->Fill(edep); break;
//           case 15-1: h_edep_at_15cm->Fill(edep); break;
//           case 20-1: h_edep_at_20cm->Fill(edep); break;
//           case 30-1: h_edep_at_30cm->Fill(edep); break;
//           case 40-1: h_edep_at_40cm->Fill(edep); break;
//           case 50-1: h_edep_at_50cm->Fill(edep); break;
//           default: break;
//         }
//       }
//     }
  
//     // std::map<int, double> edepAtCubesPerTrack;
//     for (auto& it: edepAtCubes)
//     {
//       std::tuple<double, double, double, double, double, double> cubePos = it.first;
//       // double edep = it.second;
//       std::tuple<int, double> edep = it.second;
//       std::cout << std::get<0>(edep) << " "
//                 << "[ "
//                   << "[" << std::get<0>(cubePos) << ", " << std::get<1>(cubePos) << "] "
//                   << "[" << std::get<2>(cubePos) << ", " << std::get<3>(cubePos) << "] "
//                   << "[" << std::get<4>(cubePos) << ", " << std::get<5>(cubePos) << "] "
//                 << "] "
//                 // << edep
//                 << std::get<1>(edep)
//                 << std::endl;

//       // double z0 = std::get<4>(cubePos);
//       // double z1 = std::get<5>(cubePos);
//       // edepAtCubesPerTrack.insert(edep);

//       double dE = std::get<1>(edep);
//       if (dE > 0.0)
//       {
//         switch (std::get<0>(edep))
//         {
//           case 10-1: h_edep_at_10cm_cube->Fill(dE); break;
//           case 15-1: h_edep_at_15cm_cube->Fill(dE); break;
//           case 20-1: h_edep_at_20cm_cube->Fill(dE); break;
//           case 30-1: h_edep_at_30cm_cube->Fill(dE); break;
//           case 40-1: h_edep_at_40cm_cube->Fill(dE); break;
//           case 50-1: h_edep_at_50cm_cube->Fill(dE); break;
//           default: break;
//         }
//       }
//     }

//     total_edep->Fill(totEdep);
//     // total_edep_with_gamma->Fill(totEdep+energies_of_secondary_photons);
//     // total_edep_with_gamma->Fill(totEdep+total_edep_of_gammas);
//     // h_total_edep_of_gammas->Fill(totEdep+total_edep_of_gammas);

//     // std::cout << " >>> " << totEdep << std::endl;
//     std::cout << " >>> " << totEdep << " " << energies_of_secondary_photons << " " << total_edep_of_all_tracks << std::endl;
//   }

//   std::string out_fname_total_edep = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep.pdf";
//   TCanvas* c_total_edep = new TCanvas("total_edep", "Total Energy deposit;dE/dx, MeV;N events", 1080, 720);
//   c_total_edep->cd();
//   total_edep->Draw();
//   total_edep->SetFillColor(kRed);
//   c_total_edep->SaveAs(out_fname_total_edep.c_str());
//   c_total_edep->Close();

//   // std::string out_fname_total_edep_wgamma = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep_wgamma.pdf";
//   // TCanvas* c_total_edep_wgamma = new TCanvas("total_edep_wgamma", "Total Energy deposit (with gamma);dE/dx, MeV;N events", 1080, 720);
//   // c_total_edep_wgamma->cd();
//   // total_edep_with_gamma->Draw();
//   // total_edep_with_gamma->SetFillColor(kRed);
//   // c_total_edep_wgamma->SaveAs(out_fname_total_edep_wgamma.c_str());
//   // c_total_edep_wgamma->Close();

//   std::string out_fname_total_all_trks = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-total_edep_all_tracks.pdf";
//   TCanvas* c_total_edep_all_tracks = new TCanvas("h_total_edep_all_tracks", "Total Energy deposit by all tracks;dE/dx, MeV;N events", 1080, 720);
//   c_total_edep_all_tracks->cd();
//   h_total_edep_all_tracks->Draw();
//   h_total_edep_all_tracks->SetFillColor(kRed);
//   c_total_edep_all_tracks->SaveAs(out_fname_total_all_trks.c_str());
//   c_total_edep_all_tracks->Close();

//   std::string out_fname_edep_range_2d = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_to_range_2d.pdf";
//   TCanvas* c_edep_range_2d_hist = new TCanvas("h_edep_range_2d_hist", "Energy deposit to range;Range, cm;Edep, MeV", 1080, 720);
//   c_edep_range_2d_hist->cd();
//   h_edep_range_2d_hist->Draw("COLZ");
//   c_edep_range_2d_hist->SaveAs(out_fname_edep_range_2d.c_str());
//   c_edep_range_2d_hist->Close();

//   std::string out_fname_edep_slices = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices.pdf";
//   TCanvas* c_edep_slices = new TCanvas("h_edep_slices", "Energy deposit slices", 1080, 720);
//   c_edep_slices->Divide(2,3);
//   c_edep_slices->cd(1);
//   gPad->SetLogy();
//   h_edep_at_10cm->Draw();
//   h_edep_at_10cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->cd(2);
//   gPad->SetLogy();
//   h_edep_at_15cm->Draw();
//   h_edep_at_15cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->cd(3);
//   gPad->SetLogy();
//   h_edep_at_20cm->Draw();
//   h_edep_at_20cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->cd(4);
//   gPad->SetLogy();
//   h_edep_at_30cm->Draw();
//   h_edep_at_30cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->cd(5);
//   gPad->SetLogy();
//   h_edep_at_40cm->Draw();
//   h_edep_at_40cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->cd(6);
//   gPad->SetLogy();
//   h_edep_at_50cm->Draw();
//   h_edep_at_50cm->SetFillColor(kRed);
//   // c_edep_slices->SetLogy();
//   c_edep_slices->SaveAs(out_fname_edep_slices.c_str());
//   c_edep_slices->Close();


//   std::string out_fname_edep_slices_per_cube = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube.pdf";
//   TCanvas* c_edep_slices_per_cube = new TCanvas("h_edep_slices_per_cube", "Energy deposit slices (per cube)", 1080, 720);
//   c_edep_slices_per_cube->Divide(2,3);
//   c_edep_slices_per_cube->cd(1);
//   gPad->SetLogy();
//   h_edep_at_10cm_cube->Draw();
//   h_edep_at_10cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->cd(2);
//   gPad->SetLogy();
//   h_edep_at_15cm_cube->Draw();
//   h_edep_at_15cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->cd(3);
//   gPad->SetLogy();
//   h_edep_at_20cm_cube->Draw();
//   h_edep_at_20cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->cd(4);
//   gPad->SetLogy();
//   h_edep_at_30cm_cube->Draw();
//   h_edep_at_30cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->cd(5);
//   gPad->SetLogy();
//   h_edep_at_40cm_cube->Draw();
//   h_edep_at_40cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->cd(6);
//   gPad->SetLogy();
//   h_edep_at_50cm_cube->Draw();
//   h_edep_at_50cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube->SaveAs(out_fname_edep_slices_per_cube.c_str());
//   c_edep_slices_per_cube->Close();


//   std::string out_fname_edep_slices_per_cube_vs_zL_1 = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube_vs_zL-1.pdf";
//   TCanvas* c_edep_slices_per_cube_vs_zL_1 = new TCanvas("h_edep_slices_per_cube_vs_zL_1", "Energy deposit slices (per cube vs per zLayer)", 1080, 720);
//   c_edep_slices_per_cube_vs_zL_1->Divide(2,3);
//   c_edep_slices_per_cube_vs_zL_1->cd(1);
//   gPad->SetLogy();
//   h_edep_at_10cm->Draw();
//   h_edep_at_10cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->cd(3);
//   gPad->SetLogy();
//   h_edep_at_15cm->Draw();
//   h_edep_at_15cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->cd(5);
//   gPad->SetLogy();
//   h_edep_at_20cm->Draw();
//   h_edep_at_20cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->cd(2);
//   gPad->SetLogy();
//   h_edep_at_10cm_cube->Draw();
//   h_edep_at_10cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->cd(4);
//   gPad->SetLogy();
//   h_edep_at_15cm_cube->Draw();
//   h_edep_at_15cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->cd(6);
//   gPad->SetLogy();
//   h_edep_at_20cm_cube->Draw();
//   h_edep_at_20cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_1->SaveAs(out_fname_edep_slices_per_cube_vs_zL_1.c_str());
//   c_edep_slices_per_cube_vs_zL_1->Close();

//   std::string out_fname_edep_slices_per_cube_vs_zL_2 = output_dir_tag + "/PDG" + std::to_string(main_particle_PDG) + "-edep_slices-per_cube_vs_zL-2.pdf";
//   TCanvas* c_edep_slices_per_cube_vs_zL_2 = new TCanvas("h_edep_slices_per_cube_vs_zL_2", "Energy deposit slices (per cube vs per zLayer)", 1080, 720);
//   c_edep_slices_per_cube_vs_zL_2->Divide(2,3);
//   c_edep_slices_per_cube_vs_zL_2->cd(1);
//   gPad->SetLogy();
//   h_edep_at_30cm->Draw();
//   h_edep_at_30cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->cd(3);
//   gPad->SetLogy();
//   h_edep_at_40cm->Draw();
//   h_edep_at_40cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->cd(5);
//   gPad->SetLogy();
//   h_edep_at_50cm->Draw();
//   h_edep_at_50cm->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->cd(2);
//   gPad->SetLogy();
//   h_edep_at_30cm_cube->Draw();
//   h_edep_at_30cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->cd(4);
//   gPad->SetLogy();
//   h_edep_at_40cm_cube->Draw();
//   h_edep_at_40cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->cd(6);
//   gPad->SetLogy();
//   h_edep_at_50cm_cube->Draw();
//   h_edep_at_50cm_cube->SetFillColor(kRed);
//   c_edep_slices_per_cube_vs_zL_2->SaveAs(out_fname_edep_slices_per_cube_vs_zL_2.c_str());
//   c_edep_slices_per_cube_vs_zL_2->Close();

// }