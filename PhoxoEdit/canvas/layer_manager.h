#pragma once
#include "layer.h"

_PHOXO_BEGIN

class LayerManager
{
private:
    std::deque<shared_ptr<Layer>>   m_layers;

public:
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
        try
        {
            return m_layers.at(index).get();
        }
        catch (const std::out_of_range&) { assert(false); }
        return nullptr;
    }

    const auto& all() const
    {
        return m_layers;
    }
};

_PHOXO_NAMESPACE_END
