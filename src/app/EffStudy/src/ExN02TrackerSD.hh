//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN02TrackerSD.hh,v 1.7 2006-06-29 17:47:56 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02TrackerSD_h
#define ExN02TrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "ExN02TrackerHit.hh"

#include "ExN02ND280XML.hh"

#include "logger.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02TrackerSD : public G4VSensitiveDetector
{
public:
  ExN02TrackerSD(G4String);
  ~ExN02TrackerSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);


  ///// NEW ND280 HitSegment

  /// Set the maximum sagitta for the ND280HitSegment objects created by
  /// this sensitive detector.
  void SetMaximumHitSagitta(double sagitta) {fMaximumHitSagitta = sagitta;}
  double GetMaximumHitSagitta(void) {return fMaximumHitSagitta;}

  /// Set the maximum length for the ND280HitSegment objects created by this
  /// sensitive detector.
  void SetMaximumHitLength(double length) {fMaximumHitLength = length;}
  double GetMaximumHitLength(void) {return fMaximumHitLength;}
  
  ///////////

private:
  ExN02TrackerHitsCollection* trackerCollection;
  
  ////// NEW ND280 HitSegment

  /// The hit collection id of fHits
  int HCID;
  
  /// The maximum allowed sagitta;
  double fMaximumHitSagitta;
  
  /// The maximum allowed length;
  double fMaximumHitLength;
  
  /// The last hit that was found.
  int fLastHit;

  ////////

  
  G4int NSteps_;
  
  // Define the XML input parser
  ExN02ND280XML *ND280XMLInput;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

