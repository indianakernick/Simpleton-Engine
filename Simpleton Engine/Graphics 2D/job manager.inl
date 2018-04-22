//
//  job manager.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/3/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline G2D::JobID G2D::JobManager::create(
  const size_t jobSize,
  const RenderParams &params
) {
  const JobID job = mRanges.size();
  QuadRange range;
  range.begin = mRanges.empty() ? 0 : mRanges.back().end;
  range.end = range.begin + jobSize;
  mRanges.push_back(range);
  mQuads.resize(range.end);
  mParams.push_back(params);
  return job;
}

inline void G2D::JobManager::destroy(const JobID job) {
  shiftRanges(job, -mRanges.at(job).size());
  mRanges.erase(mRanges.begin() + job);
}

inline G2D::JobID G2D::JobManager::size() const {
  return mRanges.size();
}

inline G2D::QuadRange G2D::JobManager::bufRange() const {
  if (mRanges.empty()) {
    return {0, 0};
  } else {
    return {0, mRanges.back().end};
  }
}

inline G2D::Quad *G2D::JobManager::bufQuads() {
  return mQuads.data();
}

inline G2D::QuadRange G2D::JobManager::range(const JobID job) const {
  return mRanges.at(job);
}

inline G2D::RenderParams &G2D::JobManager::params(const JobID job) {
  return mParams.at(job);
}

inline G2D::Quad *G2D::JobManager::quads(const JobID job) {
  return mQuads.data() + mRanges.at(job).begin;
}

inline void G2D::JobManager::resize(const JobID job, const size_t newSize) {
  shiftRanges(job, newSize - mRanges.at(job).size());
}

inline void G2D::JobManager::shiftRanges(const JobID job, const size_t shift) {
  for (auto r = mRanges.begin() + job + 1; r != mRanges.end(); ++r) {
    r->begin += shift;
    r->end += shift;
  }
}
