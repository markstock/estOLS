//
// estOLS - estimate the regression coefficients using ordinary least squares 
//
// (c)2022 Mark J. Stock <markjstock@gmail.com>
//
// 

#include <Eigen/Dense>

//#include <ciso646>
//#include <cstdint>
//#include <cassert>
#include <chrono>
#include <iomanip>	// for setprecision
#include <iostream>

int main(int argc, char const *argv[]) {

  std::cout << "\nestOLS - Ordinary Least Squares Solver\n\n";

  // read command line and get input
  if (argc == 2) {
    std::string infile = argv[1];
  } else {
    std::cout << "\nUsage:\n";
    std::cout << "  " << argv[0] << " -x observations.csv -y response.csv\n\n";
    std::cout << "  where observations.csv is an n (rows) by m (columns) matrix where n > m" << std::endl;
    std::cout << "  and response.csv is an n (rows) vector of responses/outcomes" << std::endl;
    return -1;
  }

  // write times to microsecond precision
  std::cout << std::setprecision(6) << std::fixed;

  // generate sample test data
  const int32_t m = 10;
  const int32_t n = 200;

  // allocate and initialize
  auto start = std::chrono::system_clock::now();
  Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, m);
  if (n*m < 1000) std::cout << "Here is the matrix A:\n" << A << std::endl;
  Eigen::VectorXd b = Eigen::VectorXd::Random(n);
  if (n*m < 1000) std::cout << "Here is the right hand side b:\n" << b << std::endl;
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "Init. time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  // solve
  //std::cout << "The least-squares solution is:\n" << A.template bdcSvd<Eigen::ComputeThinU | Eigen::ComputeThinV>().solve(b) << std::endl;

  start = std::chrono::system_clock::now();
  std::cout << "The solution using the QR decomposition is:\n" << A.colPivHouseholderQr().solve(b) << std::endl;
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "  QR time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  start = std::chrono::system_clock::now();
  std::cout << "The solution using normal equations is:\n" << (A.transpose() * A).ldlt().solve(A.transpose() * b) << std::endl;
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "  NEq time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  // normal exit
  exit(0);
}
