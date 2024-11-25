#pragma once

#include <map>

class SingletonBase
{
    public:
        SingletonBase() = default;
        virtual ~SingletonBase() {};
};

class SingletonManager
{
    public:
        template<typename T>
        static std::shared_ptr<T> get()
        {
            static_assert(std::is_base_of<SingletonBase, T>::value, "T must inherit from SingletonBase");

            const char* typeName = typeid(T).name();
            const auto& pos = s_objects.find(typeName);
            if(pos != s_objects.end())
            {
                return dynamic_pointer_cast<T>(pos->second);
            }
            std::shared_ptr<T> instance = std::make_shared<T>();
            s_objects[typeName] = instance;
            return instance;
        }

    private:
        SingletonManager() {}

        ~SingletonManager() {}

        static std::map<const char*, std::shared_ptr<SingletonBase>> s_objects;
};
