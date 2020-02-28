#ifndef SIGMAP_H_
#define SIGMAP_H_

#include <string>

namespace sigmap {
class SigmapDriver {
 public:
  SigmapDriver() {}
  ~SigmapDriver() {}
  void ParseArgsAndRun(int argc, char *argv[]);
};

class Sigmap {
 public:
  Sigmap(){}
  ~Sigmap(){}
  Sigmap(const std::string &pore_model_file_path, const std::string &signal_directory) : pore_model_file_path_(pore_model_file_path), signal_directory_(signal_directory) {}
  void Map();
 protected:
  std::string pore_model_file_path_;
  std::string signal_directory_;
  std::string output_path_;
};
} // namespace sigmap

#endif // SIGMAP_H_