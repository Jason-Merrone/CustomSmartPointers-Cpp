#pragma once

#include <stdexcept>
#include <utility>

namespace usu
{
    template <typename T>
    class unique_ptr
    {
      public:
        // Constructors
        explicit unique_ptr(T* ptr = nullptr);
        unique_ptr(unique_ptr<T>&& otherUnique) noexcept;

        // Destructor
        ~unique_ptr();

        // Assignment Operators
        unique_ptr<T>& operator=(unique_ptr<T>&& otherUnique) noexcept;

        // Dereference Operators
        T& operator*();
        const T& operator*() const;

        // Arrow Operators
        T* operator->();
        const T* operator->() const;

        // Utility Functions
        T* get() const { return rawPointer; }
        T* release();
        void reset(T* ptr = nullptr);
        void swap(unique_ptr<T>& other) noexcept;

        // Comparison Operators
        bool operator==(const unique_ptr<T>& otherUnique) const;
        bool operator!=(const unique_ptr<T>& otherUnique) const;

      private:
        T* rawPointer;
    };

    // Constructor
    template <typename T>
    unique_ptr<T>::unique_ptr(T* ptr) :
        rawPointer(ptr)
    {
    }

    // Move Constructor
    template <typename T>
    unique_ptr<T>::unique_ptr(unique_ptr<T>&& otherUnique) noexcept :
        rawPointer(otherUnique.rawPointer)
    {
        otherUnique.rawPointer = nullptr;
    }

    // Destructor
    template <typename T>
    unique_ptr<T>::~unique_ptr()
    {
        delete rawPointer;
    }

    // Move Assignment Operator
    template <typename T>
    unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& otherUnique) noexcept
    {
        if (this != &otherUnique)
        {
            delete rawPointer;
            rawPointer = otherUnique.rawPointer;
            otherUnique.rawPointer = nullptr;
        }
        return *this;
    }

    // Dereference Operator
    template <typename T>
    T& unique_ptr<T>::operator*()
    {
        if (!rawPointer)
        {
            throw std::runtime_error("Attempting to dereference a null unique_ptr.");
        }
        return *rawPointer;
    }

    template <typename T>
    const T& unique_ptr<T>::operator*() const
    {
        if (!rawPointer)
        {
            throw std::runtime_error("Attempting to dereference a null unique_ptr.");
        }
        return *rawPointer;
    }

    // Arrow Operator
    template <typename T>
    T* unique_ptr<T>::operator->()
    {
        return rawPointer;
    }

    template <typename T>
    const T* unique_ptr<T>::operator->() const
    {
        return rawPointer;
    }

    // Release
    template <typename T>
    T* unique_ptr<T>::release()
    {
        T* temp = rawPointer;
        rawPointer = nullptr;
        return temp;
    }

    // Reset
    template <typename T>
    void unique_ptr<T>::reset(T* ptr)
    {
        if (rawPointer != ptr)
        {
            delete rawPointer;
            rawPointer = ptr;
        }
    }

    // Swap
    template <typename T>
    void unique_ptr<T>::swap(unique_ptr<T>& other) noexcept
    {
        std::swap(rawPointer, other.rawPointer);
    }

    // Comparison Operators
    template <typename T>
    bool unique_ptr<T>::operator==(const unique_ptr<T>& otherUnique) const
    {
        return this->get() == otherUnique.get();
    }

    template <typename T>
    bool unique_ptr<T>::operator!=(const unique_ptr<T>& otherUnique) const
    {
        return this->get() != otherUnique.get();
    }

    // make_unique for single objects
    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
} // namespace usu