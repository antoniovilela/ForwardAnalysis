
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"

const char* DijetsTriggerEvent::name = "DijetsTriggerEvent";

DijetsTriggerEvent::DijetsTriggerEvent() {}

DijetsTriggerEvent::~DijetsTriggerEvent() {}

void DijetsTriggerEvent::reset(){
   nL1Bits_ = -1;

   l1Decision_.clear();
   l1Prescale_.clear();
   l1AlgoName_.clear();
  
   size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
   for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;
 
   hfRingEtSum_.clear();
   hfRingBitCount_.clear();
   hfRingEtSum_ = std::vector<int>(4,-1);
   hfRingBitCount_ = std::vector<int>(4,-1);
  
   leadingJetPt_ = -999.;
   leadingJetEta_ = -999.;
   leadingJetPhi_ = -999.;
  
   secondJetPt_ = -999.;
   secondJetEta_ = -999.;
   secondJetPhi_ = -999.;

   thirdJetPt_ = -999.;
   thirdJetEta_ = -999.;
   thirdJetPhi_ = -999.; 
  
   //ntowCal_ = 0;
   nJet_ = 0;
  
   /*size_t len_towET = sizeof(towET_)/sizeof(int);

   for(size_t k=0; k<len_towET;++k){ 
   towET_[k] = 0.; 
  
  }*/
 /*towET_ = -999.;
   towEta_ = -999.;
   towPhi_ = -999.;
   towE_ = -999.;
   towEm_ = -999.;
   towHad_ = -999.;
   towOe_ = -999.;*/
   /*const int nMaxTower = 10000;
   towET_[nMaxTower] = -999.;
   towEta_[nMaxTower] = -999.;
   towPhi_[nMaxTower] = -999.;
   towE_[nMaxTower] = -999.;
   towEm_[nMaxTower] = -999.;
   towHad_[nMaxTower] = -999.;
   towOe_[nMaxTower] = -999.;*/
}
