
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TPaveStats.h"

void pB1(Int_t code) {

   Int_t TOA,TOT,Cal,Hit;
   Long64_t Evt;

   TFile *f;
   Int_t lowBin = 30;
   Int_t highBin = 680;
   Long64_t lowE = 10000; 
   Int_t calN=1;
   Bool_t isRaw = false;
    TString ss = "B4_0x03_";
    
    Int_t evtN = 1; //10 M event
    
   if(code == 0x01){
     f= TFile::Open("B4_TID_before.root");
     lowBin = 30;
     highBin = 675;
     lowE = 38000;
       ss = "B4_BeforeTIDTest";
	 evtN = 2;  
   }
   else if(code == 0x02){
     f= TFile::Open("B4_TID_after_TOAScan.root");
     lowBin = 30;
     highBin = 675;
     lowE = 25000;
       ss = "B4_AfterTIDTest";
	 evtN = 2;  
   }   
   else
     {
       cout<<"Invalidated code\n";
       return;
     }
   TTree*t1 = (TTree*)f->Get("t1");

    
   t1->SetBranchAddress("TOA",&TOA);   
   t1->SetBranchAddress("TOT",&TOT);      
   t1->SetBranchAddress("Hit",&Hit);      
   t1->SetBranchAddress("Cal",&Cal);   
   t1->SetBranchAddress("Evt",&Evt);   

    Int_t bins = highBin - lowBin + 1;
    Double_t lB = lowBin - 0.5;
    Double_t hB = highBin + 0.5;
    
    Long64_t upE;
    if(evtN == 1){
        upE = lowE+61*160000;
    }
    else { //2
        upE = lowE+122*160000;
    }
    Double_t timeRes = 0.3125; //Depends on the frequency
    Long64_t period = 40000;

    TH2F *th2d = new TH2F("th2d", "Time vs TOA code", bins, lB ,hB, 160000, 0.5*timeRes, (160000-0.5)*timeRes);

 //   TH1F *th1d = new TH1F("th1d", "",160000, 0.5*timeRes, (160000-0.5)*timeRes);
    
    TH2F *hTOTTOA = new TH2F("hTOTTOA","TOT vs TOA",bins, lB ,hB, 10, 172.5,182.5);
    
   TH1F *hTOA = new TH1F("hTOA","TOA code distribution", bins, lB ,hB);
   TH1F *hBin = new TH1F("hBin","TOA bin size", bins, lB ,hB);
   TH2F *hBin2d = new TH2F("hBin2d","TOA bin size vs (TOA modula 126 and TOA/126)", 126, -0.5,125.5, 6,-0.5,5.5);

   TH1F *hTran = new TH1F("hTran","TOA transfer function", bins, lB ,hB);

   TH1F *hINL = new TH1F("hINL","TOA INL function with linear fit", bins, lB ,hB);
   TH1F *hINL2 = new TH1F("hINL2","TOA INL function without fit", bins, lB ,hB);
    TH1F *hINL3 = new TH1F("hINL3","TOA INL function from profile fit", bins, lB ,hB);

   TH1F *hDNL = new TH1F("hDNL","TOA DNL function", bins, lB ,hB);

   TH1F *hOdd = new TH1F("hOdd","",100,2000,12000);
   TH1F *hEven = new TH1F("hEven","",100,2000,12000);

   TH1F *hCal = new TH1F("hCal","Calibration code distribution",100,calN*175-50.5,calN*175+49.5);
    
   Long64_t nentries = t1->GetEntries();
  /*
    Int_t timeArray2[160000];
    Int_t timeArray[160000];
    for(Int_t ii = 0; ii<160000;ii++){
        timeArray[ii] = 0;
        timeArray2[ii] = 0;

    } //clear
    */
   for (Long64_t i=0;i<nentries;i++) {
     t1->GetEntry(i);
 /*
       if(Hit == 1 && TOA>=(lowBin-1)&&TOA<=highBin && Evt>lowE && Evt<upE){
         Int_t time = (Evt-lowE)%period;
         if(TOA%2 == 0){ timeArray[time] += 1;}
         else{timeArray[time] -= 1;}
     }*/
       
	 if(TOA>=lowBin&&TOA<=highBin&&Hit==1)//cut
	 {
         if(Evt>lowE&&Evt<upE){
             hTOTTOA->Fill(TOA,TOT);
             hTOA->Fill(TOA);
             //if((TOA-(TOA%63))/63==10){
                 th2d->Fill(TOA, timeRes*((Evt-lowE)%period));
		 Int_t calCode = isRaw?Cal-TOA:Cal;
		 hCal->Fill(calCode);
             //}
         }
	 }
   }

    hTOTTOA->SetXTitle("TOA code");
    hTOTTOA->SetYTitle("TOT code");
    hTOTTOA->SetMarkerStyle(7);
    hTOTTOA->SetMarkerColor(kRed);


    /*
    for(Int_t ii = 4; ii<160000-4;ii++){
        timeArray2[ii] = timeArray[ii-3]+timeArray[ii-2]+timeArray[ii-1]+timeArray[ii]+timeArray[ii+1]+timeArray[ii+2]+timeArray[ii+3];
    } //average
    
    Int_t TOACount = lowBin-1;
    Double_t pre_edge = 0;
    Double_t cur_edge = 0;
    for(Int_t ii = 1; ii<160000;ii++){
      if((timeArray2[ii-1]>0&&timeArray2[ii]<=0)||
         (timeArray2[ii-1]<0&&timeArray2[ii]>=0))
      {
          pre_edge = cur_edge;
          TOACount++;
          cur_edge = ii*timeRes;
          cout<<ii<<" "<<TOACount<<" cur: "<<cur_edge<<" delt: "<<cur_edge - pre_edge<<endl;
      }
   }
*/
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);

    
  hTOA->SetFillColor(kRed);
  hTOA->SetLineWidth(0);  
  hTOA->SetAxisRange(0,12000,"Y");
  hTOA->SetXTitle("TOA code");
  hTOA->SetYTitle("Counts");
  hTOA->GetXaxis()->CenterTitle();  
  hTOA->GetYaxis()->CenterTitle();  
  hTOA->Draw();
  
 //Transfer function
    Double_t sum = 0;
    Int_t n = bins;
    Double_t weight = 1.0/hTOA->GetEntries();
    
    for(Int_t i = 1; i<=n;i++){
        hBin->SetBinContent(i,hTOA->GetBinContent(i)*weight*n);
	//cout<<i<<" "<<hBin->GetBinContent(i)<<endl;
        hBin2d->SetBinContent((i-1)%126+1,(i-1-((i-1)%126))/126+1, hBin->GetBinContent(i));
        sum += hTOA->GetBinContent(i)*weight;
        hTran->SetBinContent(i,sum);
        if(i%2 == 1)hOdd->Fill(hTOA->GetBinContent(i));
        else hEven->Fill(hTOA->GetBinContent(i));
    }
    hTran->Fit("pol1","FQ");
    TF1 *fit = hTran->GetFunction("pol1");
    Double_t p0 = fit->GetParameter(0);
    Double_t p1 = fit->GetParameter(1);
    
////INL,DNL
    Double_t aa = 0;
    for(Int_t i = 1; i<=n;i++){
        Double_t y = hTran->GetBinContent(i);
        Double_t x = hTran->GetXaxis()->GetBinCenter(i);
        Double_t delty = (y-(p0+x*p1))*n;
        hINL->SetBinContent(i,delty);
        hINL2->SetBinContent(i,y*n-i);
        aa+=i;
        cout<<i<<" "<<y*n-i<<" "<<delty<<endl;
        
        hDNL->SetBinContent(i,hBin->GetBinContent(i)-1);
    }
    
    hINL->SetLineColor(kRed);
    hINL->SetMarkerColor(kBlue);
    hINL->SetMarkerStyle(7);
    hINL->SetXTitle("TOA code");
    hINL->SetYTitle("INL(LSB)");

    hINL2->SetLineColor(kRed);
    hINL2->SetMarkerColor(kBlue);
    hINL2->SetMarkerStyle(7);
    hINL2->SetXTitle("TOA code");
    hINL2->SetYTitle("INL(LSB)");

    hINL3->SetLineColor(kRed);
    hINL3->SetMarkerColor(kBlue);
    hINL3->SetMarkerStyle(7);
    hINL3->SetXTitle("TOA code");
    hINL3->SetYTitle("INL(LSB)");

    
    hBin2d->SetXTitle("TOA code modular 126");
    hBin2d->SetYTitle("TOA code divide 126");
    hBin2d->SetZTitle("TOA Bin size");

//t1->Draw("(TOA*17.77+8142)-0.625*((Evt-10000)%40000):TOA","Evt>10000&&Evt<9980000&&Hit==1&&TOA>=30&&TOA<=680");
    hOdd->SetXTitle("Counts");
    hEven->SetXTitle("Counts");
    hOdd->GetXaxis()->CenterTitle();
    hEven->GetYaxis()->CenterTitle();
    hEven->SetLineColor(kRed);
    hOdd->SetLineColor(kBlue);


    
    gStyle->SetOptFit(1111);

    th2d->SetYTitle("Time (ps)");
    th2d->SetXTitle("TOA code");
    th2d->Draw();

    th2d->SetMarkerStyle(7);
    th2d->SetMarkerColor(kBlue);
    TProfile *prof=th2d->ProfileX();
    
    prof->Fit("pol1");
    TF1* fitfunc = prof->GetFunction("pol1");
    TH2F *th2dINL = new TH2F("th2dINL", "Time Error vs TOA code", bins, lB ,hB, 400, -200, 200);
    th2dINL->SetXTitle("TOA code");
    th2dINL->SetYTitle("Time error (ps)");
    
    TH1F* hRes = new TH1F("hRes","",200,-50,50);
    hRes->SetXTitle("Residual Time error (ps)");
    hRes->SetYTitle("Count");


    Long64_t ii = 0;
    for (Long64_t i=0;i<nentries;i++) {
      t1->GetEntry(i);
      if(TOA>=lowBin&&TOA<=highBin&&Hit==1)//cut
      {
          if(Evt>lowE&&Evt<upE){
              ii++;
              Double_t fitVal = fitfunc->Eval(TOA);
              Double_t error = fitVal-timeRes*((Evt-lowE)%period);
//              if(ii%1000== 0 )cout<<ii<<" "<<TOA<<" "<<fitVal<<" "<<error<<endl;
                  th2dINL->Fill(TOA, error);
		  hRes->Fill(error);
          }
      }
    }
    
    Double_t binsize = fitfunc->GetParameter(1);
    for(Int_t ii = lowBin; ii<=highBin;ii++){
        Double_t fitVal = fitfunc->Eval(ii);
        Double_t actualTime = prof->GetBinContent(ii-lowBin+1);
        hINL3->SetBinContent(ii-lowBin+1, (actualTime-fitVal)/binsize);
    }
    //    hRes->Fit("gaus");
    th2dINL->SetMarkerColor(kRed);
    th2dINL->SetMarkerStyle(7);
    
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.5);
    gStyle->SetStatW(0.2);
    gStyle->SetStatH(0.1);

    
    prof->Draw("same");
    c1->SetGrid();    
    c1->Update();
 /*
    TPaveStats *st = (TPaveStats*)prof->FindObject("stats");
  
    st->SetX1NDC(10); //new x start position
    st->SetX2NDC(260); //new x end position
    st->SetY1NDC(10); //new x start position
    st->SetY2NDC(260); //new x end position
*/

    TString dir = "/users/dgong/workarea/TDC/TID/plots/";
    TString filename = dir+ss+th2d->GetName()+".gif";
    c1->SaveAs(filename);

    th2dINL->Draw("COLZ");
    filename = dir+ss+th2dINL->GetName()+".gif";
    c1->SaveAs(filename);

    gStyle->SetOptStat(0);
    filename = dir+ss+hINL->GetName()+".gif";
    hINL->Draw();
    c1->SaveAs(filename);

    filename = dir+ss+hINL2->GetName()+".gif";
    hINL2->Draw();
    c1->SaveAs(filename);

    filename = dir+ss+hINL3->GetName()+".gif";
	hINL3->SetMarkerStyle(1);	
    hINL3->Draw();
	
    c1->SaveAs(filename);

    TProfile *prof1 = hTOTTOA->ProfileX();
    prof1->SetMarkerColor(kBlue);
    hTOTTOA->Draw();
    prof1->Draw("same");
    filename = dir+ss+hTOTTOA->GetName()+".gif";	
    c1->SaveAs(filename);

	hBin->Draw();
	filename = dir+ss+hBin->GetName()+".gif";
    c1->SaveAs(filename);

}


