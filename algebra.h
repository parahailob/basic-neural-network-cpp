#include <vector>


namespace alg{
    std::vector<double> mult_mat_vec(std::vector<std::vector<double>> lhs, std::vector<double> rhs){
        int n = size(rhs);
        int m = size(lhs);
        std::vector<double> result(m, 0);
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                result[i] += lhs[i][j]*rhs[j];
            }
        }
        return result;
    }

    std::vector<double> add_vec(std::vector<double> lhs, std::vector<double> rhs){
        int n = size(lhs);
        std::vector<double> result(n);
        for(int i = 0; i < n; i++) result[i] = lhs[i]+rhs[i];
        return result;
    }

    std::vector<double> zeros(std::vector<double> vec){
        std::vector<double> result(size(vec), 0);
        return result;
    }

    std::vector<std::vector<double>> zeros(std::vector<std::vector<double>> mat){
        std::vector<std::vector<double>> result(size(mat), std::vector<double>(size(mat[0]), 0));
        return result;
    }
}

