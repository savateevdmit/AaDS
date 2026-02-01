#ifndef EXACTCOUNTER_H
#define EXACTCOUNTER_H

#include <set>
#include <string>

class ExactCounter {
    std::set<std::string> unique_elements;

public:
    void add(const std::string& item) {
        unique_elements.insert(item);
    }

    size_t count() const {
        return unique_elements.size();
    }

    void reset() {
        unique_elements.clear();
    }
};

#endif
