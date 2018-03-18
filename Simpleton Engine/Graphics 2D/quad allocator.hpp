//
//  quad allocator.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/3/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_quad_allocator_hpp
#define engine_graphics_2d_quad_allocator_hpp

#include "types.hpp"

namespace G2D {
  class QuadAllocator {
  public:
    QuadAllocator() = default;
    
    JobID addJob(size_t);
    void remJob(JobID);
    QuadRange jobRange(JobID) const;
    Quad *jobQuads(JobID);
    void resizeJob(JobID, size_t);
    
    QuadRange bufRange() const;
    Quad *bufQuads();
    
  private:
    std::vector<Quad> quads;
    std::vector<QuadRange> ranges;
    
    void shiftRanges(JobID, size_t);
  };
}

#include "quad allocator.inl"

#endif
