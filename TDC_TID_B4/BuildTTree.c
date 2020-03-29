#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
void BuildTTree(){
    ifstream in;             //define a in stream
    in.open("./TDC_Converted_Data_10000000P_B4_Cal=3n125_VDD1V2_80M001Hz_0.dat");
    Int_t TOA,TOT,Cal,Hit,DTOA;
    Long_t Evt = 0;
    Int_t pre_TOA = 0;
    
    TFile *f = TFile::Open("B4_TID_Before_TOA_Scan.root","RECREATE");       //Create a new ROOT file
    TTree t1("t1","TDC data on Board4");                   //Create one branch with all information from the structure.
    t1.Branch("TOA",&TOA,"TOA/I");
    t1.Branch("TOT",&TOT,"TOT/I");
    t1.Branch("Cal",&Cal,"Cal/I");
    t1.Branch("Hit",&Hit,"Hit/I");
    //t1.Branch("DTOA",&DTOA,"DTOA/I");   
    //t1.Branch("Evt",&Evt,"Evt/L");
    
    while(1){
         in >> TOA >> TOT >> Cal >> Hit;
         //DTOA = TOA-pre_TOA;
         //pre_TOA = TOA;
         if (!in.good()) break;
           //if (nlines < 5) printf("x=%8f, y=%8f, z=%8f\n",x,y,z);
         t1.Fill();
         Evt++;
    }
    t1.Print();                         //show branch information
    t1.Show(10);                        //Show Event 10
    t1.Scan();                          //show all value of the list of leaves separated by a colon
    printf("Found %ld points\n",Evt);
    in.close();
    f->Write();
}
