#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include <TH2F.h>
#include <TROOT.h>
#include <TGraphSmooth.h>
#include <TLegend.h>
#include <TF1.h>
#include <fstream>
#include <TMath.h>

Double_t kLowWavelength = 300;
Double_t kHighWavelength = 650;

void read(const char *file, std::vector<Double_t> &xs,
    std::vector<Double_t> &ys, Double_t lowX, Double_t highX,
    Double_t normY = 0.0);

void average(std::vector<Double_t> &xs,std::vector<Double_t> &ys,
    TGraph *graph1, TGraph *graph2, Double_t lowX, Double_t highX);
TGraph* normalizeIntegral(TGraph *graph, Double_t scale = 1.0);

void save(const char *prefix, TGraph *graph, bool save_xml = true,
    const char *unit = 0, Int_t limit = 0);

void process_data()
{
  // Load my style macro
  gROOT->ProcessLine(".x ${JC2ROOTSTYLE}");

  // Prepare canvas and pad
  TCanvas *canvas = new TCanvas("canvas","",600,600);
  TH2F *h = new TH2F("h","Optical Properties of PbWO_{4} & PMT;"
      "Wavelength (nm);Percent (%)",1,kLowWavelength-50,
      kHighWavelength+50,1,0.1,100.);
  //gPad->SetLogy(kTRUE);
  h->SetStats(kFALSE);
  h->Draw();

  // Setup the legend
  TLegend *legend = new TLegend(0.51,0.775,0.90,0.925);
  legend->SetFillColor(0);

  // Setup variables
  std::vector<Double_t> pbwo4_emission_xs;
  std::vector<Double_t> pbwo4_emission_ys;
  std::vector<Double_t> pbwo4_transmission_xs;
  std::vector<Double_t> pbwo4_transmission_ys;
  std::vector<Double_t> pmt_efficiency_xs;
  std::vector<Double_t> pmt_efficiency_ys;

  // Process PbWO4 emission spectrum
  Double_t pbwo4_emission_scale = 50.;
  read("pbwo4_emission.dat",pbwo4_emission_xs,pbwo4_emission_ys,kLowWavelength,
      kHighWavelength);
  TGraphSmooth *pbwo4_emission_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_emission_graph_raw = new TGraph(pbwo4_emission_xs.size(),
      pbwo4_emission_xs.data(),pbwo4_emission_ys.data());
  TGraph *pbwo4_emission_graph_unorm = pbwo4_emission_graph_smoother->SmoothSuper(
      pbwo4_emission_graph_raw);
  TGraph *pbwo4_emission_graph = normalizeIntegral(pbwo4_emission_graph_unorm,
      pbwo4_emission_scale);
  pbwo4_emission_graph->SetLineColor(1757);
  pbwo4_emission_graph->SetMarkerColor(1757);
  pbwo4_emission_graph->SetLineWidth(3.0);
  pbwo4_emission_graph->SetFillColor(0);
  pbwo4_emission_graph->Draw("SAMEC");
  save("sim_pbwo4_emission",pbwo4_emission_graph);
  legend->AddEntry(pbwo4_emission_graph,Form("PbWO_{4} Emission (x%g)",
        pbwo4_emission_scale));


  // Process PbWO4 transmission spectrum
  read("pbwo4_transmission.dat",pbwo4_transmission_xs,pbwo4_transmission_ys,
      kLowWavelength,kHighWavelength);
  TGraphSmooth *pbwo4_transmission_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_transmission_graph_raw = new TGraph(
      pbwo4_transmission_xs.size(),
      pbwo4_transmission_xs.data(),pbwo4_transmission_ys.data());
  TGraph *pbwo4_transmission_graph = pbwo4_transmission_graph_smoother->
    SmoothSuper(pbwo4_transmission_graph_raw);
  pbwo4_transmission_graph->SetLineColor(1759);
  pbwo4_transmission_graph->SetMarkerColor(1759);
  pbwo4_transmission_graph->SetLineWidth(3.0);
  pbwo4_transmission_graph->SetFillColor(0);
  pbwo4_transmission_graph->Draw("SAMEC");
  save("sim_pbwo4_transmission",pbwo4_transmission_graph);
  legend->AddEntry(pbwo4_transmission_graph,"PbWO_{4} Transmission");


  // Process PMT cathode efficiency
  read("pmt_efficiency_all.dat",pmt_efficiency_xs,pmt_efficiency_ys,
      kLowWavelength,kHighWavelength,23.0);
  TGraphSmooth *pmt_efficiency_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pmt_efficiency_graph_raw = new TGraph(pmt_efficiency_xs.size(),
      pmt_efficiency_xs.data(),pmt_efficiency_ys.data());
  TGraph *pmt_efficiency_graph  = pmt_efficiency_graph_smoother->SmoothSuper(
      pmt_efficiency_graph_raw);
  pmt_efficiency_graph->SetLineColor(1756);
  pmt_efficiency_graph->SetMarkerColor(1756);
  pmt_efficiency_graph->SetLineWidth(3.0);
  pmt_efficiency_graph->SetFillColor(0);
  pmt_efficiency_graph->Draw("SAMEC");
  save("sim_pmt_efficiency",pmt_efficiency_graph);
  legend->AddEntry(pmt_efficiency_graph,"PMT Cathode Efficiency");

  // Draw the legend
  legend->Draw();

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_optical_properties.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_optical_properties.png");


  // Clear the previous stuff
  h->Delete();
  legend->Clear();

  // Setup the histogram now
  h = new TH2F("h","PbWO_{4} Ordinary & Extra-ordinary Index Of Refraction;"
      "Wavelength (nm);Index of Refraction",1,kLowWavelength-50,
      kHighWavelength+50,1,2.1,2.6);
  h->SetStats(kFALSE);
  h->Draw();

  // Setup RINDEX variables
  std::vector<Double_t> pbwo4_rindex_ordinary_xs;
  std::vector<Double_t> pbwo4_rindex_ordinary_ys;
  std::vector<Double_t> pbwo4_rindex_extra_xs;
  std::vector<Double_t> pbwo4_rindex_extra_ys;
  std::vector<Double_t> pbwo4_rindex_average_xs;
  std::vector<Double_t> pbwo4_rindex_average_ys;

  // Process PbWO4 rindex_ordinary spectrum
  read("pbwo4_rindex_ordinary.dat",pbwo4_rindex_ordinary_xs,
      pbwo4_rindex_ordinary_ys,kLowWavelength,
      kHighWavelength);
  TGraphSmooth *pbwo4_rindex_ordinary_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_rindex_ordinary_graph_raw = new TGraph(pbwo4_rindex_ordinary_xs.size(),
      pbwo4_rindex_ordinary_xs.data(),pbwo4_rindex_ordinary_ys.data());
  TGraph *pbwo4_rindex_ordinary_graph = pbwo4_rindex_ordinary_graph_smoother->SmoothSuper(
      pbwo4_rindex_ordinary_graph_raw);
  pbwo4_rindex_ordinary_graph->SetLineColor(1757);
  pbwo4_rindex_ordinary_graph->SetMarkerColor(1757);
  pbwo4_rindex_ordinary_graph->SetLineWidth(3.0);
  pbwo4_rindex_ordinary_graph->SetFillColor(0);
  pbwo4_rindex_ordinary_graph->Draw("SAMEC");
  save("sim_pbwo4_rindex_ordinary",pbwo4_rindex_ordinary_graph,false);
  legend->AddEntry(pbwo4_rindex_ordinary_graph,"n ordinary");

  // Process PbWO4 rindex_extra spectrum
  read("pbwo4_rindex_extra.dat",pbwo4_rindex_extra_xs,
      pbwo4_rindex_extra_ys,kLowWavelength,
      kHighWavelength);
  TGraphSmooth *pbwo4_rindex_extra_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_rindex_extra_graph_raw = new TGraph(pbwo4_rindex_extra_xs.size(),
      pbwo4_rindex_extra_xs.data(),pbwo4_rindex_extra_ys.data());
  TGraph *pbwo4_rindex_extra_graph = pbwo4_rindex_extra_graph_smoother->SmoothSuper(
      pbwo4_rindex_extra_graph_raw);
  pbwo4_rindex_extra_graph->SetLineColor(1756);
  pbwo4_rindex_extra_graph->SetMarkerColor(1756);
  pbwo4_rindex_extra_graph->SetLineWidth(3.0);
  pbwo4_rindex_extra_graph->SetFillColor(0);
  pbwo4_rindex_extra_graph->Draw("SAMEC");
  save("sim_pbwo4_rindex_extra",pbwo4_rindex_extra_graph,false);
  legend->AddEntry(pbwo4_rindex_extra_graph,"n extraordinary");

  // Process PbWO4 rindex_average spectrum
  average(pbwo4_rindex_average_xs,pbwo4_rindex_average_ys,
      pbwo4_rindex_ordinary_graph,pbwo4_rindex_extra_graph,
      360.0,650.0);
  TGraphSmooth *pbwo4_rindex_average_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_rindex_average_graph_raw = new TGraph(pbwo4_rindex_average_xs.size(),
      pbwo4_rindex_average_xs.data(),pbwo4_rindex_average_ys.data());
  TGraph *pbwo4_rindex_average_graph = pbwo4_rindex_average_graph_smoother->SmoothSuper(
      pbwo4_rindex_average_graph_raw);
  pbwo4_rindex_average_graph->SetLineColor(1758);
  pbwo4_rindex_average_graph->SetMarkerColor(1758);
  pbwo4_rindex_average_graph->SetLineStyle(2);
  pbwo4_rindex_average_graph->SetLineWidth(3.0);
  pbwo4_rindex_average_graph->SetFillColor(0);
  pbwo4_rindex_average_graph->Draw("SAMEC");
  save("sim_pbwo4_rindex_average",pbwo4_rindex_average_graph,true);
  legend->AddEntry(pbwo4_rindex_average_graph,"n average");

  // Draw the legend
  legend->Draw();

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_rindex.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_rindex.png");

  // Clear the previous stuff
  h->Delete();
  legend->Clear();

  // Setup the histogram now
  h = new TH2F("h","PbWO_{4} Ordinary & Extra-ordinary Absorption Coefficients;"
      "Wavelength (nm);Absorption Coefficient #alpha (cm^{-1})",1,kLowWavelength-50,
      kHighWavelength+50,1,0.0,0.08);
  h->GetYaxis()->SetTitleOffset(1.75);
  h->SetStats(kFALSE);
  h->Draw();

  // Setup Absorption Coefficient and Length variables
  std::vector<Double_t> pbwo4_abscoefficient_ordinary_xs;
  std::vector<Double_t> pbwo4_abscoefficient_ordinary_ys;
  std::vector<Double_t> pbwo4_abscoefficient_extra_xs;
  std::vector<Double_t> pbwo4_abscoefficient_extra_ys;
  std::vector<Double_t> pbwo4_abscoefficient_average_xs;
  std::vector<Double_t> pbwo4_abscoefficient_average_ys;

  // Process PbWO4 abscoefficient_ordinary spectrum
  read("pbwo4_abscoefficient_ordinary.dat",pbwo4_abscoefficient_ordinary_xs,
      pbwo4_abscoefficient_ordinary_ys,kLowWavelength,
      kHighWavelength);
  TGraphSmooth *pbwo4_abscoefficient_ordinary_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_abscoefficient_ordinary_graph_raw = new TGraph(pbwo4_abscoefficient_ordinary_xs.size(),
      pbwo4_abscoefficient_ordinary_xs.data(),pbwo4_abscoefficient_ordinary_ys.data());
  TGraph *pbwo4_abscoefficient_ordinary_graph = pbwo4_abscoefficient_ordinary_graph_smoother->SmoothSuper(
      pbwo4_abscoefficient_ordinary_graph_raw);
  pbwo4_abscoefficient_ordinary_graph->SetLineColor(1757);
  pbwo4_abscoefficient_ordinary_graph->SetMarkerColor(1757);
  pbwo4_abscoefficient_ordinary_graph->SetLineWidth(3.0);
  pbwo4_abscoefficient_ordinary_graph->SetFillColor(0);
  pbwo4_abscoefficient_ordinary_graph->Draw("SAMEC");
  save("sim_pbwo4_abscoefficient_ordinary",pbwo4_abscoefficient_ordinary_graph,false);
  legend->AddEntry(pbwo4_abscoefficient_ordinary_graph,"n ordinary");

  // Process PbWO4 abscoefficient_extra spectrum
  read("pbwo4_abscoefficient_extra.dat",pbwo4_abscoefficient_extra_xs,
      pbwo4_abscoefficient_extra_ys,kLowWavelength,
      kHighWavelength);
  TGraphSmooth *pbwo4_abscoefficient_extra_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_abscoefficient_extra_graph_raw = new TGraph(pbwo4_abscoefficient_extra_xs.size(),
      pbwo4_abscoefficient_extra_xs.data(),pbwo4_abscoefficient_extra_ys.data());
  TGraph *pbwo4_abscoefficient_extra_graph = pbwo4_abscoefficient_extra_graph_smoother->SmoothSuper(
      pbwo4_abscoefficient_extra_graph_raw);
  pbwo4_abscoefficient_extra_graph->SetLineColor(1756);
  pbwo4_abscoefficient_extra_graph->SetMarkerColor(1756);
  pbwo4_abscoefficient_extra_graph->SetLineWidth(3.0);
  pbwo4_abscoefficient_extra_graph->SetFillColor(0);
  pbwo4_abscoefficient_extra_graph->Draw("SAMEC");
  save("sim_pbwo4_abscoefficient_extra",pbwo4_abscoefficient_extra_graph,false);
  legend->AddEntry(pbwo4_abscoefficient_extra_graph,"n extraordinary");

  // Process PbWO4 abscoefficient_average spectrum
  average(pbwo4_abscoefficient_average_xs,pbwo4_abscoefficient_average_ys,
      pbwo4_abscoefficient_ordinary_graph,pbwo4_abscoefficient_extra_graph,
      360.0,650.0);
  TGraphSmooth *pbwo4_abscoefficient_average_graph_smoother = new TGraphSmooth("supsmu");
  TGraph *pbwo4_abscoefficient_average_graph_raw = new TGraph(pbwo4_abscoefficient_average_xs.size(),
      pbwo4_abscoefficient_average_xs.data(),pbwo4_abscoefficient_average_ys.data());
  TGraph *pbwo4_abscoefficient_average_graph = pbwo4_abscoefficient_average_graph_smoother->SmoothSuper(
      pbwo4_abscoefficient_average_graph_raw);
  pbwo4_abscoefficient_average_graph->SetLineColor(1758);
  pbwo4_abscoefficient_average_graph->SetMarkerColor(1758);
  pbwo4_abscoefficient_average_graph->SetLineWidth(3.0);
  pbwo4_abscoefficient_average_graph->SetFillColor(0);
  pbwo4_abscoefficient_average_graph->Draw("SAMEC");
  save("sim_pbwo4_abscoefficient_average",pbwo4_abscoefficient_average_graph,
      false);
  legend->AddEntry(pbwo4_abscoefficient_average_graph,"n average");

  // Draw the legend
  legend->Draw();

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_abscoefficient.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_abscoefficient.png");

  // Clear the previous stuff
  h->Delete();
  legend->Clear();

  // Setup the histogram now
  h = new TH2F("h","PbWO_{4} Ordinary & Extra-ordinary Absorption Lengths;"
      "Wavelength (nm);Absorption Length #lambda (cm)",1,kLowWavelength-50,
      kHighWavelength+50,1,10.0,1600.0);
  h->GetYaxis()->SetTitleOffset(1.75);
  h->SetStats(kFALSE);
  h->Draw();


  // Setup Absorption Coefficient and Length variables
  std::vector<Double_t> pbwo4_abslength_ordinary_xs;
  std::vector<Double_t> pbwo4_abslength_ordinary_ys;
  std::vector<Double_t> pbwo4_abslength_extra_xs;
  std::vector<Double_t> pbwo4_abslength_extra_ys;
  std::vector<Double_t> pbwo4_abslength_average_xs;
  std::vector<Double_t> pbwo4_abslength_average_ys;

  // Process PbWO4 abslength_ordinary spectrum
  for(Int_t i = 0; i < pbwo4_abscoefficient_ordinary_graph->GetN(); i++) {
    pbwo4_abslength_ordinary_xs.push_back(
        pbwo4_abscoefficient_ordinary_graph->GetX()[i]);
    pbwo4_abslength_ordinary_ys.push_back(1.0/
        pbwo4_abscoefficient_ordinary_graph->GetY()[i]);
  }
  TGraph *pbwo4_abslength_ordinary_graph = new TGraph(
      pbwo4_abslength_ordinary_xs.size(),
      pbwo4_abslength_ordinary_xs.data(),
      pbwo4_abslength_ordinary_ys.data());
  pbwo4_abslength_ordinary_graph->SetLineColor(1757);
  pbwo4_abslength_ordinary_graph->SetMarkerColor(1757);
  pbwo4_abslength_ordinary_graph->SetLineWidth(3.0);
  pbwo4_abslength_ordinary_graph->SetFillColor(0);
  pbwo4_abslength_ordinary_graph->Draw("SAMEC");
  save("sim_pbwo4_abslength_ordinary",pbwo4_abslength_ordinary_graph,false);
  legend->AddEntry(pbwo4_abslength_ordinary_graph,"n ordinary");

  // Process PbWO4 abslength_extra spectrum
  for(Int_t i = 0; i < pbwo4_abscoefficient_extra_graph->GetN(); i++) {
    pbwo4_abslength_extra_xs.push_back(
        pbwo4_abscoefficient_extra_graph->GetX()[i]);
    pbwo4_abslength_extra_ys.push_back(1.0/
        pbwo4_abscoefficient_extra_graph->GetY()[i]);
  }
  TGraph *pbwo4_abslength_extra_graph = new TGraph(
      pbwo4_abslength_extra_xs.size(),
      pbwo4_abslength_extra_xs.data(),
      pbwo4_abslength_extra_ys.data());
  pbwo4_abslength_extra_graph->SetLineColor(1756);
  pbwo4_abslength_extra_graph->SetMarkerColor(1756);
  pbwo4_abslength_extra_graph->SetLineWidth(3.0);
  pbwo4_abslength_extra_graph->SetFillColor(0);
  pbwo4_abslength_extra_graph->Draw("SAMEC");
  save("sim_pbwo4_abslength_extra",pbwo4_abslength_extra_graph,false,"1./cm");
  legend->AddEntry(pbwo4_abslength_extra_graph,"n extraordinary");

  // Process PbWO4 abslength_average spectrum
  for(Int_t i = 0; i < pbwo4_abscoefficient_average_graph->GetN(); i++) {
    pbwo4_abslength_average_xs.push_back(
        pbwo4_abscoefficient_average_graph->GetX()[i]);
    pbwo4_abslength_average_ys.push_back(1.0/
        pbwo4_abscoefficient_average_graph->GetY()[i]);
  }
  TGraph *pbwo4_abslength_average_graph = new TGraph(
      pbwo4_abslength_average_xs.size(),
      pbwo4_abslength_average_xs.data(),
      pbwo4_abslength_average_ys.data());
  pbwo4_abslength_average_graph->SetLineColor(1758);
  pbwo4_abslength_average_graph->SetMarkerColor(1758);
  pbwo4_abslength_average_graph->SetLineWidth(3.0);
  pbwo4_abslength_average_graph->SetFillColor(0);
  pbwo4_abslength_average_graph->Draw("SAMEC");
  save("sim_pbwo4_abslength_average",pbwo4_abslength_average_graph,true,"cm");
  legend->AddEntry(pbwo4_abslength_average_graph,"n average");

  // Draw the legend
  legend->Draw();

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_abslength.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/pbwo4_abslength.png");

  // Clear the previous stuff
  h->Delete();
  legend->Clear();

  // Setup the histogram now
  h = new TH2F("h","MyLar (BoPET) Index Of Refraction;"
      "Wavelength (nm);Index of Refraction",1,kLowWavelength-50,
      kHighWavelength+50,1,1.62,1.7);
  h->SetStats(kFALSE);
  h->Draw();

  // Setup RINDEX variables
  std::vector<Double_t> mylar_rindex_xs;
  std::vector<Double_t> mylar_rindex_ys;

  // Process PbWO4 rindex_ordinary spectrum
  read("mylar_rindex.dat",mylar_rindex_xs,
      mylar_rindex_ys,kLowWavelength,
      kHighWavelength);
  TGraph *mylar_rindex_graph = new TGraph(mylar_rindex_xs.size(),
      mylar_rindex_xs.data(),mylar_rindex_ys.data());
  mylar_rindex_graph->SetLineColor(1757);
  mylar_rindex_graph->SetMarkerColor(1757);
  mylar_rindex_graph->SetLineWidth(3.0);
  mylar_rindex_graph->SetFillColor(0);
  mylar_rindex_graph->Draw("SAMEC");
  save("sim_mylar_rindex",mylar_rindex_graph,true,0,25);
  //legend->AddEntry(mylar_rindex_graph,"n ordinary");

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/mylar_rindex.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/mylar_rindex.png");

  // Clear the previous stuff
  h->Delete();
  legend->Clear();

  // Setup the histogram now
  h = new TH2F("h","Air Index Of Refraction;"
      "Wavelength (nm);Index of Refraction",1,kLowWavelength-50,
      kHighWavelength+50,1,1.00027,1.00030);
  h->SetStats(kFALSE);
  h->Draw();

  // Setup RINDEX variables
  std::vector<Double_t> air_rindex_xs;
  std::vector<Double_t> air_rindex_ys;

  // Process PbWO4 rindex_ordinary spectrum
  read("air_rindex.dat",air_rindex_xs,
      air_rindex_ys,kLowWavelength,
      kHighWavelength);
  TGraph *air_rindex_graph = new TGraph(air_rindex_xs.size(),
      air_rindex_xs.data(),air_rindex_ys.data());
  air_rindex_graph->SetLineColor(1757);
  air_rindex_graph->SetMarkerColor(1757);
  air_rindex_graph->SetLineWidth(3.0);
  air_rindex_graph->SetFillColor(0);
  air_rindex_graph->Draw("SAMEC");
  save("sim_air_rindex",air_rindex_graph,true,0);
  //legend->AddEntry(air_rindex_graph,"n ordinary");

  // Save canvas
  canvas->SaveAs("/home/cornejo/public_html/psim/air_rindex.pdf");
  canvas->SaveAs("/home/cornejo/public_html/psim/air_rindex.png");
}



void read(const char *file, std::vector<Double_t> &xs,
    std::vector<Double_t> &ys, Double_t lowX, Double_t highX,
    Double_t normY)
{
  ifstream in;
  in.open(file,std::ios::in);
  Double_t x = 0.0;
  Double_t y = 0.0;
  Double_t maxY = -1.0e6;
  while(
      in >> x &&
      in >> y &&
      !in.eof() ) {
    if(x >= lowX && x <= highX) {
      xs.push_back(x);
      ys.push_back(y);
      if(y > maxY)
        maxY = y;
    }
  }
  if(normY>0 && maxY > 0) {
    Double_t norm = normY/maxY;
    for(size_t i = 0; i < ys.size(); i++) {
      ys[i] *= norm;
    }
  }
}

void average(std::vector<Double_t> &xs,std::vector<Double_t> &ys,
    TGraph *graph1, TGraph *graph2, Double_t lowX, Double_t highX)
{
  if(!graph1 || !graph2)
    return;

  Int_t n1 = graph1->GetN();
  Int_t n2 = graph2->GetN();

  if(n1 < 3 || n2 < 3 )
    return;

  Double_t *datX1 = graph1->GetX();
  Double_t *datX2 = graph2->GetX();

  // Want to make sure the lowest value is within the data
  lowX = lowX > datX1[0] ? lowX : datX1[0];
  lowX = lowX > datX2[0] ? lowX : datX2[0];
  // Want to make sure the highest value is within the data
  highX = highX < datX1[n1-1] ? highX : datX1[n1-1];
  highX = highX < datX2[n2-1] ? highX : datX2[n2-1];

  Double_t step = (highX-lowX)/Double_t( n1<n2 ? n1 : n2 );
  for(Double_t x = lowX; x <= highX; x += step ) {
    xs.push_back(x);
    ys.push_back((graph1->Eval(x) + graph2->Eval(x))/2.0);
  }
}



void save(const char *prefix, TGraph *graph, bool save_xml, const char *unit,
    Int_t limit)
{
  if(!graph)
    return;

  ofstream out;
  out.open(Form("%s.dat",prefix),std::ios::out);

  std::cout << "Saving data to " << Form("%s.dat",prefix) << std::endl;

  Int_t n = graph->GetN();
  Int_t step = 1;
  if(limit>0 && n>limit)
    step = n/limit;
  Double_t *xs = graph->GetX();
  Double_t *ys = graph->GetY();
  Double_t hc = 1239.84193; // eV*nm
  Double_t e = 0.0;
  for(Int_t i = 0; i < n; i++ ) {
    out << Form("%g %g",xs[i],ys[i]) << std::endl;
  }
  out.close();

  const char* unitPrefix = !unit ? "" : Form("*%s",unit);
  // Now save the XML
  if(save_xml) {
    out.open(Form("%s.xml",prefix),std::ios::out);
    std::cout << "Generating XML file: " << Form("%s.xml",prefix) << std::endl;
    e = hc/(xs[n-1]+0.5);
    out << Form("<matrix name=\"%s\" coldim=\"2\"",prefix);
    out << std::endl;
    out << Form("  values=\"");
    //out << Form("%g*eV %g%s",e,0.0,unitPrefix);
    for(Int_t i = n-1; i >= 0; i-=step ) {
      e = hc/xs[i];
      out <<  Form(" %g*eV %g%s",e,ys[i],unitPrefix);
    }
    e = hc/(xs[0]-0.5);
    //out <<  Form(" %g*eV %g%s",e,0.0,unitPrefix);
    out << "\" />";
  }
}


TGraph* normalizeIntegral(TGraph *graph, Double_t scale)
{
  if(!graph)
    return 0;

  Int_t n = graph->GetN();
  if(n<=0)
    return 0;

  Double_t *xs = graph->GetX();
  Double_t *ys = graph->GetY();
  Double_t integral = graph->Integral();
  Double_t norm = scale*100./integral;

  std::vector<Double_t> normX;
  std::vector<Double_t> normY;
  for(Int_t i = 0; i < n; i++) {
    normX.push_back(xs[i]);
    normY.push_back(ys[i]*norm);
  }

  return new TGraph(normX.size(),normX.data(),normY.data());
}
