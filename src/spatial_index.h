#ifndef SPATIALINDEX_H_
#define SPATIALINDEX_H_

#include <string>
#include <vector>

#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"
#include "sequence_batch.h"
#include "signal_batch.h"

namespace sigmap {
struct SignalAnchorChain {
  float score;
  uint64_t start_position;
  uint64_t end_position;
  size_t num_points;
  //uint64_t *point_positions;
};

struct SignalAnchor {
  size_t target_position;
  size_t query_position;
  float distance;
  bool operator<(const SignalAnchor& a) const {
    return std::tie(target_position, query_position, distance) < std::tie(a.target_position, a.query_position, a.distance);
  }
};

class SpatialIndex {
 public:
  SpatialIndex(int min_num_seeds_required_for_mapping, const std::vector<int> &max_seed_frequencies, const std::string &index_file_path_prefix) : min_num_seeds_required_for_mapping_(min_num_seeds_required_for_mapping), max_seed_frequencies_(max_seed_frequencies), index_file_path_prefix_(index_file_path_prefix) { // for read mapping
    spatial_index_ = nullptr;
  }
  SpatialIndex(int dimension, int max_leaf, int num_threads, const std::string &index_file_path_prefix) : dimension_(dimension), max_leaf_(max_leaf), num_threads_(num_threads), index_file_path_prefix_(index_file_path_prefix) { // for index construction
    spatial_index_ = nullptr;
  }
  ~SpatialIndex(){
    if (spatial_index_ != nullptr) {
      delete spatial_index_;
    }
  }
  //void Statistics(uint32_t num_sequences, const SequenceBatch &reference);
  //void CheckIndex(uint32_t num_sequences, const SequenceBatch &reference);
  void GeneratePointCloud(const float *signal, size_t signal_length, int step_size, std::vector<std::vector<float> > &point_cloud);
  void GetSignalIndexAndPosition(size_t point_index, size_t num_signals, const std::vector<std::vector<float> > &signals, size_t &signal_index, size_t &signal_position);
  void Construct(size_t num_signals, const std::vector<std::vector<float> > &signals);
  void Save();
  void Load();
  void CollectCandiates(int max_seed_frequency, const std::vector<std::vector<float> > &point_cloud, std::vector<uint64_t> *positive_hits, std::vector<uint64_t> *negative_hits);
  void GenerateCandidatesOnOneDirection(std::vector<uint64_t> *hits, std::vector<uint64_t> *candidates);
  void GenerateCandidates(const std::vector<std::vector<float> > &point_cloud, std::vector<uint64_t> *positive_hits, std::vector<uint64_t> *negative_hits, std::vector<uint64_t> *positive_candidates, std::vector<uint64_t> *negative_candidates);

  void GenerateChains(const std::vector<std::vector<float> > &query_point_cloud, int query_point_cloud_step_size, float search_radius, size_t num_signals, const std::vector<std::vector<float> > &target_signals, std::vector<SignalAnchorChain> &positive_chains);

 protected:
  int dimension_;
  int max_leaf_ = 10;
  //int window_size_;
  int min_num_seeds_required_for_mapping_;
  std::vector<int> max_seed_frequencies_;
  int num_threads_;
  std::string index_file_path_prefix_; // For now, I have to use two files *.pt for dim, max leaf, points and *.si for spatial index.
  KDTreeVectorOfVectorsAdaptor< std::vector<std::vector<float> >, float > *spatial_index_;
  std::vector<std::vector<float> > point_cloud_; // TODO(Haowen): remove this duplicate later
};
} // namespace sigmap

#endif // SPATIALINDEX_H_