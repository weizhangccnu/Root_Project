
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
void buildTree() {
// read file TDC/TDC_Converted_Data_10000000P_B1_0.dat
// this file has 4 columns of interger data, the first one is event number
//   TString dir = gROOT->GetTutorialDir();
   ifstream in;
  // in.open("/users/dgong/workarea/TDC/TID/TDC_TID_Merged.dat");
   in.open("/users/dgong/workarea/TDC/TID/TDC_TID_Test_20000000P_B4_Pulse=0x03_VDD1V2_TOAScan.dat");
   Int_t TOA,TOT,Cal,Hit,DTOA;
   Long_t Evt = 1;
   Int_t pre_TOA = 0;
   
   TFile *f = TFile::Open("B4_TID_after_TOAScan.root","RECREATE");
   TTree t1("t1","TDC data on Board1");
   t1.Branch("TOA",&TOA,"TOA/I");
   t1.Branch("TOT",&TOT,"TOT/I");
   t1.Branch("Cal",&Cal,"Cal/I");
   t1.Branch("Hit",&Hit,"Hit/I");
   t1.Branch("DTOA",&DTOA,"DTOA/I");   
   t1.Branch("Evt",&Evt,"Evt/L");
   TString s1;
   TString s2;
   while (1) {
      in>>s1>>s2>> TOA >> TOT >> Cal >> Hit;
	  DTOA = TOA-pre_TOA;
	  pre_TOA = TOA;
      if (!in.good()) break;
//      if (nlines < 5) printf("x=%8f, y=%8f, z=%8f\n",x,y,z);
      t1.Fill();
      Evt++;
      if(Evt%10000==0)cout<<s1<<" "<<s2<<" "<<TOA<<" "<<TOT<<" "<<Cal<<" "<<Hit<<endl;
   }
   printf(" found %ld points\n",Evt);
   in.close();
   f->Write();
}
