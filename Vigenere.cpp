#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

// Hàm chuẩn hóa và chỉ giữ lại chữ cái trong khóa (chuyển thành chữ hoa)
string cleanKey(string key) {
    string clean = "";
    for(char c : key) {
        if (isalpha(c)) {
            clean += toupper(c);
        }
    }
    return clean;
}

// Hàm mã hóa Vigenere
string encryptVigenere(string text, string key) {
    string cleanK = cleanKey(key);
    if (cleanK.empty()) return "[LOI]: Khoa khong hop le.";
    
    string result = "";
    int keyIndex = 0;
    const int M = 26;

    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int P = c - base;
            
            // Vị trí chữ cái khóa (K)
            int K = cleanK[keyIndex % cleanK.length()] - 'A'; 
            
            // Mã hóa: C = (P + K) mod 26
            int C = (P + K) % M;
            result += (char)(C + base);
            
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

// Hàm giải mã Vigenere
string decryptVigenere(string text, string key) {
    string cleanK = cleanKey(key);
    if (cleanK.empty()) return "[LOI]: Khoa khong hop le.";
    
    string result = "";
    int keyIndex = 0;
    const int M = 26;

    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int P = c - base; // P là vị trí bản mã (C)
            
            // Vị trí chữ cái khóa (K)
            int K = cleanK[keyIndex % cleanK.length()] - 'A'; 
            
            // Giải mã: P = (C - K) mod 26
            int shift = P - K;
            int C = (shift % M + M) % M; // Xử lý modulo cho số âm
            
            result += (char)(C + base);
            
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    string plaintext, ciphertext, key;

    cout << "=== MA HOA VIGENERE ===" << endl;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin, plaintext);

    cout << "Nhap khoa (chi la chu cai): ";
    getline(cin, key);

    ciphertext = encryptVigenere(plaintext, key);
    cout << "Ban ma: " << ciphertext << endl;

    cout << "\n=== GIAI MA VIGENERE ===" << endl;
    cout << "Nhap chuoi ma hoa: ";
    getline(cin, ciphertext);

    cout << "Nhap khoa: ";
    getline(cin, key);

    string decrypted = decryptVigenere(ciphertext, key);
    cout << "Ban ro: " << decrypted << endl;

    return 0;
}