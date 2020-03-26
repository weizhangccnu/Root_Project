//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 16 15:43:26 2020 by ROOT version 6.18/04
// from TTree t1/TDC data on Board1
// found on file: B4_TID.root
//////////////////////////////////////////////////////////

#ifndef FilterTIDData_h
#define FilterTIDData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class FilterTIDData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           TOA;
   Int_t           TOT;
   Int_t           Cal;
   Int_t           Hit;
   Int_t           DTOA;
   Long64_t        Evt;

   // List of branches
   TBranch        *b_TOA;   //!
   TBranch        *b_TOT;   //!
   TBranch        *b_Cal;   //!
   TBranch        *b_Hit;   //!
   TBranch        *b_DTOA;   //!
   TBranch        *b_Evt;   //!

   FilterTIDData(TTree *tree=0);
   virtual ~FilterTIDData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef FilterTIDData_cxx
FilterTIDData::FilterTIDData(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("B4_TID.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("B4_TID.root");
      }
      f->GetObject("t1",tree);

   }
   Init(tree);
}

FilterTIDData::~FilterTIDData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t FilterTIDData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t FilterTIDData::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void FilterTIDData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("TOA", &TOA, &b_TOA);
   fChain->SetBranchAddress("TOT", &TOT, &b_TOT);
   fChain->SetBranchAddress("Cal", &Cal, &b_Cal);
   fChain->SetBranchAddress("Hit", &Hit, &b_Hit);
   fChain->SetBranchAddress("DTOA", &DTOA, &b_DTOA);
   fChain->SetBranchAddress("Evt", &Evt, &b_Evt);
   Notify();
}

Bool_t FilterTIDData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void FilterTIDData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t FilterTIDData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef FilterTIDData_cxx
