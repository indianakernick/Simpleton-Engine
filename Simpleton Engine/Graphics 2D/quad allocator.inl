//
//  quad allocator.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/3/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline G2D::JobID G2D::QuadAllocator::addJob(const size_t jobSize) {
  const JobID job = ranges.size();
  QuadRange range;
  if (ranges.empty()) {
    range.begin = 0;
  } else {
    range.begin = ranges.back().end;
  }
  range.end = range.begin + jobSize;
  ranges.push_back(range);
  quads.resize(range.end);
  return job;
}

inline void G2D::QuadAllocator::remJob(const JobID job) {
  shiftRanges(job, -ranges.at(job).size());
  ranges.erase(ranges.begin() + job);
}

inline G2D::QuadRange G2D::QuadAllocator::jobRange(const JobID job) const {
  return ranges.at(job);
}

inline G2D::Quad *G2D::QuadAllocator::jobQuads(const JobID job) {
  return quads.data() + ranges.at(job).begin;
}

inline void G2D::QuadAllocator::resizeJob(const JobID job, const size_t newSize) {
  shiftRanges(job, newSize - ranges.at(job).size());
}

inline G2D::QuadRange G2D::QuadAllocator::bufRange() const {
  if (ranges.empty()) {
    return {0, 0};
  } else {
    return {0, ranges.back().end};
  }
}

inline G2D::Quad *G2D::QuadAllocator::bufQuads() {
  return quads.data();
}

inline void G2D::QuadAllocator::shiftRanges(const JobID job, const size_t shift) {
  for (auto r = ranges.begin() + job + 1; r != ranges.end(); ++r) {
    r->begin += shift;
    r->end += shift;
  }
}
