#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

using namespace std;

// Ký tự độn chuẩn
const char PADDING_CHAR = 'X';

// ======================================================
// 1. HÀM HỖ TRỢ KHÓA
// ======================================================

/**
 * Lấy thứ tự hoán vị cột dựa trên chuỗi số khóa (ví dụ: "3142").
 * @param key Khóa hoán vị dưới dạng chuỗi số.
 * @returns Mảng thứ tự cột (0-indexed) để đọc/ghi, hoặc mảng rỗng nếu khóa không hợp lệ.
 */
vector<int> getNumericKeyOrder(const string& key) {
    string cleanKey = key;
    // Chỉ giữ lại chữ số
    cleanKey.erase(remove_if(cleanKey.begin(), cleanKey.end(), [](char c){ return !isdigit(c); }), cleanKey.end());
                   
    int keyLength = cleanKey.length();
    if (keyLength == 0) return {};
    
    vector<int> keyNumbers;
    for (char c : cleanKey) keyNumbers.push_back(c - '0');
    
    // Kiểm tra tính hợp lệ (phải là hoán vị của 1 đến keyLength)
    vector<int> sortedKey = keyNumbers;
    sort(sortedKey.begin(), sortedKey.end());
    for (int i = 0; i < keyLength; ++i) {
        // Khóa hợp lệ phải chứa đúng các số từ 1 đến keyLength
        if (sortedKey[i] != i + 1) return {}; 
    }

    // order[i] là index cột thực tế (0-indexed) cần đọc/ghi ở vị trí thứ (i+1).
    // Ví dụ: Khóa "3142" -> order: {2, 0, 3, 1} (Đọc Cột 2, rồi Cột 0, rồi Cột 3, rồi Cột 1)
    vector<int> order(keyLength);
    for (int i = 0; i < keyLength; ++i) {
        order[keyNumbers[i] - 1] = i; 
    }
    return order;
}

// ======================================================
// 2. MÃ HÓA HOÁN VỊ CỘT
// ======================================================

string encryptColumnar(string text, string key) {
    string cleanText = text;
    // Làm sạch và chuyển thành chữ hoa
    transform(cleanText.begin(), cleanText.end(), cleanText.begin(), ::toupper);
    cleanText.erase(remove_if(cleanText.begin(), cleanText.end(), [](char c){ return !isalpha(c); }), cleanText.end());

    vector<int> keyOrder = getNumericKeyOrder(key);
    if (keyOrder.empty()) {
        return "[LOI]: Khoa so khong hop le. Phai la hoan vi cua cac so tu 1 den N (vi du: '3142').";
    }

    int keyLength = keyOrder.size();
    int textLength = cleanText.length();
    if (textLength == 0) return "";
    int numRows = (int)ceil((double)textLength / keyLength);

    // 1. Điền ma trận theo hàng
    vector<vector<char>> matrix(numRows, vector<char>(keyLength));
    int charIndex = 0;

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < keyLength; ++c) {
            matrix[r][c] = (charIndex < textLength) ? cleanText[charIndex++] : PADDING_CHAR;
        }
    }

    // 2. Đọc theo thứ tự khóa
    string cipherText = "";
    for (int i = 0; i < keyLength; ++i) {
        int colIndex = keyOrder[i]; // Cột thực tế cần đọc
        for (int r = 0; r < numRows; ++r) {
            cipherText += matrix[r][colIndex];
        }
    }
    return cipherText;
}

// ======================================================
// 3. GIẢI MÃ HOÁN VỊ CỘT
// ======================================================

string decryptColumnar(string text, string key) {
    string cleanText = text;
    // Làm sạch và chuyển thành chữ hoa
    transform(cleanText.begin(), cleanText.end(), cleanText.begin(), ::toupper);
    cleanText.erase(remove_if(cleanText.begin(), cleanText.end(), [](char c){ return !isalpha(c); }), cleanText.end());

    vector<int> keyOrder = getNumericKeyOrder(key);
    if (keyOrder.empty()) {
        return "[LOI]: Khoa so khong hop le. Phai la hoan vi cua cac so tu 1 den N (vi du: '3142').";
    }

    int keyLength = keyOrder.size();
    int textLength = cleanText.length();
    if (textLength == 0) return "";
    int numRows = (int)ceil((double)textLength / keyLength);

    // Tính toán kích thước cột thực tế để điền
    int totalCells = keyLength * numRows;
    int numShortColumns = totalCells - textLength; // Số ô độn
    int numLongColumns = keyLength - numShortColumns; // Số cột có đủ numRows (tức là 3 hàng)

    // 1. Điền bản mã vào ma trận theo thứ tự khóa (ĐIỀN THEO CỘT)
    vector<vector<char>> matrix(numRows, vector<char>(keyLength, '\0')); // Dùng '\0' để đánh dấu ô trống
    int charIndex = 0;

    for (int i = 0; i < keyLength; ++i) {
        int columnIndexToWrite = keyOrder[i]; // Cột thực tế cần điền
        
        // Chiều dài cột thực tế: Long = numRows, Short = numRows-1
        int columnLength = (columnIndexToWrite < numLongColumns) ? numRows : numRows - 1;
        
        for (int r = 0; r < columnLength; ++r) {
            if (charIndex < textLength) {
                matrix[r][columnIndexToWrite] = cleanText[charIndex++];
            }
        }
    }

    // 2. Đọc ma trận theo hàng (thứ tự tự nhiên)
    string plainText = "";
    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < keyLength; ++c) {
             if (matrix[r][c] != '\0') { 
                plainText += matrix[r][c];
            }
        }
    }
    
    // 3. Loại bỏ ký tự độn ('X') cuối cùng
    while (plainText.length() > 0 && plainText.back() == PADDING_CHAR) {
        // Chỉ loại bỏ ký tự độn nếu nó là kết quả của việc điền ma trận.
        // Vì thuật toán điền ma trận luôn kết thúc bằng X (hoặc không điền gì), ta loại bỏ X cuối cùng.
        // Đây là suy luận vì không thể chắc chắn X cuối là độn hay ký tự gốc.
        plainText.pop_back();
    }
    
    return plainText;
}

// ======================================================
// 4. HÀM MAIN TƯƠNG TÁC
// ======================================================

int main() {
    string plaintext, ciphertext, key;

    cout << "=== MA HOA HOAN VI COT (KHOA SO) ===" << endl;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin, plaintext);

    cout << "Nhap khoa so (vi du: 3142): ";
    getline(cin, key);

    ciphertext = encryptColumnar(plaintext, key);
    cout << "Ban ma: " << ciphertext << endl;

    cout << "\n=== GIAI MA HOAN VI COT ===" << endl;
    // Chuyển kết quả mã hóa lên giải mã để kiểm tra tính nhất quán
    if (ciphertext.find("[LOI]") == string::npos) {
        cout << "Su dung ban ma vua tao: " << ciphertext << endl;
    } else {
        // Nếu có lỗi, yêu cầu nhập lại bản mã
        cout << "Nhap chuoi ma hoa: ";
        getline(cin, ciphertext);
    }
    
    cout << "Nhap khoa so: ";
    string decryptKey;
    getline(cin, decryptKey);

    string decrypted = decryptColumnar(ciphertext, decryptKey);
    cout << "Ban ro: " << decrypted << endl;

    return 0;
}