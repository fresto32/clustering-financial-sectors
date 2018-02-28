#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> overlappingTickers() {
  std::vector<std::string> tickers_1997;
  std::vector<std::string> tickers_2017;
  std::vector<std::string> tickers_overlap;
  
  int total_tickers;
  std::string temp;
  
  // Store tickers in temporary vectors
  for (int i = 0; i < 2; i++) {
    std::cout << "Enter number of tickers: ";
    std::cin >> total_tickers;

    for (int j = 0; j < total_tickers; j++) {
      std::cin >> temp;
      if (i == 0) tickers_1997.push_back(temp);
      else tickers_2017.push_back(temp);
    }
  }

  std::vector<std::string> *max;
  std::vector<std::string> *min;
  
  // Save a min and max sized vector in pointer
  if (tickers_1997.size() > tickers_2017.size()) {
    max = &tickers_1997;
    min = &tickers_2017;
  } else {
    min = &tickers_1997;
    max = &tickers_2017;
  }

  std::cout << "Made it past min/max\n";
  std::cout << "Max size is: " << max->size() << std::endl;
  std::cout << "Min size is: " << min->size() << std::endl;

  // Iterate through each twice to verify if ticker is in both
  for (int i = 0; i < max->size(); i++) {
    for (int j = 0; j < min->size(); j++) {
      if (max->at(i) == min->at(j)) {
        tickers_overlap.push_back(max->at(i));
        break;
      }
    }
  }

  std::cout << "Made it past overlap loop\n";

  return tickers_overlap;
}

std::vector<std::vector<double> > share_prices_of_contiguous_tickers(
    std::vector<std::string> contiguous_tickers) {
  
  // Open file financial_data.txt
  std::ifstream inFile;
  inFile.open("financial_data.txt");

  // Create vector of vector of doubles
  std::vector<std::vector<double> > share_prices_of_contiguous;
  std::vector<double> temp_vec;

  for (int i = 0; i < contiguous_tickers.size(); i++) {
    share_prices_of_contiguous.push_back(temp_vec);
    share_prices_of_contiguous[i].reserve(1000);
  }

  // For each line in fin...txt, if the ticker in contiguous tickers, push the
  // fifth element (from ticker, exclusive) onto vector of vector of doubles
  // at the same index
  std::string x;
  int index_of_line = 0;
  bool contiguous_ticker = false;
  int index_of_ticker = 0;

  while (inFile >> x) {
    // If index == 0, see if x in contiguous_tickers, set index_of_ticker
    if (index_of_line == 0) {
      while (index_of_ticker < contiguous_tickers.size()) {
        // If true, set flag to true
        if (contiguous_tickers[index_of_ticker] == x) {
          contiguous_ticker = true;
          std::cout << "Found ticker" << std::endl;
          break;
        }
        index_of_ticker++;
      }
    }
    // ElIf index == 5 && flag, push onto share_prices_vec at index_of_ticker
    else if ((index_of_line == 5) && (contiguous_ticker)) {
      share_prices_of_contiguous[index_of_ticker].push_back(std::stod (x));
      index_of_ticker = 0;
      contiguous_ticker = false;
      std::cout << "Reset and pushed ticker index/price" << std::endl;
    }
    // If index == 6, index = 0, else index++
    if (index_of_line == 6) index_of_line = 0;
    else index_of_line++;
    std::cout << "Reached end of loop with index: " << index_of_line;
    std::cout << std::endl;
    std::cout << "x: " << x << std::endl;
  }
  // return vector of vector
  return share_prices_of_contiguous;
}

std::vector<std::vector<double> > to_percentage_change(int delta_time,
    std::vector<std::string> tickers, 
    std::vector<std::vector<double> > share_prices) {
  // create a new vector of doubles
  std::vector<std::vector<double> > percentage_change;
  std::cout << "Made percentage\n";

  // initialize vector of vector
  std::vector<double> temp;
  for (int i = 0; i < tickers.size(); i++) {
    percentage_change.push_back(temp);
    percentage_change[i].reserve(1000);
  }
  std::cout << "Made v of v %\n";
  std::cout << "Tickers Size: " << tickers.size() << std::endl;
  std::cout << "Share_prices[2] Size: " << share_prices[2].size() << std::endl;

  double percentage_delta;
  // for each double line
  for (std::vector<std::vector<double> >::size_type i = 0; 
      i < tickers.size(); i++) {
    // for each incremenet of j to j+delta_time
    for (std::vector<double>::size_type j = 0; 
        j < share_prices[i].size();
        j = j + delta_time) {
      // push (j+delta_t-j)/j
      if (j+delta_time < share_prices[i].size()) {
      percentage_delta = (share_prices[i][j] + share_prices[i][j+delta_time]) /
        share_prices[i][j];
      percentage_change[i].push_back(percentage_delta);
      }
//      std::cout << "i and j: " << i << " " << j << std::endl;
    }
  }
  std::cout << "Out v double loop\n";
  
  // return vector of vector
  return percentage_change;
}

void print_to_kmeans(std::vector<std::string> tickers,
    std::vector<std::vector<double> > values_to_print) {

  std::ofstream outFile;
  outFile.open("kmeans_format.txt");

  int data_points = 0;
  int attributes = 0;
  int clusters = -1;
  int max_iterations = -1;

//  for (int i = 0; i < tickers.size(); i++)
//    for (int j = 0; j < 10; j++)
//      values_to_print[i].push_back(j);

  for (int i = 0; i < tickers.size(); i++) {
    for (std::vector<double>::size_type j = 0; 
        j < values_to_print[i].size(); j++) {
      outFile << values_to_print[i][j] << " ";
    }
    outFile << tickers[i] << std::endl;
  }
  outFile.close();
}

int main() {
  std::vector<std::string> overlap;

  overlap = overlappingTickers();
  
  std::cout << "\t\t --- Overlapped Tickers --- " << std::endl;

  for (int i = 0; i < overlap.size(); i++)
    std::cout << overlap[i] << std::endl;
  std::cout << "Size of overlapped tickers: " << overlap.size() << std::endl;
  
  // temp share prices vector
  std::vector<std::vector<double> > share_prices;

  share_prices = share_prices_of_contiguous_tickers(overlap);
  
  std::vector<std::vector<double> > percentage_qrtly;
  int delta = 1;  
  percentage_qrtly = to_percentage_change(delta, overlap, share_prices);
  print_to_kmeans(overlap, percentage_qrtly);

  return 0;
}
// Next step: concatenate all txt files, then remove commas w/ atom, then 
// turn into vectors of each price, then convert into vectors of each 
// daily / monthly / year change in price then make  K-means compatible data
