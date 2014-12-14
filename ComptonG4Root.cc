/*
 * 
 */
// ROOT headers
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>
#include <TRint.h>
#include <cstdlib>

int main(int argc, char** argv)
{
  // Start ComptonG4 ROOT command lined
  TRint* rint = new TRint("ComptonG4 ROOT Analyzer", &argc, argv);
  // Set some paths
  TString path = getenv("COMPTONG4");
  // So that it can work with the installed version too
  gROOT->ProcessLine(".include " + path + "/include");
  gROOT->ProcessLine("gSystem->Load(\"libCint.so\");");
  gROOT->ProcessLine("gSystem->Load(\"libComptonG4.so\");");
  // Run the interface
  rint->Run();
  // Delete object
  delete rint;
}
