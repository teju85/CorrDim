/***************************************************************************\
Tool to find the correlation dimension of a sequence.
Copyright (C) 2010 Tejaswi.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
\***************************************************************************/


#ifndef __INCLUDED_CORRDIM_H__
#define __INCLUDED_CORRDIM_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "basics.h"
#include <cmath>
#include <limits>


/**
 * Class responsible for evaluation of the correlation dimension of the input
 * data. This class is purposefully made generic to handle the data of any
 * dimension. Go through the API documentation for more details.
 *
 * Usage:
 *  CorrDim d = CorrDim(my_data, num_data, data_dim);
 *  printf("Correlation Dimension = %d\n", d.evalCorrDim(10));
 *  printf("Correlation Dimension = %d\n", d.evalCorrDim(20));
 */
class CorrDim {
public:
    /**
     * @brief Constructor of the correlation dimension evaluator.
     * @param _data the data points for which corr-dim needs to be evaluated.
     * @param _numVec number of data points.
     * @param _dim dimension of one such data point. [Defaults to 1]
     *
     * . This means that data should be of length (_numVec * _dim). It's a
     *   matrix of dimension _numVec x _dim, flattened out in row-major order.
     *
     * . 'data' must have been 'allocated' using 'new' function!
     *
     * . This class will take the responsibility of memory pointed to by 'data'.
     */
    CorrDim(REAL* _data, int _numVec, int _dim=1);

    /**
     * @brief Destructor of this class.
     *
     * This is responsible for cleaning of the allocated memory.
     */
    ~CorrDim();

    /**
     * @brief Evaluate the correlation dimension.
     * @param k number of points in the log(R) axis for evaluating corr-dim.
     * @param discardl number of points on left side to be discarded for best-fit.
     * @param discardr number of points on right side to be discarded for best-fit.
     * @param log_cr array which will contain the log(cr) values.
     * @param log_r  array which will contain the log(r) values.
     * @param inter array which will contain the best-fit log(cr) values.
     * @return the correlation dimension of the data points.
     *
     * It is the responsibility of the calling function to allocate and free
     * the memory occupied by 'log_cr', 'log_r' and 'inter'!
     * 'min_logr' and 'max_logr' should be in the range (-INF, 0]. The reason
     * being the distances would have been normalized to the range [0, 1],
     * implying that for comparison, the range you specify must be in
     * (-INF, 0]
     * This has been done mainly for the sake of simplicity and uniformity
     * for different types of maps.
     */
    REAL evalCorrDim(int k, int discardl, int discardr, REAL* log_cr, REAL* log_r, REAL* inter);

    /**
     * @brief Generate the histogram of the distance matrix
     * @param numBins number of bins in the histogram.
     * @param hist histogram bins
     * @param bins value of each bin
     *
     * It is the responsibility of the calling function to allocate and free
     * the memory occupied by 'hist' and 'bins'!
     */
    void getDistMatrixHistogram(int numBins, int* hist, REAL* bins);

private:
    /**
     * @brief Evaluates the correlation sum for the given value of 'R'.
     * @param R the distance wrt which correlation sum has to be evaluated.
     * @return the correlation sum.
     */
    REAL corrSum(REAL R);

    /**
     * @brief Evaluates the square of euclidean distance between every pair
     *
     * This will be finally used in evaluating the correlation sum of the given
     * data for a given value of 'R'.
     *
     * Why not square root?
     *   The answer is simple, the 'Heaviside' step function used to evaluate
     * the correlation sum depends on the difference between 'R' and the
     * euclidean distance between 2 points. The output of 'Heaviside' function
     * will be same even if we evaluate the difference between 'R^2' and the
     * square of euclidean distance! In this process, we are avoiding the
     * redundant square-root operation involved in the euclidean distance.
     *
     * Is there a catch?
     *   Of course! This applies only for non-single dimensional vectors. For
     * 1-d vectors, squaring will be costlier than having an absolute
     * difference between 2 numbers. So, for 1-d vectors, distance will be
     * absolute difference between 2 data points and for all other cases the
     * distance will be square of euclidean.
     */
    void evaluateDistMatrix();

    /**
     * @brief Evaluates linear least square solution
     * @param c0 slope of the line
     * @param c1 displacement of the line
     * @param x points along x-axis
     * @param y points along y-axis
     * @param n number of points
     */
    void linearLeastSquares(REAL& c0, REAL& c1, REAL* x, REAL* y, int n);

private:
    REAL* m_data;         ///< data points array
    int m_numVec;         ///< number of data points
    int m_dim;            ///< dimension of one such data point
    int m_num_ele;        ///< Total number of elements in the data
    REAL* m_dist;         ///< distance matrix for the data points
    int m_numDist;        ///< num-elements in lower triangular distance-matrix
    REAL m_div;           ///< factor used for evaluating the correlation sum
    REAL m_log_min_dist;  ///< minimum distance in the distance matrix (in log)
    REAL m_log_max_dist;  ///< maximum distance in the distance matrix (in log)
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_CORRDIM_H__
