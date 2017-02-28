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

#pragma once
#include <memory>
#include <vector>
#include "../ScanGeometry.hpp"
#include "core/export_macros.hpp"

// Geometry + Beam Space data => Sampled cartesian grid.
// Manages its own output buffer.
template <typename T>
class ICartesianator {
public:
    typedef std::unique_ptr<ICartesianator<T>> u_ptr;
    typedef std::shared_ptr<ICartesianator<T>> s_ptr;

    // Also computes Cartesian xy extents.
    virtual void SetGeometry(bcsim::ScanGeometry::ptr geometry)  = 0;

    // Indexing: num_samples_x*y_idx + x_idx
    virtual const T* GetOutputBuffer()                           = 0;

    // Set the number of output samples.
    virtual void SetOutputSize(size_t num_samples_x,
                               size_t num_samples_y)            = 0;
    
    // Get the number of output samples.
    virtual void GetOutputSize(size_t& num_samples_x,
                               size_t& num_samples_y)           = 0;

    // Indexing: num_beams*sample_idx + beam_idx
    // Needs to know number of beams and number of samples in
    // each beam.
    virtual void Process(T* in_buffer,
                         int num_beams,
                         int num_samples)                       = 0;
};

template <typename T>
class DLL_PUBLIC CpuCartesianator : public ICartesianator<T> {
public:
    CpuCartesianator();

    virtual void SetGeometry(bcsim::ScanGeometry::ptr geometry);

    virtual const T* GetOutputBuffer();

    virtual void SetOutputSize(size_t num_samples_x,
                               size_t num_samples_y);

    virtual void GetOutputSize(size_t& num_samples_x,
                               size_t& num_samples_y);

    virtual void Process(T* in_buffer, int num_beams, int num_samples);

private:

    void UpdateOutputBuffer();

    void DoSectorTransform(T* in_buffer, int num_beams, int num_range,
                           std::shared_ptr<bcsim::SectorScanGeometry> geometry);

    void DoLinearTransform(T* in_buffer, int num_beams, int num_range,
                           std::shared_ptr<bcsim::LinearScanGeometry> geometry);

private:
    bcsim::ScanGeometry::ptr    m_geometry;
    std::vector<T>              m_output_buffer;
    size_t                      m_num_samples_x;
    size_t                      m_num_samples_y;

    // auto-computed extents
    float   m_x_min;
    float   m_x_max;
    float   m_y_min;
    float   m_y_max;
};
