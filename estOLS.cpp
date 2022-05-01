//
// estOLS - estimate the regression coefficients using ordinary least squares 
//
// (c)2022 Mark J. Stock <markjstock@gmail.com>
//
// 

#include <Eigen/Dense>

#include <ciso646>	// for C++11 stuff that MSVC can't get right
#include <cstdint>	// for int32_t and the like
//#include <cassert>
#include <chrono>
#include <iomanip>	// for setprecision
#include <iostream>

int main(int argc, char const *argv[]) {

  std::cerr << "\nestOLS - Ordinary Least Squares Solver\n\n";

  // read command line and get input
  if (argc == 2) {
    std::string infile = argv[1];
  } else {
    std::cerr << "\nUsage:\n";
    std::cerr << "  " << argv[0] << " -x observations.csv -y response.csv\n\n";
    std::cerr << "  where observations.csv is an n (rows) by m (columns) matrix where n > m" << std::endl;
    std::cerr << "  and response.csv is an n (rows) vector of responses/outcomes" << std::endl;
    return -1;
  }

  // write times to microsecond precision
  std::cerr << std::setprecision(6) << std::fixed;
  // and output with sci notation and many digits
  std::cout << std::setprecision(10);

  // generate sample test data
  const int32_t m = 100;
  const int32_t n = 2000;

  // allocate and initialize
  auto start = std::chrono::system_clock::now();
  Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, m);
  if (n*m < 1000) std::cerr << "Here is the matrix A:\n" << A << std::endl;
  Eigen::VectorXd b = Eigen::VectorXd::Random(n);
  if (n*m < 1000) std::cerr << "Here is the right hand side b:\n" << b << std::endl;
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr << "Init time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  // calculate condition number
  // see https://stackoverflow.com/questions/33575478/how-can-you-find-the-condition-number-in-eigen
  if (false) {
    start = std::chrono::system_clock::now();
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);
    std::cerr << "The condition number (from SVD) is: " << cond << std::endl;
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cerr << "CN time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;
  }

  // solve

  // this isn't working, but would be too slow anyway
  //std::cout << "The least-squares solution is:\n" << A.template bdcSvd<Eigen::ComputeThinU | Eigen::ComputeThinV>().solve(b) << std::endl;

  // QR decomposition - compromise between speed and stability
  start = std::chrono::system_clock::now();
  std::cout << "The solution using the QR decomposition is:\n" << A.colPivHouseholderQr().solve(b) << std::endl;
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cerr << "QR time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  // Normal equations - fastest but less stable/precise
  start = std::chrono::system_clock::now();
  std::cout << "The solution using normal equations is:\n" << (A.transpose() * A).ldlt().solve(A.transpose() * b) << std::endl;
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cerr << "NEq time: \t[" << elapsed_seconds.count() << "] seconds" << std::endl;

  // normal exit
  exit(0);
}
