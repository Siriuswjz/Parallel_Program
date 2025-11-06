#include<iostream>
#include<vector>

using namespace std;

int partition(vector<int>& arr,int low,int high){
    int pivot = arr[low];
    int left = low;
    int right = high;
    while (left<right)
    {
        // 从右往左找第一个小于 pivot 的数
        while (left < right && arr[right] >= pivot) {
            right--;
        }
        // 从左往右找第一个大于 pivot 的数
        while (left < right && arr[left] <= pivot) {
            left++;
        }
        if(left<right) swap(arr[left],arr[right]);
    }
    swap(arr[low],arr[left]);
    return left;
}
void quicksort(vector<int>& arr,int low, int high){
    if(low<high){
        int index = partition(arr,low,high);
        quicksort(arr,low,index-1);
        quicksort(arr,index+1,high);
    }
}
int main(){
    vector<int> arr = {34, 7, 23 ,32, 5, 9};
    quicksort(arr,0,arr.size()-1);
    for ( auto& ele : arr)
    {
        cout<<ele<<" ";
    }
    
}