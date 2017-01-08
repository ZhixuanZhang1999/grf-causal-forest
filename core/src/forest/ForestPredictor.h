
#ifndef GRADIENTFOREST_FORESTPREDICTOR_H
#define GRADIENTFOREST_FORESTPREDICTOR_H

#include "RelabelingStrategy.h"
#include "SplittingRule.h"
#include "PredictionStrategy.h"

#include "Tree.h"
#include "TreeTrainer.h"
#include "Forest.h"

#include <thread>
#include <future>

class ForestPredictor {
public:
  ForestPredictor(uint num_threads,
                  std::shared_ptr<PredictionStrategy> prediction_strategy);

  std::vector<std::vector<double>> predict(const Forest& forest, Data* prediction_data);

  void writeConfusionFile(Data* prediction_data, std::vector<std::vector<double>> predictions);
  void writePredictionFile(Data* prediction_data, std::vector<std::vector<double>> predictions);

private:
  void computePredictionError(const Forest& forest, Data* prediction_data);
  void computePredictionErrorInternal(const Forest& forest,
                                      Data* prediction_data,
                                      const std::unordered_map<size_t, std::vector<size_t>>& terminal_node_IDs_by_tree);

  void predictTreesInThread(uint thread_idx,
                            const Forest& forest,
                            const Data *prediction_data,
                            bool oob_prediction,
                            std::promise<std::unordered_map<size_t, std::vector<size_t>>> promise);

  void addSampleWeights(size_t test_sample_idx,
                        std::shared_ptr<Tree> tree,
                        std::unordered_map<size_t, double> &weights_by_sampleID,
                        std::vector<size_t> terminal_node_IDs);
  void normalizeSampleWeights(std::unordered_map<size_t, double> &weights_by_sampleID);

  std::vector<std::vector<double>> predictInternal(const Forest& forest,
                                                   Data* prediction_data,
                                                   const std::unordered_map<size_t, std::vector<size_t>>& terminal_node_IDs_by_tree);

  uint num_threads;
  std::vector<uint> thread_ranges;

  std::shared_ptr<PredictionStrategy> prediction_strategy;
};


#endif //GRADIENTFOREST_FORESTPREDICTOR_H
