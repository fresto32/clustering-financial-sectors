#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

template<class Iter, class T>
extern Iter binary_find(Iter begin, Iter end, T val);

void preprocess_data(std::vector<std::string> contiguous_tickers) {
  std::ifstream inFile;
  inFile.open("financial_data.txt");

  std::ofstream outFile;
  outFile.open("financial_data_clean_03_17.txt");

  std::string x;
  int index_of_line = 0;
  bool contiguous_ticker = false;
  int index_of_ticker;

  while (inFile >> x) {
    if (index_of_line == 0) {
      if (std::binary_search(contiguous_tickers.begin(), 
            contiguous_tickers.end(), x)) {
          index_of_ticker = std::distance(contiguous_tickers.begin(),
              binary_find(contiguous_tickers.begin(), 
                contiguous_tickers.end(), x));
          contiguous_ticker = true;
          outFile << x << " ";
          std::cout << "Printed: " << x << std::endl;;
          }
    }
    if ((index_of_line == 5) &&  (contiguous_ticker)) {
      outFile << x << std::endl;
      std::cout << "Printed: " << x << std::endl;
      contiguous_ticker = false;
    }
    if (index_of_line == 6) index_of_line = 0;
    else index_of_line++;
  }
  outFile.close();
}
