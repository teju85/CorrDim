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


#ifndef __INCLUDED_CORRDIMLOWMEM_H__
#define __INCLUDED_CORRDIMLOWMEM_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "basics.h"
#include <cmath>
#include <limits>


/**
 * Class responsible for evaluation of the correlation dimension without
 * storing the distance matrix. That way, this class is the low memory
 * version of 'CorrDim'. Go through the API documentation for more details. 
 *
 * Usage:
 *  CorrDimLowMem d = CorrDimLowMem(my_data, num_data, data_dim);
 *  printf("Correlation Dimension = %d\n", d.evalCorrDim(10));
 *  printf("Correlation Dimension = %d\n", d.evalCorrDim(20));
 */
class CorrDimLowMem {
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
    CorrDimLowMem(REAL* _data, int _numVec, int _dim=1);

    /**
     * @brief Destructor of this class.
     *
     * This is responsible for cleaning of the allocated memory.
     */
    ~CorrDimLowMem();

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
     * @brief Evaluates the correlation sum for all the values of 'log_r'.
     * @param log_cr array which will contain the log(cr) values.
     * @param log_r  array which will contain the log(r) values.
     * @param num number of points in the log(R) axis for evaluating corr-dim.
     *
     * Note that the values of 'log_r' should NOT be in 'log' domain! However,
     * after the execution of this function, they'll be in 'log' domain, so do
     * the values of 'log_cr'.
     */
    void batchCorrSum(REAL* log_cr, REAL* log_r, int num);

    /**
     * @brief Evaluates the min and max values of the distance matrix.
     */
    void evaluateMinMaxDistMatrix();

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
    REAL m_div;           ///< factor used for evaluating the correlation sum
    REAL m_log_min_dist;  ///< minimum distance in the distance matrix (in log)
    REAL m_log_max_dist;  ///< maximum distance in the distance matrix (in log)
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_CORRDIMLOWMEM_H__
