#include "tester.hpp"
#include "libftpp.hpp"

bool dataStructuresTest()
{
    bool success = true;

    Pool<int> intPool;
    intPool.resize(3);

    auto& obj1 = intPool.acquire();
    auto& obj2 = intPool.acquire();
    auto& obj3 = intPool.acquire();

    success &= (obj1.operator->() != obj2.operator->());
    success &= (obj2.operator->() != obj3.operator->());
    success &= (obj1.operator->() != obj3.operator->());

    bool exceptionThrown = false;
    try {
        auto& obj4 = intPool.acquire();
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    success &= exceptionThrown;

    *obj1 = 42;
    *obj2 = 100;
    *obj3 = -7;

    success &= (*obj1 == 42);
    success &= (*obj2 == 100);
    success &= (*obj3 == -7);

    DataBuffer buffer;

    int a = 123;
    double b = 3.14;
    char c = 'X';

    buffer << a << b << c;

    int a_read = 0;
    double b_read = 0.0;
    char c_read = 0;

    buffer >> a_read >> b_read >> c_read;

    success &= (a == a_read);
    success &= (b == b_read);
    success &= (c == c_read);

    bool underflowCaught = false;
    try {
        int x;
        buffer >> x;
    } catch (const std::out_of_range&) {
        underflowCaught = true;
    }
    success &= underflowCaught;

    Pool<DataBuffer> bufferPool;
    bufferPool.resize(2);

    auto& bufObj = bufferPool.acquire();
    *bufObj << 42 << 'A';
    int val;
    char ch;
    *bufObj >> val >> ch;

    success &= (val == 42 && ch == 'A');

    {
        auto& temp = bufferPool.acquire();
        *temp << 1;
    }

    try {
        auto& temp2 = bufferPool.acquire();
        success &= true;
    } catch (...) {
        success &= false;
    }

    return success;
}