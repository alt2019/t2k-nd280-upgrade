

#include "utils.hh"



bool isPtInsideSFGD(G4String detname)
{
  if (
    detname.contains("/t2k/OA/Magnet/Basket/target1/TargetUniform") ||
    detname.contains("/t2k/OA/Magnet/Basket/target1/SuperFGD1") || 
    detname.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") ||
    detname.contains("/t2k/OA/Magnet/Basket/target1/CFBox1/SuperFGD1")
  ) return true;

  return false;
}



std::string getDetectorPlaneOfPoint(TVector3 pt)
{
  std::string left_SFGD_plane = "-";
  // std::cout << pt.X() << " " << pt.Y() << " " << pt.Z() << std::endl;
  if      (pt.X() >=  960.0     ) left_SFGD_plane = "R"; // right YZ+X
  else if (pt.X() <= -960.0     ) left_SFGD_plane = "L"; // left YZ-X
  else if (pt.Z() <= -920.0     ) left_SFGD_plane = "BWD"; // backward XY+Z
  else if (pt.Z() >=  920.0     ) left_SFGD_plane = "FWD"; // forward XY-Z
  // else if (pt.Y() >=  280.0+16.0) left_SFGD_plane = "T"; // top XZ+Y
  // else if (pt.Y() <= -280.0+16.0) left_SFGD_plane = "B"; // bottom XZ-Y
  else if (pt.Y() >=  280.0) left_SFGD_plane = "T"; // top XZ+Y
  else if (pt.Y() <= -280.0) left_SFGD_plane = "B"; // bottom XZ-Y
  // std::cout << pt.X() << " " << pt.Y() << " " << pt.Z() << " " << left_SFGD_plane << std::endl;

  return left_SFGD_plane;
}

std::string energy_best_units(double energy/*MeV*/)
{
  std::stringstream ss;
  if (energy >= 1000.0)
  {
    ss << std::setprecision(6) << energy / 1000.0 << "_GeV";
  }
  else if (energy >= 0.001 && energy < 1.0)
  {
    ss << std::setprecision(6) << energy * 1000.0 << "_KeV";
  }
  else if (energy >= 0.000001 && energy < 0.001)
  {
    ss << std::setprecision(6) << energy * 1000000.0 << "_eV";
  }
  else
  {
    ss << std::setprecision(6) << energy << "_MeV";
  }

  return ss.str();
}

TVector3 toSFGDCoordinateSystem(TVector3 pt)
{
  // double new_x = (- (pt.X() - 960.0)) / 10.0; // right coordinate system
  double new_x = (pt.X() + 960.0) / 10.0; // by default geant4 has right coordinate system
  double new_y = (pt.Y() + 280.0) / 10.0;
  double new_z = (pt.Z() + 920.0) / 10.0;
  return TVector3(new_x, new_y, new_z);
}


bool isPtInSFGDFV(TVector3 pt_in_sfgd_cs)
{
  if (
         (pt_in_sfgd_cs.X() <= 192.0 - 2.0 && pt_in_sfgd_cs.X() >= 0.0 + 2.0)
      && (pt_in_sfgd_cs.Y() <= 56.0 - 2.0 && pt_in_sfgd_cs.Y() >= 0.0 + 2.0)
      && (pt_in_sfgd_cs.Z() <= 184.0 - 2.0 && pt_in_sfgd_cs.Z() >= 0.0 + 2.0)
  ) return true;

  return false;
}


void createEventDisplay(
    TCanvas* c_event_display,
    std::string canvas_title,
    TH2F* h2_event_display_edep_xy,
    TH2F* h2_event_display_edep_yz,
    TH2F* h2_event_display_edep_xz,
    bool save,
    std::string out_fname
  ) // TODO: check for memory leaks
{
  h2_event_display_edep_xy->SetTitle(";Y, cm;X, cm");
  h2_event_display_edep_yz->SetTitle(";Z, cm;Y, cm");
  h2_event_display_edep_xz->SetTitle("");

  h2_event_display_edep_xz->SetStats(0);
  h2_event_display_edep_xy->SetStats(0);
  h2_event_display_edep_yz->SetStats(0);

  h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.05);
  h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.05);
  h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.05);

  h2_event_display_edep_yz->GetXaxis()->SetLabelSize(0.07);
  h2_event_display_edep_yz->GetYaxis()->SetLabelSize(0.07);
  h2_event_display_edep_yz->GetZaxis()->SetLabelSize(0.07);

  h2_event_display_edep_xz->GetXaxis()->SetLabelSize(0.0);
  h2_event_display_edep_xz->GetYaxis()->SetLabelSize(0.0);
  h2_event_display_edep_xz->GetZaxis()->SetLabelSize(0.02);

  h2_event_display_edep_yz->SetTitleSize(0.1, "X");
  h2_event_display_edep_yz->SetTitleSize(0.1, "Y");
  h2_event_display_edep_yz->SetTitleSize(0.1, "Z");

  h2_event_display_edep_xy->SetTitleSize(0.07, "X");
  h2_event_display_edep_xy->SetTitleSize(0.07, "Y");
  
  c_event_display->cd();

  double yz_xz_start_xy_stop_x = 0.3;
  double xy_xz_start_yz_stop_y = 0.22;
  TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
  TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
  TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);
  display_pad_xz->Draw(); // top z -- lowest
  display_pad_xy->Draw(); // top z -- middle
  display_pad_yz->Draw(); // top z -- highest

  TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
  pt->AddText(canvas_title.c_str());
  pt->SetTextSize(0.03);
  pt->Draw();

  /// set color for pads (for debugging)
  // display_pad_xy->SetFillColor(kCyan);
  // display_pad_xz->SetFillColor(kYellow-10);
  // display_pad_yz->SetFillColor(kGreen-10);

  /// set xz pad borders
  display_pad_xz->SetLeftMargin(0.03);
  // display_pad_xz->SetRightMargin(0);
  display_pad_xz->SetTopMargin(0);
  display_pad_xz->SetBottomMargin(0.0);

  /// set xy pad borders
  display_pad_xy->SetLeftMargin(0.2);
  // display_pad_xy->SetRightMargin(0.2);
  display_pad_xy->SetRightMargin(0.15);
  display_pad_xy->SetTopMargin(0);
  display_pad_xy->SetBottomMargin(0.12);
  // display_pad_xy->SetBottomMargin(0.15);
  
  /// set yz pad borders
  display_pad_yz->SetLeftMargin(0.03);
  // display_pad_yz->SetRightMargin(0);
  display_pad_yz->SetTopMargin(0);
  display_pad_yz->SetBottomMargin(0.2);

  /// draw xz
  display_pad_xz->cd();
  h2_event_display_edep_xz->Draw("COLZ");
  /// draw xy
  display_pad_xy->cd();
  h2_event_display_edep_xy->Draw("COLZ");
  /// draw yz
  display_pad_yz->cd();
  h2_event_display_edep_yz->Draw("COLZ");

  if (save)
  {
    c_event_display->SaveAs(out_fname.c_str());
  }
  
  
  /// clear canvas and histograms
  delete pt;
  display_pad_xz->Clear(); delete display_pad_xz;
  display_pad_xy->Clear(); delete display_pad_xy;
  display_pad_yz->Clear(); delete display_pad_yz;
  h2_event_display_edep_xy->Reset("ICESM");
  h2_event_display_edep_xz->Reset("ICESM");
  h2_event_display_edep_yz->Reset("ICESM");
  c_event_display->Clear();
}


void createEventDisplay(
    TCanvas* c_event_display,
    std::string canvas_title,
    TH2F* h2_event_display_edep_xy,
    TH2F* h2_event_display_edep_yz,
    TH2F* h2_event_display_edep_xz
  )
{
  h2_event_display_edep_xy->SetTitle(";Y, cm;X, cm");
  h2_event_display_edep_yz->SetTitle(";Z, cm;Y, cm");
  h2_event_display_edep_xz->SetTitle("");

  h2_event_display_edep_xz->SetStats(0);
  h2_event_display_edep_xy->SetStats(0);
  h2_event_display_edep_yz->SetStats(0);

  h2_event_display_edep_xy->GetXaxis()->SetLabelSize(0.05);
  h2_event_display_edep_xy->GetYaxis()->SetLabelSize(0.05);
  h2_event_display_edep_xy->GetZaxis()->SetLabelSize(0.05);

  h2_event_display_edep_yz->GetXaxis()->SetLabelSize(0.07);
  h2_event_display_edep_yz->GetYaxis()->SetLabelSize(0.07);
  h2_event_display_edep_yz->GetZaxis()->SetLabelSize(0.07);

  h2_event_display_edep_xz->GetXaxis()->SetLabelSize(0.0);
  h2_event_display_edep_xz->GetYaxis()->SetLabelSize(0.0);
  h2_event_display_edep_xz->GetZaxis()->SetLabelSize(0.02);

  h2_event_display_edep_yz->SetTitleSize(0.1, "X");
  h2_event_display_edep_yz->SetTitleSize(0.1, "Y");
  h2_event_display_edep_yz->SetTitleSize(0.1, "Z");

  h2_event_display_edep_xy->SetTitleSize(0.07, "X");
  h2_event_display_edep_xy->SetTitleSize(0.07, "Y");
  
  c_event_display->cd();

  double yz_xz_start_xy_stop_x = 0.3;
  double xy_xz_start_yz_stop_y = 0.22;
  TPad *display_pad_xz = new TPad("display_pad_xz", "display_pad_xz", yz_xz_start_xy_stop_x-0.02, xy_xz_start_yz_stop_y, 0.99, 0.89);
  TPad *display_pad_xy = new TPad("display_pad_xy", "display_pad_xy", 0.01, 0.13, yz_xz_start_xy_stop_x, 0.89);
  TPad *display_pad_yz = new TPad("display_pad_yz", "display_pad_yz", yz_xz_start_xy_stop_x-0.02, 0.01, 0.99, xy_xz_start_yz_stop_y);
  display_pad_xz->Draw(); // top z -- lowest
  display_pad_xy->Draw(); // top z -- middle
  display_pad_yz->Draw(); // top z -- highest

  TPaveText * pt = new TPaveText(0.01, 0.9, 0.99, 0.99);
  pt->AddText(canvas_title.c_str());
  pt->SetTextSize(0.03);
  pt->Draw();

  /// set color for pads (for debugging)
  // display_pad_xy->SetFillColor(kCyan);
  // display_pad_xz->SetFillColor(kYellow-10);
  // display_pad_yz->SetFillColor(kGreen-10);

  /// set xz pad borders
  display_pad_xz->SetLeftMargin(0.03);
  // display_pad_xz->SetRightMargin(0);
  display_pad_xz->SetTopMargin(0);
  display_pad_xz->SetBottomMargin(0.0);

  /// set xy pad borders
  display_pad_xy->SetLeftMargin(0.2);
  // display_pad_xy->SetRightMargin(0.2);
  display_pad_xy->SetRightMargin(0.15);
  display_pad_xy->SetTopMargin(0);
  display_pad_xy->SetBottomMargin(0.12);
  // display_pad_xy->SetBottomMargin(0.15);
  
  /// set yz pad borders
  display_pad_yz->SetLeftMargin(0.03);
  // display_pad_yz->SetRightMargin(0);
  display_pad_yz->SetTopMargin(0);
  display_pad_yz->SetBottomMargin(0.2);

  /// draw xz
  display_pad_xz->cd();
  h2_event_display_edep_xz->Draw("COLZ");
  /// draw xy
  display_pad_xy->cd();
  h2_event_display_edep_xy->Draw("COLZ");
  /// draw yz
  display_pad_yz->cd();
  h2_event_display_edep_yz->Draw("COLZ");
}


int particleToROOTColor(int PDG)
{
  int color = 0;
  switch (PDG)
  {
    case 11:
    {
      color = kRed;
      break;
    }
    case -11:
    {
      color = kBlue;
      break;
    }
    case 13:
    {
      color = kGreen;
      break;
    }
    case -13:
    {
      color = kCyan;
      break;
    }
    case 22:
    {
      // color = kYellow;
      // color = kGreen + 4;
      color = kGreen - 9;
      break;
    }
    case 2212:
    {
      color = kMagenta;
      break;
    }
    case 2112:
    {
      color = kMagenta-10;
      break;
    }
    case 211:
    {
      color = kAzure-4;
      break;
    }
    case -211:
    {
      color = kTeal+3;
      break;
    }
    case 111:
    {
      color = kOrange-2;
      break;
    }
    case -111:
    {
      color = kOrange+8;
      break;
    }
    default:
    {
      color = kBlack;
      break;
    }
  }
  return color;
}


// void process_mem_usage(double& vm_usage, double& resident_set)
// {
//    using std::ios_base;
//    using std::ifstream;
//    using std::string;

//    vm_usage     = 0.0;
//    resident_set = 0.0;

//    // 'file' stat seems to give the most reliable results
//    //
//    ifstream stat_stream("/proc/self/stat",ios_base::in);

//    // dummy vars for leading entries in stat that we don't care about
//    //
//    string pid, comm, state, ppid, pgrp, session, tty_nr;
//    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
//    string utime, stime, cutime, cstime, priority, nice;
//    string O, itrealvalue, starttime;

//    // the two fields we want
//    //
//    unsigned long vsize;
//    long rss;

//    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
//                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
//                >> utime >> stime >> cutime >> cstime >> priority >> nice
//                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

//    stat_stream.close();

//    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
//    vm_usage     = vsize / 1024.0;
//    resident_set = rss * page_size_kb;
// }


void process_mem_usage(double& vm_usage, double& resident_set)
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  resident_set = usage.ru_maxrss;
  vm_usage = usage.ru_maxrss + usage.ru_ixrss;

    // std::cout << "CPU time used: " << usage.ru_utime.tv_sec << " seconds "
    //           << usage.ru_utime.tv_usec << " microseconds" << std::endl;
    // std::cout << "Memory used: " << usage.ru_maxrss << " kilobytes" << std::endl;

}