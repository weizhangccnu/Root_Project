#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TPaveStats.h"
//======================================================================================//
//read root file: filename
//======================================================================================//
void Read_Rootfile(Int_t code){
    Int_t TOA, TOT, Cal, Hit;
    Long64_t Evt;

    TFile *f;
    Int_t LowBin = 30;
    Int_t HighBin = 680;

    Long64_t lowE = 10000;
    Int_t evtN = 1;             //total event number 1: 10000000, 2:20000000

    TString ss = "B4_0x03_";
    if(code == 0x01){
        printf("Code = %d\n", code);
        f = TFile::Open("B4_TID_Before_TOA_Scan.root");
        LowBin = 30;
        HighBin = 675; 
        lowE = 380000;
        ss = "B4_BeforeTIDTest";
        evtN = 1;
    }
    else{
        cout << "Input invalidated code!!!\n" << endl;
    }
    
    // read data from root file
    TTree *t1 = (TTree*)f->Get("t1");

    t1->SetBranchAddress("TOA", &TOA);
    t1->SetBranchAddress("TOT", &TOT);
    t1->SetBranchAddress("Cal", &Cal);
    t1->SetBranchAddress("Hit", &Hit);

    Long64_t nevent = t1->GetEntries();             //get the root file total event
    printf("%lld\n", nevent);
    
    Int_t bins = HighBin - LowBin + 1;
    Double_t lB = LowBin - 0.5;
    Double_t hB = HighBin - 0.5;

    Long64_t upE;
    if(evtN == 1){
        upE = lowE + 61*160000;
    }
    else{
        upE = lowE + 122*160000;
    }
    
    Double_t timeRes = 0.3125;       //depends on the frequency, unit is ps 
    Long64_t period = 40000;

    //TOA Histogram 
    TH1F *TOA_Hist1 = new TH1F("TOA_Hist1", "TOA Histogram", bins, lB, hB);
    TH1F *TOABin_Hist1 = new TH1F("TOABin_Hist1", "TOA Bin Size Histogram", bins, lB, hB);
    TH2F *TOABin_Hist2 = new TH2F("TOABin_Hist2", "TOA Bin Size 2D Histogram", 126, -0.5, 125.5, 6, -0.5, 5.5);

    TH1F *TOA_Tran = new TH1F("TOA_Tran", "TOA Transfer Function", bins, lB, hB);
    //fetch data from root file
    for(Int_t i=0; i<nevent; i++){
       t1->GetEntry(i);                             //load the i+1 event into the event data 
       if(TOA>=LowBin && TOA<=HighBin && Hit==1){
           if(i>lowE && i<upE){
                TOA_Hist1->Fill(TOA);
           }
       }
    }

    TCanvas *c1 = new TCanvas("c1","c1",1000,600);
    TCanvas *c2 = new TCanvas("c2","c2",800,600);

    //TOA_Hist1->SetFillColor(kRed);
    TOA_Hist1->SetAxisRange(0,20000,"Y");
    TOA_Hist1->SetXTitle("TOA code");
    TOA_Hist1->SetYTitle("Counts");
    TOA_Hist1->GetXaxis()->CenterTitle();
    TOA_Hist1->GetYaxis()->CenterTitle();
    TOA_Hist1->Draw();
    c1->Update();
    c1->SaveAs("./Plots/TOA_Hist1.pdf");

    //TOA Transfer function
    Double_t sum = 0;
    Double_t weight = 1.0/TOA_Hist1->GetEntries();
    printf("%.10f\n", weight);
    for(Int_t i=1; i<bins; i++){
       TOABin_Hist1->SetBinContent(i, TOA_Hist1->GetBinContent(1)*weight*bins);
       cout << i << " " << TOABin_Hist1->GetBinContent(i) << endl;
       TOABin_Hist2->SetBinContent((i-1)%126+1, (i-1-((i-1)%126))/126+1, TOABin_Hist1->GetBinContent(i));

       sum += TOA_Hist1->GetBinContent(i)*weight;
       TOA_Tran->SetBinContent(i, sum);
    }
    TOA_Tran->Fit("pol1", "FQ");
    TF1 *fit = TOA_Tran->GetFunction("pol1");
    Double_t p0 = fit->GetParameter(0);
    Double_t p1 = fit->GetParameter(1);

    
    //TCanvas *c2 = new TCanvas("c2","c2",1000,600);

    TOABin_Hist1->SetFillColor(kRed);

    TOABin_Hist1->SetAxisRange(0,695,"X");
    TOABin_Hist1->Draw();
    c1->Update();
    c1->SaveAs("./Plots/TOABin_Hist1.pdf");

    //TOABin_Hist2->SetXTitle("TOA code modular 126");
    //TOABin_Hist2->SetYTitle("TOA code divide 126");
    //TOABin_Hist2->SetYTitle("TOA Bin size");
    //TOABin_Hist2->GetXaxis()->CenterTitle();
    //TOABin_Hist2->GetYaxis()->CenterTitle();       
    //TOABin_Hist2->Draw();
    //c1->SaveAs("./Plots/TOABin_Hist2.pdf");
    
    TOA_Tran->SetAxisRange(0,1.3,"Y");
    TOA_Tran->SetXTitle("TOA code");
    TOA_Tran->SetYTitle("TOA input");
    TOA_Tran->GetXaxis()->CenterTitle();
    TOA_Tran->GetYaxis()->CenterTitle();
    TOA_Tran->Draw();
    c2->Update();
    c2->SaveAs("./Plots/TOA_Transfer_Function.pdf");

    
}

