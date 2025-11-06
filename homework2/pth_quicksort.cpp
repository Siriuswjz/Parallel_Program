#include<iostream>
#include<vector>
#include<pthread.h>
#include<random>
#include<atomic>
#include<chrono>
#include<thread>

using namespace std;

static atomic<int> thread_count(0);
static int MAX_THREADS = 8;
static int MIN_SIZE = 100000;

struct Range
{
    int low;
    int high;
    vector<double>* arr;
    Range(int l, int h, vector<double>* a):low(l),high(h),arr(a) {}
};

bool is_sorted_arr(vector<double>& arr, int n) {
    for (int i = 1; i < n; ++i) if (arr[i-1] > arr[i]) return false;
    return true;
}

int partition(vector<double>& arr, int low,int high){
    double privot = arr[low];
    int left = low;
    int right = high;
    while (left<right)
    {
        while (left<right && arr[right]>privot)  right--;
        while(left<right && arr[left]<=privot) left++;
        if (left<right) swap(arr[left],arr[right]);
    }
    swap(arr[low],arr[left]);
    return left;
}

void quicksort_seq(vector<double>& arr,int low,int high){
    if(low<high){
        int index = partition(arr,low,high);
        quicksort_seq(arr,low,index-1);
        quicksort_seq(arr,index+1,high);
    }
}

// forward declaration so thread function can call it
void quicksort_mt(vector<double>& arr,int low,int high);

// 创建新线程时 调用
void* quicksort_thread_fn(void* arg){
    Range* range = static_cast<Range*>(arg);
    quicksort_mt(*(range->arr),range->low,range->high);
    delete range;
    thread_count.fetch_sub(1);
    return nullptr;
}

void quicksort_mt(vector<double>& arr,int low,int high){
    if(high < low) return;
    if(high - low < MIN_SIZE){
        quicksort_seq(arr,low,high);
        return;
    }
    int pIndex = partition(arr,low,high);

    // 左右两边尝试并行化 优先在其中一遍创建线程 另一边当前线程处理
    bool left_threaded = false; // 标记左边是否创建了线程
    pthread_t left_tid;
    Range *left_arg = nullptr;

    int prev = thread_count.fetch_add(1);
    if(prev < MAX_THREADS){
        // 创建左线程
        left_threaded = true;
        left_arg = new Range(low, pIndex - 1, &arr);
        if(pthread_create(&left_tid,nullptr,quicksort_thread_fn,left_arg)!=0){
            // 创建失败
            thread_count.fetch_sub(1);
            left_threaded = false;
            delete left_arg;
            quicksort_seq(arr,low,pIndex-1);
        }
    }else{
        thread_count.fetch_sub(1);
        quicksort_seq(arr,low,pIndex-1);
    }
    quicksort_mt(arr,pIndex+1,high);

    // 若左侧用了线程 等待其结束
    if(left_threaded){
        pthread_join(left_tid,nullptr);
    }
}

int main(int argc, char** argv){
    int n = 1000000;
    if (argc >= 2) n = stoi(argv[1]);
    if (argc >=3) MAX_THREADS = stoi(argv[2]);
    if (argc>=4) MIN_SIZE = stoi(argv[3]);

    vector<double> v(n);
    mt19937_64 rng(12345);
    for (int i = 0; i < n; i++)
    {
        v[i] = rng();
    }

    vector<double> arr_seq(v);
    vector<double> arr_mt(v);

    //串行时间计时
    auto start = chrono::high_resolution_clock::now();
    quicksort_seq(arr_seq,0,arr_seq.size()-1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seq = end - start;
    cout<<"Sequential quicksort time: "<<elapsed_seq.count()<<" seconds. sorted ? "<<is_sorted_arr(arr_seq,arr_seq.size())<<endl;
    //多线程时间计时
    start = chrono::high_resolution_clock::now();
    thread_count.store(0);
    quicksort_mt(arr_mt,0,arr_mt.size()-1);
    while(thread_count.load() > 0) this_thread::sleep_for(chrono::milliseconds(1));
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_mt = end - start;
    cout<<"Multithreaded quicksort time: "<<elapsed_mt.count()<<" seconds. sorted ? "<<is_sorted_arr(arr_mt,arr_mt.size())<<endl;

}