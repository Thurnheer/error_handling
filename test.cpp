#include "Expected.hpp"
#include "Pipe.hpp"
#include <iostream>



Expected<int> f(int a) {
    std::cout << "running f\n";
    return a * a;
}

Expected<int> d(int a) {
    std::cout << "running d\n";
    return a + a;
}


Expected<int> e(int a, int b) {
    std::cout << "running e\n";
    return {};
    //return a + b;
}



auto myFunc = [](int a) -> Expected<int> {
    std::cout << "runing myFunc\n";
    return a + a;
};


int main() {

    Expected<int> result = Expected<int>(5) | f | d;
    Expected<int> res = (f(3), d(5)) | e | myFunc;
    Expected<int> r = (f(3), d(5)) | e | [](int a){
        std::cout << "running lambda\n";
        return e(a, a);
    };
    //std::cout << "r: " << r.value() << "\n";

    std::cout << result.value() << "\n";
    return 0;
}
