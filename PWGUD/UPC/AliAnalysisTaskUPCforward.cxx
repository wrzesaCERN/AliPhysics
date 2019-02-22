/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// c++ headers
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


// root headers
#include <TMath.h>
#include "TH1I.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH1.h"
#include <TFile.h>
#include <TF2.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TTree.h>
#include <TGraph2D.h>
#include <TStopwatch.h>
#include <TMatrixDSym.h>
#include <TFitResult.h>
#include <TLatex.h>
#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TParticle.h"
#include "TObjString.h"
#include "TList.h"
#include "TChain.h"


// aliroot headers
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliMuonTrackCuts.h"
#include "AliAODVertex.h"


// my headers
#include "AliAnalysisTaskUPCforward.h"



class AliAnalysisTaskUPCforward;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskUPCforward) // classimp: necessary for root

//_____________________________________________________________________________
AliAnalysisTaskUPCforward::AliAnalysisTaskUPCforward() : AliAnalysisTaskSE(),
                                                         fAOD(0),
                                                         fOutputList(0),
                                                         fNumberMuonsH(0),
                                                         fCounterH(0),
                                                         fEtaMuonH(0),
                                                         fRAbsMuonH(0),
                                                         fInvariantMassDistributionH(0),
                                                         fInvariantMassDistributionAtDcaH(0),
                                                         fEntriesAgainstRunNumberH(0),
                                                         fEntriesAgainstRunNumberProperlyH(0),
                                                         fInvariantMassDistributionCoherentH(0),
                                                         fInvariantMassDistributionIncoherentH(0),
                                                         fDimuonPtDistributionH(0),
                                                         fZNCEnergyAgainstEntriesH(0),
                                                         fZNAEnergyAgainstEntriesH(0),
                                                         fZNCTimeAgainstEntriesH(0),
                                                         fInvariantMassDistributionNoNeutronsH(0),
                                                         fInvariantMassDistributionOneNeutronH(0),
                                                         fInvariantMassDistributionAtLeastOneNeutronH(0),
                                                         fInvariantMassDistributionCoherentNoNeutronsH(0),
                                                         fInvariantMassDistributionCoherentOneNeutronH(0),
                                                         fInvariantMassDistributionCoherentAtLeastOneNeutronH(0),
                                                         fInvariantMassDistributionIncoherentNoNeutronsH(0),
                                                         fInvariantMassDistributionIncoherentOneNeutronH(0),
                                                         fInvariantMassDistributionIncoherentAtLeastOneNeutronH(0),
                                                         fDcaAgainstInvariantMassH(0),
                                                         fInvariantMassDistributionExtendedH(0),
                                                         fInvariantMassDistributionCoherentExtendedH(0),
                                                         fInvariantMassDistributionIncoherentExtendedH(0),
                                                         fMuonTrackCuts(0x0),
                                                         fRunNum(0),
                                                         fTracklets(0),
                                                         fL0inputs(0),
                                                         fL1inputs(0),
                                                         fZem1Energy(0),
                                                         fZem2Energy(0),
                                                         fZNCEnergy(0),
                                                         fZNAEnergy(0),
                                                         fZPCEnergy(0),
                                                         fZPAEnergy(0),
                                                         fZNATime(0),
                                                         fZNCTime(0),
                                                         fV0ADecision(-10),
                                                         fV0CDecision(-10),
                                                         fADADecision(-10),
                                                         fADCDecision(-10),
                                                         fIR1Map(0),
                                                         fIR2Map(0),
                                                         fZNATDC{0, 0, 0, 0},
                                                         fZNCTDC{0, 0, 0, 0},
                                                         fZPATDC{0, 0, 0, 0},
                                                         fZPCTDC{0, 0, 0, 0},
                                                         fV0Hits{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
                                                         fV0TotalNCells(0),
                                                         fVectorGoodRunNumbers(0),
                                                         fInvariantMassDistributionCoherentZNCzeroZNAzeroH(0),
                                                         fInvariantMassDistributionCoherentZNCzeroZNAanyH(0),
                                                         fInvariantMassDistributionCoherentZNCanyZNAzeroH(0),
                                                         fInvariantMassDistributionCoherentZNCanyZNAanyH(0),
                                                         fInvariantMassDistributionIncoherentZNCzeroZNAzeroH(0),
                                                         fInvariantMassDistributionIncoherentZNCzeroZNAanyH(0),
                                                         fInvariantMassDistributionIncoherentZNCanyZNAzeroH(0),
                                                         fInvariantMassDistributionIncoherentZNCanyZNAanyH(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}

//_____________________________________________________________________________
AliAnalysisTaskUPCforward::AliAnalysisTaskUPCforward(const char* name) : AliAnalysisTaskSE(name),
                                                                         fAOD(0),
                                                                         fOutputList(0),
                                                                         fNumberMuonsH(0),
                                                                         fCounterH(0),
                                                                         fEtaMuonH(0),
                                                                         fRAbsMuonH(0),
                                                                         fInvariantMassDistributionH(0),
                                                                         fInvariantMassDistributionAtDcaH(0),
                                                                         fEntriesAgainstRunNumberH(0),
                                                                         fEntriesAgainstRunNumberProperlyH(0),
                                                                         fInvariantMassDistributionCoherentH(0),
                                                                         fInvariantMassDistributionIncoherentH(0),
                                                                         fDimuonPtDistributionH(0),
                                                                         fZNCEnergyAgainstEntriesH(0),
                                                                         fZNAEnergyAgainstEntriesH(0),
                                                                         fZNCTimeAgainstEntriesH(0),
                                                                         fInvariantMassDistributionNoNeutronsH(0),
                                                                         fInvariantMassDistributionOneNeutronH(0),
                                                                         fInvariantMassDistributionAtLeastOneNeutronH(0),
                                                                         fInvariantMassDistributionCoherentNoNeutronsH(0),
                                                                         fInvariantMassDistributionCoherentOneNeutronH(0),
                                                                         fInvariantMassDistributionCoherentAtLeastOneNeutronH(0),
                                                                         fInvariantMassDistributionIncoherentNoNeutronsH(0),
                                                                         fInvariantMassDistributionIncoherentOneNeutronH(0),
                                                                         fInvariantMassDistributionIncoherentAtLeastOneNeutronH(0),
                                                                         fDcaAgainstInvariantMassH(0),
                                                                         fInvariantMassDistributionExtendedH(0),
                                                                         fInvariantMassDistributionCoherentExtendedH(0),
                                                                         fInvariantMassDistributionIncoherentExtendedH(0),
                                                                         fMuonTrackCuts(0x0),
                                                                         fRunNum(0),
                                                                         fTracklets(0),
                                                                         fL0inputs(0),
                                                                         fL1inputs(0),
                                                                         fZem1Energy(0),
                                                                         fZem2Energy(0),
                                                                         fZNCEnergy(0),
                                                                         fZNAEnergy(0),
                                                                         fZPCEnergy(0),
                                                                         fZPAEnergy(0),
                                                                         fZNATime(0),
                                                                         fZNCTime(0),
                                                                         fV0ADecision(-10),
                                                                         fV0CDecision(-10),
                                                                         fADADecision(-10),
                                                                         fADCDecision(-10),
                                                                         fIR1Map(0),
                                                                         fIR2Map(0),
                                                                         fZNATDC{0, 0, 0, 0},
                                                                         fZNCTDC{0, 0, 0, 0},
                                                                         fZPATDC{0, 0, 0, 0},
                                                                         fZPCTDC{0, 0, 0, 0},
                                                                         fV0Hits{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
                                                                         fV0TotalNCells(0),
                                                                         fVectorGoodRunNumbers(0),
                                                                         fInvariantMassDistributionCoherentZNCzeroZNAzeroH(0),
                                                                         fInvariantMassDistributionCoherentZNCzeroZNAanyH(0),
                                                                         fInvariantMassDistributionCoherentZNCanyZNAzeroH(0),
                                                                         fInvariantMassDistributionCoherentZNCanyZNAanyH(0),
                                                                         fInvariantMassDistributionIncoherentZNCzeroZNAzeroH(0),
                                                                         fInvariantMassDistributionIncoherentZNCzeroZNAanyH(0),
                                                                         fInvariantMassDistributionIncoherentZNCanyZNAzeroH(0),
                                                                         fInvariantMassDistributionIncoherentZNCanyZNAanyH(0)
{
    FillGoodRunVector(fVectorGoodRunNumbers);

    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it,
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskUPCforward::~AliAnalysisTaskUPCforward()
{
    // destructor
    if(fOutputList)    {delete fOutputList;}     	// at the end of your task, it is deleted
    if(fMuonTrackCuts) {delete fMuonTrackCuts;}   // from memory by calling this function
}
//_____________________________________________________________________________
void AliAnalysisTaskUPCforward::FillGoodRunVector(std::vector<Int_t> &fVectorGoodRunNumbers)
{
  fVectorGoodRunNumbers.clear();
  Int_t listOfGoodRunNumbersLHC18q[] = { 295585, 295586, 295587, 295588, 295589, 295612,
                                         295615, 295665, 295666, 295667, 295668, 295671,
                                         295673, 295675, 295676, 295677, 295714, 295716,
                                         295717, 295718, 295719, 295723, 295725, 295753,
                                         295754, 295755, 295758, 295759, 295762, 295763,
                                         295786, 295788, 295791, 295816, 295818, 295819,
                                         295822, 295825, 295826, 295829, 295831, 295854,
                                         295855, 295856, 295859, 295860, 295861, 295863,
                                         295881, 295908, 295909, 295910, 295913, 295936,
                                         295937, 295941, 295942, 295943, 295945, 295947,
                                         296061, 296062, 296063, 296065, 296066, 296068,
                                         296123, 296128, 296132, 296133, 296134, 296135,
                                         296142, 296143, 296191, 296192, 296194, 296195,
                                         296196, 296197, 296198, 296241, 296242, 296243,
                                         296244, 296246, 296247, 296269, 296270, 296273,
                                         296279, 296280, 296303, 296304, 296307, 296309,
                                         296312, 296376, 296377, 296378, 296379, 296380,
                                         296381, 296383, 296414, 296419, 296420, 296423,
                                         296424, 296433, 296472, 296509, 296510, 296511,
                                         296514, 296516, 296547, 296548, 296549, 296550,
                                         296551, 296552, 296553, 296615, 296616, 296618,
                                         296619, 296622, 296623 };
  Int_t listOfGoodRunNumbersLHC18r[] = { 296690, 296691, 296694, 296749, 296750, 296781,
                                         296784, 296785, 296786, 296787, 296791, 296793,
                                         296794, 296799, 296836, 296838, 296839, 296848,
                                         296849, 296850, 296851, 296852, 296890, 296894,
                                         296899, 296900, 296903, 296930, 296931, 296932,
                                         296934, 296935, 296938, 296941, 296966, 296967,
                                         296968, 296969, 296971, 296975, 296976, 296977,
                                         296979, 297029, 297031, 297035, 297085, 297117,
                                         297118, 297119, 297123, 297124, 297128, 297129,
                                         297132, 297133, 297193, 297194, 297196, 297218,
                                         297219, 297221, 297222, 297278, 297310, 297312,
                                         297315, 297317, 297363, 297366, 297367, 297372,
                                         297379, 297380, 297405, 297408, 297413, 297414,
                                         297415, 297441, 297442, 297446, 297450, 297451,
                                         297452, 297479, 297481, 297483, 297512, 297537,
                                         297540, 297541, 297542, 297544, 297558, 297588,
                                         297590, 297595, 297623, 297624 };
  Int_t sizeOfLHC18q = 0;
  Int_t sizeOfLHC18r = 0;
  for ( Int_t GoodRunNumberLHC18q : listOfGoodRunNumbersLHC18q ) {
        fVectorGoodRunNumbers.push_back(GoodRunNumberLHC18q);
        sizeOfLHC18q++;
  }
  for ( Int_t GoodRunNumberLHC18r : listOfGoodRunNumbersLHC18r ) {
        fVectorGoodRunNumbers.push_back(GoodRunNumberLHC18r);
        sizeOfLHC18r++;
  }
  cout << std::endl << "LHC18q GOOD RUNS:  " << std::endl;
  for ( Int_t i = 0; i < sizeOfLHC18q; i++ ) {
        cout << fVectorGoodRunNumbers.at(i) << ",   number: " << i << std::endl;
  }
  cout << std::endl << "LHC18r GOOD RUNS:  " << std::endl;
  for ( Int_t i = sizeOfLHC18q; i < sizeOfLHC18q + sizeOfLHC18r; i++ ) {
        cout << fVectorGoodRunNumbers.at(i) << ",   number: " << (i-sizeOfLHC18q) << std::endl;
  }
}
//_____________________________________________________________________________
void AliAnalysisTaskUPCforward::UserCreateOutputObjects()
{
  // create output objects
  //
  // this function is called ONCE at the start of your analysis (RUNTIME)
  // here you ceate the histograms that you want to use
  //
  // the histograms are in this case added to a tlist, this list is in the end saved
  // to an output file
  //

  //muon track cuts
  fMuonTrackCuts = new AliMuonTrackCuts("StdMuonCuts", "StdMuonCuts");
  fMuonTrackCuts->SetFilterMask(    AliMuonTrackCuts::kMuEta     |
                                    AliMuonTrackCuts::kMuThetaAbs|
                                    AliMuonTrackCuts::kMuPdca    |
                                    AliMuonTrackCuts::kMuMatchLpt   );
  fMuonTrackCuts->SetAllowDefaultParams(kTRUE);
  fMuonTrackCuts->Print("mask");



  fOutputList = new TList();          // this is a list which will contain all
                                      // of your histograms at the end of the
                                      // analysis, the contents of this list
                                      // are written to the output file

  fOutputList->SetOwner(kTRUE);       // memory management: the list is owner
                                      // of all objects it contains and will
                                      // delete them if requested

  //_______________________________
  // - Adding histograms
  fNumberMuonsH = new TH1F("fNumberMuonsH", "fNumberMuonsH", 12, -0.5, 11.5);
  fOutputList->Add(fNumberMuonsH);    // don't forget to add it to the list!

  fCounterH = new TH1F("fCounterH", "fCounterH", 24, -0.5, 23.5);
  fOutputList->Add(fCounterH);

  fEtaMuonH = new TH1F("fEtaMuonH", "fEtaMuonH", 90, -2, -5);
  fOutputList->Add(fEtaMuonH);

  fRAbsMuonH = new TH1F("fRAbsMuonH", "fRAbsMuonH", 100, 0, 100);
  fOutputList->Add(fRAbsMuonH);

  fInvariantMassDistributionH = new TH1F("fInvariantMassDistributionH", "fInvariantMassDistributionH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionH);

  fInvariantMassDistributionAtDcaH = new TH1F("fInvariantMassDistributionAtDcaH", "fInvariantMassDistributionAtDcaH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionAtDcaH);

  fEntriesAgainstRunNumberH = new TH1F("fEntriesAgainstRunNumberH", "fEntriesAgainstRunNumberH", 10000, 290000, 300000);
  fOutputList->Add(fEntriesAgainstRunNumberH);

  /* - Trying to reproduce the histogram for the RunNumbers as they always
     - show it, properly labelled. Inspiration has come from the website:
     - https://root.cern.ch/doc/master/hlabels1_8C.html
     - Let us see if it works properly.
     -
   */
  fEntriesAgainstRunNumberProperlyH = new TH1F("fEntriesAgainstRunNumberProperlyH", "fEntriesAgainstRunNumberProperlyH", 10000, 290000, 300000);
  fEntriesAgainstRunNumberProperlyH->SetStats(0);
  fEntriesAgainstRunNumberProperlyH->SetFillColor(38);
  // fEntriesAgainstRunNumberProperlyH->SetCanExtend(TH1::kAllAxes);
  fEntriesAgainstRunNumberProperlyH->LabelsDeflate();
  fOutputList->Add(fEntriesAgainstRunNumberProperlyH);

  fInvariantMassDistributionCoherentH = new TH1F("fInvariantMassDistributionCoherentH", "fInvariantMassDistributionCoherentH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentH);

  fInvariantMassDistributionIncoherentH = new TH1F("fInvariantMassDistributionIncoherentH", "fInvariantMassDistributionIncoherentH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentH);

  fDimuonPtDistributionH = new TH1F("fDimuonPtDistributionH", "fDimuonPtDistributionH", 2000, 0, 20);
  fOutputList->Add(fDimuonPtDistributionH);

  fZNCEnergyAgainstEntriesH = new TH1F("fZNCEnergyAgainstEntriesH", "fZNCEnergyAgainstEntriesH", 1000, -500, 2000);
  fOutputList->Add(fZNCEnergyAgainstEntriesH);

  fZNAEnergyAgainstEntriesH = new TH1F("fZNAEnergyAgainstEntriesH", "fZNAEnergyAgainstEntriesH", 1000, -500, 2000);
  fOutputList->Add(fZNAEnergyAgainstEntriesH);

  fZNCTimeAgainstEntriesH = new TH1F("fZNCTimeAgainstEntriesH", "fZNCTimeAgainstEntriesH", 6000, -1500, 1500);
  fOutputList->Add(fZNCTimeAgainstEntriesH);

  fInvariantMassDistributionNoNeutronsH = new TH1F("fInvariantMassDistributionNoNeutronsH", "fInvariantMassDistributionNoNeutronsH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionNoNeutronsH);

  fInvariantMassDistributionOneNeutronH = new TH1F("fInvariantMassDistributionOneNeutronH", "fInvariantMassDistributionOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionOneNeutronH);

  fInvariantMassDistributionAtLeastOneNeutronH = new TH1F("fInvariantMassDistributionAtLeastOneNeutronH", "fInvariantMassDistributionAtLeastOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionAtLeastOneNeutronH);

  fInvariantMassDistributionCoherentNoNeutronsH = new TH1F("fInvariantMassDistributionCoherentNoNeutronsH", "fInvariantMassDistributionCoherentNoNeutronsH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentNoNeutronsH);

  fInvariantMassDistributionCoherentOneNeutronH = new TH1F("fInvariantMassDistributionCoherentOneNeutronH", "fInvariantMassDistributionCoherentOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentOneNeutronH);

  fInvariantMassDistributionCoherentAtLeastOneNeutronH = new TH1F("fInvariantMassDistributionCoherentAtLeastOneNeutronH", "fInvariantMassDistributionCoherentAtLeastOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentAtLeastOneNeutronH);

  fInvariantMassDistributionIncoherentNoNeutronsH = new TH1F("fInvariantMassDistributionIncoherentNoNeutronsH", "fInvariantMassDistributionIncoherentNoNeutronsH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentNoNeutronsH);

  fInvariantMassDistributionIncoherentOneNeutronH = new TH1F("fInvariantMassDistributionIncoherentOneNeutronH", "fInvariantMassDistributionIncoherentOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentOneNeutronH);

  fInvariantMassDistributionIncoherentAtLeastOneNeutronH = new TH1F("fInvariantMassDistributionIncoherentAtLeastOneNeutronH", "fInvariantMassDistributionIncoherentAtLeastOneNeutronH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentAtLeastOneNeutronH);

  fDcaAgainstInvariantMassH = new TH2F("fDcaAgainstInvariantMassH", "fDcaAgainstInvariantMassH", 4000, 0, 40, 2000, -100, 100);
  fOutputList->Add(fDcaAgainstInvariantMassH);

  /* - These histograms have an EXTENDED range (0,20)->(0,40)
     -
   */
  fInvariantMassDistributionExtendedH = new TH1F("fInvariantMassDistributionExtendedH", "fInvariantMassDistributionExtendedH", 4000, 0, 40);
  fOutputList->Add(fInvariantMassDistributionExtendedH);

  fInvariantMassDistributionCoherentExtendedH = new TH1F("fInvariantMassDistributionCoherentExtendedH", "fInvariantMassDistributionCoherentExtendedH", 4000, 0, 40);
  fOutputList->Add(fInvariantMassDistributionCoherentExtendedH);

  fInvariantMassDistributionIncoherentExtendedH = new TH1F("fInvariantMassDistributionIncoherentExtendedH", "fInvariantMassDistributionIncoherentExtendedH", 4000, 0, 40);
  fOutputList->Add(fInvariantMassDistributionIncoherentExtendedH);


  /* - These histograms pertain the differential neutron emission analysis.
     -
   */
  fInvariantMassDistributionCoherentZNCzeroZNAzeroH = new TH1F("fInvariantMassDistributionCoherentZNCzeroZNAzeroH", "fInvariantMassDistributionCoherentZNCzeroZNAzeroH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentZNCzeroZNAzeroH);

  fInvariantMassDistributionCoherentZNCzeroZNAanyH = new TH1F("fInvariantMassDistributionCoherentZNCzeroZNAanyH", "fInvariantMassDistributionCoherentZNCzeroZNAanyH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentZNCzeroZNAanyH);

  fInvariantMassDistributionCoherentZNCanyZNAzeroH = new TH1F("fInvariantMassDistributionCoherentZNCanyZNAzeroH", "fInvariantMassDistributionCoherentZNCanyZNAzeroH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentZNCanyZNAzeroH);

  fInvariantMassDistributionCoherentZNCanyZNAanyH = new TH1F("fInvariantMassDistributionCoherentZNCanyZNAanyH", "fInvariantMassDistributionCoherentZNCanyZNAanyH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionCoherentZNCanyZNAanyH);

  fInvariantMassDistributionIncoherentZNCzeroZNAzeroH = new TH1F("fInvariantMassDistributionIncoherentZNCzeroZNAzeroH", "fInvariantMassDistributionIncoherentZNCzeroZNAzeroH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentZNCzeroZNAzeroH);

  fInvariantMassDistributionIncoherentZNCzeroZNAanyH = new TH1F("fInvariantMassDistributionIncoherentZNCzeroZNAanyH", "fInvariantMassDistributionIncoherentZNCzeroZNAanyH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentZNCzeroZNAanyH);

  fInvariantMassDistributionIncoherentZNCanyZNAzeroH = new TH1F("fInvariantMassDistributionIncoherentZNCanyZNAzeroH", "fInvariantMassDistributionIncoherentZNCanyZNAzeroH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentZNCanyZNAzeroH);

  fInvariantMassDistributionIncoherentZNCanyZNAanyH = new TH1F("fInvariantMassDistributionIncoherentZNCanyZNAanyH", "fInvariantMassDistributionIncoherentZNCanyZNAanyH", 2000, 0, 20);
  fOutputList->Add(fInvariantMassDistributionIncoherentZNCanyZNAanyH);


  //_______________________________
  // - End of the function
  PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the
  // fOutputList object. the manager will in the end take care of writing your output to file
  // so it needs to know what's in the output
}
//_____________________________________________________________________________
void AliAnalysisTaskUPCforward::NotifyRun()
{
  /// Set run number for cuts
  fMuonTrackCuts->SetRun(fInputHandler);
}
//_____________________________________________________________________________
void AliAnalysisTaskUPCforward::UserExec(Option_t *)
{
  /* - This iSelectionCounter is used as a token. So at every passing step it is
     - increased by one. All the events are supposed to pass the first step
     - obviously, but only a few get to the end. This is effect is clearly
     - noticeable in fCounterH event with the small trial local sample.
     - Almost 160k possible events at the 0-th step, while only 2k at the 4th step.
   */
  Int_t iSelectionCounter = 0; // no selection applied yet
  fCounterH->Fill(iSelectionCounter); // entering UserExec
  iSelectionCounter++;

  // get AOD event
  fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
  if(!fAOD) {
      PostData(1, fOutputList);
      return;
  }
  fCounterH->Fill(iSelectionCounter); // AOD event found
  iSelectionCounter++;

  /* - Is it the right trigger?
     - In 2018 there were the following CMUP triggers:
     - CMUP11-B-NOPF-MUFAST,
     - CMUP26-B-NOPF-MUFAST,
     - CMUP6-B-NOPF-MUFAST.
     - At this 2nd step of fCounterH, all events are in fact proper events,
     - and with the correct trigger as well.
   */
  TString trigger = fAOD->GetFiredTriggerClasses();
  if (    !(trigger.Contains("CMUP11-B-NOPF-MUFAST") ||
	          trigger.Contains("CMUP26-B-NOPF-MUFAST") ||
	          trigger.Contains("CMUP6-B-NOPF-MUFAST"))
          )  {
                    PostData(1, fOutputList);
                    return;
  }
  fCounterH->Fill(iSelectionCounter); // right trigger found
  iSelectionCounter++;

  /* - We are now checking if there were any tracks. If there were at least one,
     - then the histogram gets filled again. If not we are returning. There
     - would be no point in going further.
   */
  Int_t nTracks(fAOD->GetNumberOfTracks());
  if(nTracks<1) {
        PostData(1, fOutputList);
        return;
  }
  fCounterH->Fill(iSelectionCounter); // At least one track
  iSelectionCounter++;


  //_______________________________
  // EVENT DATA EXTRACTION
  /* - Eugeny Krishen's event data extraction. I am trying to implement it.
     - The only thing I am a bit worried about is whether it should go before or
     - after the "nTracks<1" check... I will try and switch it if it sounds
     - better. These data are used for the event selection and maybe later on
     - for track selection, but I did not get to that part yet. If after all of
     - this I remember to do so, I will come back to this point and correct this
     - statement. If you find this part, please, keep in mind to check the
     - following.
   */

  /* - Event information:
     - Run Number, maybe to select the GOOD Runs and discard the others;
     - Number of Tracklets, these are in this case the SPD tracklets, so the
     - almost unit vector roughly 2 cm between two pixels of the SPD in different
     - layers.
   */
  fRunNum    = fAOD->GetRunNumber();
  fTracklets = fAOD->GetTracklets()->GetNumberOfTracklets();

  /* - Trigger Inputs:
     - L0: ..... ;
     - L1: ..... .
   */
  fL0inputs = fAOD->GetHeader()->GetL0TriggerInputs();
  fL1inputs = fAOD->GetHeader()->GetL1TriggerInputs();

  /* - Past-future protection maps:
     - IR1: .... ;
     - IR2: .... .
   */
  fIR1Map = fAOD->GetHeader()->GetIRInt1InteractionMap();
  fIR2Map = fAOD->GetHeader()->GetIRInt2InteractionMap();

  /* - ZDC: we try to find the ZDC object data in the nano-AOD. If we cannot,
     - we return, because there would be no way to actually select the events
     - otherwise! We are here, so we could even check if there is a discrepancy
     - between good events with and without ZDC's information. Or at least, this
     - is my impression of it (filling fCounterH). ZDC information:
     - fZem1Energy:
     - fZem2Energy:
     - fZNAEnergy:
     - fZNCEnergy:
     - fZPAEnergy:
     - fZPCEnergy:
     - fZNATime:
     - fZNCTime:
     - fZNATDC[i]:
     - fZNCTDC[i]:
     - fZPATDC[i]:
     - fZPCTDC[i]:
   */
  AliAODZDC *dataZDC = dynamic_cast<AliAODZDC*>(fAOD->GetZDCData());
  if(!dataZDC) {
        PostData(1, fOutputList);
        return;
  }
  fCounterH->Fill(iSelectionCounter);
  iSelectionCounter++;

  fZem1Energy = dataZDC->GetZEM1Energy();
  fZem2Energy = dataZDC->GetZEM2Energy();
  fZNAEnergy  = dataZDC->GetZNATowerEnergy()[0];
  fZNCEnergy  = dataZDC->GetZNCTowerEnergy()[0];
  fZPAEnergy  = dataZDC->GetZPATowerEnergy()[0];
  fZPCEnergy  = dataZDC->GetZPCTowerEnergy()[0];

  fZNATime    = dataZDC->GetZNATime();
  fZNCTime    = dataZDC->GetZNCTime();

  for (Int_t i=0;i<4;i++) fZNATDC[i] = dataZDC->GetZNATDCm(i);
  for (Int_t i=0;i<4;i++) fZNCTDC[i] = dataZDC->GetZNCTDCm(i);
  for (Int_t i=0;i<4;i++) fZPATDC[i] = dataZDC->GetZPATDCm(i);
  for (Int_t i=0;i<4;i++) fZPCTDC[i] = dataZDC->GetZPCTDCm(i);

  /* - V0: we try to find the V0 object data in the nano-AOD. If we cannot,
     - we return, because there would be no way to actually select the events
     - otherwise! We are here, so we could even check if there is a discrepancy
     - between good events with and without V0's information. Or at least, this
     - is my impression of it (filling fCounterH). V0 information:
     - fV0ADecision: ..... ;
     - fV0CDecision: ..... .
     -
     -
     -
     - Plot the V0 variables to try to understand whether it is cells we are
     - talking about or boolean responses or something else altogether.
  */
  AliVVZERO *dataVZERO = dynamic_cast<AliVVZERO*>(fAOD->GetVZEROData());
  if(!dataVZERO) {
        PostData(1, fOutputList);
        return;
  }
  fCounterH->Fill(iSelectionCounter);
  iSelectionCounter++;

  fV0ADecision = dataVZERO->GetV0ADecision();
  fV0CDecision = dataVZERO->GetV0CDecision();


  //_____________________________________
  // RUN SELECTION
  /* - This part is the run selection. We call the std::find() method of the
     - <algorithm> library STL. This returns a kTRUE if you find the value you
     - are looking for inside the vector. So, what happens is that I look for
     - the Run Numbers inside the vector containing them. If I cannot find
     - them I move on to the next event.
     -
   */
   auto findRunNumber = std::find(  std::begin(fVectorGoodRunNumbers),
                                    std::end(fVectorGoodRunNumbers),
                                    fRunNum
                                    );
   if (findRunNumber != std::end(fVectorGoodRunNumbers)) {
        // std::cout << "fVectorGoodRunNumbers DOES     contain: " << fRunNum << std::endl;
   } else {
        // std::cout << "fVectorGoodRunNumbers does not contain: " << fRunNum << std::endl;
        PostData(1, fOutputList);
        return;
   }
  // END RUN SELECTION
  //_____________________________________



  /* - We have to get the number of fired V0C cells. So firstly, we get the
     - boolean information about the hit cells for all V0. This is done through
     - the GetBBFlag(i) method, where 0<i<32 stands for the V0C cells and
     - 32<i<64 for the V0A cells. Then I thought the easiest way to check
     - whether the number of fired V0C cells is above 2 is just to add up the
     - boolean numbers for 0<i<32. Let's see.
     -
     - Weird fact: this doesn't seem to work... I have changed it so that if
     - the single cell has recorded a signal (kTRUE) then it adds up to the
     - total number of cells. Hope for the best.
     -
     - I am an idiot!!!!!! I have to reset the variable everytime!!!!
   */
  fV0TotalNCells = 0;
  for(Int_t iV0Hits = 0; iV0Hits < 64; iV0Hits++) {
        fV0Hits[iV0Hits] = dataVZERO->GetBBFlag(iV0Hits);
        if(fV0Hits[iV0Hits] == kTRUE) {
              // if(iV0Hits < 32) fV0TotalNCells += fV0Hits[iV0Hits];
              if(iV0Hits < 32) fV0TotalNCells += 1;
        }
        // std::cout << "fV0Hits[iV0Hits = " << iV0Hits << ", fRunNum=" << fRunNum << "] = " << fV0Hits[iV0Hits] << endl;
        // std::cout << "fV0TotalNCells (fRunNum = " << fRunNum << ") = " << fV0TotalNCells << endl;
  }

  /* - AD: we try to find the AD object data in the nano-AOD. If we cannot,
     - we return, because there would be no way to actually select the events
     - otherwise! We are here, so we could even check if there is a discrepancy
     - between good events with and without AD's information. Or at least, this
     - is my impression of it (filling fCounterH). AD information:
     - fADADecision: small detector in ALICE, ADA and ADC at large distances;
     - fADCDecision: again, maybe check whether it is cells or boolean, same as V0.
  */
  // AD
  AliVAD *dataAD = dynamic_cast<AliVAD*>(fAOD->GetADData());
  if(dataAD) {
        fCounterH->Fill(iSelectionCounter);
        iSelectionCounter++;

        fADADecision = dataAD->GetADADecision();
        fADCDecision = dataAD->GetADCDecision();
  }
  // END EVENT DATA EXTRACTION
  //_______________________________
  // EVENT SELECTION
  /* - This is Eugeny Krishen's event selection from the talk in 14/1/2019 for
     - the PWG-UD (UPC oriented) meeting. The event selection requires:
     - CMUP11-B triggers;
     - Maximum 2 V0C cells fired;
     - Empty V0A decision;
     - Empty ADA decision;
     - Empty ADC decision;
     - 0 tracklets in SPD;
     - Exactly 2 unlike-sign muons;
   */
  /* - CMUP11-B triggers: I have to check with my supervisor, but this requirement
     - may have already been satisfied with the requirements for the trigger info
   */
  /* - Maximum 2 V0C cells fired:
     -
   */
  /* - Empty V0A decision
     - Empty ADA decision
     - Empty ADC decision
   */
  if(fV0ADecision != 0) {
       PostData(1, fOutputList);
       return;
  }
  if(fADADecision != 0) {
       PostData(1, fOutputList);
       return;
  }
  if(fADCDecision != 0) {
       PostData(1, fOutputList);
       return;
  }
  /* - 0 tracklets in SPD
     - Is it like this?? Not too sure what fTracklets was!
   */
  if(fTracklets != 0) {
       PostData(1, fOutputList);
       return;
  }
  /* - Maximum 2 V0C cells fired.
     -
     - Trying a more readable and immediate approach.
   */
  // if( !(fV0TotalNCells < 2) ) {
  //      PostData(1, fOutputList);
  //      return;
  // }
  if( fV0TotalNCells > 2 ) {
       PostData(1, fOutputList);
       return;
  }



  /* - We are finally at the starting point. We loop over the tracks and select
     - the good muons. Later on everything should happen in this loop. Let us
     - see what the future has in hold.
     -
     - Saturday: I moved the creation of the AliAODTrack* track outside of the
     - loop as it would have been otherwise created for each single iteration.
     - This could have caused massive memory issues especially to grid. I have
     - added a second AliAODTrack* track[2] to hold the second supposed muon.
     - Now this is ready to send the information to two TLorentzVectors to
     - obtain the invariant mass of the J/Psi through the Mag() method of the
     - class. Hope for the best.
   */
  Int_t nGoodMuons = 0;
  AliAODTrack* track[2];
  track[0]         = 0x0;
  track[1]         = 0x0;
  for(Int_t iTrack(0); iTrack < nTracks; iTrack++) {
    /* - This should be another form of event selection.
       - I am basically requesting the presence of TWO good muons only.
       - Later I will be checking whether of they are likesign or unlikesign.
     */
    if(nGoodMuons > 2) {
         PostData(1, fOutputList);
         return;
    }
    // get track
    // AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(iTrack));
    // if(!track) return;
    //
    // // is it a good muon track?
    // if(!track->IsMuonTrack()) continue;
    // if(!fMuonTrackCuts->IsSelected(track)) continue;

    track[nGoodMuons] = static_cast<AliAODTrack*>(fAOD->GetTrack(iTrack));
    if(!track[nGoodMuons]) return;

    // is it a good muon track?
    if(!track[nGoodMuons]->IsMuonTrack()) {
        // track[nGoodMuons] = 0x0;
        continue;
    }
    if(!fMuonTrackCuts->IsSelected(track[nGoodMuons])) {
        // track[nGoodMuons] = 0x0;
        continue;
    }

    // MUON SELECTION
    /* - This is Eugeny Krishen's MUON selection from the talk in 14/1/2019 for
       - the PWG-UD (UPC oriented) meeting. The event selection requires:
       - Muon trigger matching >=2 (1 GeV/c threshold);
       - (-4) < eta < (-2.5);
       - (17.5 cm) < R_{abs} < (89.5 cm);
       - p \times DCA cut;
    */

    // increase counter
    nGoodMuons++;

    // fill muon info
    // fEtaMuonH->Fill(track->Eta());
    // fRAbsMuonH->Fill(track->GetRAtAbsorberEnd());

  }
  /* - We need EXACTLY 2 good muons !!!!!
     -
   */
  if( nGoodMuons != 2 ) {
        PostData(1, fOutputList);
        return;
  }
  /* - Implementing the track cut on the unlike muons
   * -
   */
  if( (track[0]->Charge()) == (track[1]->Charge()) ) {
        PostData(1, fOutputList);
        return;
  }
  for(Int_t iFilling = 0; iFilling < nGoodMuons; iFilling++) {
        fEtaMuonH ->Fill(track[iFilling]->Eta());
        fRAbsMuonH->Fill(track[iFilling]->GetRAtAbsorberEnd());
  }
  // store muons
  fNumberMuonsH->Fill(nGoodMuons);
  fEntriesAgainstRunNumberH->Fill(fRunNum);
  /* - This is the last part of my try to obtain a proper RunNumbers histogram...
     -
   */
  fEntriesAgainstRunNumberProperlyH->Fill( Form("%d", fRunNum) , 1 );
  if (nGoodMuons>0) fCounterH->Fill(iSelectionCounter); // At least one good muon
  iSelectionCounter++;



  /* - Finally the core!!!
   * - What will be happening is that we will instantiate TLorentzVectors to
   * - obtain the invariant mass of the dimuon system. If everything goes fine
   * - after this we should be able to obtain the peak of the J/Psi. But
   * - things never go as expected, so who knows!
   */
  TLorentzVector muons[2];
  TLorentzVector possibleJPsi;
  for(int indexMuon = 0; indexMuon < 2; indexMuon++) {
        muons[indexMuon].SetPtEtaPhiM(   track[indexMuon]->Pt(),
                                         track[indexMuon]->Eta(),
                                         track[indexMuon]->Phi(),
                                         TDatabasePDG::Instance()->GetParticle(13)->Mass()
                                       );
        possibleJPsi += muons[indexMuon];
  }
  fInvariantMassDistributionH->Fill(possibleJPsi.Mag());
  fInvariantMassDistributionExtendedH->Fill(possibleJPsi.Mag());

  /* - This is a TH2F histogram filled with DCA against the invariant mass of
     - the dimuon pair. This should be plotted as LEGO or as a heat map...
     - Is this the correct way of doing it?? I have to ask to my supervisor!!
     -
   */
  for(int indexMuonForDCA = 0; indexMuonForDCA < 2; indexMuonForDCA++) {
        fDcaAgainstInvariantMassH->Fill(possibleJPsi.Mag(), track[indexMuonForDCA]->DCA());
  }

  /* - Now we are evaluating the pt of the dimuon pair. Generally speaking,
     - if such a pt is less than 0.25 GeV/c then it fills the coherent
     - component, otherwise the incoherent component. At this point we may fill
     - even the dimuon pt distribution histogram and see if it looks like Kay's.
     -
   */
  Double_t ptOfTheDimuonPair = possibleJPsi.Pt();
  if( ptOfTheDimuonPair < 0.25) {
        fInvariantMassDistributionCoherentH->Fill(possibleJPsi.Mag());
        fInvariantMassDistributionCoherentExtendedH->Fill(possibleJPsi.Mag());
  } else {
        fInvariantMassDistributionIncoherentH->Fill(possibleJPsi.Mag());
        fInvariantMassDistributionIncoherentExtendedH->Fill(possibleJPsi.Mag());
  }
  fDimuonPtDistributionH->Fill(ptOfTheDimuonPair);





  //_____________________________________
  // STARTING THE CHECK FOR THE UPC GROUP
  /* - Here we will be using the information about the fMomentumAtDCA, to try
     - and see if there are any differences compared to before. Hopefully this
     - will be doing something. Maybe not. However there is already a major
     - difference with the way they are implemented. Here,
     - fMomentumAtDCA = [px, py, pz], while
     - fMomentum      = [pt, eta,phi]
     -
     -
     - RESULTS: the information here is too imprecise to be properly used.
     -          Switching to the usual fMomentum only. With the information
     -          at the DCA it is impossible to resolve the PsiPrime peak.
     -
   */
  TLorentzVector muonsAtDCA[2];
  TLorentzVector possibleJPsiAtDCA;
  for(int indexMuonAtDCA = 0; indexMuonAtDCA < 2; indexMuonAtDCA++) {
        /* - If dummy values then we cannot compute the J/Psi...
           - The dummy values are -999. but I do not trust the threshold for the
           - comparison... Better safe than sorry.
         */
        if( (track[indexMuonAtDCA]->PxAtDCA()) < -998. ) {
              PostData(1, fOutputList);
              return;
        }
        if( (track[indexMuonAtDCA]->PyAtDCA()) < -998. ) {
              PostData(1, fOutputList);
              return;
        }
        if( (track[indexMuonAtDCA]->PzAtDCA()) < -998. ) {
              PostData(1, fOutputList);
              return;
        }
        Double_t EnergyOfTheTrack = TMath::Sqrt(  track[indexMuonAtDCA]->PxAtDCA()*track[indexMuonAtDCA]->PxAtDCA() +
                                                  track[indexMuonAtDCA]->PyAtDCA()*track[indexMuonAtDCA]->PyAtDCA() +
                                                  track[indexMuonAtDCA]->PzAtDCA()*track[indexMuonAtDCA]->PzAtDCA() +
                                                  TDatabasePDG::Instance()->GetParticle(13)->Mass()*TDatabasePDG::Instance()->GetParticle(13)->Mass()/1000000
                                                  );
        muonsAtDCA[indexMuonAtDCA].SetPxPyPzE(   track[indexMuonAtDCA]->PxAtDCA(),
                                                 track[indexMuonAtDCA]->PyAtDCA(),
                                                 track[indexMuonAtDCA]->PzAtDCA(),
                                                 EnergyOfTheTrack
                                                 );
        possibleJPsiAtDCA += muonsAtDCA[indexMuonAtDCA];
  }
  fInvariantMassDistributionAtDcaH->Fill(possibleJPsiAtDCA.Mag());
  // END THE CHECK
  //_____________________________________




  /* - Now this is a critical part of  the analysis. What happens next is a
     - differential analysisin terms of the energy perceived by the neutron ZDC.
     - What it means is that now we may cut on those sensible values to select
     - only those J/Psi candidates falling under a certain peak of the neutron
     - ZNC energy distribution. It will be seen that the fZNCEnergyAgainstEntriesH
     - plot will present many gaussian-like peaks. Each peak represent an
     - increasingly large number of neutrons seen by the ZNC.
     -
     - Starting from the first peak, 0n, then 1n, hopefully 2n, but anything
     - else is more like a guess. If my understanding is good enough, even the
     - 2n peak requires user input to facilitate the minimizer's job.
     -
     - So, first thing first, Guillermo Contreras has suggested the preliminary
     - cut on the ZDC time, quoting:
     - "The energy value makes sense only when the time information is not
     - -999... You can choose times |t|<5 ns to plot the energy distributions
     - in the neutron ZDC".
     -
     - This happens with the request |fZNCTime|<5 if I have understood correctly.
     - After this we can fill whatever histogram we want to.
     -
   */
  fZNCTimeAgainstEntriesH->Fill(fZNCTime);
  if( fZNCTime > -5.0 ) {
    if( fZNCTime < 5.0 ) {
          /* At any levels, this means |fZNCTime| < 5. */
          fZNCEnergyAgainstEntriesH->Fill(fZNCEnergy);
          fZNAEnergyAgainstEntriesH->Fill(fZNAEnergy);

          /* - Now this offers the oppurtunity to do differential mass studies.
             - This can be seen here. When we try to do everything while cutting
             - on the ZNC energy.
             -
             - I don't if by the time you will be reading these lines of mine
             - the ZDC calibration will be done or not. For now I am Implementing
             - the cut based on Evgeny Kryshen's plot. Then we will see in the
             - future.
             -
           */
          if( fZNCEnergy > -300 ) {
                    if( fZNCEnergy < 125 ) {
                            fInvariantMassDistributionNoNeutronsH->Fill(possibleJPsi.Mag());
                            if( ptOfTheDimuonPair < 0.25) {
                                  fInvariantMassDistributionCoherentNoNeutronsH->Fill(possibleJPsi.Mag());
                            } else {
                                  fInvariantMassDistributionIncoherentNoNeutronsH->Fill(possibleJPsi.Mag());
                            }
                    } else if( fZNCEnergy < 375 ) {
                            fInvariantMassDistributionOneNeutronH->Fill(possibleJPsi.Mag());
                            fInvariantMassDistributionAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            if( ptOfTheDimuonPair < 0.25) {
                                  fInvariantMassDistributionCoherentOneNeutronH->Fill(possibleJPsi.Mag());
                                  fInvariantMassDistributionCoherentAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            } else {
                                  fInvariantMassDistributionIncoherentOneNeutronH->Fill(possibleJPsi.Mag());
                                  fInvariantMassDistributionIncoherentAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            }
                    } else  {
                            fInvariantMassDistributionAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            if( ptOfTheDimuonPair < 0.25) {
                                  fInvariantMassDistributionCoherentAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            } else {
                                  fInvariantMassDistributionIncoherentAtLeastOneNeutronH->Fill(possibleJPsi.Mag());
                            }
                    }
          }
    }
  }

  //_____________________________________
  // DIFFERENTIAL ANALYSIS NEUTRON EMISSION
  if( fZNCTime > -5.0 ) {
    if( fZNCTime < 5.0 ) {
          /* At any levels, this means |fZNCTime| < 5. */
          if( fZNCEnergy > -300 ) {
                      if( fZNCEnergy < 125 ) {
                                  if( fZNAEnergy > -300 ) {
                                              if( fZNAEnergy < 125 ) {
                                                          if( ptOfTheDimuonPair < 0.25) {
                                                                    fInvariantMassDistributionCoherentZNCzeroZNAzeroH->Fill(possibleJPsi.Mag());
                                                          } else {
                                                                    fInvariantMassDistributionIncoherentZNCzeroZNAzeroH->Fill(possibleJPsi.Mag());
                                                          }
                                              } else {
                                                          if( ptOfTheDimuonPair < 0.25) {
                                                                    fInvariantMassDistributionCoherentZNCzeroZNAanyH->Fill(possibleJPsi.Mag());
                                                          } else {
                                                                    fInvariantMassDistributionIncoherentZNCzeroZNAanyH->Fill(possibleJPsi.Mag());
                                                          }
                                              }
                                  }
                      } else {
                                  if( fZNAEnergy > -300 ) {
                                              if( fZNAEnergy < 125 ) {
                                                          if( ptOfTheDimuonPair < 0.25) {
                                                                    fInvariantMassDistributionCoherentZNCanyZNAzeroH->Fill(possibleJPsi.Mag());
                                                          } else {
                                                                    fInvariantMassDistributionIncoherentZNCanyZNAzeroH->Fill(possibleJPsi.Mag());
                                                          }
                                              } else {
                                                          if( ptOfTheDimuonPair < 0.25) {
                                                                    fInvariantMassDistributionCoherentZNCanyZNAanyH->Fill(possibleJPsi.Mag());
                                                          } else {
                                                                    fInvariantMassDistributionIncoherentZNCanyZNAanyH->Fill(possibleJPsi.Mag());
                                                          }
                                              }
                                  }

                      }
          }
    }
  }


  // post the data
  PostData(1, fOutputList);
}
//_____________________________________________________________________________
void AliAnalysisTaskUPCforward::Terminate(Option_t *)
{
    cout << endl;
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
