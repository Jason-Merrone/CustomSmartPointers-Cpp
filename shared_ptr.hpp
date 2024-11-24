#pragma once
#include <iostream>

// Standard Shared Pointer
namespace usu
{
    template <typename T>
    class shared_ptr
    {
      public:
        explicit shared_ptr(T* ptr = nullptr);
        shared_ptr(shared_ptr<T>& otherShared);
        shared_ptr(shared_ptr<T>&& otherShared);

        // Destructor
        ~shared_ptr();

        // Returns a pointer to the raw pointer
        T* get() { return this->rawPointer; }
        // Returns the reference count
        unsigned int use_count() { return *(this->refCount); }

        shared_ptr<T>& operator=(const shared_ptr<T>& otherShared);
        T* operator->() { return get(); }
        T operator*() { return *(get()); }

      private:
        unsigned int* refCount;
        T* rawPointer;
    };

    template <typename T>
    shared_ptr<T>::shared_ptr(T* ptr) :
        refCount(nullptr), rawPointer(ptr)
    {
        rawPointer = ptr;
        refCount = new unsigned int(1);
    }

    // Copy constructor
    template <typename T>
    shared_ptr<T>::shared_ptr(shared_ptr<T>& otherShared)
    {
        refCount = otherShared.refCount;
        rawPointer = otherShared.rawPointer;
        if (refCount)
        {
            (*refCount)++;
        }
    }

    // Move constructor
    template <typename T>
    shared_ptr<T>::shared_ptr(shared_ptr<T>&& otherShared)
    {
        refCount = otherShared.refCount;
        rawPointer = otherShared.rawPointer;
        otherShared.rawPointer = nullptr;
        otherShared.refCount = nullptr;
    }

    // Destructor
    template <typename T>
    shared_ptr<T>::~shared_ptr()
    {
        if (refCount)
        {
            (*refCount)--;
            if (*refCount == 0)
            {
                delete rawPointer;
                delete refCount;
            }
        }
    }

    template <typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& otherShared)
    {
        // Avoid self-assignment
        if (this != &otherShared)
        {
            // Decrement the refCount of the current object
            if (refCount)
            {
                (*refCount)--;
                if (*refCount == 0)
                {
                    delete rawPointer;
                    delete refCount;
                }
            }
            rawPointer = otherShared.rawPointer;
            refCount = otherShared.refCount;

            if (refCount)
            {
                (*refCount)++;
            }
        }
        return *this;
    }

    template <typename T, typename... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        return shared_ptr<T>(new T(std::forward<Args>(args)...));
    }

    // Array shared pointer

    template <typename T>
    class shared_ptr<T[]>
    {
      public:
        explicit shared_ptr(T* ptr = nullptr, size_t size = 0);
        shared_ptr(const shared_ptr<T[]>& otherShared);
        shared_ptr(shared_ptr<T[]>&& otherShared) noexcept;

        // Destructor
        ~shared_ptr();

        shared_ptr<T[]>& operator=(const shared_ptr<T[]>& otherShared);
        shared_ptr<T[]>& operator=(shared_ptr<T[]>&& otherShared) noexcept;

        T& operator[](size_t index) const;

        size_t size() const { return this->arraySize; }

        unsigned int use_count() const { return (refCount) ? *refCount : 0; }

      private:
        unsigned int* refCount;
        T* rawPointer;
        size_t arraySize;
    };

    // Constructor
    template <typename T>
    shared_ptr<T[]>::shared_ptr(T* ptr, size_t size) :
        refCount(nullptr), rawPointer(ptr), arraySize(size)
    {
        if (ptr)
        {
            refCount = new unsigned int(1);
        }
    }

    // Copy constructor
    template <typename T>
    shared_ptr<T[]>::shared_ptr(const shared_ptr<T[]>& otherShared) :
        refCount(otherShared.refCount), rawPointer(otherShared.rawPointer), arraySize(otherShared.arraySize)
    {
        if (refCount)
        {
            (*refCount)++;
        }
    }

    // Move constructor
    template <typename T>
    shared_ptr<T[]>::shared_ptr(shared_ptr<T[]>&& otherShared) noexcept
        :
        refCount(otherShared.refCount),
        rawPointer(otherShared.rawPointer), arraySize(otherShared.arraySize)
    {
        otherShared.refCount = nullptr;
        otherShared.rawPointer = nullptr;
        otherShared.arraySize = 0;
    }

    // Destructor
    template <typename T>
    shared_ptr<T[]>::~shared_ptr()
    {
        if (refCount)
        {
            (*refCount)--;
            if (*refCount == 0)
            {
                delete[] rawPointer;
                delete refCount;
            }
        }
    }

    // Copy assignment operator
    template <typename T>
    shared_ptr<T[]>& shared_ptr<T[]>::operator=(const shared_ptr<T[]>& otherShared)
    {
        if (this != &otherShared)
        {
            // Decrement current object
            if (refCount)
            {
                (*refCount)--;
                if (*refCount == 0)
                {
                    delete[] rawPointer;
                    delete refCount;
                }
            }

            // Copy data from otherShared
            rawPointer = otherShared.rawPointer;
            refCount = otherShared.refCount;
            arraySize = otherShared.arraySize;

            // Increment the refCoun
            if (refCount)
            {
                (*refCount)++;
            }
        }
        return *this;
    }

    // Move assignment operator
    template <typename T>
    shared_ptr<T[]>& shared_ptr<T[]>::operator=(shared_ptr<T[]>&& otherShared) noexcept
    {
        if (this != &otherShared)
        {
            // Decrement current object's refCount
            if (refCount)
            {
                (*refCount)--;
                if (*refCount == 0)
                {
                    delete[] rawPointer;
                    delete refCount;
                }
            }

            // Transfer ownership from otherShared
            rawPointer = otherShared.rawPointer;
            refCount = otherShared.refCount;
            arraySize = otherShared.arraySize;

            otherShared.rawPointer = nullptr;
            otherShared.refCount = nullptr;
            otherShared.arraySize = 0;
        }
        return *this;
    }

    // Overloaded [] operator
    template <typename T>
    T& shared_ptr<T[]>::operator[](size_t index) const
    {
        if (!rawPointer)
        {
            throw std::runtime_error("Attempting to access elements of a null shared_ptr.");
        }
        if (index >= arraySize)
        {
            throw std::out_of_range("Index out of bounds.");
        }
        return rawPointer[index];
    }

    template <typename T, unsigned int N>
    shared_ptr<T[]> make_shared_array()
    {
        return shared_ptr<T[]>(new T[N], N);
    }

} // namespace usu