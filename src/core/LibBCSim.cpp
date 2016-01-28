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
#include "LibBCSim.hpp"
#include "algorithm/CpuFixedAlgorithm.hpp"
#include "algorithm/CpuSplineAlgorithm.hpp"
#ifdef BCSIM_ENABLE_CUDA
    #include "algorithm/GpuFixedAlgorithm.hpp"
    #include "algorithm/GpuSplineAlgorithm1.hpp"
    #include "algorithm/GpuSplineAlgorithm2.hpp"
#endif

namespace bcsim {

IAlgorithm::s_ptr Create(const std::string& sim_type) {
    if (sim_type == "fixed") {
        return IAlgorithm::s_ptr(new CpuFixedAlgorithm);
    } else if (sim_type == "spline") {
        return IAlgorithm::s_ptr(new CpuSplineAlgorithm);
#ifdef BCSIM_ENABLE_CUDA
    } else if (sim_type == "gpu_fixed") {
        return IAlgorithm::s_ptr(new GpuFixedAlgorithm);
    } else if (sim_type == "gpu_spline1") {
        return IAlgorithm::s_ptr(new GpuSplineAlgorithm1);
    } else if (sim_type == "gpu_spline2") {
        return IAlgorithm::s_ptr(new GpuSplineAlgorithm2);
#endif
    } else {
        throw std::runtime_error("Illegal algorithm type: " + sim_type);
    }
}

}   // end namespace
