//#include "ND280RootGeometryManager.hh"
#include "ND280TrajectoryPoint.hh"
#include "ExN02Constants.hh"

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4StepStatus.hh>

#include <G4AttDefStore.hh>
#include <G4AttDef.hh>
#include <G4AttValue.hh>
#include <G4UnitsTable.hh>

//#include <TGeoManager.h>

//#include <TND280Log.hxx>

#include "logger.hh"

G4Allocator<ND280TrajectoryPoint> aND280TrajPointAllocator;

ND280TrajectoryPoint::ND280TrajectoryPoint()
  : fTime(0.), fMomentum(0.,0.,0.), fEdep(0), fStepLength(0),
    fStepDeltaLyz(0), fStepStatus(fUndefined), fIsOnBoundary(false), 
    fPhysVolName("OutofWorld"), 
    fLogVolName("OutofWorld"), 
    fPrevPosition(0,0,0),
    fPostPosition(0,0,0), fSavePoint(false),
    fPreProcessName("NONE"), fPostProcessName("NONE"),
    fMaterialName("OutofWorldMaterial"),
    fPreProcessType(G4ProcessType::fNotDefined), fPostProcessType(G4ProcessType::fNotDefined)
    /*, fPreMaterialName(""), fPostMaterialName("")*/ { }

// This is the one used --> See ND280Trajectory::AppendStep(G4Step)
ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)
  : G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition())
{
  // fPreProcessName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)'"; // debug
  // fPostProcessName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)'"; // debug
  // // fPreMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)'"; // debug
  // // fPostMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)'"; // debug
  // fMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)'"; // debug
  fPreProcessName = "NONE(aStep)"; // debug
  fPostProcessName = "NONE(aStep)"; // debug
  // fPreMaterialName = "NONE(aStep)"; // debug
  // fPostMaterialName = "NONE(aStep)"; // debug
  fMaterialName = "NONE(aStep)"; // debug
  fPreProcessType = G4ProcessType::fNotDefined;
  fPostProcessType = G4ProcessType::fNotDefined;

  fTime = aStep->GetPostStepPoint()->GetGlobalTime();
  
  // Need momentum at postStep because the first point 
  // of the trajectory is taken w/ ND280TrajectoryPoint(G4Track)
  // when only the preStep is available!!!
  
  fMomentum = aStep->GetPostStepPoint()->GetMomentum();
  fEdep = aStep->GetTotalEnergyDeposit();
  fStepLength = aStep->GetStepLength() / CLHEP::mm;
  
  G4ThreeVector prestepPos = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector poststepPos = aStep->GetPostStepPoint()->GetPosition();
  //G4double prestep_x = prestepPos.x() / CLHEP::mm;
  G4double prestep_y = prestepPos.y() / CLHEP::mm;
  G4double prestep_z = prestepPos.z() / CLHEP::mm;
  //G4double poststep_x = poststepPos.x() / CLHEP::mm;
  G4double poststep_y = poststepPos.y() / CLHEP::mm;
  G4double poststep_z = poststepPos.z() / CLHEP::mm;  
  G4double stepdeltalyz = sqrt( (prestep_y - poststep_y)*(prestep_y - poststep_y) +
				(prestep_z - poststep_z)*(prestep_z - poststep_z) );
  fStepDeltaLyz = stepdeltalyz;
  
  fStepStatus = aStep->GetPostStepPoint()->GetStepStatus();
  
  // Use postStep convention, though the SDManager refers to the preStep detector
  if (aStep->GetPostStepPoint()->GetPhysicalVolume()) {
    fPhysVolName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    fLogVolName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    fMaterialName = aStep->GetPreStepPoint()->GetPhysicalVolume()
                         ->GetLogicalVolume()->GetMaterial()->GetName();
  }
  //if (aStep->GetPreStepPoint()->GetPhysicalVolume()) {
  //fPhysVolName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  //}
  else {
    fPhysVolName = "OutOfWorld";
    fLogVolName = "OutOfWorld";
    fMaterialName = "OutOfWorldMaterial";
  }


  // fPreMaterialName = (aStep->GetPreStepPoint()->GetMaterial()) ? aStep->GetPreStepPoint()->GetMaterial()->GetName() : "OutOfWorldMaterial";
  // fPostMaterialName = (aStep->GetPostStepPoint()->GetMaterial()) ? aStep->GetPostStepPoint()->GetMaterial()->GetName() : "OutOfWorldMaterial";
  
  if(aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary)
    fIsOnBoundary = true;
  else 
    fIsOnBoundary = false;

  fPrevPosition = aStep->GetPreStepPoint()->GetPosition();
  fPostPosition = aStep->GetPostStepPoint()->GetPosition();

  fSavePoint = false;

  /// !!! NEW !!!
  if (aStep->GetPreStepPoint()->GetProcessDefinedStep())
  {
    fPreProcessName = aStep->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
    fPreProcessType = aStep->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessType();
  }
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep())
  {
    fPostProcessName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    fPostProcessType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  }

  const std::vector< const G4Track * > * SecondaryTracksAtThisStep = aStep->GetSecondaryInCurrentStep();
  if (SecondaryTracksAtThisStep)
  {
    for (auto trk: *SecondaryTracksAtThisStep)
    {
      int ID = trk->GetTrackID();

      std::string process_name;
      const G4VProcess* proc = trk->GetCreatorProcess();
      if (proc) process_name = proc->GetProcessName();
      else process_name = "primary";

      G4ThreeVector position = trk->GetPosition();

      // fSecondariesIds.push_back(ID);
      // fSecondariesIds.push_back({trk->GetPDG(), trk->GetKineticEnergy()});
      // fSecondariesIds.push_back({trk->GetParticleDefinition()->GetPDGEncoding(), trk->GetKineticEnergy()});
      fSecondariesIds.push_back({
        trk->GetParticleDefinition()->GetPDGEncoding(),
        trk->GetKineticEnergy(),
        trk->GetMomentum().x(),
        trk->GetMomentum().y(),
        trk->GetMomentum().z(),
        process_name,
        position.x(),
        position.y(),
        position.z()
      });

      std::stringstream _log_msg;
      _log_msg << "[Track: " << aStep->GetTrack()->GetTrackID() << ","
                        << aStep->GetTrack()->GetParentID() << " "
                        << aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() << "] "
               << "Secondary track: ID:" << ID << ","
                                         <<" PDG:" << trk->GetParticleDefinition()->GetPDGEncoding() << ","
                                         << " Ek0:" << trk->GetKineticEnergy();
      log("DBG", std::string(__FILE__), std::to_string(__LINE__), std::string(__func__), _log_msg);
    }
  }
}

// Used for the first point 
ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)
    : G4TrajectoryPoint(aTrack->GetPosition())
  {
    // fPreProcessName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)'"; // debug
    // fPostProcessName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)'"; // debug
    // // fPreMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)'"; // debug
    // // fPostMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)'"; // debug
    // fMaterialName = "called in 'ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)'"; // debug
    fPreProcessName = "NONE(aTrack)"; // debug
    fPostProcessName = "NONE(aTrack)"; // debug
    // fPreMaterialName = "NONE(aTrack)"; // debug
    // fPostMaterialName = "NONE(aTrack)"; // debug
    fMaterialName = "NONE(aTrack)"; // debug
    fPreProcessType = G4ProcessType::fNotDefined;
    fPostProcessType = G4ProcessType::fNotDefined;
  
    fTime = aTrack->GetGlobalTime();
    fMomentum = aTrack->GetMomentum(); // It takes the preStep
    
    // FIRST POINT OF THE TRACK --> BEFORE ANY STEP (ONLY PRE-STEP)!!! 
    fEdep = 0.;         
    fStepLength = 0.;       
    fStepDeltaLyz = 0.;       
    fStepStatus = fUndefined;  
    //
    
    if (aTrack->GetVolume()){
      fPhysVolName = aTrack->GetVolume()->GetName();
      fLogVolName = aTrack->GetVolume()->GetLogicalVolume()->GetName();
      fMaterialName = aTrack->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
    }
    else{
      fPhysVolName = "OutOfWorld";
      fLogVolName = "OutOfWorld";
      fMaterialName = "OutOfWorldMaterial";
    }

    // fPreMaterialName = (
    //     aTrack->GetStep()->GetPreStepPoint()->GetMaterial()) ? aTrack->GetStep()->GetPreStepPoint()->GetMaterial()->GetName() : "OutOfWorldMaterial";
    // fPostMaterialName = (
    //     aTrack->GetStep()->GetPostStepPoint()->GetMaterial()
    //   ) ? aTrack->GetStep()->GetPostStepPoint()->GetMaterial()->GetName() : "OutOfWorldMaterial";
    
    fPrevPosition = aTrack->GetPosition();
    fPostPosition = aTrack->GetPosition(); // in the first point pre=post

    fIsOnBoundary = false; // first track point not on boundary

    fSavePoint = false;

    /// !!! NEW !!!
    // if (aTrack->GetStep()->GetPreStepPoint()->GetProcessDefinedStep()) // segfault
    // {
    //   fPreProcessName = aTrack->GetStep()->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
    // }
    // if (aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep())
    // {
    //   fPostProcessName = aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    // }
    if (!aTrack->GetCreatorProcess())
    {
      fPreProcessName = "primary"; // no creator process
      fPostProcessName = "primary"; // no creator process
      fPreProcessType = G4ProcessType::fNotDefined;
      fPostProcessType = G4ProcessType::fNotDefined;
    }
    else
    {
      fPreProcessName = aTrack->GetCreatorProcess()->GetProcessName(); // no creator process
      fPostProcessName = aTrack->GetCreatorProcess()->GetProcessName(); // no creator process
      fPreProcessType = aTrack->GetCreatorProcess()->GetProcessType();
      fPostProcessType = aTrack->GetCreatorProcess()->GetProcessType();
    }

    // const std::vector< const G4Track * > * SecondaryTracksAtThisStep = aStep->GetSecondaryInCurrentStep();
    // if (SecondaryTracksAtThisStep)
    // {
    //   for (auto trk: *SecondaryTracksAtThisStep)
    //   {
    //     int ID = trk->GetTrackID();
    //     fSecondariesIds.push_back(ID);
    //   }
    // }
}

ND280TrajectoryPoint::ND280TrajectoryPoint(const ND280TrajectoryPoint &right)
    : G4TrajectoryPoint(right) {
    fTime = right.fTime;
    fMomentum = right.fMomentum;
    fEdep = right.fEdep;
    fStepLength = right.fStepLength;
    fStepDeltaLyz = right.fStepDeltaLyz;
    fStepStatus = right.fStepStatus;
    fPhysVolName = right.fPhysVolName;
    fLogVolName = right.fLogVolName;
    fPrevPosition = right.fPrevPosition;
    fPostPosition = right.fPostPosition;
    fIsOnBoundary = right.fIsOnBoundary;
    fSavePoint = right.fSavePoint;
    fPreProcessName  = right.fPreProcessName;
    fPostProcessName = right.fPostProcessName;
    // fPreMaterialName = right.fPreMaterialName;
    // fPostMaterialName = right.fPostMaterialName;
    fMaterialName = right.fMaterialName;
    fPreProcessType = right.fPreProcessType;
    fPostProcessType = right.fPostProcessType;
    fSecondariesIds = right.fSecondariesIds;
}

void ND280TrajectoryPoint::MarkPoint(){
  fSavePoint = true;
}

ND280TrajectoryPoint::~ND280TrajectoryPoint() { }

const std::map<G4String,G4AttDef>* ND280TrajectoryPoint::GetAttDefs() const {
    G4bool isNew;

    std::map<G4String,G4AttDef>* store
        = G4AttDefStore::GetInstance("ND280TrajectoryPoint",isNew);

    if (isNew) {

        G4String Time("Time");
        (*store)[Time] =
            G4AttDef(Time, "Time", "Physics","G4BestUnit","G4double");

        G4String Momentum("Momentum");
        (*store)[Momentum] =
            G4AttDef(Momentum, "Momentum", "Physics",
                     "G4BestUnit","G4ThreeVector");

        G4String Edep("Edep");
        (*store)[Edep] =
            G4AttDef(Edep, "Edep", "Physics",
                     "G4BestUnit","G4double");

        G4String StepLength("StepLength");
        (*store)[StepLength] =
            G4AttDef(StepLength, "StepLength", "Physics",
                     "G4BestUnit","G4double");

        G4String StepDeltaLyz("StepDeltaLyz");
        (*store)[StepDeltaLyz] =
            G4AttDef(StepDeltaLyz, "StepDeltaLyz", "Physics",
                     "G4BestUnit","G4double");

        G4String StepStatus("StepStatus");
        (*store)[StepStatus] =
            G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

        G4String VolumeName("VolumeName");
        (*store)[VolumeName] =
            G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");

        G4String VolumeNode("VolumeNode");
        (*store)[VolumeNode] =
            G4AttDef(VolumeNode, "VolumeNode", "Physics", "", "G4int");

        G4String PreProcessName("PreProcessName");
        (*store)[PreProcessName] =
            G4AttDef(PreProcessName, "PreProcessName", "Physics", "", "G4String");

        G4String PostProcessName("PostProcessName");
        (*store)[PostProcessName] =
            G4AttDef(PostProcessName, "PostProcessName", "Physics", "", "G4String");

        // G4String PreMaterialName("PreMaterialName");
        // (*store)[PreMaterialName] =
        //     G4AttDef(PreMaterialName, "PreMaterialName", "Physics", "", "G4String");

        // G4String PostMaterialName("PostMaterialName");
        // (*store)[PostMaterialName] =
        //     G4AttDef(PostMaterialName, "PostMaterialName", "Physics", "", "G4String");

        G4String MaterialName("MaterialName");
        (*store)[MaterialName] =
            G4AttDef(MaterialName, "MaterialName", "Physics", "", "G4String");

        G4String PreProcessType("PreProcessType");
        (*store)[PreProcessType] =
            G4AttDef(PreProcessType, "PreProcessType", "Physics", "", "G4String");

        G4String PostProcessType("PostProcessType");
        (*store)[PostProcessType] =
            G4AttDef(PostProcessType, "PostProcessType", "Physics", "", "G4String");

        G4String SecondariesIds("SecondariesIds");
        (*store)[SecondariesIds] =
            G4AttDef(SecondariesIds, "SecondariesIds", "Physics", "", "G4String");

    }
    return store;
}

std::vector<G4AttValue>* ND280TrajectoryPoint::CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

    values->push_back(G4AttValue("Momentum",
                                 G4BestUnit(fMomentum,"Momentum"),""));

    values->push_back(G4AttValue("Edep",
                                 G4BestUnit(fEdep,"Energy"),""));

    values->push_back(G4AttValue("StepLength",
                                 G4BestUnit(fStepLength,"Length"),""));

    values->push_back(G4AttValue("StepDeltaLyz",
                                 G4BestUnit(fStepDeltaLyz,"Length"),""));

    values->push_back(G4AttValue("StepStatus",fStepStatus,""));

    values->push_back(G4AttValue("PhysVolName",fPhysVolName,""));

    values->push_back(G4AttValue("LogVolName",fLogVolName,""));

    values->push_back(G4AttValue("PreProcessName",fPreProcessName,""));
    values->push_back(G4AttValue("PostProcessName",fPostProcessName,""));
    // values->push_back(G4AttValue("PreMaterialName",fPreMaterialName,""));
    // values->push_back(G4AttValue("PostMaterialName",fPostMaterialName,""));
    values->push_back(G4AttValue("MaterialName",fMaterialName,""));
    values->push_back(G4AttValue("PreProcessType",fPreProcessType,""));
    values->push_back(G4AttValue("PostProcessType",fPostProcessType,""));
    // values->push_back(G4AttValue("SecondariesIds",fSecondariesIds,""));

    //#ifdef G4ATTDEBUG
    //ND280Info(G4AttCheck(values,GetAttDefs()));
    //#endif

    return values;
}

// int ND280TrajectoryPoint::GetVolumeNode() const {
//     gGeoManager->PushPath();
//     int node
//         = ND280RootGeometryManager::Get()->GetNodeId(0.5*(GetPosition()
//                                                           +fPrevPosition));
//     gGeoManager->PopPath();
//     return node;
// }
