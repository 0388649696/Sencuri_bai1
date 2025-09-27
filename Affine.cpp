#include <iostream>
#include <string>
#include <cctype>
#include <numeric> // Dùng cho std::gcd
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

// Hàm tìm nghịch đảo modulo a^-1 sao cho (a * a_inv) % m == 1
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1; // Không có nghịch đảo
}

// Hàm mã hóa Affine
string encryptAffine(string text, int a, int b) {
    string result = "";
    const int M = 26;
    
    if (std::gcd(a, M) != 1) {
        return "[LOI]: Khoa 'a' va 26 phai nguyen to cung nhau (UCLN != 1).";
    }

    for (char c : text) {
        if (isupper(c)) {
            int P = c - 'A';
            int C = (a * P + b) % M;
            result += (char)(C + 'A');
        } else if (islower(c)) {
            int P = c - 'a';
            int C = (a * P + b) % M;
            result += (char)(C + 'a');
        } else {
            result += c;
        }
    }
    return result;
}

// Hàm giải mã Affine
string decryptAffine(string text, int a, int b) {
    string result = "";
    const int M = 26;
    
    if (std::gcd(a, M) != 1) {
        return "[LOI]: Khoa 'a' va 26 phai nguyen to cung nhau (UCLN != 1).";
    }
    
    int aInv = modInverse(a, M);
    if (aInv == -1) {
        return "[LOI]: Khong tim thay nghich dao modulo.";
    }

    for (char c : text) {
        if (isupper(c)) {
            int P = c - 'A'; // P là vị trí bản mã (C)
            int C_shifted = P - b;
            C_shifted = (C_shifted % M + M) % M; // Đảm bảo số dương
            int P_decrypted = (aInv * C_shifted) % M;
            result += (char)(P_decrypted + 'A');
        } else if (islower(c)) {
            int P = c - 'a';
            int C_shifted = P - b;
            C_shifted = (C_shifted % M + M) % M;
            int P_decrypted = (aInv * C_shifted) % M;
            result += (char)(P_decrypted + 'a');
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    string plaintext, ciphertext;
    int a, b;

    cout << "=== MA HOA AFFINE ===" << endl;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin, plaintext);

    cout << "Nhap khoa a (UCLN(a, 26)=1, vi du: 3, 5, 7, 9...): ";
    cin >> a;
    cout << "Nhap khoa b (0-25): ";
    cin >> b;

    ciphertext = encryptAffine(plaintext, a, b);
    cout << "Ban ma: " << ciphertext << endl;

    cin.ignore(); 

    cout << "\n=== GIAI MA AFFINE ===" << endl;
    cout << "Nhap chuoi ma hoa: ";
    getline(cin, ciphertext);

    cout << "Nhap khoa a: ";
    cin >> a;
    cout << "Nhap khoa b: ";
    cin >> b;

    string decrypted = decryptAffine(ciphertext, a, b);
    cout << "Ban ro: " << decrypted << endl;

    return 0;
}