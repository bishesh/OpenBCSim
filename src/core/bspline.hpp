/*
Copyright (c) 2015, Sigurd Storve
All rights reserved.

Licensed under the BSD license.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdexcept>
#include <cmath>
#include <vector>
#include <utility>

namespace bspline_storve {

// Test if floating point number is approximately zero.
template <typename T>
bool _floatIsZero(T value, double eps=1e-6) {
    return std::abs(value) < eps;
}

// Compute num/dev with the special rule that 0/0 is 0.
template <typename T>
T _specialDiv(T num, T den) {
    if (_floatIsZero(num) && _floatIsZero(den)) {
        return 0.0;
    } else {
        return num / den;
    }
}

// Compute B-splines directly using recursive definition.        
//  j:  B-spline basis no.
//  p:  Polynomial degree
//  knots: knot vector
template <typename T>
T bsplineBasis(int j, int p, T x, const std::vector<T>& knots) {
    if (p == 0 ) {
        if ( (x >= knots[j]) && (x < knots[j+1]) ) {
            return 1.0;
        } else {
            return 0.0;
        }
    } else {
        T left = _specialDiv((x-knots[j])*bsplineBasis(j,p-1,x,knots), knots[j+p]-knots[j]);
        T right = _specialDiv((knots[j+1+p]-x)*bsplineBasis(j+1,p-1,x,knots), knots[j+1+p]-knots[j+1]);
        return left + right;
    }
}

// Determine which knot span a parameter value is in.
// Throws std::runtime_error if interval cannot be found.
template <typename T>
int compute_knot_interval(const std::vector<T>& knots, T t) {
    for (int i = 0; i < static_cast<int>(knots.size())-1; i++) {
        if ((t >= knots[i]) && (t < knots[i+1])) {
            return i;
        }
    }
    throw std::runtime_error(std::string(__FUNCTION__) + " : could not determine knot interval");
}

// Compute lower and upper sum indices for the non-zero basis
// functions. Both are inclusive.
// Throw std::runtime_error on error.
template <typename T>
std::pair<int, int> get_lower_upper_inds(const std::vector<T>& knots, T t, int degree) {
    const int mu = compute_knot_interval(knots, t);
    return std::make_pair(mu-degree, mu);
}

// Create a p + 1 - regular uniform knot vector for a given number of control points
// Throws if n is too small
template <typename T>
std::vector<T> uniform_regular_knot_vector(int n, int p, T t0, T t1) {
    std::vector<T> res;

    //The minimum length of a p + 1 - regular knot vector is 2 * (p + 1)
    if (n < p + 1) {
        throw std::runtime_error("Too small n for a uniform regular knot vector");
    }

    // p + 1 copies of t0 left and p + 1 copies of t1 right but one of each in linspace
    for (int i = 0; i < p; i++) res.push_back(t0);
    const auto num_middle = n + 1 - p;
    for (int i = 0; i < num_middle; i++) {
        res.push_back(t0 + i*(t1 - t0) / (num_middle-1));
    }
    for (int i = 0; i < p; i++) res.push_back(t1);
    return res;
}

// Returns the control point abscissa for the control polygon of a one-dimensional spline.
template <typename T>
std::vector<T> control_points(int p, const std::vector<T>& knots) {
    std::vector<T> res;
    for (int i = 0; i < static_cast<int>(knots.size()) - p - 1; i++)  {
        T knot_avg = static_cast<T>(0.0);
        for (int j = i + 1; j < i + 1 + p; j++) {
            knot_avg += knots[j] / p;
        }
        res.push_back(knot_avg);
    }
    return res;
}

}   // end namespace
