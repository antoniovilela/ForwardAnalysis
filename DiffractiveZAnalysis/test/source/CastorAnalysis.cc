//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsCastorAnalysissAnalysis#Macro_Analysis
//

//#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "CastorAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace diffractiveZAnalysis;
using namespace eventInfo;
using namespace reweight;

#define PI 3.14159265

static inline void loadBar(int x, int n, int r, int w)
{
  // Modified
  // http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/

  // Only update r times.
  if ( x % (n/r) != 0 ) return;

  // Calculuate the correlation of complete-to-incomplete.
  double correlation = x/(double)n;
  int   c     = correlation * w;

  // Show the percentage complete.
  printf("%3d%%[", (int)(correlation*100) );

  // Show the load bar.
  for (int x=0; x<c; x++)
    printf("=");

  for (int x=c; x<w; x++)
    printf(" ");

  // ANSI Control codes to go back to the
  // previous line and clear it.
  // printf("]\n33[F33[J");

  printf("\r"); // Move to the first column
  fflush(stdout);
}


void CastorAnalysis::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  std::string fdirectory = processinput + "/ProcessedTree";
  tr = (TTree*)inf->Get(fdirectory.c_str());
  eventdiff = new DiffractiveEvent();
  eventCastor = new DiffractiveZEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  Castor = tr->GetBranch("DiffractiveZAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  Castor->SetAddress(&eventCastor);
  info->SetAddress(&eventinfo);

}

void CastorAnalysis::CreateHistos(){

  std::string step0 = "without_cuts"; 
  std::string step1 = "with_trigger"; 
  std::string step2 = "step2"; 
  std::string step3 = "step3"; 
  std::string step4 = "step4"; 
  std::string step5 = "step5"; 
  std::string step6 = "step6"; 
  std::string step7 = "step7"; 
  std::string step8 = "GapHFMinus";
  std::string step9 = "GapHFPlus";

  Folders.push_back(step0);
  Folders.push_back(step1);
  Folders.push_back(step2);
  Folders.push_back(step3);
  Folders.push_back(step4);
  Folders.push_back(step5);
  Folders.push_back(step6);
  Folders.push_back(step7);
  Folders.push_back(step8);
  Folders.push_back(step9);

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

    char castor_centroid[300];
    sprintf(castor_centroid,"CastorCentroid_%s",Folders.at(j).c_str());
    TH2F *histo_castor_centroid = new TH2F(castor_centroid,"Castor Centroid Energy; x[cm]; y[cm]",30,-15,15,30,-15,15);
    m_hVector_histo_castor_centroid.push_back(histo_castor_centroid);

    char castor_centroid_phi[300];
    sprintf(castor_centroid_phi,"CastorCentroidPhi_%s",Folders.at(j).c_str());
    TH1F *histo_castor_centroid_phi = new TH1F(castor_centroid_phi,"Castor Centroid Energy; Sector(#phi,i) [degree]; NEvents",720,0,360);
    m_hVector_histo_castor_centroid_phi.push_back(histo_castor_centroid_phi);

    char name1[300];
    sprintf(name1,"ECaloVsEta_%s",Folders.at(j).c_str());
    TH2F *histo_ECaloVsEta = new TH2F(name1,"Calorimeter Energy X #eta; #eta; Energy [GeV]", 500, -6, 6, 100, 0., 1000.);
    m_hVector_ECaloVsEta.push_back(histo_ECaloVsEta);

    char name2[300];
    sprintf(name2,"sumECastorMinus_%s",Folders.at(j).c_str());
    TH1F *histo_sumECastorMinus = new TH1F(name2,"Castor Sum of Energy; Energy [GeV]; N events",6000,0,3000);
    m_hVector_sumECastorMinus.push_back(histo_sumECastorMinus);

    char name3[300];
    sprintf(name3,"ECastorSector_%s",Folders.at(j).c_str());
    TH2F *histo_ECastorSector = new TH2F(name3,"Castor Energy X Sector; Sector; Energy [GeV]", 17, 0, 17, 44, 0., 220.);
    m_hVector_ECastorSector.push_back(histo_ECastorSector);

    char name4[300];
    sprintf(name4,"ECaloVsEtaTProf_%s",Folders.at(j).c_str());
    TProfile *histo_ECaloVsEtaTProf = new TProfile(name4,"Calorimeter Energy X #eta; #eta; <Energy> [GeV]", 100, -6, 6, 0., 1000.);
    m_hVector_ECaloVsEtaTProf.push_back(histo_ECaloVsEtaTProf);

    char name5[300];
    sprintf(name5,"ECastorSectorTProf_%s",Folders.at(j).c_str());
    TProfile *histo_ECastorSectorTProf = new TProfile(name5,"Castor Energy X Sector; Sector; <Energy> [GeV]", 100, 0, 17, 0., 7000.);
    m_hVector_ECastorSectorTProf.push_back(histo_ECastorSectorTProf); 

    char name6[300];
    sprintf(name6,"ECastorSectorBin1D_%s",Folders.at(j).c_str());
    TH1F *histo_ECastorSectorBin1D = new TH1F(name6,"Castor Energy X Sector; Sector; Energy [GeV]", 17, 0, 17);
    m_hVector_ECastorSectorBin1D.push_back(histo_ECastorSectorBin1D);

    char name7[300];
    sprintf(name7,"EnergyHFMinusVsCastor_%s",Folders.at(j).c_str());
    TH2F *histo_ET_Calos_n = new TH2F(name7,"HF^{-} and Castor; #sum Energy HF^{-}; #sum Energy Castor [GeV]; N events", 1000, 0., 1000., 6000, 0., 3000. );
    m_hVector_etcalos_n.push_back(histo_ET_Calos_n);

    char name8[300];
    sprintf(name8,"EnergyHFPlusVsCastorTProf_%s",Folders.at(j).c_str());
    TProfile *histo_EnergyHFPlusVsCastorTProf = new TProfile(name8,"HF^{+} and Castor; #sum Energy HF^{+}; #sum Energy Castor [GeV]; N events", 1000, 0., 1000., 0., 3000. );
    m_hVector_EnergyHFPlusVsCastorTProf.push_back(histo_EnergyHFPlusVsCastorTProf);

    char name9[300];
    sprintf(name9,"EnergyHFMinusVsCastorTProf_%s",Folders.at(j).c_str());
    TProfile *histo_EnergyHFMinusVsCastorTProf = new TProfile(name9,"HF^{-} and Castor; #sum Energy HF^{-}; #sum Energy Castor [GeV]; N events", 1000, 0., 1000., 0., 3000. );
    m_hVector_EnergyHFMinusVsCastorTProf.push_back(histo_EnergyHFMinusVsCastorTProf);

    char name10[300];
    sprintf(name10,"sumECastorAndSumHFMinus_%s",Folders.at(j).c_str());
    TH1F *histo_sumECastorAndHFMinus = new TH1F(name10,"HF^{-} and Castor Sum of Energy; Energy [GeV]; N events",6000,0,3000);
    m_hVector_sumECastorAndHFMinus.push_back(histo_sumECastorAndHFMinus);

    char name11[300];
    sprintf(name11,"CastorMultiplicity_%s",Folders.at(j).c_str());
    TH1F *histo_CastorMultiplicity = new TH1F(name11,"Castor: number of sectors with activity; #Sectors; N events",17,0,17);
    m_hVector_CastorMultiplicity.push_back(histo_CastorMultiplicity);

    char name12[300];
    sprintf(name12,"CastorMultiplicityVsLumi_%s",Folders.at(j).c_str());
    TH2F *histo_CastorMultiplicityVsLumi = new TH2F(name12,"CastorMultiplicity Vs Luminosity; Luminosity per Bunch [#mub^{-1}s^{-1}]; Castor Multiplicity",5000,0,2,17,0,17);
    m_hVector_CastorMultiplicityVsLumi.push_back(histo_CastorMultiplicityVsLumi);

    char name13[300];
    sprintf(name13,"RunNumber_%s",Folders.at(j).c_str());
    TH1F *histo_RunNumber = new TH1F(name13,"Run Number; Run Number; N Event",16000,134000,150000);
    m_hVector_RunNumber.push_back(histo_RunNumber);

    char name14[300];
    sprintf(name14,"RunNumberZeroCastor_%s",Folders.at(j).c_str());
    TH1F *histo_RunNumberZeroCastor = new TH1F(name14,"Run Number; Run Number; N Event",16000,134000,150000);
    m_hVector_RunNumberZeroCastor.push_back(histo_RunNumberZeroCastor);

    char name15[300];
    sprintf(name15,"RunNumberHighCastor_%s",Folders.at(j).c_str());
    TH1F *histo_RunNumberHighCastor = new TH1F(name15,"Run Number; Run Number; N Event",16000,134000,150000);
    m_hVector_RunNumberHighCastor.push_back(histo_RunNumberHighCastor);

    char name16[300];
    sprintf(name16,"SectorVsTotalCastorEnergyTProf_%s",Folders.at(j).c_str());
    TProfile *histo_SectorVsTotalCastorEnergyTProf = new TProfile(name16,"Castor Multiplicity Vs CastorEnergy; # Multiplicity; Castor Energy [GeV]",17,0,17,0,1500);
    m_hVector_SectorVsTotalCastorEnergyTProf.push_back(histo_SectorVsTotalCastorEnergyTProf);

    char name17[300];
    sprintf(name17,"SectorVsTotalCastorEnergy_%s",Folders.at(j).c_str());
    TH2F *histo_SectorVsTotalCastorEnergy = new TH2F(name17,"Castor Multiplicity Vs CastorEnergy; # Multiplicity; Castor Energy [GeV]",17,0,17,6000,0,3000);
    m_hVector_SectorVsTotalCastorEnergy.push_back(histo_SectorVsTotalCastorEnergy);

    for (int se=1; se<17; se++){
      m_hVector_TotalEnergySectors.push_back( std::vector<TH1F*>() );
      m_hVector_Sector_EnergyVsMultiplicity.push_back( std::vector<TH2F*>() );

      char name18[300];
      char name18t[300];
      sprintf(name18,"TotalEnergySector%d_%s",se,Folders.at(j).c_str());
      sprintf(name18t,"#sum Energy, Castor Sector %d; #sum E_{modules 1,2,3,4,5} [GeV]; N events", se);
      TH1F *histo_TotalEnergySectors = new TH1F(name18,name18t, 1000,0,500);
      m_hVector_TotalEnergySectors[se-1].push_back(histo_TotalEnergySectors);

      char name19[300];
      char name19t[300];
      sprintf(name19,"Sector%d_EnergyVsMultiplicity_%s",se,Folders.at(j).c_str());
      sprintf(name19t,"Energy for Sector %d Vs Multiplicity; Multiplicity; #sum Energy_{sector} [GeV]", se);
      TH2F *histo_Sector_EnergyVsMultiplicity = new TH2F(name19,name19t,17,0,17,1000,0,500);
      m_hVector_Sector_EnergyVsMultiplicity[se-1].push_back(histo_Sector_EnergyVsMultiplicity);

    }

    for (int bs=1; bs<5; bs++){
      m_hVector_sumEHFplusBinSlice.push_back( std::vector<TH1F*>() );
      m_hVector_sumEHFminusBinSlice.push_back( std::vector<TH1F*>() );
      m_hVector_sumECastorMinusBinSlice.push_back( std::vector<TH1F*>() );

      char name20[300];
      sprintf(name20,"sumEHFplusBinSlice%d_%s",bs,Folders.at(j).c_str());
      TH1F *histo_sumEHFplusBinSlice = new TH1F(name20,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHFplusBinSlice[bs-1].push_back(histo_sumEHFplusBinSlice);

      char name21[300];
      sprintf(name21,"sumEHFminusBinSlice%d_%s",bs,Folders.at(j).c_str());
      TH1F *histo_sumEHFminusBinSlice = new TH1F(name21,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHFminusBinSlice[bs-1].push_back(histo_sumEHFminusBinSlice);

      char name22[300]; 
      sprintf(name22,"sumECastorMinusBinSlice%d_%s",bs,Folders.at(j).c_str());
      TH1F *histo_sumECastorMinusBinSlice = new TH1F(name22,"Castor Sum of Energy; Energy [GeV]; N events",6000,0,3000);
      m_hVector_sumECastorMinusBinSlice[bs-1].push_back(histo_sumECastorMinusBinSlice);
    }

    char name23[300];
    sprintf(name23,"CastorBadChannelVsRun_%s",Folders.at(j).c_str());
    TH2F *histo_CastorBadChannelVsRun = new TH2F(name23,"Castor Bad Channel Vs Run; # Castor Bad Channel(id); Run",226,0,226,16000,134000,150000);
    m_hVector_CastorBadChannelVsRun.push_back(histo_CastorBadChannelVsRun);

    char name24[300];
    sprintf(name24,"CastorBadRuns_%s",Folders.at(j).c_str());
    TH1F *histo_CastorBadRuns = new TH1F(name24,"Castor Bad Run Number; Bad Run Number; N Event",16000,134000,150000);
    m_hVector_CastorBadRuns.push_back(histo_CastorBadRuns);

    char name25[300];
    sprintf(name25,"CastorBadChannels_%s",Folders.at(j).c_str());
    TH1F *histo_CastorBadChannels = new TH1F(name25,"Castor Bad Channels Number; Channel(Id); N Event",225,0,225);
    m_hVector_CastorBadChannels.push_back(histo_CastorBadChannels);

  }

}

void CastorAnalysis::FillHistos(int index){

  sumCastorEnergy = 0.; 
  sumCastorAndHFMinusEnergy = 0.;
  SectorZeroCastorCounter = 0.;
  num_phi = 0.;
  num_x_centroid = 0;
  num_y_centroid = 0.;
  x_temp = 0.;
  y_temp = 0.;
  x_centroid = 0.;
  y_centroid = 0.;
  phi_average = 0.;
  SectorCastorHit = 0;

  double castorId[16] = {11.25,33.75,56.25,78.75,101.25,123.75,146.25,168.75,191.25,213.75,236.25,258.75,281.25,303.75,326.75,348.75};

  for (int i=0; i < 16; i++){
    CastorEnergySector[i]=0;
    if (i==4 || i==5){
      CastorEnergySector[i]=eventCastor->GetCastorModule2Energy(i)+eventCastor->GetCastorModule3Energy(i)+eventCastor->GetCastorModule4Energy(i)+eventCastor->GetCastorModule5Energy(i);
    }else{
      CastorEnergySector[i]=eventCastor->GetCastorModule1Energy(i)+eventCastor->GetCastorModule2Energy(i)+eventCastor->GetCastorModule3Energy(i)+eventCastor->GetCastorModule4Energy(i)+eventCastor->GetCastorModule5Energy(i);
    }
  }

  for (l=0; l<16;l++){
    if (CastorEnergySector[l] > SectorThreshold){
      ++SectorCastorHit;
      sumCastorEnergy+=CastorEnergySector[l];
      x_temp = 15*cos(castorId[l]*PI/180.0);
      y_temp = 15*sin(castorId[l]*PI/180.0);
      num_phi += castorId[l]*CastorEnergySector[l];
      num_x_centroid += x_temp*CastorEnergySector[l];
      num_y_centroid += y_temp*CastorEnergySector[l];
      m_hVector_ECastorSector.at(index)->Fill(l+1,CastorEnergySector[l]);
      m_hVector_ECastorSectorTProf.at(index)->Fill(l+1,CastorEnergySector[l]);
      m_hVector_ECastorSectorBin1D.at(index)->Fill(l+1,CastorEnergySector[l]);
    }
    else{
      sumCastorEnergy+=0;
      ++SectorZeroCastorCounter;
      num_x_centroid += 0;
      num_y_centroid += 0;
      num_phi += 0;      
    }
  }

  if (sumCastorEnergy > 0.){
    x_centroid = num_x_centroid/sumCastorEnergy;
    y_centroid = num_y_centroid/sumCastorEnergy;
    phi_average = num_phi/sumCastorEnergy;
    m_hVector_histo_castor_centroid.at(index)->Fill(x_centroid,y_centroid);
    m_hVector_histo_castor_centroid_phi.at(index)->Fill(phi_average);
  }

  for (k=0; k<eventCastor->GetEachTowerCounter();k++){
    m_hVector_ECaloVsEta.at(index)->Fill(eventCastor->GetEachTowerEta(k),eventCastor->GetEachTowerEnergy(k));
    m_hVector_ECaloVsEtaTProf.at(index)->Fill(eventCastor->GetEachTowerEta(k),eventCastor->GetEachTowerEnergy(k));
  }

  sumCastorAndHFMinusEnergy = sumCastorEnergy+eventdiff->GetSumEnergyHFMinus();

  m_hVector_sumECastorMinus.at(index)->Fill(sumCastorEnergy); 
  m_hVector_etcalos_n.at(index)->Fill(eventdiff->GetSumEnergyHFMinus(),sumCastorEnergy);
  m_hVector_EnergyHFPlusVsCastorTProf.at(index)->Fill(eventdiff->GetSumEnergyHFPlus(),sumCastorEnergy);
  m_hVector_EnergyHFMinusVsCastorTProf.at(index)->Fill(eventdiff->GetSumEnergyHFMinus(),sumCastorEnergy);
  m_hVector_sumECastorAndHFMinus.at(index)->Fill(sumCastorAndHFMinusEnergy);
  m_hVector_CastorMultiplicity.at(index)->Fill(SectorCastorHit);
  m_hVector_CastorMultiplicityVsLumi.at(index)->Fill(eventinfo->GetInstLumiBunch(),SectorCastorHit);
  if (SectorCastorHit < 1) m_hVector_RunNumberZeroCastor.at(index)->Fill(eventdiff->GetRunNumber());
  if (SectorCastorHit > 15) m_hVector_RunNumberHighCastor.at(index)->Fill(eventdiff->GetRunNumber());
  m_hVector_RunNumber.at(index)->Fill(eventdiff->GetRunNumber());
  m_hVector_SectorVsTotalCastorEnergy.at(index)->Fill(SectorCastorHit,sumCastorEnergy);
  m_hVector_SectorVsTotalCastorEnergyTProf.at(index)->Fill(SectorCastorHit,sumCastorEnergy);

  for (int l=0; l<eventCastor->GetCastorNumberBadChannels(); l++){
    m_hVector_CastorBadChannelVsRun.at(index)->Fill(eventCastor->GetCastorBadChannels(l),eventdiff->GetRunNumber());
    m_hVector_CastorBadChannels.at(index)->Fill(eventCastor->GetCastorBadChannels(l));
    if (eventCastor->GetCastorBadChannels(l) >=1 && eventCastor->GetCastorBadChannels(l) <= 80) m_hVector_CastorBadRuns.at(index)->Fill(eventdiff->GetRunNumber());
  }

  for(int id=0; id < 16; id++){
    if (CastorEnergySector[id] > SectorThreshold){
      m_hVector_TotalEnergySectors[id].at(index)->Fill(CastorEnergySector[id]);
      m_hVector_Sector_EnergyVsMultiplicity[id].at(index)->Fill(SectorCastorHit,CastorEnergySector[id]);
    }
  }


  if (SectorCastorHit >= 0 && SectorCastorHit <= 4){
    m_hVector_sumECastorMinusBinSlice[0].at(index)->Fill(sumCastorEnergy);
    m_hVector_sumEHFplusBinSlice[0].at(index)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_sumEHFminusBinSlice[0].at(index)->Fill(eventdiff->GetSumEnergyHFMinus());
  }


  if (SectorCastorHit >= 5 && SectorCastorHit <= 8){
    m_hVector_sumECastorMinusBinSlice[1].at(index)->Fill(sumCastorEnergy);
    m_hVector_sumEHFplusBinSlice[1].at(index)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_sumEHFminusBinSlice[1].at(index)->Fill(eventdiff->GetSumEnergyHFMinus());
  }

  if (SectorCastorHit >= 9 && SectorCastorHit <= 12){
    m_hVector_sumECastorMinusBinSlice[2].at(index)->Fill(sumCastorEnergy);
    m_hVector_sumEHFplusBinSlice[2].at(index)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_sumEHFminusBinSlice[2].at(index)->Fill(eventdiff->GetSumEnergyHFMinus());
  }

  if (SectorCastorHit >= 13 && SectorCastorHit <= 16){
    m_hVector_sumECastorMinusBinSlice[3].at(index)->Fill(sumCastorEnergy);
    m_hVector_sumEHFplusBinSlice[3].at(index)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_sumEHFminusBinSlice[3].at(index)->Fill(eventdiff->GetSumEnergyHFMinus());
  }

}

void CastorAnalysis::SaveHistos(){

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){
    m_hVector_histo_castor_centroid[j]->Write();
    m_hVector_histo_castor_centroid_phi[j]->Write();

    m_hVector_ECastorSector[j]->Write();
    m_hVector_ECastorSectorTProf[j]->Write();
    m_hVector_ECastorSectorBin1D[j]->Write();
    m_hVector_ECaloVsEta[j]->Write();
    m_hVector_ECaloVsEtaTProf[j]->Write();
    m_hVector_sumECastorMinus[j]->Write();
    m_hVector_etcalos_n[j]->Write();
    m_hVector_EnergyHFPlusVsCastorTProf[j]->Write();
    m_hVector_EnergyHFMinusVsCastorTProf[j]->Write();
    m_hVector_sumECastorAndHFMinus[j]->Write();
    m_hVector_CastorMultiplicity[j]->Write();
    m_hVector_CastorMultiplicityVsLumi[j]->Write();
    m_hVector_RunNumberZeroCastor[j]->Write();
    m_hVector_RunNumberHighCastor[j]->Write();
    m_hVector_RunNumber[j]->Write();
    m_hVector_SectorVsTotalCastorEnergy[j]->Write();
    m_hVector_SectorVsTotalCastorEnergyTProf[j]->Write();
    m_hVector_CastorBadChannelVsRun[j]->Write();
    m_hVector_CastorBadRuns[j]->Write();
    m_hVector_CastorBadChannels[j]->Write();  

    for (int id=0; id<16; id++){
      m_hVector_TotalEnergySectors[id].at(j)->Write();
      m_hVector_Sector_EnergyVsMultiplicity[id].at(j)->Write();
      m_hVector_Sector_EnergyVsMultiplicity[id].at(j)->Write();
    }

    for (int id=0; id<4; id++){
      m_hVector_sumECastorMinusBinSlice[id].at(j)->Write();
      m_hVector_sumEHFplusBinSlice[id].at(j)->Write();
      m_hVector_sumEHFminusBinSlice[id].at(j)->Write();
    }

  }

}

void CastorAnalysis::Run(std::string filein_, std::string processname_, std::string savehistofile_, std::string switchtrigger_, int optTrigger_, double lepton1pt_, double lepton2pt_, int nVertex_, std::string typesel_, double SectorThreshold_){

  bool debug = false;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  switchtrigger = switchtrigger_;
  nVertex = nVertex_;
  optTrigger = optTrigger_;
  lepton1pt = lepton1pt_;
  lepton2pt = lepton2pt_;
  typesel = typesel_;
  SectorThreshold = SectorThreshold_;

  std::string selStatus;
  std::string TriggerStatus;

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << " There is no the file " << filein << " or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    return;

  }

  if (check1.GetDirectory(processname.c_str())){
    LoadFile(filein,processname);
  }

  else{
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << " There is no directory/path " << processname << std::endl;
    std::cout << " in the file." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;
  }

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");
  std::ofstream outstring(outtxt);

  outstring << "" << std::endl;
  outstring << "<< Gold Events >>" << std::endl;
  outstring << "" << std::endl;
  outstring << "Please, insert this events in another text file to be used by PickEvent Tool. " << std::endl;
  outstring << "Twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents " << std::endl;
  outstring << ">>---------------------------------------------------------------------- " << std::endl;

  int NEVENTS = tr->GetEntries();
  int triggercounter[20]={0};
  int totalT=0;

  isoTk1 = -999.;
  isoTk2 = -999.;
  isoEcal1 = -999.;
  isoEcal2 = -999.;
  isoHcal1 = -999.;
  isoHcal2 = -999.;  
  innerHits1 = -999;
  Dcot1 = -999.;
  Dist1 = -999.;
  DeltaEtaTkClu1 = -999.;
  DeltaPhiTkClu1 = -999.;
  sigmaIeIe1 = -999.;
  HE1 = -999.;
  innerHits2 = -999;
  Dcot2 = -999.;
  Dist2 = -999.;
  DeltaEtaTkClu2 = -999.;
  DeltaPhiTkClu2 = -999.;
  sigmaIeIe2 = -999.;
  HE2 = -999.;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status;  

  for(int i=0;i<NEVENTS;i++){

    tr->GetEntry(i);

    for (int nt=0;nt<20;nt++){
      if(eventCastor->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }


    if (!debug){
      if (i==0) {
	std::cout << "" << std::endl;
	std::cout<< "Status Bar" << std::endl;
	std::cout << "" << std::endl;
      }
      loadBar(i,NEVENTS,100,100);
    }

    if (debug){
      std::cout << " " << std::endl;
      std::cout << "# of active towers: " << eventCastor->GetEachTowerCounter() << std::endl;
      std::cout << " " <<std::endl;
      for (int ic=0; ic<eventCastor->GetEachTowerCounter(); ic++){
	std::cout << "Each Tower Energy: " << eventCastor->GetEachTowerEnergy(ic) << std::endl;
      }
      std::cout << " " <<std::endl;
    }

    if (debug){
      if( i % 1000 == 0 ){
	std::cout << "\nEvent " << i << std::endl;
	std::cout << "Nr. events Bx 0: " << eventinfo->GetNPileUpBx0() << std::endl;
	std::cout << "Lumi per Bunch: " << eventinfo->GetInstLumiBunch() << std::endl;
      }
    }

    bool trigger = false;
    bool vertex = false;
    bool diffseln = false;
    bool diffselp = false;

    bool presel = false;
    bool charge = false;
    bool dimass = false;
    bool nSel = false;
    bool eleBarrel1 = false;
    bool eleEndCap1 = false;
    bool eleBarrel2 = false;
    bool eleEndCap2 = false; 
    bool candSel = false;
    bool isoBarrel1 = false;
    bool isoEndCap1 = false;
    bool isoBarrel2 = false;
    bool isoEndCap2 = false;
    bool isolation = false;

    if (switchtrigger == "trigger_all_electron"){
      if (eventCastor->GetHLTPath(0) || eventCastor->GetHLTPath(1) || eventCastor->GetHLTPath(2) || eventCastor->GetHLTPath(3) || eventCastor->GetHLTPath(4) || eventCastor->GetHLTPath(5) || eventCastor->GetHLTPath(6)) trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger << ", Trigger All Electron accepted." << std::endl;
      TriggerStatus = "trigger_all_electron";
    }
    else if (switchtrigger == "trigger"){
      if (eventCastor->GetHLTPath(optTrigger)) trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger <<", trigger accepted." << std::endl;
      TriggerStatus = "trigger";
    }
    else if (switchtrigger == "no_trigger") {
      trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger << ", no trigger." << std::endl; 
      TriggerStatus = "no_trigger";
    }
    else{
      exit(EXIT_FAILURE);
    }

    if (eventdiff->GetNVertex() == nVertex) vertex = true;
    if ((eventdiff->GetEtaMinFromPFCands() > -3.)) diffseln = true;
    if ((eventdiff->GetEtaMaxFromPFCands() <  3.)) diffselp = true;

    if (typesel == "RecoElectron"){
      selStatus = "Reco::Electron";
      isoTk1 = eventCastor->GetLeadingElectronTkDr03()/eventCastor->GetLeadingElectronPt();
      isoEcal1 = eventCastor->GetLeadingElectronEcalDr03()/eventCastor->GetLeadingElectronPt();
      isoHcal1 = eventCastor->GetLeadingElectronHcalDr03()/eventCastor->GetLeadingElectronPt();
      isoTk2 = eventCastor->GetSecondElectronTkDr03()/eventCastor->GetSecondElectronPt();
      isoEcal2 = eventCastor->GetSecondElectronEcalDr03()/eventCastor->GetSecondElectronPt();
      isoHcal2 = eventCastor->GetSecondElectronHcalDr03()/eventCastor->GetSecondElectronPt();
      innerHits1 = eventCastor->GetLeadingElectronInnerHits();
      Dcot1 = eventCastor->GetLeadingElectronDCot();
      Dist1 = eventCastor->GetLeadingElectronDist();
      DeltaEtaTkClu1 = eventCastor->GetLeadingElectronDeltaEtaTkClu();
      DeltaPhiTkClu1 = eventCastor->GetLeadingElectronDeltaPhiTkClu();
      sigmaIeIe1 = eventCastor->GetLeadingElectronSigmaIeIe();
      HE1 = eventCastor->GetLeadingElectronHE();
      innerHits2 = eventCastor->GetSecondElectronInnerHits();
      Dcot2 = eventCastor->GetSecondElectronDCot();
      Dist2 = eventCastor->GetSecondElectronDist();
      DeltaEtaTkClu2 = eventCastor->GetSecondElectronDeltaEtaTkClu();
      DeltaPhiTkClu2 = eventCastor->GetSecondElectronDeltaPhiTkClu();
      sigmaIeIe2 = eventCastor->GetSecondElectronSigmaIeIe();
      HE2 = eventCastor->GetSecondElectronHE();

      if (eventCastor->GetLeadingElectronPt() > lepton1pt && eventCastor->GetSecondElectronPt() > lepton2pt) presel = true;
      if (eventCastor->GetLeadingElectronCharge()*eventCastor->GetSecondElectronCharge()==-1) charge = true;
      if (eventCastor->GetDiElectronMass() > 60. && eventCastor->GetDiElectronMass() < 110.) dimass = true;
      if (eventCastor->GetElectronsN() > 1) nSel = true;

      //Isolation Electron
      if ((fabs (eventCastor->GetLeadingElectronEta()) <= 1.4442) ){
	if (isoTk1<0.09 && isoEcal1<0.07 && isoHcal1<0.10) isoBarrel1 = true;
      }

      if ((fabs (eventCastor->GetLeadingElectronEta()) >= 1.5660) && (fabs (eventCastor->GetLeadingElectronEta()) <= 2.5)){
	if (isoTk1<0.04 && isoEcal1<0.05 && isoHcal1<0.025) isoEndCap1 = true;
      }

      if ((fabs (eventCastor->GetSecondElectronEta()) <= 1.4442) ){
	if (isoTk2<0.09 && isoEcal2<0.07 && isoHcal2<0.10) isoBarrel2 = true;
      }

      if ((fabs (eventCastor->GetSecondElectronEta()) >= 1.5660) && (fabs (eventCastor->GetSecondElectronEta()) <= 2.5)){
	if (isoTk2<0.04 && isoEcal2<0.05 && isoHcal2<0.025) isoEndCap2 = true;
      }

      if ((isoEndCap1 || isoBarrel1) && (isoEndCap2 || isoBarrel2)) isolation = true;

      //Electron Candidate Selection
      if ((fabs (eventCastor->GetLeadingElectronEta()) <= 1.4442) ){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.004 && fabs (DeltaPhiTkClu1) < 0.06 && sigmaIeIe1 < 0.01 && HE1 < 0.04 ) eleBarrel1 = true;
      }

      if ((fabs (eventCastor->GetLeadingElectronEta()) >= 1.5660) && (fabs (eventCastor->GetLeadingElectronEta()) <= 2.5)){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.007 && fabs (DeltaPhiTkClu1) < 0.03 && sigmaIeIe1 < 0.03 && HE1 < 0.025) eleEndCap1 = true;
      }

      if ((fabs (eventCastor->GetSecondElectronEta()) <= 1.4442) ){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.004 && fabs (DeltaPhiTkClu2) < 0.06 && sigmaIeIe2 < 0.01 && HE2 < 0.04 ) eleBarrel2 = true;
      }

      if ((fabs (eventCastor->GetSecondElectronEta()) >= 1.5660) && (fabs (eventCastor->GetSecondElectronEta()) <= 2.5)){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.007 && fabs (DeltaPhiTkClu2) < 0.03 && sigmaIeIe2 < 0.03 && HE2 < 0.025) eleEndCap2 = true;
      }

      if ((eleEndCap1 || eleBarrel1) && (eleEndCap2 || eleBarrel2)) candSel = true;
    }

    else if (typesel == "RecoMuon"){
      selStatus = "Reco::Muon";
      if (eventCastor->GetLeadingMuonPt() > lepton1pt && eventCastor->GetSecondMuonPt() > lepton2pt) presel = true;
      if (eventCastor->GetLeadingMuonCharge()*eventCastor->GetSecondMuonCharge()==-1) charge = true;
      if (eventCastor->GetDiMuonMass() > 60. && eventCastor->GetDiMuonMass() < 110.) dimass = true;
      if (eventCastor->GetMuonsN() > 1) nSel = true;
      if (eventCastor->GetLeadingMuonSumPtR03() < 3 && eventCastor->GetSecondMuonSumPtR03() < 3 ) { 
	isolation = true;
	candSel = true;
      }

    }

    else if (typesel == "PatElectron"){
      selStatus = "Pat::Electron";
      isoTk1 = eventCastor->GetPatElectron1TkDr03()/eventCastor->GetPatElectron1Pt();
      isoEcal1 = eventCastor->GetPatElectron1EcalDr03()/eventCastor->GetPatElectron1Pt();
      isoHcal1 = eventCastor->GetPatElectron1HcalDr03()/eventCastor->GetPatElectron1Pt();
      isoTk2 = eventCastor->GetPatElectron2TkDr03()/eventCastor->GetPatElectron2Pt();
      isoEcal2 = eventCastor->GetPatElectron2EcalDr03()/eventCastor->GetPatElectron2Pt();
      isoHcal2 = eventCastor->GetPatElectron2HcalDr03()/eventCastor->GetPatElectron2Pt();
      innerHits1 = eventCastor->GetPatElectron1InnerHits();
      Dcot1 = eventCastor->GetPatElectron1DCot();
      Dist1 = eventCastor->GetPatElectron1Dist();
      DeltaEtaTkClu1 = eventCastor->GetPatElectron1DeltaEtaTkClu();
      DeltaPhiTkClu1 = eventCastor->GetPatElectron1DeltaPhiTkClu();
      sigmaIeIe1 = eventCastor->GetPatElectron1SigmaIeIe();
      HE1 = eventCastor->GetPatElectron1HE();
      innerHits2 = eventCastor->GetPatElectron2InnerHits();
      Dcot2 = eventCastor->GetPatElectron2DCot();
      Dist2 = eventCastor->GetPatElectron2Dist();
      DeltaEtaTkClu2 = eventCastor->GetPatElectron2DeltaEtaTkClu();
      DeltaPhiTkClu2 = eventCastor->GetPatElectron2DeltaPhiTkClu();
      sigmaIeIe2 = eventCastor->GetPatElectron2SigmaIeIe();
      HE2 = eventCastor->GetPatElectron2HE();

      if (eventCastor->GetPatElectron1Pt() > lepton1pt && eventCastor->GetPatElectron2Pt() > lepton2pt) presel = true;
      if (eventCastor->GetPatElectron1Charge()*eventCastor->GetPatElectron2Charge()==-1) charge = true;
      if (eventCastor->GetPatDiElectronMass() > 60. && eventCastor->GetPatDiElectronMass() < 110.) dimass = true;
      if (eventCastor->GetPatNElectron() > 1) nSel = true;


      //Isolation Electron
      if ((fabs (eventCastor->GetPatElectron1Eta()) <= 1.4442) ){
	if (isoTk1<0.09 && isoEcal1<0.07 && isoHcal1<0.10) isoBarrel1 = true;
      }

      if ((fabs (eventCastor->GetPatElectron1Eta()) >= 1.5660) && (fabs (eventCastor->GetPatElectron1Eta()) <= 2.5)){
	if (isoTk1<0.04 && isoEcal1<0.05 && isoHcal1<0.025) isoEndCap1 = true;
      }

      if ((fabs (eventCastor->GetPatElectron2Eta()) <= 1.4442) ){
	if (isoTk2<0.09 && isoEcal2<0.07 && isoHcal2<0.10) isoBarrel2 = true;
      }

      if ((fabs (eventCastor->GetPatElectron2Eta()) >= 1.5660) && (fabs (eventCastor->GetPatElectron2Eta()) <= 2.5)){
	if (isoTk2<0.04 && isoEcal2<0.05 && isoHcal2<0.025) isoEndCap2 = true;
      }

      if ((isoEndCap1 || isoBarrel1) && (isoEndCap2 || isoBarrel2)) isolation = true;

      //Electron Candidate Selection
      if ((fabs (eventCastor->GetPatElectron1Eta()) <= 1.4442) ){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.004 && fabs (DeltaPhiTkClu1) < 0.06 && sigmaIeIe1 < 0.01 && HE1 < 0.04 ) eleBarrel1 = true;
      }

      if ((fabs (eventCastor->GetPatElectron1Eta()) >= 1.5660) && (fabs (eventCastor->GetPatElectron1Eta()) <= 2.5)){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.007 && fabs (DeltaPhiTkClu1) < 0.03 && sigmaIeIe1 < 0.03 && HE1 < 0.025) eleEndCap1 = true;
      }

      if ((fabs (eventCastor->GetPatElectron2Eta()) <= 1.4442) ){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.004 && fabs (DeltaPhiTkClu2) < 0.06 && sigmaIeIe2 < 0.01 && HE2 < 0.04 ) eleBarrel2 = true;
      }

      if ((fabs (eventCastor->GetPatElectron2Eta()) >= 1.5660) && (fabs (eventCastor->GetPatElectron2Eta()) <= 2.5)){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.007 && fabs (DeltaPhiTkClu2) < 0.03 && sigmaIeIe2 < 0.03 && HE2 < 0.025) eleEndCap2 = true;
      }

      if ((eleEndCap1 || eleBarrel1) && (eleEndCap2 || eleBarrel2)) candSel = true;

    }

    else if(typesel == "PatMuon"){
      selStatus = "Pat::Muon";
      if (eventCastor->GetPatMuon1Pt() > lepton1pt && eventCastor->GetPatMuon2Pt() > lepton2pt) presel = true;
      if (eventCastor->GetPatMuon1Charge()*eventCastor->GetPatMuon2Charge()==-1) charge = true;
      if (eventCastor->GetPatDiMuonMass() > 60. && eventCastor->GetPatDiMuonMass() < 110.) dimass = true;
      if (eventCastor->GetPatNMuon() > 1) nSel = true; 
      if (eventCastor->GetPatMuon1SumPtR03() < 3 && eventCastor->GetPatMuon2SumPtR03() < 3 ) {
	candSel = true;
	isolation = true;
      }


    }

    else{
      exit(EXIT_FAILURE);
    }

    if(switchtrigger == "trigger" || switchtrigger == "trigger_all_electron"){ 
      FillHistos(0); 
      if(trigger) {
	++totalT;
	FillHistos(1);
      } 
      if(trigger && vertex && presel) FillHistos(2);
      if(trigger && vertex && presel && nSel) FillHistos(3);
      if(trigger && vertex && presel && nSel && charge) FillHistos(4);
      if(trigger && vertex && presel && nSel && charge && dimass) FillHistos(5);
      if(trigger && vertex && presel && nSel && charge && dimass && isolation) FillHistos(6);
      if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel) FillHistos(7);
      if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln) FillHistos(8);
      if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp) FillHistos(9);
    }

    else if (switchtrigger =="no_trigger"){
      --totalT;
      FillHistos(0);
      if(vertex && presel) FillHistos(2);
      if(vertex && presel && nSel) FillHistos(3);
      if(vertex && presel && nSel && charge) FillHistos(4);
      if(vertex && presel && nSel && charge && dimass) FillHistos(5);
      if(vertex && presel && nSel && charge && dimass && isolation) FillHistos(6);
      if(vertex && presel && nSel && charge && dimass && isolation && candSel) FillHistos(7);
      if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln) FillHistos(8);
      if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp) FillHistos(9);
    }

    else{
      exit(EXIT_FAILURE);
    }

  }

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << ">> Input file: " << filein << std::endl;
  outstring << ">> Output file: " << savehistofile << std::endl;
  outstring << ">> TTree Name: " << processname << std::endl;
  outstring << " " << std::endl;
  outstring << "<< OPTIONS >>" << std::endl; 
  outstring << " " << std::endl;
  outstring << ">> Trigger: " << TriggerStatus << std::endl;
  outstring << ">> # Vertex: " << nVertex << std::endl;
  outstring << ">> Lepton1(pT) > " << lepton1pt <<std::endl;
  outstring << ">> Lepton2(pT) > " << lepton2pt <<std::endl;
  outstring << ">> Type of Selection: " << selStatus << std::endl;
  outstring << ">> Sector Castor Threshold [GeV]: " << SectorThreshold <<  std::endl;
  outstring << " " << std::endl;
  outstring << "<< TRIGGER >> " << std::endl;
  outstring << " " << std::endl;
  outstring << "Total Number of Events: " <<  NEntries << std::endl;
  outstring << "Number of Triggered Events: " << totalT << " | Option: " << optTrigger << std::endl;
  outstring << "(if negative, trigger option is off)" << std::endl;
  outstring << "" << std::endl; 
  outstring << "Each Trigger Fired: " <<  std::endl;
  outstring << "Trigger 0: " << triggercounter[0] << std::endl;
  outstring << "Trigger 1: " << triggercounter[1] << std::endl;
  outstring << "Trigger 2: " << triggercounter[2] << std::endl;
  outstring << "Trigger 3: " << triggercounter[3] << std::endl;
  outstring << "Trigger 4: " << triggercounter[4] << std::endl;
  outstring << "Trigger 5: " << triggercounter[5] << std::endl;
  outstring << "Trigger 6: " << triggercounter[6] << std::endl;
  outstring << "Trigger 7: " << triggercounter[7] << std::endl;
  outstring << "Trigger 8: " << triggercounter[8] << std::endl;
  outstring << "Trigger 9: " << triggercounter[9] << std::endl;
  outstring << "Trigger 10: " << triggercounter[10] << std::endl;
  outstring << "Trigger 11: " << triggercounter[11] << std::endl;
  outstring << "Trigger 12: " << triggercounter[12] << std::endl;
  outstring << "Trigger 13: " << triggercounter[13] << std::endl;
  outstring << "Trigger 14: " << triggercounter[14] << std::endl;
  outstring << "Trigger 15: " << triggercounter[15] << std::endl;
  outstring << "Trigger 16: " << triggercounter[16] << std::endl;
  outstring << "Trigger 17: " << triggercounter[17] << std::endl;
  outstring << "Trigger 18: " << triggercounter[18] << std::endl;
  outstring << "Trigger 19: " << triggercounter[19] << std::endl;
  outstring << "" << std::endl;

  outf->cd();
  SaveHistos();
  outf->Close();
  std::cout << "\n" << std::endl;
}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

int main(int argc, char **argv)
{

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string processname_;
  std::string savehistofile_;
  double lepton1pt_;
  double lepton2pt_;
  int nVertex_;
  int optTrigger_;
  std::string switchtrigger_;
  std::string typesel_;
  double SectorThreshold_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0) filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0) processname_ = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0) savehistofile_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0) switchtrigger_ = argv[4];
  if (argc > 5 && strcmp(s1,argv[5]) != 0) optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0) lepton1pt_ = atof(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0) lepton2pt_ = atof(argv[7]);
  if (argc > 8 && strcmp(s1,argv[8]) != 0) nVertex_ = atoi(argv[8]);
  if (argc > 9 && strcmp(s1,argv[9]) != 0) typesel_ = argv[9];
  if (argc > 10 && strcmp(s1,argv[10]) != 0) SectorThreshold_ = atof(argv[10]);

  std::cout << " " << std::endl;
  std::cout << ">>>>> Run with the Options <<<<< " << std::endl;
  std::cout << "Filename, PATTuple: " << filein_ << std::endl;
  std::cout << "ProcessName, PATTuple: " << processname_ << std::endl;
  std::cout << "Output name, histograms: " << savehistofile_ << std::endl;
  std::cout << "Trigger Switch: " << switchtrigger_ << std::endl;
  std::cout << "Trigger Path Option: " << optTrigger_ << std::endl;
  std::cout << "Lepton 1, pT [GeV]: " << lepton1pt_ << std::endl;
  std::cout << "Lepton 2, pT [GeV]: " << lepton2pt_ << std::endl;
  std::cout << "# Vertex: " << nVertex_ << std::endl;
  std::cout << "Type of Selection: " << typesel_ << std::endl;
  std::cout << "Sector Castor Threshold [GeV]: " << SectorThreshold_ <<  std::endl;
  std::cout << "" << std::endl;

  if (switchtrigger_=="trigger" || switchtrigger_=="no_trigger" || switchtrigger_=="trigger_all_electron") {}
  else{
    std::cout << "Please Insert type of Swithtrigger: " << std::endl;
    std::cout << "1) trigger: run with trigger. Need optTrigger >=0;" << std::endl;
    std::cout << "2) trigger_all_electron: all trigger electron path will be accepted. Do not require optTrigger." << std::endl;
    std::cout << "3) no_trigger: run without trigger." << std::endl;
    return 0;
  }

  if (typesel_=="RecoMuon" || typesel_=="RecoElectron" || typesel_=="PatElectron" || typesel_=="PatMuon" ) {}
  else{
    std::cout << "Please Insert type of Selections: " << std::endl;
    std::cout << "1) RecoMuon: selections with Reco::Muon." << std::endl;
    std::cout << "2) RecoElectron: selections with Reco::Electron." << std::endl;
    std::cout << "3) PatMuon: selections with Pat::Muon." << std::endl;
    std::cout << "4) PatElectron: selections with Pat::Electron." << std::endl;
    return 0;
  }

  if (nVertex_ <= 0 || optTrigger_ < 0 || lepton1pt_ < 0 || lepton2pt_ < 0 || SectorThreshold_ < 0 ){
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << " Pay attention on the input numbers parameters" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << ">> Requirements:                             " << std::endl;
    std::cout << "I)   nVertex_ > 0 " << std::endl;
    std::cout << "II)  optTrigger >= 0" << std::endl;
    std::cout << "III) mcweight_ > 0" << std::endl;
    std::cout << "IV)  Lepton1pt_ and Lepton2pt_ >= 0" << std::endl;  
    std::cout << "----------------------------------------------" << std::endl; 
    return 0;
  }

  CastorAnalysis* CastorRun = new CastorAnalysis();
  CastorRun->CreateHistos();
  CastorRun->Run(filein_, processname_, savehistofile_, switchtrigger_, optTrigger_, lepton1pt_, lepton2pt_, nVertex_, typesel_, SectorThreshold_);
  return 0;
}

#endif
