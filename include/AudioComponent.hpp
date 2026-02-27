#pragma once

#include <vector>
#include <print>

#include "NodeConnector.hpp"


template <typename T>
class AudioComponent
{
public:
    size_t id;
    NodeConnector connector;

    AudioComponent() : id(0), connector(NodeConnector()) {};

    void process(const std::vector<float>& in, std::vector<float>& out)
    {
        static_cast<T*>(this)->processImpl(in,out);
    }

};

