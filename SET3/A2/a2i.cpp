#include <iostream>
#include <vector>

void insertionSort(std::vector<int> &arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int> &arr, int left, int mid, int right, std::vector<int> &temp) {
    for (int i = left; i <= right; ++i) {
        temp[i] = arr[i];
    }

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (temp[i] <= temp[j]) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = temp[j++];
        }
    }

    while (i <= mid) {
        arr[k++] = temp[i++];
    }

    while (j <= right) {
        arr[k++] = temp[j++];
    }
}

void hybridMergeSort(std::vector<int> &arr, int left, int right, std::vector<int> &temp, int threshold) {
    if (left >= right) {
        return;
    }

    if (right - left + 1 <= threshold) {
        insertionSort(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    hybridMergeSort(arr, left, mid, temp, threshold);
    hybridMergeSort(arr, mid + 1, right, temp, threshold);
    merge(arr, left, mid, right, temp);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    std::vector<int> temp(n);
    const int threshold = 15;
    hybridMergeSort(arr, 0, n - 1, temp, threshold);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }

    return 0;
}