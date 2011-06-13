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



#include "CorrDim.h"




CorrDim::CorrDim(REAL* _data, int _numVec, int _dim/*=1*/) {
    m_data = _data;
    m_numVec = _numVec;
    m_dim = _dim;
    m_num_ele = m_numVec * m_dim;
    // number of elements in lower triangular distance-matrix
    m_numDist = ((m_numVec - 1) * m_numVec) >> 1;
    m_dist = new REAL[m_numDist];
    m_div = (REAL) (m_numVec * m_numVec);
    m_log_min_dist = std::numeric_limits<REAL>::max();
    m_log_max_dist = -1;
    evaluateDistMatrix();
}


CorrDim::~CorrDim() {
    if(m_data != NULL) {
        delete m_data;
    }
    if(m_dist != NULL) {
        delete m_dist;
    }
}


REAL CorrDim::evalCorrDim(int k, int discardl, int discardr,
                          REAL* log_cr, REAL* log_r, REAL* inter) {
    // evaluate corr-sum for every value of 'R'
    REAL step = (m_log_max_dist - m_log_min_dist) / k;
    REAL start = m_log_min_dist + step;
    for(int i=0;i<k;i++,start+=step) {
        log_r[i] = start;
        REAL R = (REAL) exp(start);
        log_cr[i] = (REAL) log(corrSum(R));
    }
    // least squares
    REAL c0, c1;
    int n = k - (discardl + discardr);
    linearLeastSquares(c0, c1, log_r+discardl, log_cr+discardl, n);
    // interpolated values
    for(int i=0;i<k;i++) {
        inter[i] = (c0 * log_r[i]) + c1;
    }
    return c0;
}


REAL CorrDim::corrSum(REAL R) {
    int sum = 0;
    int i, j, posi;
    // in case m_dim > 1, we would need to compare squares
    if(m_dim > 1) {
        R = R * R;
    }
    for(i=0;i<m_numVec;i++) {
        posi = ((i - 1) * i) >> 1;
        for(j=0;j<i;j++) {
            if(m_dist[posi+j] < R) {
                sum += 2;
            }
        }
    }
    return sum / m_div;
}


void CorrDim::evaluateDistMatrix() {
    int i, j, k, d;
    // don't use 'square' for 1-d vectors. They are costly!
    if(m_dim == 1) {
        for(i=0,d=0;i<m_numVec;i++) {
            for(j=0;j<i;j++,d++) {
                m_dist[d] = (REAL) std::abs(m_data[i] - m_data[j]);
                // min and max
                if(m_dist[d] > 0) {
                    if(m_dist[d] > m_log_max_dist) {
                        m_log_max_dist = m_dist[d];
                    }
                    else if(m_dist[d] < m_log_min_dist) {
                        m_log_min_dist = m_dist[d];
                    }
                }
            }
        }
    } // m_dim == 1
    else {
        for(i=0,d=0;i<m_numVec;i++) {
            REAL* x = m_data + (i * m_dim);
            for(j=0;j<i;j++,d++) {
                REAL* y = m_data + (j * m_dim);
                m_dist[d] = 0;
                for(k=0;k<m_dim;k++) {
                    REAL temp = x[k] - y[k];
                    m_dist[d] += (temp * temp);
                }
                // min and max
                if(m_dist[d] > 0) {
                    if(m_dist[d] > m_log_max_dist) {
                        m_log_max_dist = m_dist[d];
                    }
                    else if(m_dist[d] < m_log_min_dist) {
                        m_log_min_dist = m_dist[d];
                    }
                }
            }
        }
        m_log_min_dist = (REAL) sqrt(m_log_min_dist);
        m_log_max_dist = (REAL) sqrt(m_log_max_dist);
    } // m_dim == 1
    m_log_min_dist = (REAL) log(m_log_min_dist);
    m_log_max_dist = (REAL) log(m_log_max_dist);
}

void CorrDim::linearLeastSquares(REAL& c0, REAL& c1, REAL* x,
                                 REAL* y, int n) {
    // Xc = Y   OR   X'Xc = X'Y
    // c0 * xi + c1 = yi
    REAL x_sum2 = 0;
    REAL x_sum = 0;
    REAL y_sum = 0;
    REAL xy_sum = 0;
    for(int i=0;i<n;i++) {
        xy_sum += (x[i] * y[i]);
        y_sum += y[i];
        x_sum += x[i];
        x_sum2 += (x[i] * x[i]);
    }
    c1 = ((x_sum * xy_sum) - (x_sum2 * y_sum)) / ((x_sum * x_sum) - (n * x_sum2));
    c0 = (xy_sum - (x_sum * c1)) / x_sum2;
}

void CorrDim::getDistMatrixHistogram(int numBins, int* hist, REAL* bins) {
    REAL min, max, step;
    min = (REAL) exp(m_log_min_dist);
    max = (REAL) exp(m_log_max_dist);
    step = (max - min) / numBins;
    for(int i=0;i<numBins;i++) {
        bins[i] = min + (i * step);
        hist[i] = 0;
    }
    if(m_dim == 1) {
        for(int i=0;i<m_numDist;i++) {
            int loc = (int) ((m_dist[i] - min) / step);
            if(loc >= numBins) {
                loc = numBins - 1;
            }
            hist[loc]++;
        }
    }
    else {
        for(int i=0;i<m_numDist;i++) {
            REAL d = (REAL) sqrt(m_dist[i]);
            int loc = (int) ((d - min) / step);
            hist[loc]++;
        }
    }
}
