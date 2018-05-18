
#pragma once

namespace Atmos
{
    template<class T>
    class Singleton
    {
    private:
        static T &instance;

        static void Instantiate(const T&) {}
        static T& Create()
        {
            static T t;
            Instantiate(instance);
            return t;
        }

        Singleton(const Singleton &arg) = delete;
        Singleton& operator=(const Singleton &arg) = delete;
    protected:
        Singleton() = default;
    public:
        static T& Instance()
        {
            return instance;
        }
    };

    template<class T>
    T& Singleton<T>::instance = Singleton<T>::Create();
}