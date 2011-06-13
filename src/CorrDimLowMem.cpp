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



#include "CorrDimLowMem.h"




CorrDimLowMem::CorrDimLowMem(REAL* _data, int _numVec, int _dim/*=1*/) {
    m_data = _data;
    m_numVec = _numVec;
    m_dim = _dim;
    m_num_ele = m_numVec * m_dim;
    m_div = (REAL) (m_numVec * m_numVec);
    m_log_min_dist = std::numeric_limits<REAL>::max();
    m_log_max_dist = -1;
    evaluateMinMaxDistMatrix();
}


CorrDimLowMem::~CorrDimLowMem() {
    if(m_data != NULL) {
        delete m_data;
    }
}


void CorrDimLowMem::evaluateMinMaxDistMatrix() {
    int i, j, k;
    // don't use 'square' for 1-d vectors. They are costly!
    if(m_dim == 1) {
        for(i=0;i<m_numVec;i++) {
            for(j=0;j<i;j++) {
                REAL d = (REAL) std::abs(m_data[i] - m_data[j]);
                // min and max
                if(d > 0) {
                    if(d > m_log_max_dist) {
                        m_log_max_dist = d;
                    }
                    else if(d < m_log_min_dist) {
                        m_log_min_dist = d;
                    }
                }
            }
        }
    } // m_dim == 1
    else {
        for(i=0;i<m_numVec;i++) {
            REAL* x = m_data + (i * m_dim);
            for(j=0;j<i;j++) {
                REAL* y = m_data + (j * m_dim);
                REAL d = 0;
                for(k=0;k<m_dim;k++) {
                    REAL temp = x[k] - y[k];
                    d += (temp * temp);
                }
                // min and max
                if(d > 0) {
                    if(d > m_log_max_dist) {
                        m_log_max_dist = d;
                    }
                    else if(d < m_log_min_dist) {
                        m_log_min_dist = d;
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


REAL CorrDimLowMem::evalCorrDim(int k, int discardl, int discardr, REAL* log_cr, REAL* log_r, REAL* inter) {
    // evaluate corr-sum for every value of 'R'
    REAL step = (m_log_max_dist - m_log_min_dist) / k;
    REAL start = m_log_min_dist + step;
    for(int i=0;i<k;i++,start+=step) {
        log_r[i] = (REAL) exp(start);  // later convert this to 'log'
        log_cr[i] = 0;
    }
    batchCorrSum(log_cr, log_r, k);
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


void CorrDimLowMem::batchCorrSum(REAL* log_cr, REAL* log_r, int num) {
    int i, j, k;
    // don't use 'square' for 1-d vectors. They are costly!
    if(m_dim == 1) {
        for(i=0;i<m_numVec;i++) {
            for(j=0;j<i;j++) {
                REAL d = (REAL) std::abs(m_data[i] - m_data[j]);
                for(k=0;k<num;k++) {
                    if(d < log_r[k]) {
                        for(;k<num;k++) {
                            log_cr[k]++;
                        }
                        break;
                    }
                }
            }
        }
    } // m_dim == 1
    else {
        for(k=0;k<num;k++) {
            log_r[k] *= log_r[k];
        }
        for(i=0;i<m_numVec;i++) {
            REAL* x = m_data + (i * m_dim);
            for(j=0;j<i;j++) {
                REAL* y = m_data + (j * m_dim);
                REAL d = 0;
                for(k=0;k<m_dim;k++) {
                    REAL temp = x[k] - y[k];
                    d += (temp * temp);
                }
                for(k=0;k<num;k++) {
                    if(d < log_r[k]) {
                        for(;k<num;k++) {
                            log_cr[k]++;
                        }
                        break;
                    }
                }
            }
        }
        for(k=0;k<num;k++) {
            log_r[k] = (REAL) sqrt(log_r[k]);
        }
    } // m_dim == 1
    for(k=0;k<num;k++) {
        log_cr[k] = (REAL) log(log_cr[k] / m_div);
        log_r[k] = (REAL) log(log_r[k]);
    }
    return;
}


void CorrDimLowMem::linearLeastSquares(REAL& c0, REAL& c1, REAL* x, REAL* y, int n) {
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

void CorrDimLowMem::getDistMatrixHistogram(int numBins, int* hist, REAL* bins) {
    REAL min, max, step;
    min = (REAL) exp(m_log_min_dist);
    max = (REAL) exp(m_log_max_dist);
    step = (max - min) / numBins;
    for(int i=0;i<numBins;i++) {
        bins[i] = min + (i * step);
        hist[i] = 0;
    }
    int i, j, k;
    // don't use 'square' for 1-d vectors. They are costly!
    if(m_dim == 1) {
        for(i=0;i<m_numVec;i++) {
            for(j=0;j<i;j++) {
                REAL d = (REAL) std::abs(m_data[i] - m_data[j]);
                int loc = (int) ((d - min) / step);
                if(loc >= numBins) {
                    loc = numBins - 1;
                }
                hist[loc]++;
            }
        }
    } // m_dim == 1
    else {
        for(i=0;i<m_numVec;i++) {
            REAL* x = m_data + (i * m_dim);
            for(j=0;j<i;j++) {
                REAL* y = m_data + (j * m_dim);
                REAL d = 0;
                for(k=0;k<m_dim;k++) {
                    REAL temp = x[k] - y[k];
                    d += (temp * temp);
                }
                d = (REAL) sqrt(d);
                int loc = (int) ((d - min) / step);
                hist[loc]++;
            }
        }
    } // m_dim == 1
}
