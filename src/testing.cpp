#include "network.h"
#include <bits/stdc++.h>
#include "Dataset.hpp"

typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> ddVector;
typedef std::vector<std::vector<std::vector<double>>> dddVector;
typedef std::vector<std::pair<dVector, dVector>> Data;
typedef std::vector<std::pair<dVector, int>> DataTest;

int main(){

    Mnist mnist_training;
    ddVector training_images = mnist_training.readTrainingFile("train-images-idx3-ubyte");
    dVector training_labels = mnist_training.readLabelFile("train-labels-idx1-ubyte");
    
    
    
    Mnist mnist_test;
    ddVector test_images = mnist_test.readTrainingFile("t10k-images-idx3-ubyte");
    dVector test_labels = mnist_test.readLabelFile("t10k-labels-idx1-ubyte");

    Data training_data;
    for(int i = 0; i < training_images.size(); i++){
        dVector labels(10, 0);
        labels[int(training_labels[i])] = 1.0;
        for(double& ele: training_images[i]) ele /= 255.0;
        training_data.push_back(make_pair(training_images[i], labels));
    }
    DataTest test_data;
    for(int i = 0; i < test_images.size(); i++){
        for(double& ele: test_images[i]) ele /= 255.0;
        test_data.push_back(make_pair(test_images[i], int(test_labels[i])));
    }
    std::cout << "Reading complete" << std::endl;
    neural::Network net({784, 28, 10});
    std::cout << "Network created" << std::endl;
    net.SGD(training_data, 30, 10, 0.9, test_data);

    // dVector vec1 = {1,2,3,4};
    // dVector vec2 = {1,2,3};
    // ddVector res = alg::mult_vec_vec(vec2, vec1);
    // res = alg::transpose(res);
    // std::cout << std::endl;
    // for(auto& row: res){
    //     for(auto& ele:row) std::cout << ele << " ";
    //     std::cout << std::endl;
    // } 

    return 0;
}