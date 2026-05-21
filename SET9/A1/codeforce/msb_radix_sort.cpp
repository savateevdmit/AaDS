#include <iostream>
#include <vector>
#include <string>

int charAt(const std::string& s, int d) {
    if (d < (int)s.length()) {
        return (unsigned char)s[d];
    }
    return -1;
}

void msbRadixSortRec(std::vector<std::string>& arr, int low, int high, int d, std::vector<std::string>& aux) {
    if (high <= low) return;

    const int R = 256;
    int count[R + 2] = {0};

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        count[c + 2]++;
    }

    for (int r = 0; r < R + 1; r++) {
        count[r + 1] += count[r];
    }

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = low; i <= high; i++) {
        arr[i] = aux[i - low];
    }

    for (int r = 0; r < R; r++) {
        msbRadixSortRec(arr, low + count[r], low + count[r + 1] - 1, d + 1, aux);
    }
}

void msbRadixSort(std::vector<std::string>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    std::vector<std::string> aux(n);
    msbRadixSortRec(arr, 0, n - 1, 0, aux);
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    msbRadixSort(arr);
    
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << "\n";
    }
    
    return 0;
}