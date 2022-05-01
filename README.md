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

	make
	./estOLS -test

If that works, then prepare your ? matrix and vector of observations in two comma-separated value files and run

	./estOLS -x matrix.csv -y observations.csv

## Use this as a library in your own code
Here's how to do that.

## How to get help
Go to [Issues](https://github.com/markstock/estOLS/issues) (just below the title bar on this page), and "create an issue."

## Thanks
Thanks to the writers of [Eigen3](https://eigen.tuxfamily.org/dox/index.html) for their excellent matrix mathematics package.
The image of linear regression is from Hutcheson, G. D. (2011). Ordinary Least-Squares Regression. In L. Moutinho and G. D.
Hutcheson, The SAGE Dictionary of Quantitative Management Research. Pages 224-228 [[link]](https://datajobs.com/data-science-repo/OLS-Regression-[GD-Hutcheson].pdf)
