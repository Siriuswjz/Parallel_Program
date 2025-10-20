#include<iostream>
#include<vector>
#include<omp.h>
#include<cmath>
using namespace std;

// double sin_tayolr(double x,int max_terms){
//     double result = 0.0;
//     double current = x;
//     double scaler= x * x;
//     for (int i = 0; i < max_terms; i++)
//     {
//         result += current;
//         current = (-scaler)/((2*i+2)*(2*i+3)) * current;

//     }
//     return result;
// }

double sin_taylor_serial(double x , int max_terms){
    double result = 0.0;
    for (int i = 0; i < max_terms; i++)
    {
        int power = 2 * i + 1;
        double factorial = 1.0;
        for (int j = 1; j <= power; j++)
        {
            factorial *= j;
        }
        result += pow(-1,i) * pow(x,power) / factorial;
    }
    return result;
}

double sin_taylor_omp(double x , int max_terms){
    double result = 0.0;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < max_terms; i++)
    {
        int power = 2 * i + 1;
        double factorial = 1.0;
        for (int j = 1; j <= power; j++)
        {
            factorial *= j;
        }
        result += pow(-1,i) * pow(x,power) / factorial;
    }
    return result;
}

int main(){
    int max_terms = 100000;  
    double x = M_PI/4;
    vector<int> thread_counts = {1, 2, 4, 6, 8, 10};  
    
    // 首先测量串行版本的时间作为基准
    double start = omp_get_wtime();
    double result_serial = sin_taylor_serial(x, max_terms);
    double end = omp_get_wtime();
    double serial_time = end - start;
    
    // 输出到CSV文件
    FILE* file = fopen("performance_results.csv", "w");
    fprintf(file, "threads,serial_time,parallel_time,speedup\n");
    
    // 测试不同线程数的并行性能
    for(int num_threads : thread_counts) {
        omp_set_num_threads(num_threads);  
        
        start = omp_get_wtime();
        double result_parallel = sin_taylor_omp(x, max_terms);
        end = omp_get_wtime();
        double parallel_time = end - start;
        
        double speedup = serial_time / parallel_time;
        
        // 写入CSV文件
        fprintf(file, "%d,%.6f,%.6f,%.3f\n", num_threads, serial_time, parallel_time, speedup);
    }
    
    fclose(file);
    
    cout << "串行结果: " << result_serial << endl;
    cout << "理论值 sin(π/4): " << sin(M_PI/4) << endl;
    
    return 0;
}