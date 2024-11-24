#include "shared_ptr.hpp"
#include "unique_ptr.hpp"

#include <iostream>
#include <memory>
#include <string>
class MyClass
{
  public:
    MyClass() = default;
    MyClass(std::string name) :
        m_name(name)
    {
    }
    MyClass(std::string name, unsigned int rank) :
        m_name(name),
        m_rank(rank)
    {
    }
    MyClass(unsigned int rank, int) :
        // Not giving the second parameter a name to prevent a compiler warning
        m_rank(rank)
    {
    }
    void showMessage(std::string msg)
    {
        std::cout << msg << std::endl;
    }
    std::string m_name = "anonymous";
    unsigned int m_rank = 0;
};
void demoSharedPointer();
void demoUniquePointer();
// ------------------------------------------------------------------
//
// Shared memory assignment: usu::shared_ptr usu::unique_ptr
//
// ------------------------------------------------------------------
int main()
{
    demoSharedPointer();
    demoUniquePointer();
    return 0;
}
void demoSharedPointer()
{
    std::cout << std::endl
              << "--- Shared Pointers ---\n\n";
    std::shared_ptr<int> p1 = std::make_shared<int>();
    usu::shared_ptr<int> p2 = usu::make_shared<int>();
    usu::shared_ptr<std::string> p2a = usu::make_shared<std::string>();
    std::shared_ptr<int> p3 = std::make_shared<int>(10);
    usu::shared_ptr<int> p4 = usu::make_shared<int>(10);
    std::shared_ptr<std::string> p5 = std::make_shared<std::string>();
    usu::shared_ptr<std::string> p6 = usu::make_shared<std::string>();
    usu::shared_ptr<std::string> p6b = usu::make_shared<std::string>("Hi Mom!");
    std::shared_ptr<MyClass> p7 = std::make_shared<MyClass>();
    usu::shared_ptr<MyClass> p8 = usu::make_shared<MyClass>();
    std::shared_ptr<MyClass> p9 = std::make_shared<MyClass>("Test");
    usu::shared_ptr<MyClass> p10 = usu::make_shared<MyClass>("Test");
    std::shared_ptr<MyClass> p11 = std::make_shared<MyClass>("Test", 2);
    usu::shared_ptr<MyClass> p12 = usu::make_shared<MyClass>("Test", 2);
    std::shared_ptr<MyClass> p13 = std::make_shared<MyClass>(1, 2);
    usu::shared_ptr<MyClass> p14 = usu::make_shared<MyClass>(1, 2);
    std::cout << std::endl;
    std::cout << "--- Reference Counts ---" << std::endl;
    std::cout << "p13 : " << p13.use_count() << std::endl;
    std::cout << "P14 : " << p14.use_count() << std::endl;
    auto p13b = p13;
    auto p14b = p14;
    std::cout << "auto p13b = p13" << std::endl;
    std::cout << "auto p14b = p14" << std::endl;
    std::cout << "p13 : " << p13.use_count() << std::endl;
    std::cout << "P14 : " << p14.use_count() << std::endl;
    p13b = p11;
    p14b = p12;
    std::cout << "auto p13b = p11" << std::endl;
    std::cout << "auto p14b = p12" << std::endl;
    std::cout << "p11 : " << p11.use_count() << std::endl;
    std::cout << "P12 : " << p12.use_count() << std::endl;
    std::cout << "p13b : " << p13b.use_count() << std::endl;
    std::cout << "P14b : " << p14b.use_count() << std::endl;
    std::cout << "p13 : " << p13.use_count() << std::endl;
    std::cout << "P14 : " << p14.use_count() << std::endl;
    std::cout << std::endl;
    std::cout << "--- Member Access ---" << std::endl;
    p13->showMessage("from p13");
    p14->showMessage("from p14");
    (*p13).showMessage("from (*p13)");
    (*p14).showMessage("from (*p14)");
    MyClass* x = p13.get();
    x->showMessage("from p13.get()");
    MyClass* y = p14.get();
    y->showMessage("from p14.get()");
    usu::shared_ptr<MyClass[]> a1 = usu::make_shared_array<MyClass, 4>();
    for (unsigned int i = 0; i < a1.size(); i++)
    {
        a1[i].showMessage("this is a test");
    }
}
void demoUniquePointer()
{
    std::cout << std::endl
              << "--- Unique Pointers ---\n\n";
    std::unique_ptr<int> p1 = std::make_unique<int>(10);
    usu::unique_ptr<int> p2 = usu::make_unique<int>(10);
    // std::unique_ptr<int> p1b = p1; // Causes a compile error (and it should)
    // usu::unique_ptr<int> p2b = p2; // Causes a compile error (and it should)
    std::cout << "from *p1 : " << *p1 << std::endl;
    std::unique_ptr<int> p3;
    p3 = std::move(p1);
    // std::cout << *p1 << std::endl; // Causes a runtime error (and it should)
    std::cout << "from *p3 : " << *p3 << std::endl;
    std::cout << "from *p2 : " << *p2 << std::endl;
    usu::unique_ptr<int> p4;
    p4 = std::move(p2);
    // std::cout << *p2 << std::endl; // Causes a runtime error (and it should)
    std::cout << "from *p4 : " << *p4 << std::endl;
}