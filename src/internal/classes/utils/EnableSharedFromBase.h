#pragma once

#include <memory>

template<class T>
class enable_shared_from_base
    : public std::enable_shared_from_this<T>
{
    protected:
        template <class Derived>
        std::shared_ptr<Derived> shared_from_base()
        {
            return std::dynamic_pointer_cast<Derived>(this->shared_from_this());
        }
};
