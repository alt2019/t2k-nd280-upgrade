

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

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"

#include "src/SVG.hh"


int main(int argc, char **argv)
{
  xmlns::XMLObject* xo = new xmlns::XMLObject("test", "new");
  std::string xo_s = xo->render(2);
  std::cout << xo_s << std::endl;

  return 0;
}