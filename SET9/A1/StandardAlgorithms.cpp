#include "StandardAlgorithms.h"

bool lessThan(const std::string& a, const std::string& b, long long& cnt) {
    int n = (int) a.size();
    int m = (int) b.size();
    int lim = n;
    if (m < lim) {
        lim = m;
    }

    for (int i = 0; i < lim; ++i) {
        cnt++;
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }

    return n < m;
}

bool lessThanOrEqual(const std::string& a, const std::string& b, long long& cnt) {
    int n = (int) a.size();
    int m = (int) b.size();
    int lim = n;
    if (m < lim) {
        lim = m;
    }

    for (int i = 0; i < lim; ++i) {
        cnt++;
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }

    return n <= m;
}

int partitionArray(std::vector<std::string>& a, int l, int r, long long& cnt) {
    std::string x = a[r];
    int i = l - 1;

    for (int j = l; j < r; ++j) {
        if (lessThan(a[j], x, cnt)) {
            i++;
            std::string t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }

    std::string t = a[i + 1];
    a[i + 1] = a[r];
    a[r] = t;
    return i + 1;
}

void standardQuickSort(std::vector<std::string>& a, int l, int r, long long& cnt) {
    if (l >= r) {
        return;
    }

    int p = partitionArray(a, l, r, cnt);
    standardQuickSort(a, l, p - 1, cnt);
    standardQuickSort(a, p + 1, r, cnt);
}

void mergeParts(std::vector<std::string>& a, int l, int mid, int r, long long& cnt) {
    int n1 = mid - l + 1;
    int n2 = r - mid;

    std::vector<std::string> left(n1);
    std::vector<std::string> right(n2);

    for (int i = 0; i < n1; ++i) {
        left[i] = a[l + i];
    }
    for (int i = 0; i < n2; ++i) {
        right[i] = a[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (lessThanOrEqual(left[i], right[j], cnt)) {
            a[k] = left[i];
            i++;
        } else {
            a[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        a[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        a[k] = right[j];
        j++;
        k++;
    }
}

void standardMergeSort(std::vector<std::string>& a, int l, int r, long long& cnt) {
    if (l >= r) {
        return;
    }

    int mid = l + (r - l) / 2;
    standardMergeSort(a, l, mid, cnt);
    standardMergeSort(a, mid + 1, r, cnt);
    mergeParts(a, l, mid, r, cnt);
}