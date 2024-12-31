#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <stack>

using namespace std;
using namespace std::chrono;

vector<int> gen_random_array(int size, int upper) {
    vector<int> arr(size, 0);
    for (int i = 0; i < size; i++) arr[i] = rand() % upper;
    return arr;
}

void swap(int& a, int& b) noexcept{
    int temp = a;
    a = b;
    b = temp;
}

void print_array(const vector<int>& arr) {
    for (const auto& i : arr) cout << i << " ";
    cout << endl;
}

void BubbleSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n - 1; i >= 0; i--) {
        for(int j = 0; j < i; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

void SelectSort(vector<int>& arr) {
    for(int i = 0; i < arr.size(); i++) {
        int minIndex = i;
        for(int j = i; j < arr.size(); j++) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        swap(arr[i], arr[minIndex]);
    }
}

void InsertSort(vector<int>& arr, int low, int high) {
    for(int i = low; i <= high; i++) {
        int j = low;
        while(arr[i] > arr[j])
            ++j;
        for(int k = j; k < i; k++)
            swap(arr[i], arr[k]);
    }
}

vector<int> Merge(int low, int mid, int high, const vector<int>& arr) {
    //to merge array A at [low, mid], and array B at[mid + 1, high]
    int i = low, j = mid + 1, k = low;
    vector<int> result(arr.size());
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j])
            result[k++] = arr[i++];
        else
            result[k++] = arr[j++];
    }
    while(i <= mid)
        result[k++] = arr[i++];
    while(j <= high)
        result[k++] = arr[j++];
    return result;
}

void MergeSort(vector<int>& arr, int low, int high) {
    if (low >= high)
        return;
    int mid = low + (high - low) / 2;
    MergeSort(arr, low, mid);
    MergeSort(arr, mid + 1, high);
    vector<int> tmp = Merge(low, mid, high, arr);
    for(int i = low; i <= high; i++)
        arr[i] = tmp[i];
}

int partition(vector<int>& arr, int low, int high, int pivotMethod) {
    int pivot = low;
    switch (pivotMethod) {
        case 1:
            pivot = low + (high - low) / 2;
            break;
        case 2: {
            int a = arr[low], b = arr[low + (high - low) / 2], c = arr[high];
            if ( (b - a) * (b - c) < 0 )
                pivot = low + (high - low) / 2;
            else if ( (c - a) * (c - b) < 0 )
                pivot = high;
            break;
        }
        case 3:
            pivot = rand() % (high - low + 1) + low;
            break;
        default:
            break;
    }
    while(low < high) {
        while(arr[low] < arr[pivot] && low < high)
            ++low;
        while(arr[high] >= arr[pivot] && high > low)
            --high;
        pivot = low == pivot ? high : high == pivot ? low : pivot;
        swap(arr[low], arr[high]);
    }
    swap(arr[pivot], arr[low]);
    return low;
}

void QuickSort(vector<int>& arr, int low, int high) {
    if (low >= high)
        return;
    int mid = partition(arr, low, high, 0);
    QuickSort(arr, low, mid);
    QuickSort(arr, mid + 1, high);
}

void QuickSortOptimizedVer1(vector<int>& arr, int low, int high) {
    if (low >= high)
        return;
    if (high - low <= 10)
        InsertSort(arr, low, high);
    else {
        int mid = partition(arr, low, high, 1);
        QuickSort(arr, low, mid);
        QuickSort(arr, mid + 1, high);
    }
}

void QuickSortOptimizedVer2(vector<int>& arr, int low, int high) {
    stack<pair<int, int>> task;
    task.push(make_pair(low, high));

    while(!task.empty()) {
        auto curTask = task.top();
        task.pop();
        int low = curTask.first, high = curTask.second;
        if (low >= high)
            continue;
        if (high - low <= 10)
            InsertSort(arr, low, high);
        else {
            int mid = partition(arr, low, high, 3); //method 3 is random pivot
            if (mid - low < high - mid) {
                //left sub-problem is smaller, deal with it first
                task.push(make_pair(mid + 1, high));
                task.push(make_pair(low, mid));
            } else {
                task.push(make_pair(low, mid));
                task.push(make_pair(mid + 1, high));
            }
        }
    }
}

void heapify_down(vector<int>& arr, int start) {
    int cur = start, n = static_cast<int>(arr.size());
    while (2 * cur + 1 < n) {
        int left_child = 2 * cur + 1;
        int to_swap = -1;
        if (arr[cur] > arr[left_child])
            to_swap = left_child;
        if (int right_child = 2 * cur + 2;
            right_child < n && arr[cur] > arr[right_child] && arr[right_child] < arr[left_child])
            to_swap = right_child;
        if (to_swap == -1)
            return;
        swap(arr[to_swap], arr[cur]);
        cur = to_swap;
    }
}

int top_heap(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    int top_element = arr[0];
    arr[0] = arr[n - 1];
    arr.pop_back();
    heapify_down(arr, 0);
    return top_element;
}

void HeapSort(vector<int>& arr) {
    //constrcut heap
    vector<int> heap;
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify_down(arr, i);
    //top heap one by one
    while( !arr.empty() )
        heap.emplace_back(top_heap(arr));
    arr = heap;
}

void estimateSort(std::vector<int>& arr, function<void(std::vector<int>&)> sortFunction, bool printArray) {
    vector<int> arrCopy = arr;
    auto start = high_resolution_clock::now();
    sortFunction(arrCopy); // 调用传入的排序函数
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Sort scale: " << arr.size() << ", cost: " << duration.count() << " ms" << endl;
    cout << "efficiency: " << static_cast<float>(arr.size()) / duration.count() << endl;
    if (printArray) {
        cout << "Unsorted array:" << endl;
        print_array(arr);
        cout << "Sorted array:" << endl;
        print_array(arrCopy);
    }
    cout << endl;
}

void TestAllSort(vector<int>& arr, bool printArray) {
    cout << "Testing Bubble Sort:" << endl;
    estimateSort(arr, [](auto& container){ BubbleSort(container); }, printArray);

    cout << "Testing Selection Sort:" << endl;
    estimateSort(arr, [](auto& container){ SelectSort(container); }, printArray);

    cout << "Testing Insertion Sort:" << endl;
    estimateSort(arr, [](auto& container){ InsertSort(container, 0, container.size() - 1); }, printArray);

    cout << "Testing Merge Sort:" << endl;
    estimateSort(arr, [](auto& container){ MergeSort(container, 0, container.size() - 1); }, printArray);

    cout << "Testing Quick Sort:" << endl;
    estimateSort(arr, [](auto& container){ QuickSort(container, 0, container.size() - 1); }, printArray);

    cout << "Testing ver1_Optimized Quick Sort:" << endl;
    estimateSort(arr, [](auto& container){ QuickSortOptimizedVer1(container, 0, container.size() - 1); }, printArray);

    cout << "Testing ver2_Optimized Quick Sort:" << endl;
    estimateSort(arr, [](auto& container){ QuickSortOptimizedVer2(container, 0, container.size() - 1); }, printArray);

    cout << "Testing Heap Sort:" << endl;
    estimateSort(arr, [](auto& container){ HeapSort(container); }, printArray);
}

int main() {
    vector<int> arr = gen_random_array(50000, INT_MAX);
    TestAllSort(arr, false);
    //BubbleSort(arr);
    //SelectSort(arr);
    //InsertSort(arr, 0, arr.size() - 1);
    //MergeSort(arr, 0, arr.size() - 1);
    //QuickSort(arr, 0, arr.size() - 1);
    //QuickSortOptimizied(arr, 0, arr.size() - 1);
    //HeapSort(arr);
    return 0;
}
