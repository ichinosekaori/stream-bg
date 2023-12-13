#include <vector>
#include <array>
#include <iostream>

struct image {
    std::vector<std::vector<std::array<uint8_t, 3>>> data;
    image(int n, int m) {
        data.resize(n);
        for (int i = 0; i < n; i++) data[i].resize(m);
    }
    uint8_t &operator()(int i, int j, int c) { return data[i][j][c]; }
    uint8_t operator()(int i, int j, int c) const { return data[i][j][c]; }
};

std::ostream &operator<<(std::ostream &s, const image &im) {
    s << "P3\n" << im.data[0].size() << " " << im.data.size() << "\n255\n";
    for (int i = 0; i < (int)im.data.size(); i++) {
        for (int j = 0; j < (int)im.data[i].size(); j++) {
            for (int k = 0; k < 3; k++) {
                if (k) s << " ";
                s << (int)im(i, j, k);
            }
            s << "\n";
        }
    }
    return s;
}
