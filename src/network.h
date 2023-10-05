#include <vector>
#include <random>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iostream>
#include <cassert>
#include "algebra.h"

using namespace alg;

typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> ddVector;
typedef std::vector<std::vector<std::vector<double>>> dddVector;
typedef std::vector<std::pair<dVector, dVector>> Data;

namespace neural{

    double sigmoid(double z){
        return 1.0/(1.0 + std::exp(z*(-1)));
    }

    dVector sigmoid_prime(dVector z){
        dVector temp(z.size(), 0);
        for(int i = 0; i < z.size(); i++){
            double s = sigmoid(z[i]);
            temp[i] = s*(1-s);
        }
        return temp;
    }

    dVector sigmoid(dVector z){
        dVector temp(z.size(), 0);
        for(int i = 0; i < z.size(); i++) temp[i] = sigmoid(z[i]);
        return temp;
    }

    class Network{
        public:
            int num_layers;
            std::vector<int> sizes;
            ddVector biases;
            dddVector weights;

            Network(std::vector<int> _sizes){
                sizes = _sizes;
                num_layers = _sizes.size();
                std::random_device rd{};
                std::mt19937 generator{rd()};
                std::normal_distribution<double> distribution{0.0, 1.0};
                for(int i = 1; i < num_layers; i++){
                    std::vector<double> bias(sizes[i]);
                    for(int j = 0; j < sizes[i]; j++) bias[j] = distribution(generator);
                    biases.push_back(bias);
                }
                for(int i = 0; i < num_layers-1; i++){
                    std::vector<std::vector<double>> temp_w(sizes[i+1], std::vector<double>(sizes[i]));
                    for(int j = 0; j < sizes[i+1]; j++) for(int k = 0; k < sizes[i]; k++) temp_w[j][k] = distribution(generator);
                    weights.push_back(temp_w);
                }
            }

            dVector feed_forward(std::vector<double> a){
                for(int i = 0; i < num_layers-1; i++){
                    a = sigmoid(add_vec(mult_mat_vec(weights[i], a), biases[i]));   
                }
                return a;
            }

            void SGD(Data training_data, int epochs, int mini_batch_size, double eta, std::vector<std::pair<dVector, int>> test_data = {}){
                int n_test = test_data.size();
                int n = training_data.size();
                for(int j = 0; j < epochs; j++){
                    auto rng = std::default_random_engine {};
                    std::shuffle(training_data.begin(), training_data.end(), rng);
                    std::vector<Data> mini_batches;
                    for(int k = 0; k + mini_batch_size < n ; k += mini_batch_size){
                        Data to_push(training_data.begin()+k, training_data.begin()+k+mini_batch_size);
                        mini_batches.push_back(to_push);                       
                    }
                    for(Data& mini_batch: mini_batches){
                        update_mini_batch(mini_batch, eta);
                    }
                    if(n_test > 0){
                        std::cout << "Epoch " << j << ": "<< evaluate(test_data)<<  "/" << n_test << std::endl;
                        
                    } else{
                        std::cout << "Epoch "<< j << " complete" << std::endl;
                    }
                }
            }

            void update_mini_batch(Data& mini_batch, double eta){
                ddVector nabla_b;
                for(int i = 0; i < size(biases); i++) nabla_b.push_back(zeros(biases[i]));
                dddVector nabla_w;
                for(int i = 0; i < size(weights); i++) nabla_w.push_back(zeros(weights[i]));
                for(auto& batch: mini_batch){
                    auto x = batch.first;
                    auto y = batch.second;
                    ddVector delta_nabla_b;
                    for(int i = 0; i < size(biases); i++) delta_nabla_b.push_back(zeros(biases[i]));
                    dddVector delta_nabla_w;
                    for(int i = 0; i < size(weights); i++) delta_nabla_w.push_back(zeros(weights[i]));
                    backdrop(x,y, delta_nabla_b, delta_nabla_w);
                    for(int i = 0; i < nabla_b.size(); i++) nabla_b[i] = add_vec(delta_nabla_b[i], nabla_b[i]);
                    for(int i = 0; i < nabla_w.size(); i++) nabla_w[i] = add_mat(delta_nabla_w[i], nabla_w[i]);
                    
                }
                for(int i = 0; i < size(weights); i++) weights[i] = sub_mat(weights[i], mult_mat_sca(nabla_w[i], (eta/((double)mini_batch.size()))));
                for(int i = 0; i < size(biases); i++) biases[i] = sub_vec(biases[i], mult_vec_sca(nabla_b[i], (eta/((double)mini_batch.size()))));
            }

            void backdrop(dVector x, dVector y, ddVector& nabla_b, dddVector& nabla_w){
                
                dVector activation = x;
                ddVector activations;
                activations.push_back(x);
                ddVector zs;
                for(int i = 0; i < biases.size(); i++){
                    
                    dVector z = add_vec(mult_mat_vec(weights[i], activation), biases[i]);
                    assert(z.size() == biases[i].size());
                    
                    
                    zs.push_back(z);
                    activation = sigmoid(z);
                    assert(activation.size() == z.size());
                    activations.push_back(activation);
                    
                }
                
                assert(activations.back().size() == y.size() && activations.back().size() == zs.back().size());
                dVector delta = mult_vec_ele(cost_derivative(activations.back(), y), sigmoid_prime(zs.back()));
                
                nabla_b.back() = delta;
                nabla_w.back() = mult_vec_vec(delta, activations[activations.size()-2]);
                
                for(int l = 2; l < num_layers; l++){
                    dVector z = zs[zs.size()-l];
                    dVector sp = sigmoid_prime(z);
                    delta = mult_vec_ele(mult_mat_vec(transpose(weights[weights.size()-l+1]), delta), sp);
                    nabla_b[nabla_b.size() - l] = delta;
                    nabla_w[nabla_w.size() - l] = mult_vec_vec(delta, activations[activations.size()-l-1]);
                }
                
                return;
            }

            dVector cost_derivative(dVector output_activations, dVector y){
                return sub_vec(output_activations, y);
            }

            int evaluate(std::vector<std::pair<dVector, int>> test_data){
                int result = 0;
                for(auto& test: test_data){
                    dVector x = feed_forward(test.first);
                    assert(x.size() == 10);
                    int y = test.second;
                    int test_result = std::max_element(x.begin(), x.end()) - x.begin();
                    if(test_result == y) result++;
                }

                return result;
            }

    };

}