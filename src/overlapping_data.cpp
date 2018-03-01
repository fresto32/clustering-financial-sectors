#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

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

template<class Iter, class T>
Iter binary_find(Iter begin, Iter end, T val)
{
    // Finds the lower bound in at most log(last - first) + 1 comparisons
    Iter i = std::lower_bound(begin, end, val);

    if (i != end && !(val < *i))
        return i; // found
    else
        return end; // not found
}

std::vector<std::vector<double> > share_prices_of_contiguous_tickers(
    std::vector<std::string> contiguous_tickers) {
  
  // Open file financial_data.txt
  std::ifstream inFile;
  inFile.open("financial_data_clean_shorter.txt");

  // Create vector of vector of doubles
  std::vector<std::vector<double> > *share_prices_of_contiguous = 
    new std::vector<std::vector<double> >;
  std::vector<double> temp_vec;
  share_prices_of_contiguous->resize(contiguous_tickers.size());

  for (int i = 0; i < contiguous_tickers.size(); i++) {
 //   share_prices_of_contiguous.push_back(temp_vec);
    share_prices_of_contiguous->at(i).reserve(1000);
  }

  // For each line in fin...txt, if the ticker in contiguous tickers, push the
  // fifth element (from ticker, exclusive) onto vector of vector of doubles
  // at the same index
  std::string x;
  int index_of_line = 0;
  int index_of_ticker = 0;

  while (inFile >> x) {
    // If index == 0, see if x in contiguous_tickers, set index_of_ticker
    if (index_of_line == 0) {
      index_of_ticker = std::distance(contiguous_tickers.begin(), 
          binary_find(contiguous_tickers.begin(),
          contiguous_tickers.end(), x));
      std::cout << "Ticker index:      ------- :" << index_of_ticker << "\n";
      index_of_line = 1;
    }
    // ElIf index == 1 push onto share_prices_vec at index_of_ticker
    else if (index_of_line == 1) {
      share_prices_of_contiguous->at(index_of_ticker).push_back(std::stod (x));
//      std::cout << "Reset and pushed ticker index/price: " << 
//        share_prices_of_contiguous->at(index_of_ticker).back() << std::endl;
      index_of_ticker = 0;
      index_of_line = 0;
    }
//    std::cout << x << std::endl;
  }
  // return vector of vector
  return *share_prices_of_contiguous;
}

std::vector<std::vector<double> > to_percentage_change(int delta_time,
    std::vector<std::string> tickers, 
    std::vector<std::vector<double> > share_prices) {
  // create a new vector of doubles
  std::vector<std::vector<double> > *percentage_change = new 
    std::vector<std::vector<double> >;

  std::cout << "Made percentage\n";

  // initialize vector of vector
  percentage_change->resize(tickers.size());
  std::vector<double> temp;
  for (int i = 0; i < tickers.size(); i++) {
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
      percentage_change->at(i).push_back(percentage_delta);
      }
//      std::cout << "i and j: " << i << " " << j << std::endl;
    }
  }
  std::cout << "Out v double loop\n";
  
  // return vector of vector
  return *percentage_change;
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

extern void preprocess_data(std::vector<std::string> contiguous_tickers);

int main() {
  // The following are the overlapping tickers between 1977 and 2017, note that
  // the overlaop() function may replace the below array.
  const char *vec_init[] = {"AAA", "AAD", "AAR", "ABA", "ABC", "ABR", "ABT",
    "ADH", "ADN", "ADX", "AFI", "AGL", "AGR", "AGS", "AHX", "ALK", "ALL",
    "AMC", "AMG", "AML", "ANL", "ANZ", "AOF", "AOG", "ARA", "ARB", "ARF",
    "ARG", "ARL", "ASL", "AST", "ATP", "AVG", "BAH", "BEN", "BHP", "BKW",
    "BMT", "BOA", "BOC", "BPT", "BRL", "BRN", "CAA", "CAG", "CAR", "CBA",
    "CBL", "CCL", "CGL", "CGN", "CIM", "CIP", "CMC", "CNW", "COG", "COH",
    "CRO", "CSL", "CSR", "CTO", "CTX", "CUE", "CVC", "CVN", "CWP", "DEV",
    "DFM", "DJW", "DRA", "DUI", "EGD", "EGL", "ELT", "EML", "EMP", "EQT", 
    "ERA", "ESE", "ESI", "FAR", "FGR", "FIG", "FLT", "FOR", "FXJ", "GBG", 
    "GFY", "GMA", "GME", "GMN", "GPT", "GUD", "GUL", "HAR", "HGO", "HIL", 
    "HLX", "HVN", "IAM", "ICT", "IFT", "IMD", "ING", "IPH", "IRC", "JRV", 
    "KCN", "KGM", "KMT", "LKO", "LLC", "MAH", "MAT", "MAY", "MBK", "MCP",
    "MDI", "MED", "MGC", "MGG", "MGL", "MLT", "MPL", "MRC", "MRV", "MTM",
    "NAB", "NCM", "NCR", "NML", "NOR", "NTL", "OEC", "OML", "OSH", "PMC",
    "PMP", "PNI", "POW", "PPK", "PPT", "PRL", "PSA", "QAN", "QBE", "RAC",
    "RBL", "RCT", "REH", "RGS", "RIC", "RSG", "SAS", "SBM", "SCP", "SDG",
    "SDI", "SGP", "SGR", "SHL", "SHV", "SMC", "SMI", "SMM", "SPX", "SRI",
    "SRO", "STA", "STM", "STO", "SUR", "SYD", "TAH", "TAP", "TCL", "TGG",
    "TRY", "VLT", "VRL", "WBA", "WBC", "WES", "WHF", "WMC", "WMI", "WOW",
    "WPL", "WTP", "ZEL"};

  std::vector<std::string> overlap(vec_init, vec_init+190);
  
  std::cout << "\t\t --- Overlapped Tickers --- " << std::endl;

  for (int i = 0; i < overlap.size(); i++)
    std::cout << "\"" <<  overlap[i] << "\"" << ", ";
  std::cout << "Size of overlapped tickers: " << overlap.size() << std::endl;
  
  // to preprocess financial_data.txt uncomment below line 
//  preprocess_data(overlap);

  // temp share prices vector
  std::vector<std::vector<double> > share_prices;
  share_prices = share_prices_of_contiguous_tickers(overlap);

  // Displaying ticker data
  for (int i = 0; i < share_prices.size(); i++) {
    std::cout << overlap[i] << ": ";
    for (int j = 0; j < share_prices[i].size(); j++) {
      std::cout << share_prices[i][j] << ", ";
    }
    std::cout << "\n\n\n\n\n\n\n" << "\t\t ------ NEW TICKER ------ \n";
  }
  
  std::vector<std::vector<double> > percentage_qrtly;
  int delta = 5;  
  percentage_qrtly = to_percentage_change(delta, overlap, share_prices);
  print_to_kmeans(overlap, percentage_qrtly);

  return 0;
}
// Next step: concatenate all txt files, then remove commas w/ atom, then 
// turn into vectors of each price, then convert into vectors of each 
// daily / monthly / year change in price then make  K-means compatible data
