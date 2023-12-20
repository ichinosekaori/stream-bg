#include <fstream>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "ppm.h"

using namespace std;

const int w = 1920, h = 1080;
const double k = -0.2;
const int v = 1, r = 40, db = 100;
const int dx = db / (1 + k * k);
const int col[2] = {0x59cbe8, 0xf5acb9};

mt19937 rng(1);
uniform_int_distribution<int> dist01(0, 1);
uniform_int_distribution<int> dist(0, dx - 1);

int sq(int x) { return x * x; }

image average(const image &im, int s) {
    int h = im.data.size(), w = im.data[0].size();
    image out(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int c = 0; c < 3; c++) {
                int sum = 0, cnt = 0;
                for (int a = i - s; a <= i + s; a++) {
                    if (a < 0 || a >= h) continue;
                    for (int b = j - s; b <= j + s; b++) {
                        if (b < 0 || b >= w) continue;
                        sum += im(a, b, c);
                        cnt++;
                    }
                }
                out(i, j, c) = sum / cnt;
            }
        }
    }
    return out;
}

int main() {
    image im(h, w);
    vector<pair<int, int>> bases;
    for (int b = 0, l = 0; b + k * w < h; b += db, l++) {
        bases.emplace_back(b, dist(rng));
    }
    for (int f = 0; f < dx; f++) {
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                for (int k = 0; k < 3; k++) im(i, j, k) = 0xef;
        for (int l = 0; l < (int)bases.size(); l++) {
            auto [b, x0] = bases[l];
            for (int x = (x0 + f * (l % 2 ? -1 : 1) + dx) % dx - dx; x < w + dx;
                 x += dx) {
                int y = x * k + b;
                int c = col[l % 2];
                for (int xx = x - r; xx <= x + r; xx++) {
                    if (xx < 0 || xx >= w) continue;
                    for (int yy = y - r; yy <= y + r; yy++) {
                        if (yy < 0 || yy >= h) continue;
                        if (sq(yy - y) + sq(xx - x) > sq(r)) continue;
                        for (int k = 0; k < 3; k++)
                            im(yy, xx, k) = c >> (16 - k * 8) & 255;
                    }
                }
            }
        }
        stringstream name_ss;
        name_ss << "circles-" << f << ".ppm";
        ofstream s(name_ss.str());
        s << average(im, 1);
    }
}
