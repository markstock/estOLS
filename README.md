# estOLS
Ordinary Least Squares problem, guide, and solver

## Fitting a line/plane to experimental data
If you want the best-fit line or plane to a set of disordered observations, you want to solve a least squares problem.
A simple example is linear regression: finding a straight line to best fit a set of (x,y) observations, as illustrated below. The unknown line is defined by two parameters: slope and intercept; and we seek the set of parameters that minimizes the squared error of the observations (y) vs. their inputs (x).

![linearregression](ols_example.png?raw=true "Linear regression sample")

Practical problems typically consist of a large number of samples, and more than one dimension of inputs (x), or "explanatory variables." A curve, plane, or hyperplane is then found to best fit the observations. That is what ordinary least squares finds.

## Ways to do this
Excel's `linest` command will solve single and multiple linear regression problems, see [here](https://support.microsoft.com/en-us/office/linest-function-84d7d0d9-6e50-4101-977a-fa7abf772b6d).

Matlab's [fit](https://www.mathworks.com/help/curvefit/fit.html) and [lsqcurvefit](https://www.mathworks.com/help/optim/ug/lsqcurvefit.html) commands do this, also see their writeup on [least squares fitting](https://www.mathworks.com/help/curvefit/least-squares-fitting.html).

A number of tutorials demonstrate ordinary least squares in R, like [here](https://www.r-econometrics.com/methods/ols/) and [here](https://www.r-bloggers.com/2017/07/ordinary-least-squares-ols-linear-regression-in-r/) and [here](https://www.ssc.wisc.edu/sscc/pubs/RFR/RFR_Regression.html).

Or use this software from the command line or use this library in your own C or C++ code.

## Build and run this software
On a RHEL/Fedora/CentOS workstation, you can install all prerequisites using:

	sudo dnf install make gcc-c++ eigen3-devel

or on Ubuntu with:

	sudo apt install build-essential
	sudo apt-get install libeigen3-dev

When that's done, try the following:

	git clone https://github.com/markstock/estOLS.git
	cd estOLS
	make
	./estOLS -test

If that works, then prepare your regression matrix and vector of observations in two comma-separated value files (or use the sample files provided) and run

	./estOLS -x xmat.csv -y obsv.csv > out.csv

or

	./estOLS -x xmat.csv -y obsv.csv -o out.csv

## Use this as a library in your own code
Here's how to do that.

## Performance
We ran this code on a set of random inputs (both regression matrix and observations [-1..1]) in double precision using the normal equations solution method and timed the calculation on an AMD Ryzen 9 3950X. One set of problems set the number of observations to be 2 times the number of independent variables, and a second set used a factor of 10. The performance for a variety of problem sizes (number of independent variables m) appears below. Total run time seems to scale as O(m^3 n^{0.75}).

m     | n      | time (sec) | n      | time (sec)
------|--------|------------|--------|-----------
500   | 1000   | 0.013250   | 5000   | 0.049815
1000  | 2000   | 0.098714   | 10000  | 0.397258
2000  | 4000   | 0.802492   | 20000  | 3.195111
5000  | 10000  | 18.00390   | 50000  | 55.97039
10000 | 20000  | 141.7969   | 100000 | 437.0685

![performance](perf.png?raw=true "Performance vs. m")

## How to get help
Go to [Issues](https://github.com/markstock/estOLS/issues) (just below the title bar on this page), and "create an issue."

## Thanks
Thanks to the writers of [Eigen3](https://eigen.tuxfamily.org/dox/index.html) for their excellent matrix mathematics package.
The image of linear regression is from Hutcheson, G. D. (2011). Ordinary Least-Squares Regression. In L. Moutinho and G. D.
Hutcheson, The SAGE Dictionary of Quantitative Management Research. Pages 224-228 [[link]](https://datajobs.com/data-science-repo/OLS-Regression-[GD-Hutcheson].pdf)
