#pragma once
#include "layer.h"

_PHOXO_BEGIN
using std::shared_ptr;

class LayerManager final
{
private:
    std::vector<shared_ptr<Layer>>   m_layers;

public:
    bool inRange(int index) const
    {
        return (index >= 0) && (index < (int)m_layers.size());
    }

    void add(const shared_ptr<Layer>& layer, int index)
    {
        if (index == -1)
            index = (int)m_layers.size();

        if (index >= 0 && index <= (int)m_layers.size())
        {
            m_layers.insert(m_layers.begin() + index, layer);
        }
        else
        {
            assert(false);
        }
    }

    Layer* at(int index) const
    {
        if (!inRange(index))
        {
            assert(false);
            return nullptr;
        }

        return m_layers[index].get();
    }

    const auto& all() const
    {
        return m_layers;
    }
};

_PHOXO_NAMESPACE_END
