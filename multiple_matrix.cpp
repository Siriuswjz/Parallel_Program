#include<iostream>
#include<cstdlib>
#include<omp.h>
#include<random>
using namespace std;

double* parallel_matrix_multiple(const double* A, const double* B, int m, int n, int p){
    double* res = new double[(size_t)m * p];
    // initialize result to 0
    for (size_t i = 0; i < (size_t)m * p; ++i) res[i] = 0.0;

    #pragma omp parallel
    {
        #pragma omp single
        cout << "OpenMP: using " << omp_get_num_threads() << " threads (max: " << omp_get_max_threads() << ")" << std::endl;

        #pragma omp for
        for (int i = 0; i < m; ++i) {
            for (int k = 0; k < n; ++k) {
                double a_ik = A[(size_t)i * n + k];
                const double* b_row = B + (size_t)k * p;
                double* r_row = res + (size_t)i * p;
                for (int j = 0; j < p; ++j) {
                    r_row[j] += a_ik * b_row[j];
                }
            }
        }
    }
    return res;
}

// vector<vector<double>> parallel_matrix_multiple(
//     vector<vector<double>>& A,
//     vector<vector<double>>& B){
    
//     int m = A.size();
//     int n = A[0].size();
//     int p = B[0].size();
//     vector<vector<double>> res(m, vector<double>(p, 0.0));

//     #pragma omp parallel for
//     for(int i = 0; i < m; i++){
//         for (int j = 0; j < p; j++){
//             double sum = 0.0; 
//             for (int k = 0; k < n; k++){
//                 sum += A[i][k] * B[k][j];
//             }
//             res[i][j] = sum; 
//         }
//     }
//     return res;
// }

double* init_matrix(int rows,int cols){
    double* mat = new double[(size_t)rows * cols];
    mt19937_64 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0,1.0);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            mat[(size_t)i * cols + j] = dist(rng);
        }
    }
    return mat;
}

int main(int argc, char** argv){
    int rows = 8000;
    int cols = 8000;
    int p = 8000; // result columns
    if (argc >= 2) rows = std::atoi(argv[1]);
    if (argc >= 3) cols = std::atoi(argv[2]);
    if (argc >= 4) p = std::atoi(argv[3]);

    cout << "Matrix A: " << rows << " x " << cols << ", B: " << cols << " x " << p << "\n";
    double* m1 = init_matrix(rows, cols);
    double* m2 = init_matrix(cols, rows);

    double start = omp_get_wtime();
    double* res = parallel_matrix_multiple(m1, m2, rows, cols, p);
    double end = omp_get_wtime();

    cout << "Parallel multiply time: " << (end - start) << " seconds" << endl;

    // free memory
    delete[] m1;
    delete[] m2;
    delete[] res;
    return 0;
}
