#ifndef anadimuon_h
#define anadimuon_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLatex.h>
#include <TF1.h>
#include "TMath.h"
#include "TLegend.h"
#include "TLorentzVector.h"

// auxiliary function for fitting
Double_t signal(Double_t *x, Double_t *par);
Double_t backgr(Double_t *x, Double_t *par);
Double_t fitfun(Double_t *x, Double_t *par);

class anadimuon {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TFile    *_outFile; 
   Double_t _nev;
   Double_t _mmin;
   Double_t _mmax;
   Int_t    _sdig;

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   TLorentzVector  *dimuon_p4;
   TLorentzVector  *muonP_p4;
   TLorentzVector  *muonN_p4;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_dimuon_p4;   //!
   TBranch        *b_muonP_p4;   //!
   TBranch        *b_muonN_p4;   //!

   anadimuon(TTree *tree=0);
   virtual ~anadimuon();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     GetSpectrum();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void     FitPeak();
   virtual void     FitPeakRoofit();
   virtual void     FillHisto(TH1F* hist);
   virtual void     SaveHisto(TH1F* hist, Int_t log = 0);
   virtual void     SelectPeak();

};

#endif

#ifdef anadimuon_cxx
anadimuon::anadimuon(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("data.root:/");
      dir->GetObject("oniaTree",tree);
   }
   Init(tree);

   _outFile = new TFile("plots/histos.root","update"); 

}

anadimuon::~anadimuon()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
 _outFile->Close();
 delete _outFile;
}

Int_t anadimuon::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t anadimuon::LoadTree(Long64_t entry)
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

void anadimuon::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   dimuon_p4 = 0;
   muonP_p4 = 0;
   muonN_p4 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("dimuon_p4", &dimuon_p4, &b_dimuon_p4);
   fChain->SetBranchAddress("muonP_p4", &muonP_p4, &b_muonP_p4);
   fChain->SetBranchAddress("muonN_p4", &muonN_p4, &b_muonN_p4);

   Notify();
}

Bool_t anadimuon::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void anadimuon::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t anadimuon::Cut(Long64_t entry) 
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.

  double mass = dimuon_p4->M();
  double pt1 = muonN_p4->Pt();
  double pt2 = muonP_p4->Pt();
  if(pt1<10 || pt2<10) return -1;
  if(mass>200) return -1;
  //cout << "Cut mass:" << mass << endl;

   return 1;
}

#endif // #ifdef anadimuon_cxx
