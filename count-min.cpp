#include <iostream>
#include <functional>
#include <vector>
#include <numeric>

using namespace std;

// Use the count-min sketch algorithm
function<int(string)> createHashFunction(int d, int size) {
  return [size, d](string in) {
      unsigned long int sum = 0;
      for (auto it = in.begin(); it != in.end(); it++) {
         sum += *it;
      }
      return  (sum * d) % size;
  };
}

int main() {

    // Epsilon: ~0.24
    // Delta: ~0.25
    int d = 10;
    int w = 50;

    vector<function<int(string)>> vh(d);
    vector<vector<pair<string, int>>>
                        vd(d, vector<pair<string,int>>(w, make_pair("", 0)));
    for (auto i = 0; i < d; i++) {
        vh[i] = createHashFunction(i, d);
    }

    // Read from stdin
    string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        auto delim = line.find(" ");
        auto ip = line.substr(0, delim - 1);
        auto bytes = line.substr(delim + 1, line.length() - delim + 1);
        for (auto i = 0; i < d; i++) {
            auto vi = vd[i];
            auto hi = vh[i](ip);
            vi[hi] = make_pair(ip, vi[hi].second + atoi(bytes.c_str()));
        }
//        std::cout << line << std::endl;
//        std::cout << ip << std::endl;
//        std::cout << bytes << std::endl;

    }
    return 0;
}
