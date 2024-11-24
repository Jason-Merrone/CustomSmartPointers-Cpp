#include <iostream>

namespace usu
{
    template <typename T>
    class unique_ptr
    {
      public:
        unique_ptr();
        explicit unique_ptr(T* ptr);
        unique_ptr(unique_ptr<T>&& otherShared);

        // Destructor
        ~unique_ptr();

        // Returns a pointer to the raw pointer
        const T* get() { return this->rawPointer; }

        // Releases the raw pointer and returns it
        T* release();

        unique_ptr<T>& operator=(unique_ptr<T>&& otherUnique);
        T* operator->() { return get(); }
        T operator*() { return *(get()); }
        bool operator==(const unique_ptr<T>& otherUnique);
        bool operator!=(const unique_ptr<T>& otherUnique);

      private:
        unsigned int* refCount;
        T* rawPointer;
    };

    // Default constructor
    template <typename T>
    unique_ptr<T>::unique_ptr()
    {
        rawPointer = nullptr;
    }

    template <typename T>
    unique_ptr<T>::unique_ptr(T* ptr)
    {
        rawPointer = ptr;
    }

    // Move constructor
    template <typename T>
    unique_ptr<T>::unique_ptr(unique_ptr<T>&& otherUnique)
    {
        rawPointer = otherUnique.rawPointer;
        otherUnique.rawPointer = nullptr;
    }

    // Destructor
    template <typename T>
    unique_ptr<T>::~unique_ptr()
    {
        delete rawPointer;
    }

    // Move operator
    template <typename T>
    unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& otherUnique)
    {
        if (this != &otherUnique)
        {
            delete rawPointer;                   // Delete the currently managed object
            rawPointer = otherUnique.rawPointer; // Transfer ownership
            otherUnique.rawPointer = nullptr;    // Nullify the source
        }
        return *this;
    }

    // Make unique function
    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    // Checks if both unique pointers contain the same raw pointer
    template <typename T>
    bool unique_ptr<T>::operator==(const unique_ptr<T>& otherUnique)
    {
        if (otherUnique->rawPointer == rawPointer)
        {
            return true;
        }
        return false;
    }

    // Checks if both unique pointers contain different raw pointer
    template <typename T>
    bool unique_ptr<T>::operator!=(const unique_ptr<T>& otherUnique)
    {
        if (otherUnique->rawPointer != rawPointer)
        {
            return true;
        }
        return false;
    }

    template <typename T>
    T* unique_ptr<T>::release()
    {
        T* temp = rawPointer;
        rawPointer = nullptr;
        return temp;
    }

} // namespace usu