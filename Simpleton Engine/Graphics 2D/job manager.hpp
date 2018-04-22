//
//  job manager.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/3/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_job_manager_hpp
#define engine_graphics_2d_job_manager_hpp

#include "types.hpp"

namespace G2D {
  class JobManager {
  public:
    JobManager() = default;
    
    JobID create(size_t, const RenderParams & = {});
    void destroy(JobID);
    
    JobID size() const;
    QuadRange bufRange() const;
    Quad *bufQuads();
    
    QuadRange range(JobID) const;
    RenderParams &params(JobID);
    Quad *quads(JobID);
    void resize(JobID, size_t);
    
  private:
    std::vector<Quad> mQuads;
    std::vector<QuadRange> mRanges;
    std::vector<RenderParams> mParams;
    
    void shiftRanges(JobID, size_t);
  };
}

#include "job manager.inl"

#endif
