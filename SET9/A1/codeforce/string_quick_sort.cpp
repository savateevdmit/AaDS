#include <iostream>
#include <vector>
#include <string>

int charAt(const std::string& s, int d) {
    if (d < (int)s.length()) {
        return (unsigned char)s[d];
    }
    return -1;
}

void stringQuickSort(std::vector<std::string>& arr, int low, int high, int d) {
    if (low >= high) {
        return;
    }

    std::string temp = arr[low];
    arr[low] = arr[high];
    arr[high] = temp;

    int lt = low;
    int gt = high;
    int v = charAt(arr[low], d);
    int i = low + 1;

    while (i <= gt) {
        int t = charAt(arr[i], d);
        if (t < v) {
            std::string tmp = arr[lt];
            arr[lt] = arr[i];
            arr[i] = tmp;
            lt++;
            i++;
        } else if (t > v) {
            std::string tmp = arr[gt];
            arr[gt] = arr[i];
            arr[i] = tmp;
            gt--;
        } else {
            i++;
        }
    }

    stringQuickSort(arr, low, lt - 1, d);
    if (v >= 0) {
        stringQuickSort(arr, lt, gt, d + 1);
    }
    stringQuickSort(arr, gt + 1, high, d);
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    stringQuickSort(arr, 0, n - 1, 0);
    
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << "\n";
    }
    
    return 0;
}