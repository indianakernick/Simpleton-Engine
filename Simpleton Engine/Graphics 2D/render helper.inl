//
//  render helper.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline void G2D::render(Renderer &renderer, JobManager &jobMan) {
  renderer.writeQuads(jobMan.bufRange(), jobMan.bufQuads());
  for (JobID numJobs = jobMan.size(), job = 0; job != numJobs; ++job) {
    renderer.render(jobMan.range(job), jobMan.params(job));
  }
}
