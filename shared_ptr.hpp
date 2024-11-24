#include <iostream>

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

} // namespace usu