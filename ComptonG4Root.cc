/*
 * 
 */
// ROOT headers
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>
#include <TRint.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
  // Start ComptonG4 ROOT command lined
  TRint* rint = new TRint("ComptonG4 ROOT Analyzer", &argc, argv);
  // Set some paths
  TString path = getenv("COMPTONG4");
  // So that it can work with the installed version too
  gROOT->ProcessLine(".include " + path + "/include");
  // Load either CINT for ROOT5 or less, and cling for ROOT 6 and above
  if(gROOT->GetVersionInt()<60000) {
    gROOT->ProcessLine("gSystem->Load(\"libCint.so\");");
  } else {
    gROOT->ProcessLine("gSystem->Load(\"libCling.so\");");
  }
  // Load the dynamic library for ComptonG4 (this adds all the dictionaries)
  gROOT->ProcessLine("gSystem->Load(\"libComptonG4.so\");");

  // Run the interface
  rint->Run();
  // Delete object
  delete rint;
}
