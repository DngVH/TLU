#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include <algorithm>
using namespace std;

class DoThi {
public:
    map<int, vector<int>> dSachKe;
    map<int, map<int, int>> chiPhi;

    void themCanh(int dDau, int dCuoi, int cost) {
        dSachKe[dDau].push_back(dCuoi);
        dSachKe[dCuoi].push_back(dDau);
        chiPhi[dDau][dCuoi] = cost;
        chiPhi[dCuoi][dDau] = cost;
    }

    vector<int> DSachKe1Dinh(int dinh) {
        return dSachKe[dinh];
    }

    int layChiPhi(int dDau, int dCuoi) {
        return chiPhi[dDau][dCuoi];
    }
};

// Hàm A* t?m ðý?ng ði t? ð?nh start ð?n ð?nh goal
int heuristic(int current, int goal) {
    return abs(current - goal);  // Hàm heuristic d?a trên kho?ng cách tuy?t ð?i
}

vector<int> timDuongDiAStar(DoThi& doThi, int batDau, int mucTieu) {
    map<int, int> luug;  // chi phí t? ði?m b?t ð?u ð?n ði?m hi?n t?i
    map<int, int> luuf;  // t?ng chi phí (g + h) cho m?i ði?m
    map<int, int> cameFrom;  // lýu l?i các ði?m trý?c ðó ð? xây d?ng ðý?ng ði

    // Priority queue v?i chi phí th?p nh?t ði ð?u
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> openSet;
    openSet.push({0, batDau});  // Ðýa ði?m b?t ð?u vào openSet
    luug[batDau] = 0;
    luuf[batDau] = heuristic(batDau, mucTieu);  // f = g + h

    // Ð?m b?o r?ng các ði?m chýa ðý?c thãm có giá tr? vô cùng (gi?i h?n)
    const int INF = 1e9;

    while (!openSet.empty()) {
        int current = openSet.top().second;
        openSet.pop();

        if (current == mucTieu) {
            vector<int> duongDi;
            while (cameFrom.find(current) != cameFrom.end()) {
                duongDi.push_back(current);
                current = cameFrom[current];
            }
            duongDi.push_back(batDau);
            reverse(duongDi.begin(), duongDi.end());
            return duongDi;  // Tr? v? ðý?ng ði t? batDau ð?n mucTieu
        }

        // L?p qua t?t c? các ði?m k? bên ði?m hi?n t?i
        for (int keBen : doThi.DSachKe1Dinh(current)) {
            int DiemCua_g_TamThoi = luug[current] + doThi.layChiPhi(current, keBen);

            // Ki?m tra n?u t?m ðý?c chi phí nh? hõn cho ði?m k? bên
            if (DiemCua_g_TamThoi < luug[keBen] || luug.find(keBen) == luug.end()) {
                cameFrom[keBen] = current;  // Lýu l?i ði?m ð?n
                luug[keBen] = DiemCua_g_TamThoi;  // C?p nh?t chi phí t? ði?m b?t ð?u
                luuf[keBen] = luug[keBen] + heuristic(keBen, mucTieu);  // C?p nh?t f = g + h
                openSet.push({luuf[keBen], keBen});  // Thêm ði?m k? vào openSet v?i f
            }
        }
    }

    return {};  // Không t?m th?y ðý?ng ði
}

bool minimax(DoThi& doThi, int hienTai, int mucTieu, map<int, bool>& daTham, vector<int>& duongDi, bool dgditoiuu) {
    if (hienTai == mucTieu) {
        duongDi.push_back(mucTieu);
        return true;
    }

    daTham[hienTai] = true;

    if (dgditoiuu) {
        int dinhTotNhat = -1;
        for (int keBen : doThi.DSachKe1Dinh(hienTai)) {
            if (!daTham[keBen]) {
                vector<int> duongDiPhu;
                if (minimax(doThi, keBen, mucTieu, daTham, duongDiPhu, false)) {
                    dinhTotNhat = keBen;
                    duongDi = duongDiPhu;
                    break;
                }
            }
        }
        if (dinhTotNhat != -1) {
            duongDi.insert(duongDi.begin(), hienTai);
            daTham[hienTai] = false;
            return true;
        }
    } else {
        for (int keBen : doThi.DSachKe1Dinh(hienTai)) {
            if (!daTham[keBen]) {
                vector<int> duongDiPhu;
                if (minimax(doThi, keBen, mucTieu, daTham, duongDiPhu, true)) {
                    duongDi = duongDiPhu;
                    duongDi.insert(duongDi.begin(), hienTai);
                    daTham[hienTai] = false;
                    return true;
                }
            }
        }
    }

    daTham[hienTai] = false;
    return false;
}

int tinhChiPhi(const vector<int>& duongDi, DoThi& doThi) {
    int chiPhiT = 0;
    for (size_t i = 1; i < duongDi.size(); ++i) {
        chiPhiT += doThi.layChiPhi(duongDi[i - 1], duongDi[i]);
    }
    return chiPhiT;
}

int main() {
    DoThi doThi;
    int soDinh, soCanh;
    cout << "Nhap so dinh: "; cin >> soDinh;
    cout << "Nhap so canh: "; cin >> soCanh;
    cout << "Nhap cac canh (dinh bat dau, dinh ket thuc, chi phi):\n";
    for (int i = 0; i < soCanh; ++i) {
        int diemDau, diemCuoi, chiPhiCanh;
        cin >> diemDau >> diemCuoi >> chiPhiCanh;
        doThi.themCanh(diemDau, diemCuoi, chiPhiCanh);
    }
    int batDau, mucTieu;
    cout << "Nhap dinh bat dau: "; cin >> batDau;
    cout << "Nhap dinh dich: "; cin >> mucTieu;

    // T?m ðý?ng ði A*
    vector<int> duongDiAStar = timDuongDiAStar(doThi, batDau, mucTieu);
    if (!duongDiAStar.empty()) {
        cout << "Duong di A* (duong di ngan nhat): ";
        for (int dinh : duongDiAStar) {
            cout << dinh << " ";
        }
        cout << endl;
        cout << "Chi phi duong di A*: " << tinhChiPhi(duongDiAStar, doThi) << endl;
    } else {
        cout << "Khong tim thay duong di A*!" << endl;
    }

    // T?m ðý?ng ði Minimax
    vector<int> duongDiMinimax;
    map<int, bool> daTham;
    bool timThay = minimax(doThi, batDau, mucTieu, daTham, duongDiMinimax, true);
    if (timThay) {
        cout << "Duong di Minimax (duong di toi uu): ";
        for (int dinh : duongDiMinimax) {
            cout << dinh << " ";
        }
        cout << endl;
        cout << "Chi phi duong di Minimax: " << tinhChiPhi(duongDiMinimax, doThi) << endl;
    } else {
        cout << "Khong tim thay duong di Minimax!" << endl;
    }

    return 0;
}

