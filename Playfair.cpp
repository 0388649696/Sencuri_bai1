#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

// Ký t? d?m
const char PADDING_CHAR = 'X';

// ======================================================
// 1. HÀM H? TR? PLAYFAIR
// ======================================================

/**
 * L?c và làm s?ch chu?i: Lo?i b? ký t? không ph?i ch? cái, chuy?n hoa, thay J b?ng I.
 */
string filterText(string text) {
    string clean = "";
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    for (char c : text) {
        if (isalpha(c)) {
            clean += (c == 'J' ? 'I' : c); // Quy u?c I = J
        }
    }
    return clean;
}

/**
 * T?o ma tr?n khóa 5x5 t? khóa.
 * Ð?ng th?i di?n vào map 'pos' v? trí (hàng, c?t) c?a t?ng ký t?.
 */
vector<vector<char>> generatePlayfairMatrix(const string& key, map<char, pair<int, int>>& pos) {
    string keyClean = filterText(key);
    string ALPHABET_25 = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // B? J
    vector<vector<char>> matrix(5, vector<char>(5));
    set<char> seen;
    string chars;

    // 1. Thêm ký t? t? khóa
    for (char c : keyClean) {
        if (seen.find(c) == seen.end()) {
            seen.insert(c);
            chars += c;
        }
    }

    // 2. Thêm ký t? còn l?i t? b?ng ch? cái
    for (char c : ALPHABET_25) {
        if (seen.find(c) == seen.end()) {
            chars += c;
        }
    }

    // 3. Xây d?ng ma tr?n và map v? trí
    for (int i = 0; i < 25; i++) {
        int r = i / 5;
        int c = i % 5;
        matrix[r][c] = chars[i];
        pos[chars[i]] = {r, c};
    }

    return matrix;
}

/**
 * Ti?n x? lý b?n rõ: Chèn 'X' cho c?p trùng nhau, thêm 'X' n?u s? lu?ng l?.
 */
string preprocessPlayfairText(string text) {
    string cleanText = filterText(text);
    string processedText = "";
    
    for (size_t i = 0; i < cleanText.length(); i++) {
        processedText += cleanText[i];
        
        // N?u có ch? cái ti?p theo
        if (i + 1 < cleanText.length()) {
            // N?u hai ch? cái trùng nhau, chèn X và không tang i
            if (cleanText[i] == cleanText[i + 1]) {
                processedText += PADDING_CHAR;
            } else {
                // N?u khác nhau, thêm ch? cái ti?p theo và tang i
                processedText += cleanText[i + 1];
                i++; 
            }
        }
    }

    // N?u s? ch? cái l?, thêm X ? cu?i
    if (processedText.length() % 2 != 0) {
        processedText += PADDING_CHAR;
    }
    return processedText;
}

// ======================================================
// 2. MÃ HÓA/GI?I MÃ PLAYFAIR
// ======================================================

string playfairCipher(string text, string key, bool encrypt) {
    if (key.empty()) return "[LOI]: Khoa Playfair khong duoc de trong.";

    // 1. T?o ma tr?n và Ti?n x? lý b?n rõ
    map<char, pair<int, int>> pos;
    vector<vector<char>> matrix = generatePlayfairMatrix(key, pos);
    string processedText = preprocessPlayfairText(text);
    string result = "";
    
    if (processedText.empty()) return "";

    // D?ch chuy?n: +1 cho mã hóa, -1 (t?c là +4 mod 5) cho gi?i mã
    const int shift = encrypt ? 1 : 4; 

    for (size_t i = 0; i < processedText.length(); i += 2) {
        char char1 = processedText[i];
        char char2 = processedText[i + 1];
        
        pair<int, int> p1 = pos.at(char1);
        pair<int, int> p2 = pos.at(char2);
        
        int r1 = p1.first, c1 = p1.second;
        int r2 = p2.first, c2 = p2.second;
        
        char res1, res2;

        // Quy t?c 1: Cùng hàng
        if (r1 == r2) {
            int new_c1 = (c1 + shift) % 5;
            int new_c2 = (c2 + shift) % 5;
            res1 = matrix[r1][new_c1];
            res2 = matrix[r2][new_c2];
            
        } 
        // Quy t?c 2: Cùng c?t
        else if (c1 == c2) {
            int new_r1 = (r1 + shift) % 5;
            int new_r2 = (r2 + shift) % 5;
            res1 = matrix[new_r1][c1];
            res2 = matrix[new_r2][c2];
            
        } 
        // Quy t?c 3: Hình ch? nh?t (Khác hàng và c?t)
        else {
            // Thay b?ng ch? cùng hàng, c?t c?a ch? kia (không ph? thu?c vào encrypt/decrypt)
            res1 = matrix[r1][c2];
            res2 = matrix[r2][c1];
        }
        
        result += res1;
        result += res2;
    }
    return result;
}

// ======================================================
// 3. HÀM MAIN TUONG TÁC
// ======================================================

int main() {
    string plaintext, ciphertext, key;

    cout << "=== MA HOA PLAYFAIR ===" << endl;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin, plaintext);

    cout << "Nhap khoa (chi la chu cai): ";
    getline(cin, key);

    // Mã hóa
    ciphertext = playfairCipher(plaintext, key, true);
    if (ciphertext.find("[LOI]") == string::npos) {
        cout << "Ban ma: " << ciphertext << endl;
    } else {
        cout << ciphertext << endl;
    }


    cout << "\n=== GIAI MA PLAYFAIR ===" << endl;
    
    // S? d?ng b?n mã v?a t?o (n?u h?p l?) ho?c nh?p l?i
    string decryptText = ciphertext;
    if (ciphertext.find("[LOI]") != string::npos) {
        cout << "Nhap chuoi ma hoa: ";
        getline(cin, decryptText);
    } else {
        cout << "Su dung ban ma vua tao: " << decryptText << endl;
    }
    
    cout << "Nhap khoa: ";
    string decryptKey;
    getline(cin, decryptKey);

    // Gi?i mã
    string decrypted = playfairCipher(decryptText, decryptKey, false);
    cout << "Ban ro: " << decrypted << " (Co the chua ky tu don 'X')" << endl;

    return 0;
}