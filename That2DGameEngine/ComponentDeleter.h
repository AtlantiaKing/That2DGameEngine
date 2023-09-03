#pragma once
#include <iostream>

namespace that
{
    class Component;

    struct ComponentDeleter // deleter
    {
        void operator()(Component* p) const;
    };
}
