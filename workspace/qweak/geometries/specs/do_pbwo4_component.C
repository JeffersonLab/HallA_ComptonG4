{
  ifstream in;
  in.open("pbwo4_emission.dat",ios::in);
  Double_t f = 0;
  Double_t i = 0;
  std::vector<Double_t> fs;
  std::vector<Double_t> is;
  while(
      in >> f &&
      in >> i &&
      !in.eof() ) {
    fs.push_back(f);
    is.push_back(i);
  }
  Double_t hc = 1239.84193; // eV*nm
  for(UInt_t k = fs.size()-1; k>0; k-- ) {
    std::cout << hc/fs[k] << "*eV " << is[k] << " ";
  }
  std::cout << std::endl;
}
