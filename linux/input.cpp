#include <iostream>

using namespace std;

int main (int argc, char* argv[]) {
    cout << "参数个数： " << argc << endl;
    for (int i = 0; i < argc; ++i) {
        cout << "参数" << i << ": " << argv[i] << " " << endl;
    }
    return 0;
}
