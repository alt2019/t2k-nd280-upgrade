#include "AnaTreeConverterEvent.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "Units.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "PIDUtils.hxx"
//********************************************************************
AnaTreeConverterEvent::AnaTreeConverterEvent():InputConverter("ND280upEvents"){
  //********************************************************************

  _spill = NULL;

  _isMC = false;
  _softwareVersion = "";

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;
  hefficiency_target=NULL;

}

//********************************************************************
bool AnaTreeConverterEvent::Initialize(){
  //********************************************************************
  AddChain("ND280upEvents");

  ND280upEvents = GetChain("ND280upEvents");

  fChain = ND280upEvents;

  // Set branch addresses and branch pointers
  nd280UpEvent = new TND280UpEvent();
  if (!ND280upEvents) {
    std::cout<<"non fChain"<<std::endl;
    return false;}
  fCurrent = -1;
  //-------------- Header ---------------------------------
  if (ND280upEvents) {
    ND280upEvents->SetBranchAddress("Event", &nd280UpEvent);;
  }

  fefficiency_target = new TFile("$HIGHLANDIOROOT/data/target_recon_efficiency.root");
  hefficiency_target = (TH2D*)fefficiency_target->Get("theta_length_Eff");

  _ECal_reco_eff = new BinnedParams(std::string(getenv("HIGHLANDIOROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("HIGHLANDIOROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC);
  
  _randomGen = new TRandom3();
  
  return true;
}

//********************************************************************
AnaTreeConverterEvent::~AnaTreeConverterEvent(){
  //********************************************************************

  if (!fChain) return;
  delete nd280UpEvent;
  if (ND280upEvents         ) delete   ND280upEvents          ->GetCurrentFile();
  if(hefficiency_target) delete hefficiency_target;
 
  if(fefficiency_target) delete fefficiency_target;
  
}

//****************************************************************************
bool AnaTreeConverterEvent::AddFileToTChain(const std::string& inputString){
  //****************************************************************************

  std::cout << "AnaTreeConverterEvent::AddFileToTChain(). Adding file: " << inputString << std::endl;


  // Chain only the directories we are interested in

  if (ND280upEvents            )          ND280upEvents->AddFile(inputString.c_str());

  // Read one entry from the tree tree such that Run and Subrun are available
  ND280upEvents->GetEntry(ND280upEvents->GetEntries() - 1);


  // The previous attributes
  _previousFile         = inputString;
  _previousRunID        = 0;
  _previousSubrunID     = 0;
  
  
  return 1;
}


//*****************************************************************************
Int_t AnaTreeConverterEvent::ReadEntries(Long64_t& entry) {
  //*****************************************************************************
  
  Int_t entry_temp = ND280upEvents->GetEntry(entry);

  //-------- Increase the cache size to speed up reading Branches ----------
  static bool first = false;
  if (first){
    if( ND280upEvents ) {
      ND280upEvents->SetCacheSize(200000000);
      ND280upEvents->AddBranchToCache("*",kTRUE);
    }
    first=false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t AnaTreeConverterEvent::GetSpill(Long64_t& entry, AnaSpillC*& spill){
  //*****************************************************************************

  static std::string currentfilename("");

  // Read contents of entry (which correspond to one Spill)
  if (!fChain) return 0;

  std::string filename(ND280upEvents->GetFile()->GetName());

  if( filename != currentfilename ) {
    std::cout << " Running on file: " << filename << std::endl;
    currentfilename = filename;
    
    //load geometry 
    //    ND::hgman().LoadGeometry(filename);
  }

  Int_t entry_temp = ReadEntries(entry);

  if (entry_temp > 0) {
    
    // Create an instance of the Spill
    spill = MakeSpill();
    
    // Cast it to AnaSpillB
    _spill = dynamic_cast<AnaSpill*>(spill);
    
    FillInfo(_spill,entry);
  }
  else{
    std::cout << "Failed in reading entry " << entry << std::endl;
  }
  

  entry++;

  if (entry%1000==0 || entry == _nentries)
    std::cout << "entry: " << entry << " of " << _nentries << " (" << (100*entry/_nentries) << "%)" << std::endl;
  return entry_temp;
}

//********************************************************************
void AnaTreeConverterEvent::IncrementPOTBySpill() {
  //********************************************************************
  
}

//*****************************************************************************
void AnaTreeConverterEvent::FillInfo(AnaSpill* spill, Int_t entry){
  //*****************************************************************************
  
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *dynamic_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = 0;
  info.SubRun = 0;
  info.Event  = entry;
  info.IsMC   = 1;

  spill->Beam = MakeBeam();

  
  // beam related information
  FillBeamInfo(dynamic_cast<AnaBeam*>(spill->Beam));


  // True vertex information
  FillTrueInfo(spill);

  AnaBunch* bunch = MakeBunch();
  // All information about each bunch
  FillBunchInfo(spill->TrueVertices, bunch);

  spill->Bunches.push_back(bunch);
}
//*****************************************************************************
void AnaTreeConverterEvent::FillBeamInfo(AnaBeam* beam){
  //*****************************************************************************

  beam->GoodSpill = true;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueInfo(AnaSpill* spill){
  //*****************************************************************************

  // Fill the true vertices vector
  spill->TrueVertices.clear();
  spill->TrueParticles.clear();
  int NTracks = nd280UpEvent->GetNTracks();
  int nParts = std::min((int)NMAXTRUEPARTICLES, NTracks);
  int NVertices = nd280UpEvent->GetNVertices();
  if(NVertices>1) return;

  int nVertices = std::min((int)NMAXTRUEVERTICES, 1);
  TND280UpVertex *nd280UpVertex = dynamic_cast<TND280UpVertex*>(nd280UpEvent->GetVertex(0));

  AnaTrueVertex* trueVertex = MakeTrueVertex();
  FillTrueVertexInfo(nd280UpVertex, trueVertex);
    
  trueVertex->nTrueParticles = 0;

  // allocate a big array. Once it is filled we will resize it
  anaUtils::CreateArray(trueVertex->TrueParticles, NMAXTRUEPARTICLES);

  AnaTrueParticleB* highestMomTrack=0;
  for (int i = 0; i < nd280UpEvent->GetNTracks(); i++) {
    TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(nd280UpEvent->GetTrack(i));

    AnaTrueParticleB* truePart = MakeTrueParticle();

    if (!highestMomTrack ||
	highestMomTrack->Momentum < truePart->Momentum)
      highestMomTrack = truePart;

    if ( (trueVertex->NuPDG == 14 && nd280UpTrack->GetPDG() == 13) ||
	 (trueVertex->NuPDG == 12 && nd280UpTrack->GetPDG() == 11) ||
	 (trueVertex->NuPDG == -14 && nd280UpTrack->GetPDG() == -13) ||
	 (trueVertex->NuPDG == -12 && nd280UpTrack->GetPDG() == -11) ) {
      if(nd280UpTrack->GetInitMom().Mag() > 0){
	//std::cout << "ERROR: LeptonMom is -999 (something wrong in RooTrackerVtx ?), setting it from TruthTrajectoryModule (" << truthTraj->InitMomentum.Vect().Mag() << "), for vertex ID " << true_vertex->ID << std::endl;
	trueVertex->LeptonMom = nd280UpTrack->GetInitMom().Mag();
	trueVertex->LeptonPDG = nd280UpTrack->GetPDG();
	trueVertex->Q2 = - (nd280UpTrack->GetInitMom() - nd280UpVertex->GetInTrack(0)->GetInitMom()).Mag2();
	trueVertex->LeptonMom = nd280UpTrack->GetInitMom().Mag();
           
      }
    } else if (  nd280UpTrack->GetPDG() == 2212 && nd280UpTrack->GetParentID() == 0) {
      // if many, take the first one, that should be the first primary one (not from FSI)
      if (trueVertex->ProtonMom < 0) {
	trueVertex->ProtonMom = nd280UpTrack->GetInitMom().Mag();
	if (trueVertex->ProtonMom > 0)
	  anaUtils::VectorToArray((1. / trueVertex->ProtonMom) * nd280UpTrack->GetInitMom(), trueVertex->ProtonDir);
      }
    } // end if proton
      // If no rooTrackerVtx, fill true pion vars, using the TruthTrajectoryModule (old way)
    else if (  (nd280UpTrack->GetPDG() == 211  || nd280UpTrack->GetPDG() == -211) && nd280UpTrack->GetParentID() == 0) {
      // if many, take the highest momentum one, that should be most likely to be reconstructed
      if (trueVertex->PionMom < nd280UpTrack->GetInitMom().Mag()) {
	trueVertex->PionMom = nd280UpTrack->GetInitMom().Mag();
	if (trueVertex->PionMom > 0)
	  anaUtils::VectorToArray((1. / trueVertex->PionMom) * nd280UpTrack->GetInitMom(), trueVertex->PionDir);
      }
    } // end if pion

    FillTrueParticleInfo(nd280UpVertex, nd280UpTrack, truePart);
    spill->TrueParticles.push_back(truePart);
    truePart->TrueVertex = trueVertex;
    trueVertex->TrueParticlesVect.push_back(truePart);

    int index = ParticleId::GetParticle(nd280UpTrack->GetPDG(),false);
    trueVertex->NPrimaryParticles[index]++;
    if (abs(nd280UpTrack->GetPDG()) > 1000 && abs(nd280UpTrack->GetPDG()) < 10000) trueVertex->NPrimaryParticles[ParticleId::kBaryons]++;
    if (abs(nd280UpTrack->GetPDG()) > 100 && abs(nd280UpTrack->GetPDG()) < 1000) trueVertex->NPrimaryParticles[ParticleId::kMesons]++;
    if (abs(nd280UpTrack->GetPDG()) > 10 && abs(nd280UpTrack->GetPDG()) < 19) trueVertex->NPrimaryParticles[ParticleId::kLeptons]++;
    if (nd280UpTrack->GetPDG() == +12 || nd280UpTrack->GetPDG() == +14 || nd280UpTrack->GetPDG() == +16) trueVertex->NPrimaryParticles[ParticleId::kNeutrinos]++;
    if (nd280UpTrack->GetPDG() == -12 || nd280UpTrack->GetPDG() == -14 || nd280UpTrack->GetPDG() == -16) trueVertex->NPrimaryParticles[ParticleId::kAntiNeutrinos]++;

    //delete nd280UpTrack;
  }

  if (highestMomTrack)
    if (highestMomTrack->DetCrossingsVect.size()>0)
      trueVertex->Detector = highestMomTrack->DetCrossingsVect[0]->Detector;

  trueVertex->NPrimaryParticles[ParticleId::kPions] = trueVertex->NPrimaryParticles[ParticleId::kPi0]   +
    trueVertex->NPrimaryParticles[ParticleId::kPiPos] +
    trueVertex->NPrimaryParticles[ParticleId::kPiNeg] ;
  trueVertex->NPrimaryParticles[ParticleId::kKaons] = trueVertex->NPrimaryParticles[ParticleId::kK0]     +
    trueVertex->NPrimaryParticles[ParticleId::kAntiK0] +
    trueVertex->NPrimaryParticles[ParticleId::kK0L]    +
    trueVertex->NPrimaryParticles[ParticleId::kK0S]    +
    trueVertex->NPrimaryParticles[ParticleId::kKPos]   +
    trueVertex->NPrimaryParticles[ParticleId::kKNeg]   ;
	  
  //delete nd280UpVertex;

  spill->TrueVertices.push_back(trueVertex);

}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueVertexInfo(TND280UpVertex* ndtrueVertex, AnaTrueVertex* trueVertex){
  //*****************************************************************************

  trueVertex->ID       = ndtrueVertex->GetVertexID();
  trueVertex->NuPDG    = ndtrueVertex->GetInTrack(0)->GetPDG();
  trueVertex->NuEnergy = ndtrueVertex->GetInTrack(0)->GetInitKinEnergy();
  trueVertex->NuMom    = ndtrueVertex->GetInTrack(0)->GetInitMom().Mag();
  trueVertex->NuDir[0] = ndtrueVertex->GetInTrack(0)->GetInitMom().X()/trueVertex->NuMom;
  trueVertex->NuDir[1] = ndtrueVertex->GetInTrack(0)->GetInitMom().Y()/trueVertex->NuMom;
  trueVertex->NuDir[2] = ndtrueVertex->GetInTrack(0)->GetInitMom().Z()/trueVertex->NuMom;
  trueVertex->TargetPDG= ndtrueVertex->GetInTrack(1)->GetPDG();

  trueVertex->Position[0] = ndtrueVertex->GetPosition().X(); //*units::mm;
  trueVertex->Position[1] = ndtrueVertex->GetPosition().Y();//*units::mm;
  trueVertex->Position[2] = ndtrueVertex->GetPosition().Z();//*units::mm;
  trueVertex->Position[3] = ndtrueVertex->GetTime();//*units::s;
    
  trueVertex->ReacCode  =ndtrueVertex->GetReacMode();

}


//*****************************************************************************
void AnaTreeConverterEvent::FillBunchInfo(std::vector<AnaTrueVertexB*>& TrueVertices, AnaBunch* bunch){
  //*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
 
  for (std::vector<AnaTrueVertexB*>::const_iterator it = TrueVertices.begin(); it != TrueVertices.end(); ++it) {

    std::vector<AnaTrueParticleB*> trueParticles = (*it)->TrueParticlesVect;
  
    for (std::vector<AnaTrueParticleB*>::const_iterator itp = trueParticles.begin(); itp != trueParticles.end(); ++itp) {
      AnaTrackB* part = NULL;
      if ( fabs( (*itp)->Charge )<1e-3 ) continue;
      if (*itp) {
        AnaTrack* part = dynamic_cast<AnaTrack*> (MakeTrack());

        Fill_Tracks_Recon_From_True((*itp), part);
        bunch->Particles.push_back(part);

      }
    }



    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(*it, vertex, 1);
    vertex->TrueVertex = *it;

    vertex->ParticlesVect = bunch->Particles;
    bunch->Vertices.push_back(vertex);
  }
}



//*****************************************************************************
void AnaTreeConverterEvent::FillVertexInfo(AnaTrueVertexB * truevertex, AnaVertexB* vertex, int bunch){
  //*****************************************************************************

  anaUtils::CopyArray(truevertex->Position,vertex->Position,4);
  vertex->Bunch         = bunch;
  truevertex->Bunch=bunch;

}
//*****************************************************************************
AnaTrueObjectC* AnaTreeConverterEvent::FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles){
  //*****************************************************************************


  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == g4id) return trueParticles[i];
  }

  return NULL;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueParticleInfo(TND280UpVertex* trueVertex, TND280UpTrack* upTrack, AnaTrueParticleB* truePart){
  //*****************************************************************************
  
  if (upTrack == NULL ) {
    truePart->ID = 0;
    truePart->PDG = 0;
    truePart->ParentID = 0;
    anaUtils::VectorToArray(TLorentzVector(0, 0, 0, 0), truePart->Position);
    truePart->Momentum = 0;
    anaUtils::VectorToArray(TVector3(0, 0, 0), truePart->Direction);
    truePart->Charge = 0;
    truePart->VertexID = -1;
    return;
  }
  truePart->ID = upTrack->GetTrackID();
  truePart->PDG = upTrack->GetPDG();
  truePart->ParentID = upTrack->GetParentID();
  truePart->CosTheta=upTrack->GetInitCosTheta();
  double tmpp=-9999;
  double tmpd=9999;
  int ti=-1;
  int td=-1;
  if (upTrack->GetNPoints() > 0) {
    for (int ipt = 0; ipt < upTrack->GetNPoints(); ipt++) {

      if (upTrack->GetPoint(ipt)->GetTime() > tmpp ){
        ti=ipt;
        tmpp=upTrack->GetPoint(ipt)->GetTime();
      }
      if (upTrack->GetPoint(ipt)->GetTime() < tmpd) {
        td=ipt;
        tmpd=upTrack->GetPoint(ipt)->GetTime();
      }
    }
  }
  if(ti>=0 && td>=0){
    anaUtils::VectorToArray(upTrack->GetPoint(td)->GetPostPosition(), truePart->Position);
 
    anaUtils::VectorToArray(upTrack->GetPoint(ti)->GetPostPosition(), truePart->PositionEnd);
    truePart->PositionEnd[3]=upTrack->GetPoint(ti)->GetTime();
    truePart->Position[3]=upTrack->GetPoint(td)->GetTime();
    if(upTrack->GetPoint(ti)->GetMomentum().Mag()>0){
      truePart->stopped=0;
    }else{
      truePart->stopped=1;
    }
  }
  //}
  truePart->Momentum = upTrack->GetInitMom().Mag();
  double mass = 0.;
  if     (abs(upTrack->GetPDG())==211)  mass = 139.570; // pi+-
  else if(abs(upTrack->GetPDG())==13)   mass = 105.658; // muon
  else if(abs(upTrack->GetPDG())==2212) mass = 938.272; // proton
  else if(abs(upTrack->GetPDG())==11)   mass = 0.511;   // electron

  anaUtils::VectorToArray((1 / truePart->Momentum)*upTrack->GetInitMom(), truePart->Direction);
  truePart->Charge = upTrack->GetCharge();
  truePart->VertexID = trueVertex->GetVertexID();
  truePart->Length=upTrack->GetSDLength();
  truePart->EDeposit=upTrack->GetSDTotalEnergyDeposit();
  truePart->EKin=mass+upTrack->GetInitKinEnergy();
  FindSegments(upTrack,truePart);

  
}
void AnaTreeConverterEvent::FindSegments(TND280UpTrack* upTrack,AnaTrueParticleB* truePart){

  double TPCUp1Length = 0;
  double TPCUp2Length = 0;
  double TPCDown1Length = 0;
  double TPCDown2Length = 0;
  double ForwTPC1Length = 0;
  double ForwTPC2Length = 0;
  double ForwTPC3Length = 0;
  double Target1Length = 0;
  double Target2Length = 0;
  double FGD1Length = 0;
  double FGD2Length = 0;
  double DsECalLength = 0;
  double P0DECalLength = 0;
  double BrlECalLength = 0;
  double ToFLength = 0;
  double TopUpZ = 9999;
  double TopDownZ = 9999;
  double BotUpZ = 9999;
  double BotDownZ = 9999;
  double LeftUpZ = 9999;
  double LeftDownZ = 9999;
  double RightUpZ = 9999;
  double RightDownZ = 9999;
  double BackUpZ = 9999;
  double BackDownZ = 9999;
  double FrontUpZ = 9999;
  double FrontDownZ = 9999;

  double TPCUp1Z = 9999;
  double TPCUp2Z = 9999;
  double TPCDown1Z = 9999;
  double TPCDown2Z = 9999;
  double ForwTPC1Z = 9999;
  double ForwTPC2Z = 9999;
  double ForwTPC3Z = 9999;
  double Target1Z = 9999;
  double Target2Z = 9999;
  double FGD1Z = 9999;
  double FGD2Z = 9999;
  double DsECalZ = 9999;
  double P0DECalZ = 9999;
  double BrlECalZ = 9999;
  double ToFZ = 9999;

  double TPCUp1fZ = -9999;
  double TPCUp2fZ = -9999;
  double TPCDown1fZ = -9999;
  double TPCDown2fZ = -9999;
  double ForwTPC1fZ = -9999;
  double ForwTPC2fZ = -9999;
  double ForwTPC3fZ = -9999;
  double Target1fZ = -9999;
  double Target2fZ = -9999;
  double FGD1fZ = -9999;
  double FGD2fZ = -9999;
  double DsECalfZ = -9999;
  double P0DECalfZ = -9999;
  double BrlECalfZ = -9999;
  double ToFfZ = -9999;
  double TopUpfZ = -9999;
  double TopDownfZ = -9999;
  double BotUpfZ = -9999;
  double BotDownfZ = -9999;
  double LeftUpfZ = -9999;
  double LeftDownfZ = -9999;
  double RightUpfZ = -9999;
  double RightDownfZ = -9999;
  double BackUpfZ = -9999;
  double BackDownfZ = -9999;
  double FrontUpfZ = -9999;
  double FrontDownfZ = -9999;

  double TPCUp1LengthYZ = 0;
  double TPCUp2LengthYZ = 0;
  double TPCDown1LengthYZ = 0;
  double TPCDown2LengthYZ = 0;
  double ForwTPC1LengthYZ = 0;
  double ForwTPC2LengthYZ = 0;
  double ForwTPC3LengthYZ = 0;
  double Target1LengthYZ = 0;
  double Target2LengthYZ = 0;
  double FGD1LengthYZ = 0;
  double FGD2LengthYZ = 0;
  double DsECalLengthYZ = 0;
  double P0DECalLengthYZ = 0;
  double BrlECalLengthYZ = 0;
  double ToFLengthYZ = 0;

  double TPCUp1Edep = 0;
  double TPCUp2Edep = 0;
  double TPCDown1Edep = 0;
  double TPCDown2Edep = 0;
  double ForwTPC1Edep = 0;
  double ForwTPC2Edep = 0;
  double ForwTPC3Edep = 0;
  double Target1Edep = 0;
  double Target2Edep = 0;
  double FGD1Edep = 0;
  double FGD2Edep = 0;
  double DsECalEdep = 0;
  double P0DECalEdep = 0;
  double BrlECalEdep = 0;
  double ToFEdep = 0;

  TND280UpTrackPoint* lastTPCUp1 = NULL;
  TND280UpTrackPoint* lastTPCUp2 = NULL;
  TND280UpTrackPoint* lastTPCDown1 = NULL;
  TND280UpTrackPoint* lastTPCDown2 = NULL;
  TND280UpTrackPoint* lastForwTPC1 = NULL;
  TND280UpTrackPoint* lastForwTPC2 = NULL;
  TND280UpTrackPoint* lastForwTPC3 = NULL;
  TND280UpTrackPoint* lastTarget1 = NULL;
  TND280UpTrackPoint* lastTarget2 = NULL;
  TND280UpTrackPoint* lastFGD1 = NULL;
  TND280UpTrackPoint* lastFGD2 = NULL;
  TND280UpTrackPoint* lastDsECal = NULL;
  TND280UpTrackPoint* lastP0DECal = NULL;
  TND280UpTrackPoint* lastBrlECal = NULL;
  TND280UpTrackPoint* lastToF = NULL;

  TND280UpTrackPoint* lastTopUp = NULL;
  TND280UpTrackPoint* lastTopDown = NULL;
  TND280UpTrackPoint* lastBotUp = NULL;
  TND280UpTrackPoint* lastBotDown = NULL;
  TND280UpTrackPoint* lastLeftUp = NULL;
  TND280UpTrackPoint* lastLeftDown = NULL;
  TND280UpTrackPoint* lastRightUp = NULL;
  TND280UpTrackPoint* lastRightDown = NULL;
  TND280UpTrackPoint* lastBackUp = NULL;
  TND280UpTrackPoint* lastBackDown = NULL;
  TND280UpTrackPoint* lastFrontUp = NULL;
  TND280UpTrackPoint* lastFrontDown = NULL;

  TND280UpTrackPoint* firstTPCUp1 = NULL;
  TND280UpTrackPoint* firstTPCUp2 = NULL;
  TND280UpTrackPoint* firstTPCDown1 = NULL;
  TND280UpTrackPoint* firstTPCDown2 = NULL;
  TND280UpTrackPoint* firstForwTPC1 = NULL;
  TND280UpTrackPoint* firstForwTPC2 = NULL;
  TND280UpTrackPoint* firstForwTPC3 = NULL;
  TND280UpTrackPoint* firstTarget1 = NULL;
  TND280UpTrackPoint* firstTarget2 = NULL;
  TND280UpTrackPoint* firstFGD1 = NULL;
  TND280UpTrackPoint* firstFGD2 = NULL;
  TND280UpTrackPoint* firstDsECal = NULL;
  TND280UpTrackPoint* firstP0DECal = NULL;
  TND280UpTrackPoint* firstBrlECal = NULL;
  TND280UpTrackPoint* firstToF = NULL;

  TND280UpTrackPoint* firstTopUp = NULL;
  TND280UpTrackPoint* firstTopDown = NULL;
  TND280UpTrackPoint* firstBotUp = NULL;
  TND280UpTrackPoint* firstBotDown = NULL;
  TND280UpTrackPoint* firstLeftUp = NULL;
  TND280UpTrackPoint* firstLeftDown = NULL;
  TND280UpTrackPoint* firstRightUp = NULL;
  TND280UpTrackPoint* firstRightDown = NULL;
  TND280UpTrackPoint* firstBackUp = NULL;
  TND280UpTrackPoint* firstBackDown = NULL;
  TND280UpTrackPoint* firstFrontUp = NULL;
  TND280UpTrackPoint* firstFrontDown = NULL;



  for (int ip = 0; ip < upTrack->GetNPoints(); ip++) {
    TND280UpTrackPoint* Tpoint = upTrack->GetPoint(ip);
//    std:;cout<<Tpoint->GetPhysVolName()<<std::endl;
    if (Tpoint->GetPhysVolName().find("TPCUp1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < TPCUp1Z) {
        lastTPCUp1 = Tpoint;
        TPCUp1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp1fZ) {
        firstTPCUp1 = Tpoint;
        TPCUp1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("TPCUp2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < TPCUp2Z) {
        lastTPCUp2 = Tpoint;
        TPCUp2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp2fZ) {
        firstTPCUp2 = Tpoint;
        TPCUp2fZ = Tpoint->GetMomentum().Mag();
      }

    }
     if (Tpoint->GetPhysVolName().find("TPCDown1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown1Z) {
        lastTPCDown1 = Tpoint;
        TPCDown1Z = Tpoint->GetMomentum().Mag();
      }
     if (Tpoint->GetMomentum().Mag() > TPCDown1fZ) {
        firstTPCDown1 = Tpoint;
        TPCDown1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("TPCDown2") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown2Z) {
        lastTPCDown2 = Tpoint;
        TPCDown2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCDown2fZ) {
        firstTPCDown2 = Tpoint;
        TPCDown2fZ = Tpoint->GetMomentum().Mag();
      }


    }
    if (Tpoint->GetPhysVolName().find("ForwTPC1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < ForwTPC1Z) {
        lastForwTPC1 = Tpoint;
        ForwTPC1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ForwTPC1fZ) {
        firstForwTPC1 = Tpoint;
        ForwTPC1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ForwTPC2Z) {
        lastForwTPC2 = Tpoint;
        ForwTPC2Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC2fZ) {
        firstForwTPC2 = Tpoint;
        ForwTPC2fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC3") != std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < ForwTPC3Z) {
        lastForwTPC3 = Tpoint;
        ForwTPC3Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC3fZ) {
        firstForwTPC3 = Tpoint;
        ForwTPC3fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < Target1Z) {
        lastTarget1 = Tpoint;
        Target1Z = Tpoint->GetMomentum().Mag();
      }
    if (Tpoint->GetMomentum().Mag() > Target1fZ) {
        firstTarget1 = Tpoint;
        Target1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target2") != std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < Target2Z) {
        lastTarget2 = Tpoint;
        Target2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > Target2fZ) {
        firstTarget2 = Tpoint;
        Target2fZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("DsECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < DsECalZ) {
        lastDsECal = Tpoint;
        DsECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > DsECalfZ) {
        firstDsECal = Tpoint;
        DsECalfZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("P0DECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < P0DECalZ) {
        lastP0DECal = Tpoint;
        P0DECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > P0DECalfZ) {
        firstP0DECal = Tpoint;
        P0DECalfZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("BrlECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < BrlECalZ) {
        lastBrlECal = Tpoint;
        BrlECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > BrlECalfZ) {
        firstBrlECal = Tpoint;
        BrlECalfZ = Tpoint->GetMomentum().Mag();
      }

    }

    if (Tpoint->GetPhysVolName().find("ToF/TopUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < TopUpZ) {
        lastTopUp = Tpoint;
        TopUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TopUpfZ) {
        firstTopUp = Tpoint;
        TopUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

    if (Tpoint->GetPhysVolName().find("ToF/TopDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < TopDownZ) {
        lastTopDown = Tpoint;
        TopDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TopDownfZ) {
        firstTopDown = Tpoint;
        TopDownfZ = Tpoint->GetMomentum().Mag();
      }

    }
  if (Tpoint->GetPhysVolName().find("ToF/BotUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < TopUpZ) {
        lastBotUp = Tpoint;
        BotUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TopUpfZ) {
        firstBotUp = Tpoint;
        BotUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BotDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < TopDownZ) {
        lastBotDown = Tpoint;
        BotDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TopDownfZ) {
        firstBotDown = Tpoint;
        BotDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/LeftUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < LeftUpZ) {
        lastLeftUp = Tpoint;
        LeftUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > LeftUpfZ) {
        firstLeftUp = Tpoint;
        LeftUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/RightUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < RightUpZ) {
        lastRightUp = Tpoint;
        RightUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > RightUpfZ) {
        firstRightUp = Tpoint;
        RightUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/RightDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < RightDownZ) {
        lastRightDown = Tpoint;
        RightDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > RightDownfZ) {
        firstRightDown = Tpoint;
        RightDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/LeftDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < LeftDownZ) {
        lastLeftDown = Tpoint;
        LeftDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > LeftDownfZ) {
        firstLeftDown = Tpoint;
        LeftDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BackDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < BackDownZ) {
        lastBackDown = Tpoint;
        BackDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > BackDownfZ) {
        firstBackDown = Tpoint;
        BackDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BackUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < BackUpZ) {
        lastBackUp = Tpoint;
        BackUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > BackUpfZ) {
        firstBackUp = Tpoint;
        BackUpfZ = Tpoint->GetMomentum().Mag();
      }

    }



  if (Tpoint->GetPhysVolName().find("ToF/FrontDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < FrontDownZ) {
        lastFrontDown = Tpoint;
        FrontDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > FrontDownfZ) {
        firstFrontDown = Tpoint;
        FrontDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/FrontUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < FrontUpZ) {
        lastFrontUp = Tpoint;
        FrontUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > FrontUpfZ) {
        firstFrontUp = Tpoint;
        FrontUpfZ = Tpoint->GetMomentum().Mag();
      }

    }



}
  int nCrossers=0;
  if(firstTPCUp1){
        nCrossers++;
  }
  if(firstTPCUp2){
       nCrossers++;
  }
  if(firstTPCDown1){
       nCrossers++;
  }
   if(firstTPCDown2){
       nCrossers++;
  }
   if(firstForwTPC1){
       nCrossers++;
  }
   if(firstForwTPC2){
       nCrossers++;
  }
   if(firstForwTPC3){
       nCrossers++;
  }
  if(firstTarget1){
       nCrossers++;
  }
   if(firstTarget2){
       nCrossers++;
  }
   if(firstDsECal){
       nCrossers++;
  }
   if(firstP0DECal){
       nCrossers++;
  }

   if(firstBrlECal){
       nCrossers++;
  }

   if(firstTopUp){
       nCrossers++;
  }
   if(firstTopDown){
       nCrossers++;
  }

   if(firstBotUp){
       nCrossers++;
  }
   if(firstBotDown){
       nCrossers++;
  }
   if(firstLeftUp){
       nCrossers++;
  }
   if(firstLeftDown){
       nCrossers++;
  }
   if(firstFrontDown){
       nCrossers++;
  }
   if(firstFrontUp){
       nCrossers++;
  }

   if(firstBackDown){
       nCrossers++;
  }
   if(firstBackUp){
       nCrossers++;
  }

  truePart->nDetCrossings=0;

  //  anaUtils::CreateArray(truePart->DetCrossings, nCrossers);
  if(firstTPCUp1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    // firstTPCUp1->PrintTrackPoint();
    //lastTPCUp1->PrintTrackPoint();
    detCross->EntrancePosition[0] = firstTPCUp1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp1->GetTime();
    detCross->ExitPosition[0] = lastTPCUp1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp1();
    detCross->EDeposit=upTrack->GetEdepTPCUp1();
    detCross->SegLength=upTrack->GetLengthTPCUp1();
    detCross->Detector_name=TString(firstTPCUp1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTPCUp2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCUp2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp2->GetTime();
    detCross->ExitPosition[0] = lastTPCUp2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp2();
    detCross->EDeposit=upTrack->GetEdepTPCUp2();
    detCross->SegLength=upTrack->GetLengthTPCUp2();
    detCross->Detector_name=TString(firstTPCUp2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp2);
    truePart->DetCrossingsVect.push_back(detCross);
  }

  if (firstTPCDown1) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown1->GetTime();
    detCross->ExitPosition[0] = lastTPCDown1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown1();
    detCross->EDeposit=upTrack->GetEdepTPCDown1();
    detCross->SegLength=upTrack->GetLengthTPCDown1();
    detCross->Detector_name=TString(firstTPCDown1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown1);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if(firstTPCDown2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown2->GetTime();
    detCross->ExitPosition[0] = lastTPCDown2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown2();
    detCross->EDeposit=upTrack->GetEdepTPCDown2();
    detCross->SegLength=upTrack->GetLengthTPCDown2();
    detCross->Detector_name=TString(firstTPCDown2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC1->GetTime();
    detCross->ExitPosition[0] = lastForwTPC1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC1->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC1();
    detCross->EDeposit=upTrack->GetEdepForwTPC1();
    detCross->SegLength=upTrack->GetLengthForwTPC1();
    detCross->Detector_name=TString(firstForwTPC1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC2->GetTime();
    detCross->ExitPosition[0] = lastForwTPC2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC2->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC2();
    detCross->EDeposit=upTrack->GetEdepForwTPC2();
    detCross->SegLength=upTrack->GetLengthForwTPC2();
    detCross->Detector_name=TString(firstForwTPC2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC3){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC3->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC3->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC3->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC3->GetTime();
    detCross->ExitPosition[0] = lastForwTPC3->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC3->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC3->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC3->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC3->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC3->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC3->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC3->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC3->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC3->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC3();
    detCross->EDeposit=upTrack->GetEdepForwTPC3();
    detCross->Detector_name=TString(firstForwTPC3->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC3);
    detCross->SegLength=upTrack->GetLengthForwTPC3();
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget1->GetTime();
    detCross->ExitPosition[0] = lastTarget1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget1->GetTime();
    detCross->EntranceMomentum[0] = firstTarget1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget1->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthTarget1();
    detCross->DeltaLYZ=upTrack->GetLengthTarget1();
    detCross->EDeposit=upTrack->GetEdepTarget1();
    detCross->Detector_name=TString(firstTarget1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget1);

    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget2->GetTime();
    detCross->ExitPosition[0] = lastTarget2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget2->GetTime();
    detCross->EntranceMomentum[0] = firstTarget2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget2->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthTarget2();
    detCross->DeltaLYZ=upTrack->GetLengthTarget2();
    detCross->EDeposit=upTrack->GetEdepTarget2();
    detCross->Detector_name=TString(firstTarget2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstFGD1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstFGD1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFGD1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFGD1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFGD1->GetTime();
    detCross->ExitPosition[0] = lastFGD1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFGD1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFGD1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFGD1->GetTime();
    detCross->EntranceMomentum[0] = firstFGD1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFGD1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFGD1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFGD1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFGD1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFGD1->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthFGD1();
    detCross->DeltaLYZ=upTrack->GetLengthFGD1();
    detCross->EDeposit=upTrack->GetEdepFGD1();
    detCross->Detector_name=TString(firstFGD1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFGD1);

    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstFGD2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstFGD2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFGD2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFGD2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFGD2->GetTime();
    detCross->ExitPosition[0] = lastFGD2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFGD2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFGD2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFGD2->GetTime();
    detCross->EntranceMomentum[0] = firstFGD2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFGD2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFGD2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFGD2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFGD2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFGD2->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthFGD2();
    detCross->DeltaLYZ=upTrack->GetLengthFGD2();
    detCross->EDeposit=upTrack->GetEdepFGD2();
    detCross->Detector_name=TString(firstFGD2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFGD2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstDsECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstDsECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstDsECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstDsECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstDsECal->GetTime();
    detCross->ExitPosition[0] = lastDsECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastDsECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastDsECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastDsECal->GetTime();
    detCross->EntranceMomentum[0] = firstDsECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstDsECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstDsECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastDsECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastDsECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastDsECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthDsECal();
    detCross->DeltaLYZ=upTrack->GetLengthDsECal();
    detCross->EDeposit=upTrack->GetEdepDsECal();
    detCross->Detector_name=TString(firstDsECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kDsECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstP0DECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstP0DECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstP0DECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstP0DECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstP0DECal->GetTime();
    detCross->ExitPosition[0] = lastP0DECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastP0DECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastP0DECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastP0DECal->GetTime();
    detCross->EntranceMomentum[0] = firstP0DECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstP0DECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstP0DECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastP0DECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastP0DECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastP0DECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthP0DECal();
    detCross->DeltaLYZ=upTrack->GetLengthP0DECal();
    detCross->EDeposit=upTrack->GetEdepP0DECal();
    detCross->Detector_name=TString(firstP0DECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kP0DECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstBrlECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    //std::cout<<"firstBrlECal->GetPostPosition().X() "<<lastBrlECal->GetPostPosition().X()<<" "<<lastBrlECal->GetPostPosition().Y()<<" "<<lastBrlECal->GetPostPosition().Z()<<std::endl;
    detCross->EntrancePosition[0] = firstBrlECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBrlECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBrlECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBrlECal->GetTime();
    detCross->ExitPosition[0] = lastBrlECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBrlECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBrlECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBrlECal->GetTime();
    detCross->EntranceMomentum[0] = firstBrlECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBrlECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBrlECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBrlECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBrlECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBrlECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthBrlECal();
    detCross->DeltaLYZ=upTrack->GetLengthBrlECal();
    detCross->EDeposit=upTrack->GetEdepBrlECal();
    detCross->Detector_name=TString(firstBrlECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBrlECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstTopUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstTopUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTopUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTopUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTopUp->GetTime();
    detCross->ExitPosition[0] = lastTopUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTopUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTopUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTopUp->GetTime();
    detCross->EntranceMomentum[0] = firstTopUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTopUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTopUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTopUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTopUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTopUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstTopUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTopUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstBotUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstBotUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBotUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBotUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBotUp->GetTime();
    detCross->ExitPosition[0] = lastBotUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBotUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBotUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBotUp->GetTime();
    detCross->EntranceMomentum[0] = firstBotUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBotUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBotUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBotUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBotUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBotUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstBotUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBotUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstTopDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstTopDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTopDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTopDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTopDown->GetTime();
    detCross->ExitPosition[0] = lastTopDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTopDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTopDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTopDown->GetTime();
    detCross->EntranceMomentum[0] = firstTopDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTopDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTopDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTopDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTopDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTopDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstTopDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTopDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstBotDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstBotDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBotDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBotDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBotDown->GetTime();
    detCross->ExitPosition[0] = lastBotDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBotDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBotDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBotDown->GetTime();
    detCross->EntranceMomentum[0] = firstBotDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBotDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBotDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBotDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBotDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBotDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstBotDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBotDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstLeftUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstLeftUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstLeftUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstLeftUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstLeftUp->GetTime();
    detCross->ExitPosition[0] = lastLeftUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastLeftUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastLeftUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastLeftUp->GetTime();
    detCross->EntranceMomentum[0] = firstLeftUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstLeftUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstLeftUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastLeftUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastLeftUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastLeftUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstLeftUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kLeftUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstLeftDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstLeftDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstLeftDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstLeftDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstLeftDown->GetTime();
    detCross->ExitPosition[0] = lastLeftDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastLeftDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastLeftDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastLeftDown->GetTime();
    detCross->EntranceMomentum[0] = firstLeftDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstLeftDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstLeftDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastLeftDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastLeftDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastLeftDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstLeftDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kLeftDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstRightUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstRightUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstRightUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstRightUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstRightUp->GetTime();
    detCross->ExitPosition[0] = lastRightUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastRightUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastRightUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastRightUp->GetTime();
    detCross->EntranceMomentum[0] = firstRightUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstRightUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstRightUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastRightUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastRightUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastRightUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstRightUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kRightUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstRightDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstRightDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstRightDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstRightDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstRightDown->GetTime();
    detCross->ExitPosition[0] = lastRightDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastRightDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastRightDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastRightDown->GetTime();
    detCross->EntranceMomentum[0] = firstRightDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstRightDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstRightDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastRightDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastRightDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastRightDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstRightDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kRightDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }


  if (firstBackDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstBackDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBackDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBackDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBackDown->GetTime();
    detCross->ExitPosition[0] = lastBackDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBackDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBackDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBackDown->GetTime();
    detCross->EntranceMomentum[0] = firstBackDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBackDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBackDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBackDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBackDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBackDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstBackDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBackDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstBackUp) {

    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstBackUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBackUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBackUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBackUp->GetTime();
    detCross->ExitPosition[0] = lastBackUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBackUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBackUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBackUp->GetTime();
    detCross->EntranceMomentum[0] = firstBackUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBackUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBackUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBackUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBackUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBackUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstBackUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBackUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstFrontDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstFrontDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFrontDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFrontDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFrontDown->GetTime();
    detCross->ExitPosition[0] = lastFrontDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFrontDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFrontDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFrontDown->GetTime();
    detCross->EntranceMomentum[0] = firstFrontDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFrontDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFrontDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFrontDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFrontDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFrontDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstFrontDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFrontDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstFrontUp) {

    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstFrontUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFrontUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFrontUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFrontUp->GetTime();
    detCross->ExitPosition[0] = lastFrontUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFrontUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFrontUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFrontUp->GetTime();
    detCross->EntranceMomentum[0] = firstFrontUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFrontUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFrontUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFrontUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFrontUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFrontUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstFrontUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFrontUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  

}


bool AnaTreeConverterEvent::IsReconstructedTarget(double length,double theta){
  int bin =hefficiency_target->FindBin(theta,length);

  double prob=hefficiency_target->GetBinContent(bin);
 
  if(gRandom->Uniform()>prob)
    return 1;
  else
    return 0;
  
}


bool AnaTreeConverterEvent::IsReconstructedECal(TVector3 P, TString det){

  float reco_eff_Brl, FGDmatch_eff_Brl;
  float reco_eff_Ds,  FGDmatch_eff_Ds;

  TVector3 entryNormal_vect(0,0,0);
    
  if (det.Contains("RightClam") &&
      det.Contains("BotLeftTopRight"))
    entryNormal_vect.SetY(1);  // (+Y)
  else if (det.Contains("RightClam") &&
	   det.Contains("TopLeftBotRight"))
    entryNormal_vect.SetY(-1); // (-Y)
  else if (det.Contains("LeftClam") &&
	   det.Contains("BotLeftTopRight"))
    entryNormal_vect.SetY(-1); // (-Y)
  else if (det.Contains("LeftClam") &&
	   det.Contains("TopLeftBotRight"))
    entryNormal_vect.SetY(1);  // (+Y)
  else if (det.Contains("LeftSide"))
    entryNormal_vect.SetX(1);  // (+X)
  else if (det.Contains("RightSide"))
    entryNormal_vect.SetX(-1); // (-X)
  else if (det.Contains("POD/USECal"))
    entryNormal_vect.SetZ(-1); // (-Z)
  else
    entryNormal_vect.SetZ(1);  // (+Z)

  float mom = P.Mag();
  float cos = P.Dot(entryNormal_vect)/mom;
  
  if (!_ECal_reco_eff->GetBinValues(mom, reco_eff_Brl, reco_eff_Ds)) return false;
  if (!_ECal_FGDmatch_eff->GetBinValues(cos, FGDmatch_eff_Brl, FGDmatch_eff_Ds)) return false;

  // throw two random numbers between 0 and 1
  double r_eff[2];
  _randomGen->RndmArray(2, r_eff);

  // select artificially only a fraction of the events, 
  // by applying the reconstruction/FGD-ECal match efficiencies
  if (det.Contains("DsECal")) {
    if (r_eff[1] < FGDmatch_eff_Ds)
      return true;
  }
  else {
    if (r_eff[0] < reco_eff_Brl && r_eff[1] < FGDmatch_eff_Brl)
      return true;
  }
  return false;
  
}


void AnaTreeConverterEvent::Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticle, AnaTrack* reconParticle) {
  
  reconParticle->UniqueID       = trueParticle->ID;
  reconParticle->Momentum       = trueParticle->Momentum;
  reconParticle->Charge         = trueParticle->Charge;
  reconParticle->Length         = trueParticle->Length;
  reconParticle->EDeposit       = trueParticle->EDeposit;
  reconParticle->EKin           = trueParticle->EKin;
  anaUtils::CopyArray(trueParticle->Direction, reconParticle->DirectionStart, 3);
  anaUtils::CopyArray(trueParticle->Position,  reconParticle->PositionStart, 4);
  anaUtils::CopyArray(trueParticle->PositionEnd,   reconParticle->PositionEnd, 4);
  reconParticle->MomentumEle = -10000;
  reconParticle->MomentumMuon = -10000;
  reconParticle->MomentumProton = -10000;
  reconParticle->MomentumErrorEle = -10000;
  reconParticle->MomentumErrorMuon = -10000;
  reconParticle->MomentumErrorProton = -10000;
  reconParticle->nTargetSegments = 0;
  reconParticle->nFGDSegments = 0;
  reconParticle->nTPCSegments = 0;
  reconParticle->TrueObject=dynamic_cast<AnaTrueObjectC*>(trueParticle);
  for (int i = 0; i < trueParticle->DetCrossingsVect.size(); i++) {

    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)) {
      AnaTPCParticleB* seg = dynamic_cast<AnaTPCParticleB*>(MakeTPCTrack());
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
 
      seg->Detectors=trueParticle->DetCrossingsVect[i]->Detector;
 
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      seg->Momentum = mom;
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      //  anaUtils::ComputeTPCPull(*seg,*reconParticle);
      reconParticle->TPCSegments[reconParticle->nTPCSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget2)) {
      AnaTargetParticleB* seg = dynamic_cast<AnaTargetParticleB*>(MakeTargetTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed = IsReconstructedTarget(seg->DeltaLYZ, trueParticle->CosTheta);
      reconParticle->TargetSegments[reconParticle->nTargetSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFGD1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFGD2)) {
      AnaFGDParticleB* seg = dynamic_cast<AnaFGDParticleB*>(MakeFGDTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed = IsReconstructedTarget(seg->DeltaLYZ, trueParticle->CosTheta);
      reconParticle->FGDSegments[reconParticle->nFGDSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kDsECal) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBrlECal) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kP0DECal)) {
      AnaECalParticleB* seg = dynamic_cast<AnaECalParticleB*>(MakeECalTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed =
	IsReconstructedECal(anaUtils::ArrayToTVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum),
			    trueParticle->DetCrossingsVect[i]->Detector_name);
      reconParticle->ECalSegments[reconParticle->nECalSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFrontUp)||
       SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBackUp) ||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTopUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBotUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kLeftUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kRightUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFrontDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBackDown) ||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTopDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBotDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kLeftDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kRightDown)
        ) {
          std::cout<<"tof"<<std::endl;
      AnaToFParticleB* seg = dynamic_cast<AnaToFParticleB*>(MakeToFTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      reconParticle->ToFSegments[reconParticle->nToFSegments++] = seg;

    }
  }

}
