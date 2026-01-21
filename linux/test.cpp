#include <iostream>
#include <vector>
#include <numeric>
#include <string>

using namespace std;

int main() {
    vector<int> res(5,0);
    for (int i = 0; i < res.size(); ++i) {
        res[i] = i;
    }
    int sumNum = accumulate(res.begin(), res.end(), 0);
    string cOut = "Hello gbd";
    cout << "========================" << endl;
    cout << cOut << endl;
    cout << sumNum << endl;
    cout << "========================" << endl;
    return 0;
}
