//
// estOLS - estimate the regression coefficients using ordinary least squares 
//
// (c)2022 Mark J. Stock <markjstock@gmail.com>
//
// 

#include <Eigen/Dense>

#include <ciso646>	// for C++11 stuff that MSVC can't get right
#include <cstdint>	// for int32_t and the like
#include <cstdlib>	// for atoi
#include <string>	// for string, stod
#include <cassert>
#include <vector>
#include <chrono>
#include <iomanip>	// for setprecision
#include <iostream>
#include <fstream>
#include <cctype>	// for isdigit

//
// read a matrix or vector from a file
//
Eigen::MatrixXd read_matrix_csv(const std::string infile) {

  // the entries are stored as tempvec=[a,b,c,d,e,f], that is the variable "tempvec" is a row vector
  // later on, this vector is mapped into the Eigen matrix format
  std::vector<double> tempvec;

  // input data filestream
  std::ifstream indatafs(infile);
  assert(indatafs.is_open() && "Could not open input matrix csv file");
 
  // this variable is used to store the row of the matrix that contains commas 
  std::string rowstring;
 
  // this variable is used to store the matrix entry as a string;
  std::string valasstr;
 
  // this variable is used to track the number of rows
  int32_t nrows = 0;
  int32_t ncols = 0;

  // read row-by-row
  while (std::getline(indatafs, rowstring)) {
    std::stringstream rowstream(rowstring);
    int32_t thisncols = 0;
 
    // read pieces of the stream until a comma, and store it in nextval
    while (std::getline(rowstream, valasstr, ',')) {
      tempvec.push_back(std::stod(valasstr));   // convert the string to double and add to the vector
      ++thisncols;
    }

    if (nrows == 0) {
      // this is the first row!
      ncols = thisncols;
      std::cerr << "file (" << infile << ") has " << ncols << " columns..." << std::flush;
    } else {
      assert(thisncols == ncols && "Row found with different number of columns");
      //assert(("Row found with "+std::to_string(thisncols)+" columns instead of "+std::to_string(ncols), thisncols == ncols));
    }

    // increment rows
    ++nrows;
  }
  indatafs.close();

  std::cerr << "and " << nrows << " rows" << std::endl;

  // here we convet the vector variable into the matrix and return the resulting object, 
  // note that tempvec.data() is the pointer to the first memory location at which the entries of the vector tempvec are stored;
  return Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(tempvec.data(), nrows, ncols);
}

//
// basic usage
//
static void usage(char const *progname) {
  std::cerr << "Usage:\n";
  std::cerr << "  " << progname << " -x regression.csv -y observations.csv [-qr] [-o output.csv] [-t m n]\n\n";
  std::cerr << "  where regression.csv is an n (rows) by m (columns) matrix where n > m, (one row per line),\n";
  std::cerr << "  and observations.csv is an n (rows) vector of responses/outcomes (one per line),\n";
  std::cerr << "  alternatively use -t to perform a speed test with the given n and m.\n";
  std::cerr << "  Toggle -qr to use QR decomposition if the default normal equations fail .\n";
  std::cerr << "  Output is in csv format and goes to stdout unless -o is used.\n";
  exit(1);
}

//
// entry and exit
//
int main(int argc, char const *argv[]) {

  std::cerr << "\nestOLS - Ordinary Least Squares Solver\n\n";

  //
  // setup
  //

  // default values
  std::string inmatrixfile;
  std::string inobservfile;
  std::string outfile;
  bool speedtest = false;
  bool use_qr = false;
  size_t n = 10;
  size_t m = 2;

  // parse the command-line arguments
  if (argc < 4) usage(argv[0]);
  for (int iarg=1; iarg<argc; ++iarg) {
    if (strncmp(argv[iarg], "-x", 2) == 0) {
      inmatrixfile = argv[++iarg];
      std::cout << "input matrix file is " << inmatrixfile << "\n";
    } else if (strncmp(argv[iarg], "-y", 2) == 0) {
      inobservfile = argv[++iarg];
      std::cout << "input observations file is " << inobservfile << "\n";
    } else if (strncmp(argv[iarg], "-t", 2) == 0) {
      if (std::isdigit(argv[iarg+1][0])) m = atoi(argv[++iarg]); else usage(argv[0]);
      if (std::isdigit(argv[iarg+1][0])) n = atoi(argv[++iarg]); else usage(argv[0]);
      // hold off on size checking until later
      speedtest = true;
    } else if (strncmp(argv[iarg], "-o", 2) == 0) {
      outfile = argv[++iarg];
      std::cout << "output weights file is " << outfile << "\n";
    } else if (strncmp(argv[iarg], "-qr", 3) == 0) {
      // force QR decomposition (if normal equations are slow)
      use_qr = true;
    } else {
      // fail on everything else
      usage(argv[0]);
    }
  }

  // write times to microsecond precision
  std::cerr << std::setprecision(6) << std::fixed;
  // and output with sci notation and many digits
  std::cout << std::setprecision(10);

  // set up our primary linear algebra variables
  Eigen::MatrixXd Xmat;		// regression matrix
  Eigen::VectorXd y;		// observations
  Eigen::VectorXd B;		// Betas - the solution

  //
  // input
  //

  if (speedtest) {

    // that's going to take forever
    assert(n*m < 2000000000 && "Test matrix very large - are you sure?");

    std::cout << "Running speed test with m=" << m << " and n="<< n << "\n";

    // allocate and initialize sample data
    auto start = std::chrono::system_clock::now();
    Xmat = Eigen::MatrixXd::Random(n, m);
    if (n*m < 1000) std::cerr << "Here is the matrix Xmat:\n" << Xmat << std::endl;
    y = Eigen::VectorXd::Random(n);
    if (n*m < 1000) std::cerr << "Here is the right hand side y:\n" << y << std::endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "Init time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  } else {

    // read from files
    // from https://aleksandarhaber.com/eigen-matrix-library-c-tutorial-saving-and-loading-data-in-from-a-csv-file/

    assert((not inmatrixfile.empty()) && "No input matrix file given on command-line");
    assert((not inobservfile.empty()) && "No input observations file given on command-line");

    Xmat = read_matrix_csv(inmatrixfile);
    if (Xmat.size()<300) std::cerr << "Xmat is\n" << Xmat << std::endl;

    // now read in the file with the observations, one per line
    Eigen::MatrixXd ytemp = read_matrix_csv(inobservfile);
    if (ytemp.cols() > 1) std::cerr << "  " << inobservfile << " has more than one column, using only the first column\n";

    // pull out just the left column
    y = ytemp.leftCols(1);
    if (y.size()<30) std::cerr << "yvec is\n" << y << std::endl;

    //if (y.cols() > 1) std::cerr <<
    assert(Xmat.rows() == y.rows() && "Row counts in X and y do not agree!");
  }

  //
  // run checks
  //

  // confirm input (not all that user-friendly, but good for now)
  assert(n > m && "Input matrix must have n > m (more rows than columns)");
  assert(m > 0 && "Input must have m > 0 (at least one Beta to solve for)");

  // look for rows that are linear combinations of other rows?

  // calculate condition number
  // see https://stackoverflow.com/questions/33575478/how-can-you-find-the-condition-number-in-eigen
  if (false) {
    auto start = std::chrono::system_clock::now();
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(Xmat);
    double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);
    std::cerr << "The condition number (from SVD) is: " << cond << std::endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "CN time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;
  }

  //
  // solve
  //

  // this isn't working, but would be too slow anyway
  //std::cout << "The least-squares solution is:\n" << Xmat.template bdcSvd<Eigen::ComputeThinU | Eigen::ComputeThinV>().solve(y) << std::endl;

  auto start = std::chrono::system_clock::now();

  if (use_qr) {
    // QR decomposition - compromise between speed and stability
    B = Xmat.colPivHouseholderQr().solve(y);
    //std::cout << "The solution using the QR decomposition is:\n" << B << std::endl;
    std::cerr << "QR time: \t[";

  } else {
    // Normal equations - fastest but less stable/precise
    B = (Xmat.transpose() * Xmat).ldlt().solve(Xmat.transpose() * y);
    //std::cout << "The solution using normal equations is:\n" << B << std::endl;
    std::cerr << "NEq time: \t[";
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr << elapsed_seconds.count() << "] seconds" << std::endl;

  // calculate p-values and R^2
  if (false) {
    // this is best done in R, Matlab, or a specific-build statistics software
  }

  //
  // output
  //

  // let it be standard comma-sep-value
  const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");
  if (outfile.empty()) {
    // send to stdout
    std::cerr << "The solution parameters/coefficients are:\n";
    std::cout << B.format(CSVFormat) << std::endl;
  } else {
    // send to file
    // from https://aleksandarhaber.com/eigen-matrix-library-c-tutorial-saving-and-loading-data-in-from-a-csv-file/
    std::ofstream file(outfile);
    if (file.is_open()) {
      file << B.format(CSVFormat) << std::endl;
      file.close();
    }
  }

  // normal exit
  exit(0);
}
