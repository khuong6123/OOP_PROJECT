#ifndef COMMON_H
#define COMMON_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Common {
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    using String = std::string;
    template<typename T>
    using Vector = std::vector<T>;
    template<typename K, typename V>
    using HashMap = std::unordered_map<K, V>;
}

#endif // COMMON_H