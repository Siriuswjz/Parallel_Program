#include <iostream>
#include <random>
#include <cstdlib>
#include<omp.h>

using namespace std;

double** allocate_matrix(int rows, int cols){
    if (rows <= 0 || cols <= 0) return nullptr;
    double* data = new double[(size_t)rows * cols];
    double** mat = new double*[rows];
    for (int i = 0; i < rows; ++i) mat[i] = data + (size_t)i * cols;
    return mat;
}

void free_matrix(double** mat){
    if (!mat) return;
    delete[] mat[0];
    delete[] mat;  
}

void init_matrix(double** mat, int rows, int cols){
    mt19937_64 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            mat[i][j] = dist(rng);
        }
    }
}

void multiply_matrix_omp(double** A, double** B, double** C, int rows, int cols){
    #pragma omp parallel for
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            C[i][j] = 0.0;

    #pragma omp parallel for
    for (int i = 0; i < rows; ++i){
        for (int k = 0; k < cols; ++k){
            double a = A[i][k];
            double* brow = B[k];
            double* crow = C[i];
            for (int j = 0; j < cols; ++j){
                crow[j] += a * brow[j];
            }
        }
    }
}
void multiply_matrix(double** A, double** B, double** C, int rows, int cols){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
        {
            C[i][j]=0;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int k = 0; k < cols; k++)
        {
            double a = A[i][k];
            double* brow = B[k];
            double* crow = C[i];
            for(int j = 0; j < cols; j++){
                crow[j]+= a * brow[j];
            }
        }
    }
}

int main(){
    int rows = 8000;
    int cols = 8000;

    double** A = allocate_matrix(rows, cols);
    double** B = allocate_matrix(rows, cols);
    double** C = allocate_matrix(rows, cols);

    init_matrix(A, rows, cols);
    init_matrix(B, rows, cols);

    double start = omp_get_wtime();
    multiply_matrix_omp(A, B, C, rows, cols);
    double end = omp_get_wtime();
    cout << "Parallel multiply time: " << (end - start) << " seconds" << endl;

    start = omp_get_wtime();
    multiply_matrix(A,B,C,rows,cols);
    end = omp_get_wtime();
    cout << "multiply time: " << (end - start) << " seconds" << endl;

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    return 0;
}
