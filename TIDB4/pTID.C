
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
void pTID() {
// read file TDC/TDC_Converted_Data_10000000P_B1_0.dat
// this file has 4 columns of interger data, the first one is event number
//   TString dir = gROOT->GetTutorialDir();

   Int_t TOA,TOT,Cal,Hit;
   Long64_t Evt;
   TFile *f = TFile::Open("B4_TID.root");
   TTree*t1 = (TTree*)f->Get("t1");
   TTree *t2 = new TTree("t2", "TID events");
   Int_t Points = 0;     
   Int_t TID_Evts = 0;
   Float_t TOA_Mean, TOA_RMS;
   Float_t TOT_Mean, TOT_RMS;
   Float_t Cal_Mean, Cal_RMS;
   t2->Branch("Points",&Points,"Points/I");
   t2->Branch("TID_Evts",&TID_Evts,"TID_Evts/I");
   t2->Branch("TOA_Mean",&TOA_Mean,"TOA_Mean/F");
   t2->Branch("TOA_RMS",&TOA_RMS,"TOA_RMS/F");  
   t2->Branch("TOT_Mean",&TOT_Mean,"TOT_Mean/F");
   t2->Branch("TOT_RMS",&TOT_RMS,"TOT_RMS/F");  
    t2->Branch("Cal_Mean",&Cal_Mean,"Cal_Mean/F");
   t2->Branch("Cal_RMS",&Cal_RMS,"Cal_RMS/F");  
   
   t1->SetBranchAddress("TOA",&TOA);   
   t1->SetBranchAddress("TOT",&TOT);      
   t1->SetBranchAddress("Hit",&Hit);      
   t1->SetBranchAddress("Cal",&Cal);   
   t1->SetBranchAddress("Evt",&Evt);   

   TH1F *hTOA = new TH1F("hTOA","TOA code distribution", 1024, -0.5,1023.5);
   TH1F *hCal = new TH1F("hCal","TOA code distribution", 1024, -0.5,1023.5);
   TH1F *hTOT = new TH1F("hTOT","TOA code distribution", 1024, -0.5,1023.5);

   Long64_t nentries = t1->GetEntries();
 
   for (Long64_t i=0;i<nentries;i++) {
     t1->GetEntry(i);
	 if(TOA>=30&&TOA<=680&&Hit==1)//cut
	 {
		hTOA->Fill(TOA); 
		hTOT->Fill(TOT);
		hCal->Fill(Cal);
	 }
	 if(i%1000==999){
		cout<<TID_Evts<<" "<<hTOA->GetEntries()<<endl;
		cout<<hTOA->GetMean()<<" "<<hTOA->GetRMS()<<" "<<hTOA->GetEntries()<<endl;
		cout<<hTOT->GetMean()<<" "<<hTOT->GetRMS()<<" "<<hTOT->GetEntries()<<endl; 
		cout<<hCal->GetMean()<<" "<<hCal->GetRMS()<<" "<<hCal->GetEntries()<<endl; 
		TID_Evts++;
		Points = hTOA->GetEntries();
		TOA_Mean = hTOA->GetMean();
		TOA_RMS = hTOA->GetRMS();
		TOT_Mean = hTOT->GetMean();
		TOT_RMS = hTOT->GetRMS();
		Cal_Mean = hCal->GetMean();
		Cal_RMS = hCal->GetRMS();
		t2->Fill();
		hTOA->Reset();
		hTOT->Reset();
		hCal->Reset();
		
	 }
   }	 

   TCanvas *c1 = new TCanvas("c1","c1",800,600);
  hTOA->SetFillColor(kRed);
  hTOA->SetLineWidth(0);  
  hTOA->Draw();
  c1->Update();
  TFile *f1 = new TFile("B4_TID_Filter.root","RECREATE");
  f1->cd();
  t2->Write();
  
 /*
   TGraph *gr = new TGraph(n+1,x,y);
   TGraph *igr = new TGraph(n+1,ix,iy);

    
    igr->SetLineColor(kRed);
    igr->SetLineWidth(2);
    igr->SetMarkerColor(kRed);
    igr->SetMarkerStyle(7);

    auto g = new TMultiGraph();

    gr->SetLineColor(kBlue);
    gr->SetLineWidth(2);
    gr->SetMarkerColor(kBlue);
    gr->SetMarkerStyle(7);
    gr->GetXaxis()->SetRange(0,655);
    gr->GetYaxis()->SetRange(0,800);
    igr->GetXaxis()->SetRange(0,655);
    igr->GetYaxis()->SetRange(0,800);
    
    gStyle->SetFitFormat("7.5g");
    gStyle->SetOptFit(0111);
    
    gr->SetTitle("TOA transfer function");
    igr->SetTitle("Ideal transfer function");
*/

}
