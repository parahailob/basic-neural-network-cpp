#include <vector>
#include <random>
#include <cmath>
#include <tuple>
#include <algorithm>
#include "algebra.h"

using namespace alg;
typedef std::vector<std::pair<std::vector<double>, std::vector<double>>> Data;

namespace neural{

    double sigmoid(double z){
        return 1.0/(1.0 + std::exp(-z));
    }

    std::vector<double> sigmoid(std::vector<double> z){
        std::vector<double> temp(size(z));
        for(int i = 0; i < size(z); i++) temp[i] = sigmoid(z[i]);
        return temp;
    }

    class Network{
        public:
            int num_layers;
            std::vector<int> sizes;
            std::vector<std::vector<double>> biases;
            std::vector<std::vector<std::vector<double>>> weights;

            Network(std::vector<int> _sizes){
                sizes = _sizes;
                num_layers = size(_sizes);
                std::default_random_engine generator;
                std::normal_distribution<double> distribution(0, 1);
                for(int i = 1; i < num_layers; i++){
                    std::vector<double> bias(sizes[i]);
                    for(auto& b: bias) b = distribution(generator);
                    biases.push_back(bias);
                }
                for(int i = 0; i < num_layers-1; i++){
                    std::vector<std::vector<double>> temp_w(sizes[i+1], std::vector<double>(sizes[i]));
                    for(auto& row: temp_w) for(auto& cell: row) cell = distribution(generator);
                    weights.push_back(temp_w);
                }
            }

            std::vector<double> feed_forward(std::vector<double> a){
                for(int i = 0; i < num_layers-1; i++){
                    a = sigmoid(add_vec(mult_mat_vec(weights[i], a), biases[i]));
                }

                return a;
            }

            void SGD(Data training_data, int epochs, int mini_batch_size, double eta, auto test_data = nullptr){
                int n_test = -1;
                if(test_data != nullptr) n_test = size(test_data);
                int n = size(training_data);
                for(int j = 0; j < epochs; j++){
                    auto rng = std::default_random_engine {};
                    std::shuffle(training_data.begin(), training_data.end(), rng);
                    std::vector<Data> mini_batches;
                    for(int k = 0; k < n - mini_batch_size; k += mini_batch_size) mini_batches.push_back(training_data.begin()+k, training_data.begin()+k+mini_batch_size);
                    for(auto& mini_batch: mini_batches){
                        update_mini_batch(mini_batch, eta);
                    }
                    if(n_test != -1){
                        std::cout << std::format("Epoch {}: {} / {}", j, evaluate(test_data), n_test) << std::endl;
                    } else{
                        std::cout << std::format("Epoch {} complete", j) << std::endl;
                    }
                }
            }

            void update_mini_batch(Data mini_batch, double eta){
                std::vector<std::vector<double>> nabla_b(size(biases));
                for(int i = 0; i < size(biases); i++) nabla_b[i] = zeros(biases[i]);
                std::vector<std::vector<std::vector<double>>> nabla_w(size(weights));
                for(int i = 0; i < size(weights); i++) nabla_w[i] = zeros(weights[i]);
                for(auto& batch: mini_batch){
                    auto x = batch.first;
                    auto y = batch.second;
                    
                }

            }

    };

}