//This root file is used to plot latch setup time data
//Author: Wei Zhang
//Date: April 23, 2020 
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMath.h"
#include "TPaveStats.h"


//.dat file store into root file
void BuildTTree(){
    ifstream in;                //define a in stream
    in.open("./Latch_1-0_Setup_Time.dat");
    Float_t Volt, Setuptime;

    TFile *f = TFile::Open("Latch_1-0_Setup_Time.root", "RECREATE");        //Create a new ROOT File
    TTree t1("t1", "Setup time 0-1");           //Create one branch with all information from the structure
    t1.Branch("Volt", &Volt, "Volt/F");
    t1.Branch("Setuptime", &Setuptime, "Setuptime/F");

    while(1){
        in >> Volt >> Setuptime;
        if(!in.good()) break;
        t1.Fill();
    }
    t1.Print();                 //show branch information
    t1.Show(1);                 //Show Entries 1 content
    t1.Scan();                  //show all Entries
    in.close();
    f->Write();                 //Write root file

}

//plot setup time
void Latch_Setup_Time(){
    Float_t Volt1, Setuptime1;
    Float_t Volt2, Setuptime2;
    TFile *f1, *f2;               
    f1 = TFile::Open("Latch_1-0_Setup_Time.root");
    f2 = TFile::Open("Latch_0-1_Setup_Time.root");
    
    TTree *t1 = (TTree*)f1->Get("t1");
    TTree *t2 = (TTree*)f2->Get("t1");

    t1->SetBranchAddress("Volt", &Volt1);
    t1->SetBranchAddress("Setuptime", &Setuptime1);

    t2->SetBranchAddress("Volt", &Volt2);
    t2->SetBranchAddress("Setuptime", &Setuptime2);


    Int_t n = t1->GetEntries();
    printf("Total Entries: %d\n", n);
    Double_t x1[n], y1[n];
    Double_t x2[n], y2[n];

    for(Int_t i=0; i<n; i++){                   //Read .root file row by row
        t1->GetEntry(i);
        x1[i] = Volt1;
        y1[i] = Setuptime1;
    }

    for(Int_t i=0; i<n; i++){                   //Read .root file row by row
        t2->GetEntry(i);
        x2[i] = Volt2;
        y2[i] = Setuptime2;
    }
    TCanvas *c1 = new TCanvas("c1", "A simple Graph Example", 800, 500);

    TGraph* gr1 = new TGraph(n, x1, y1);

    c1->SetGrid();                              // show figure grid
    gr1->SetTitle("Latch 1-0 setup time");      
    gr1->SetMarkerColor(4);
    gr1->SetMarkerStyle(5);
    gr1->SetLineWidth(3);

    gr1->GetXaxis()->SetTitle("Latch Power Voltage [V]");
    gr1->GetXaxis()->CenterTitle();
    gr1->GetYaxis()->SetTitle("Setup Time [ps]");
    gr1->GetYaxis()->CenterTitle();

    gr1->Draw("APL*");
    c1->Update();
    c1->SaveAs("Latch_1-0_Setup_Time.png");     //save plotted figure

    TGraph* gr2 = new TGraph(n, x2, y2);

    c1->SetGrid();
    gr2->SetTitle("Latch 1-0 setup time");
    gr2->SetMarkerColor(4);
    gr2->SetMarkerStyle(5);
    gr2->SetLineWidth(3);

    gr2->GetXaxis()->SetTitle("Latch Power Voltage [V]");
    gr2->GetXaxis()->CenterTitle();
    gr2->GetYaxis()->SetTitle("Setup Time [ps]");
    gr2->GetYaxis()->CenterTitle();

    gr2->Draw("APL*");
    c1->Update();
    c1->SaveAs("Latch_0-1_Setup_Time.png");
    return 0;
}
