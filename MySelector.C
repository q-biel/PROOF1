#define MySelector_cxx
// The class definition in MySelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("MySelector.C")
// root> T->Process("MySelector.C","some options")
// root> T->Process("MySelector.C+")
//

#include "MySelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TStopwatch.h>
#include <iostream>
#include <fstream>




TStopwatch timer;
TH1F *h_x, *h_y;
TFile *out_file;
TDatime tBegin, tNow;
double realTime = 0;
double cpuTime = 0;

void MySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   TString option = GetOption();
   Info("Begin", "starting analysis with process option: %s", option.Data());
   
   printf("-------------Begin of Job-------------------");
   out_file = new TFile("outFile.root", "RECREATE");
   tBegin.Set();
   tBegin.Print();
   timer.Reset();
   timer.Start(kTRUE);
   

}

void MySelector::SlaveBegin(TTree * tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
   
   Init(tree);

   TString option = GetOption();
   Info("SlaveBegin","starting analysis with process option: %s (tree: %p)", option.Data(), tree);

   
   h_x = new TH1F("x", "x", 100,-10,10);
   h_y = new TH1F("y", "y", 100, -10, 10);
   fOutput -> AddAll(gDirectory->GetList());
   

}

Bool_t MySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MySelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   
   
   GetEntry(entry);
   //++fProcessed;
   h_x->Fill(x);
   h_y->Fill(y);
   return kTRUE;
}

void MySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  //printf("Number of Events: %u, elapsed time : %i sec, rate: %g evts/s\n",fNumberOfEvents, tNow.Convert()-tBegin.Convert(), float(fNumberOfEvents)/(tNow.Convert()-tBegin.Convert()));
  
}

void MySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   
   h_x = dynamic_cast<TH1F *>(fOutput->FindObject("x"));
   TCanvas x_result("x_result", "X", 100,100,300,300);
   h_x->Draw();
   x_result.SaveAs("x.png");
   h_y = dynamic_cast<TH1F *>(fOutput->FindObject("y"));
   TCanvas y_result("y_result", "Y", 100,100,300,300);
   h_y->Draw();
   y_result.SaveAs("y.png");
   h_x->Write();
   h_y->Write();
   out_file->Close();
   tNow.Set();
   realTime = timer.RealTime();
   cpuTime = timer.CpuTime();
   printf("---------------End of Job---------------------");
   tNow.Print();
   //printf("Terminate, processed %d events\n", fProcessed);
   printf("Real time: %.2f sec \n CPU time: %.2f sec \n", realTime, cpuTime); 
   ofstream myfile;
   myfile.open("timing.txt", ios::app);
   myfile << realTime << endl;
   myfile.close();
   


}
