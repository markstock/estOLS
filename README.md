# estOLS
Ordinary Least Squares problem, guide, and solver

## Fitting a line/plane to experimental data
If you want the best-fit line or plane to a set of disordered observations, you want to solve a least squares problem.
A simple example is linear regression: finding a straight line to best fit a set of (x,y) observations, as illustrated below. The unknown line is defined by two parameters: slope and intercept; and we seek the set of parameters that minimizes the squared error of the observations vs. their inputs.

Practical problems typically consist of inputs in more than one dimension, sometimes much more, and observations for each vector of inputs. A curve, plane, or hyperplane is then found to best fit the observations. That is what ordinary least squares finds.

## Ways to do this
Excel's `linest` command will solve the simpler linear regression problem, see links here.

Matlab's ? command, see documentation.

In R, you can do ...

Or use this software from the command line or use this library in your own C or C++ code.

## Build and run this software
On a Linux workstation, you can see if Eigen3 is installed. If not, you or an administrator can run

    sudo dnf install eigen3

When that's done, try

    make
    ./estOLS -test

If that works, then prepare your ? matrix and vector of observations in two comma-separated value files and run

	./estOLS -x matrix.csv -y observations.csv

## Use this as a library in your own code
Here's how to do that.

## How to get help
Go to Issues, just below the title bar on this page, and ask your question.

## Thanks
Thanks to the writers of Eigen3 for their thorough matrix mathematics package.
