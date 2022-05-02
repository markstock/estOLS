//
// estOLS - estimate the regression coefficients using ordinary least squares 
//
// (c)2022 Mark J. Stock <markjstock@gmail.com>
//
// 

#include <Eigen/Dense>

#include <ciso646>	// for C++11 stuff that MSVC can't get right
#include <cstdint>	// for int32_t and the like
#include <cassert>
#include <chrono>
#include <iomanip>	// for setprecision
#include <iostream>
#include <fstream>
#include <cctype>	// for isdigit

//
// basic usage
//
static void usage(char const *progname) {
  std::cerr << "Usage:\n";
  std::cerr << "  " << progname << " -x observations.csv -y response.csv [-o output.csv] [-t m n]\n\n";
  std::cerr << "  where observations.csv is an n (rows) by m (columns) matrix where n > m,\n";
  std::cerr << "  and response.csv is an n (rows) vector of responses/outcomes,\n";
  std::cerr << "  alternatively use -t to perform a speed test with the given n and m.\n";
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
  std::string inmatrixfile = "mat.csv";
  std::string inobservfile = "observ.csv";
  std::string outfile = "";
  bool speedtest = false;
  size_t n = 10;
  size_t m = 2;

  // parse the command-line arguments
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
      std::cout << "running speed test with m=" << m << " and n="<< n << "\n";
    } else if (strncmp(argv[iarg], "-o", 2) == 0) {
      outfile = argv[++iarg];
      std::cout << "output weights file is " << outfile << "\n";
    // add -cn for finding condition number
    // add -qr to force QR decomposition (if normal equations are slow)
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

  // Normal equations - fastest but less stable/precise
  if (true) {
    auto start = std::chrono::system_clock::now();
    B = (Xmat.transpose() * Xmat).ldlt().solve(Xmat.transpose() * y);
    //std::cout << "The solution using normal equations is:\n" << B << std::endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "NEq time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;
  }

  // QR decomposition - compromise between speed and stability
  if (false) {
    auto start = std::chrono::system_clock::now();
    B = Xmat.colPivHouseholderQr().solve(y);
    //std::cout << "The solution using the QR decomposition is:\n" << B << std::endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "QR time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;
  }

  //
  // output
  //

  // let it be standard comma-sep-value
  const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");
  if (outfile.empty()) {
    // send to stdout
    std::cout << B.format(CSVFormat) << std::endl;
  } else {
    // send to file
    // from https://aleksandarhaber.com/eigen-matrix-library-c-tutorial-saving-and-loading-data-in-from-a-csv-file/
    std::ofstream file(outfile);
    if (file.is_open()) {
      file << B.format(CSVFormat);
      file.close();
    }
  }

  // normal exit
  exit(0);
}
