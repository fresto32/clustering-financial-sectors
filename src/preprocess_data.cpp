#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void preprocess_data(std::vector<std::string> contiguous_tickers) {
  std::ifstream inFile;
  inFile.open("financial_data_shorter.txt");

  std::ofstream outFile;
  outFile.open("financial_data_clean_shorter.txt");

  std::string x;
  int index_of_line = 0;
  bool contiguous_ticker = false;

  while (inFile >> x) {
    if (index_of_line == 0) {
      for (int i = 0; i < contiguous_tickers.size(); i++) {
        if (contiguous_tickers[i] == x) {
          contiguous_ticker = true;
          outFile << x << " ";
          std::cout << "Printed ticker: " << x << std::endl;
          break;
        }
      }
    }
    if ((index_of_line == 5) && (contiguous_ticker)) {
      outFile << x << std::endl;
      std::cout << "Printed: " << x << std::endl;
      contiguous_ticker = false;
    }
    if (index_of_line == 6) index_of_line = 0;
    else index_of_line++;
  }
  outFile.close();
}
