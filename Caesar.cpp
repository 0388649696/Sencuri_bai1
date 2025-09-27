#include <iostream>
#include <string>
using namespace std;

// Hàm mã hóa Caesar
string encryptCaesar(string text, int key) {
    string result = "";
    for (char c : text) {
        if (isupper(c)) {
            result += char((c - 'A' + key) % 26 + 'A');
        } else if (islower(c)) {
            result += char((c - 'a' + key) % 26 + 'a');
        } else {
            result += c; // giữ nguyên ký tự không phải chữ cái
        }
    }
    return result;
}

// Hàm giải mã Caesar
string decryptCaesar(string text, int key) {
    string result = "";
    for (char c : text) {
        if (isupper(c)) {
            result += char((c - 'A' - key + 26) % 26 + 'A');
        } else if (islower(c)) {
            result += char((c - 'a' - key + 26) % 26 + 'a');
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    string plaintext, ciphertext;
    int key;

    cout << "=== MA HOA CAESAR ===" << endl;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin, plaintext);

    cout << "Nhap khoa (0-25): ";
    cin >> key;

    ciphertext = encryptCaesar(plaintext, key);
    cout << "Ban ma: " << ciphertext << endl;

    cin.ignore(); // xóa buffer trước khi nhập tiếp

    cout << "\n=== GIAI MA CAESAR ===" << endl;
    cout << "Nhap chuoi ma hoa: ";
    getline(cin, ciphertext);

    cout << "Nhap khoa (0-25): ";
    cin >> key;

    string decrypted = decryptCaesar(ciphertext, key);
    cout << "Ban ro: " << decrypted << endl;

    return 0;
}