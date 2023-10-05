#include <vector>


typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> ddVector;
typedef std::vector<std::vector<std::vector<double>>> dddVector;

namespace alg{
    dVector mult_mat_vec(ddVector lhs, dVector rhs){
        int n = size(lhs);
        int m = size(rhs);
        //std::cout << n << " " << m << std::endl;
        dVector result(n, 0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                result[i] += lhs[i][j]*rhs[j];
            }
        }
        return result;
    }

    dVector sub_vec(dVector lhs, dVector rhs){
        int n = size(lhs);
        dVector result(n);
        for(int i = 0; i < n; i++) result[i] = lhs[i]-rhs[i];
        return result;
    }

    dVector add_vec(dVector lhs, dVector rhs){
        int n = size(lhs);
        dVector result(n);
        for(int i = 0; i < n; i++) result[i] = lhs[i]+rhs[i];
        return result;
    }

    dVector zeros(dVector vec){
        dVector result(size(vec), 0);
        return result;
    }

    ddVector zeros(ddVector mat){
        ddVector result(size(mat), dVector(size(mat[0]), 0));
        return result;
    }

    ddVector mult_mat_sca(ddVector matrix, double scalar){
        ddVector result(matrix.size(), dVector(matrix[0].size(), 0));
        for(int i = 0; i < matrix.size(); i++) for(int j = 0; j < matrix[0].size(); j++) result[i][j] = matrix[i][j]*scalar;
        return result;
    }

    ddVector add_mat(ddVector lhs, ddVector rhs){
        int n = size(lhs);
        int m = size(lhs[0]);
        ddVector result(n, dVector(m, 0));
        for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) result[i][j] = lhs[i][j] + rhs[i][j];
        return result;
    }

    ddVector sub_mat(ddVector lhs, ddVector rhs){
        int n = size(lhs);
        int m = size(lhs[0]);
        ddVector result(n, dVector(m, 0));
        for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) result[i][j] = lhs[i][j] - rhs[i][j];
        return result;
    }

    dVector mult_vec_sca(dVector vec, double scalar){
        dVector result(vec.size(), 0);
        for(int i = 0; i < vec.size(); i++) result[i] = vec[i]*scalar;
        return result;
    }

    dVector mult_vec_ele(dVector lhs, dVector rhs){
        int n = size(lhs);
        dVector res(n);
        for(int i = 0; i < n; i++) res[i] = lhs[i]*rhs[i];
        return res;
         
    }

    ddVector mult_vec_vec(dVector lhs, dVector rhs){
        int n = size(lhs);
        int m = size(rhs);
        ddVector res(n, dVector(m, 0));
        for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) res[i][j] = lhs[i]*rhs[j];
        return res;
    }

    ddVector transpose(ddVector mat){
        int n = mat.size();
        int m = mat[0].size();
        ddVector result(m, dVector(n));
        for(int i = 0; i < m; i++) for(int j = 0; j < n; j++) result[i][j] = mat[j][i];
        return result;
    }


}

