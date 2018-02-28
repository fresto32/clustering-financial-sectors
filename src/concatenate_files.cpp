#include <iostream>
#include <fstream>
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include <algorithm>

struct comparator {
  bool operator() (boost::filesystem::path i, boost::filesystem::path j)
  {
    return (i>j);
  }
} pathsComparator;

void concatenate_files() {
  boost::filesystem::path targetDir("./1997-2017");
  boost::filesystem::recursive_directory_iterator it(targetDir), eod;

  std::string x;
  
  std::vector<boost::filesystem::path> paths_vec;

  BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod)) {
    if (boost::filesystem::is_regular_file(p)) {
      std::cout << p.string() << std::endl;
      paths_vec.push_back(p); 
    }
  }

  std::sort(paths_vec.rbegin(), paths_vec.rend(), pathsComparator);

  std::ifstream inFile;
  std::ofstream outFile;
  outFile.open ("financial_data.txt");

  for (std::vector<boost::filesystem::path>::const_iterator 
      it(paths_vec.begin()); it != paths_vec.end(); it++) {
    std::cout << "File: " << *it << std::endl;
    inFile.open(it->string());
      
    while (inFile >> x) outFile << x << std::endl;
    inFile.close();
    }
  outFile.close();
}

int main() {
  concatenate_files();

  return 0;
}
