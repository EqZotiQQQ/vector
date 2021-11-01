#include "vector.h"

using std::cout;

using namespace no_std;

int main() {
    Vector<int> vec1;
    cout << &vec1 << '\n';
    Vector<int> vec2(vec1);
    cout << &vec1 << '\n';
    cout << &vec2 << '\n';
    Vector<int> vec3(std::move(vec1));
    cout << &vec1 << '\n';
    cout << &vec2 << '\n';
    cout << &vec3 << '\n';
    return 0;
}
