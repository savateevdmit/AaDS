#include <iostream>
#include <vector>
#include <string>

struct StringLCP {
    std::string str;
    int lcp;
};

void lcpCompare(const std::string& a, const std::string& b, int k, int& cmp, int& lcp) {
    int lenA = a.length();
    int lenB = b.length();
    int minLen = lenA < lenB ? lenA : lenB;
    int match = k;

    while (match < minLen) {
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
        if (a[match] < b[match]) {
            cmp = -1;
        } else {
            cmp = 1;
        }
    }
}

std::vector<StringLCP> stringMerge(std::vector<StringLCP>& P, std::vector<StringLCP>& Q) {
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
            lcpCompare(P[i].str, Q[j].str, P[i].lcp, cmp, h);
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

std::vector<StringLCP> stringMergeSortRec(std::vector<std::string>& arr, int left, int right) {
    if (left == right) {
        std::vector<StringLCP> res;
        StringLCP item;
        item.str = arr[left];
        item.lcp = 0;
        res.push_back(item);
        return res;
    }

    int mid = left + (right - left) / 2;
    std::vector<StringLCP> P = stringMergeSortRec(arr, left, mid);
    std::vector<StringLCP> Q = stringMergeSortRec(arr, mid + 1, right);
    
    return stringMerge(P, Q);
}

void stringMergeSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    std::vector<StringLCP> res = stringMergeSortRec(arr, 0, arr.size() - 1);
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = res[i].str;
    }
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    stringMergeSort(arr);
    
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << "\n";
    }
    
    return 0;
}