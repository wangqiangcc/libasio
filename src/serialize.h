#pragma once

#include <cereal/archives/binary.hpp>

namespace serialize {

template <typename T>
std::string cereal_serialize(T&& t) {
    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(std::forward<T>(t));
    return ss.str();
}

template <typename T>
T cereal_deserialize(std::string s) {
    std::stringstream ss(s);
    cereal::BinaryInputArchive iarchive(ss);
    T d;
    iarchive(d);
    return d;
}

template <typename T>
std::shared_ptr<T> cereal_deserialize_ptr(std::string s) {
    std::stringstream ss(s);
    cereal::BinaryInputArchive iarchive(ss);
    std::shared_ptr<T> d;
    iarchive(d);
    return d;
}

} // end namespace serialize
