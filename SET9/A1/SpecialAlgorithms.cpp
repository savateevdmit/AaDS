#include "SpecialAlgorithms.h"
#include <algorithm>

int charAt(const std::string& s, int d, long long& comps) {
    if (d < (int)s.length()) {
        comps++;
        return (unsigned char)s[d];
    }
    return -1;
}

//  stringQuickSort
void stringQuickSort(std::vector<std::string>& arr, int low, int high, int d, long long& comps) {
    if (low >= high) return;

    std::string temp = arr[low];
    arr[low] = arr[high];
    arr[high] = temp;

    int lt = low;
    int gt = high;
    int v = charAt(arr[low], d, comps);
    int i = low + 1;

    while (i <= gt) {
        int t = charAt(arr[i], d, comps);
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

    stringQuickSort(arr, low, lt - 1, d, comps);
    if (v >= 0) {
        stringQuickSort(arr, lt, gt, d + 1, comps);
    }
    stringQuickSort(arr, gt + 1, high, d, comps);
}

// stringMergeSort
struct StringLCP {
    std::string str;
    int lcp;
};

void lcpCompare(const std::string& a, const std::string& b, int k, int& cmp, int& lcp, long long& comps) {
    int lenA = a.length();
    int lenB = b.length();
    int minLen = lenA < lenB ? lenA : lenB;
    int match = k;

    while (match < minLen) {
        comps++;
        if (a[match] != b[match]) {
            break;
        }
        match++;
    }

    lcp = match;
    if (match == lenA && match == lenB) {
        cmp = 0;
    } else if (match == lenA) {
        cmp = -1;
    } else if (match == lenB) {
        cmp = 1;
    } else {
        comps++; 
        if (a[match] < b[match]) {
            cmp = -1;
        } else {
            cmp = 1;
        }
    }
}

std::vector<StringLCP> stringMerge(std::vector<StringLCP>& P, std::vector<StringLCP>& Q, long long& comps) {
    std::vector<StringLCP> R;
    int m = P.size();
    int f = Q.size();
    R.reserve(m + f);
    
    int i = 0, j = 0;

    while (i < m && j < f) {
        if (P[i].lcp > Q[j].lcp) {
            R.push_back(P[i]);
            i++;
        } else if (P[i].lcp < Q[j].lcp) {
            R.push_back(Q[j]);
            j++;
        } else {
            int cmp, h;
            lcpCompare(P[i].str, Q[j].str, P[i].lcp, cmp, h, comps);
            if (cmp < 0) {
                R.push_back(P[i]);
                i++;
                Q[j].lcp = h;
            } else {
                R.push_back(Q[j]);
                j++;
                P[i].lcp = h;
            }
        }
    }

    while (i < m) {
        R.push_back(P[i]);
        i++;
    }
    while (j < f) {
        R.push_back(Q[j]);
        j++;
    }
    return R;
}

std::vector<StringLCP> stringMergeSortRec(std::vector<std::string>& arr, int left, int right, long long& comps) {
    if (left == right) {
        std::vector<StringLCP> res;
        StringLCP item;
        item.str = arr[left];
        item.lcp = 0;
        res.push_back(item);
        return res;
    }

    int mid = left + (right - left) / 2;
    std::vector<StringLCP> P = stringMergeSortRec(arr, left, mid, comps);
    std::vector<StringLCP> Q = stringMergeSortRec(arr, mid + 1, right, comps);
    
    return stringMerge(P, Q, comps);
}

void stringMergeSort(std::vector<std::string>& arr, long long& comps) {
    if (arr.empty()) return;
    std::vector<StringLCP> res = stringMergeSortRec(arr, 0, arr.size() - 1, comps);
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = res[i].str;
    }
}

// msdRadixSort
void msdRadixSortRec(std::vector<std::string>& arr, int low, int high, int d, std::vector<std::string>& aux, long long& comps, bool useCutoff) {
    if (high <= low) return;

    if (useCutoff && (high - low + 1) < 74) {
        stringQuickSort(arr, low, high, d, comps);
        return;
    }

    const int R = 256;
    int count[R + 2] = {0};

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d, comps);
        count[c + 2]++;
    }

    for (int r = 0; r < R + 1; r++) {
        count[r + 1] += count[r];
    }

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d, comps); 
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = low; i <= high; i++) {
        arr[i] = aux[i - low];
    }

    for (int r = 0; r < R; r++) {
        msdRadixSortRec(arr, low + count[r], low + count[r + 1] - 1, d + 1, aux, comps, useCutoff);
    }
}

void msdRadixSort(std::vector<std::string>& arr, long long& comps) {
    int n = arr.size();
    if (n <= 1) return;
    std::vector<std::string> aux(n);
    msdRadixSortRec(arr, 0, n - 1, 0, aux, comps, false);
}

// msdRadixSort с переключением на string QuickSort
void msdRadixSortWithCutoff(std::vector<std::string>& arr, long long& comps) {
    int n = arr.size();
    if (n <= 1) return;
    std::vector<std::string> aux(n);
    msdRadixSortRec(arr, 0, n - 1, 0, aux, comps, true);
}