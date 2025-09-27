// ======================================================
// PHẦN 1: CHỨC NĂNG CHUYỂN ĐỔI TAB (UI)
// ======================================================

document.addEventListener('DOMContentLoaded', () => {
    // 1. Lắng nghe sự kiện click trên các nút tab
    const algorithmBtns = document.querySelectorAll('.nut-btn');
    algorithmBtns.forEach(button => {
        button.addEventListener('click', () => {
            const algorithm = button.dataset.algorithm;
            setActiveAlgorithm(algorithm);
        });
    });
});

/**
 * Đặt thuật toán hoạt động (hiển thị panel nội dung và nút active)
 * @param {string} algorithmName - Tên thuật toán (caesar, affine, ...)
 */
function setActiveAlgorithm(algorithmName) {
    // Xóa active khỏi tất cả nút
    document.querySelectorAll('.nut-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    // Xóa active khỏi tất cả panel
    document.querySelectorAll('.cipher-panel').forEach(panel => {
        panel.classList.remove('active');
    });

    // Thêm active cho nút và panel tương ứng
    document.querySelector(`.nut-btn[data-algorithm="${algorithmName}"]`).classList.add('active');
    document.getElementById(algorithmName).classList.add('active');
}

// ======================================================
// ======================================================
// PHẦN 2: CÁC HÀM HỖ TRỢ CHUNG
// ======================================================

const ALPHABET_SIZE = 26;
const A_CODE = 'A'.charCodeAt(0);

/** * Lấy vị trí chữ cái (0-25), bỏ qua ký tự không phải chữ cái
 * @param {string} char 
 * @returns {number|null}
 */
function getCharIndex(char) {
    if (!char.match(/[a-z]/i)) return null;
    return char.toUpperCase().charCodeAt(0) - A_CODE;
}

/** * Tìm nghịch đảo modulo (dùng cho Affine)
 * Tìm x sao cho (a * x) % m == 1
 * @param {number} a
 * @param {number} m
 * @returns {number} x
 */
function modInverse(a, m) {
    a = a % m;
    for (let x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1; // Không có nghịch đảo
}

/**
 * Tạo ma trận khóa 5x5 từ khóa. (Quy ước I=J)
 * @param {string} key - Khóa đầu vào.
 * @returns {Array<Array<string>>} Ma trận 5x5.
 */
function generatePlayfairMatrix(key) {
    const ALPHABET_25 = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // Bỏ qua J
    let processedKey = key.toUpperCase().replace(/J/g, 'I').replace(/[^A-Z]/g, '');
    let matrix = [];
    let seen = new Set();
    let chars = [];

    // 1. Thêm các ký tự từ khóa vào
    for (const char of processedKey) {
        if (!seen.has(char)) {
            seen.add(char);
            chars.push(char);
        }
    }

    // 2. Thêm các ký tự còn lại từ ALPHABET_25
    for (const char of ALPHABET_25) {
        if (!seen.has(char)) {
            chars.push(char);
        }
    }

    // 3. Xây dựng ma trận 5x5
    for (let i = 0; i < 5; i++) {
        matrix.push(chars.slice(i * 5, (i + 1) * 5));
    }
    return matrix;
}

/**
 * Tiền xử lý bản rõ: gộp I/J, chèn 'X' cho cặp trùng, thêm 'X' nếu lẻ.
 * @param {string} text - Bản rõ.
 * @returns {string} Bản rõ đã được tiền xử lý (chỉ chữ cái).
 */
function preprocessPlayfairText(text) {
    let cleanText = text.toUpperCase().replace(/J/g, 'I').replace(/[^A-Z]/g, '');
    let processedText = '';
    
    for (let i = 0; i < cleanText.length; i++) {
        processedText += cleanText[i];
        
        // Nếu có chữ cái tiếp theo và chúng trùng nhau
        if (i + 1 < cleanText.length && cleanText[i] === cleanText[i + 1]) {
            processedText += 'X'; // Chèn chữ X vào giữa
        }
    }

    // Nếu số chữ cái lẻ, thêm X ở cuối
    if (processedText.length % 2 !== 0) {
        processedText += 'X';
    }
    return processedText;
}

/**
 * Lấy thứ tự hoán vị cột dựa trên chuỗi số khóa (ví dụ: "3142").
 * Khóa phải là hoán vị của các số từ 1 đến độ dài khóa.
 * @param {string} key - Khóa hoán vị dưới dạng chuỗi số (ví dụ: "3142").
 * @returns {Array<number>|null} Mảng thứ tự cột (0-indexed) để đọc/ghi, hoặc null nếu khóa không hợp lệ.
 */
function getNumericKeyOrder(key) {
    const cleanKey = key.replace(/[^0-9]/g, '');
    const keyLength = cleanKey.length;
    if (keyLength === 0) return null;

    const keyNumbers = Array.from(cleanKey).map(Number);
    const expectedNumbers = Array.from({length: keyLength}, (_, i) => i + 1);
    
    // Kiểm tra tính hợp lệ: Phải là hoán vị của 1 đến n, không trùng lặp
    if (keyNumbers.length !== keyLength || [...keyNumbers].sort((a, b) => a - b).join('') !== expectedNumbers.join('')) {
        return null; 
    }
    
    // order[i] sẽ chứa index cột thực tế (0-indexed) cần đọc/ghi ở vị trí thứ (i+1).
    // Ví dụ: Khóa "3142" -> order[0] = 2 (Cột 2 đọc đầu tiên), order[1] = 0 (Cột 0 đọc thứ hai), ...
    const order = Array(keyLength);
    keyNumbers.forEach((colValue, originalPosition) => {
        // colValue (1-based value) là thứ tự đọc. originalPosition (0-based index) là cột thực tế.
        order[colValue - 1] = originalPosition;
    });

    return order; 
}

// ======================================================
// PHẦN 3: LOGIC THUẬT TOÁN
// ======================================================

// --- A. Caesar Cipher ---
function caesarCipher(text, key, encrypt) {
    let result = '';
    
    // Khóa dịch chuyển (shift) không cần thiết phải tính trước
    // const shift = encrypt ? key : (ALPHABET_SIZE - key);

    for (let i = 0; i < text.length; i++) {
        let char = text[i];
        let P = getCharIndex(char); // Lấy vị trí 0-25

        if (P !== null) {
            let C;
            
            if (encrypt) {
                // Mã hóa: C = (P + k) mod 26
                C = (P + key) % ALPHABET_SIZE;
            } else {
                // Giải mã: P = (C - k) mod 26
                let shift = P - key;
                // Áp dụng phép toán modulo chuẩn cho số âm
                C = (shift % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
            }
            
            result += String.fromCharCode(C + A_CODE);
        } else {
            result += char; // Giữ nguyên ký tự không phải chữ cái
        }
    }
    return result.toUpperCase();
}

function caesarEncrypt() {
    const text = document.getElementById('caesar-text').value;
    const key = parseInt(document.getElementById('caesar-shift').value);
    document.getElementById('caesar-result').value = caesarCipher(text, key, true);
}

function caesarDecrypt() {
    const text = document.getElementById('caesar-text').value;
    const key = parseInt(document.getElementById('caesar-shift').value);
    document.getElementById('caesar-result').value = caesarCipher(text, key, false);
}

// --- B. Affine Cipher (Đã sửa: Tuân thủ M=26) ---
function gcd(a, b) {
    // Hàm tìm Ước chung lớn nhất (GCD) để kiểm tra tính hợp lệ của khóa a
    while (b) {
        [a, b] = [b, a % b];
    }
    return a;
}

function affineCipher(text, a, b, encrypt) {
    let result = '';
    const textUpper = text.toUpperCase();
    const M_SIZE = ALPHABET_SIZE; // M=26

    // 1. Kiểm tra tính hợp lệ của Khóa A theo M=26
    if (gcd(a, M_SIZE) !== 1) {
        return `[LỖI]: Khóa a (${a}) không hợp lệ. Khóa a và 26 phải nguyên tố cùng nhau (GCD(a, 26) = 1).`;
    }

    // 2. Tìm nghịch đảo cho giải mã
    const aInv = modInverse(a, M_SIZE);

    for (let i = 0; i < textUpper.length; i++) {
        let char = textUpper[i];
        let P = getCharIndex(char);

        if (P !== null) {
            let C;
            if (encrypt) {
                // Mã hóa: C = (aP + b) mod 26
                C = (a * P + b) % M_SIZE;
            } else {
                // Giải mã: P = a_inv * (C - b) mod 26
                let C_shifted = P - b;
                // Đảm bảo kết quả dương trước khi nhân
                C_shifted = (C_shifted % M_SIZE + M_SIZE) % M_SIZE; 
                C = (aInv * C_shifted) % M_SIZE;
            }
            result += String.fromCharCode(C + A_CODE);
        } else {
            result += char;
        }
    }
    return result;
}

function affineEncrypt() {
    const text = document.getElementById('affine-text').value;
    const a = parseInt(document.getElementById('affine-a').value);
    const b = parseInt(document.getElementById('affine-b').value);
    document.getElementById('affine-result').value = affineCipher(text, a, b, true);
}

function affineDecrypt() {
    const text = document.getElementById('affine-text').value;
    const a = parseInt(document.getElementById('affine-a').value);
    const b = parseInt(document.getElementById('affine-b').value);
    document.getElementById('affine-result').value = affineCipher(text, a, b, false);
}
function vigenereEncrypt() {
    const text = document.getElementById('vigenere-text').value;
    const key = document.getElementById('vigenere-key').value;
    document.getElementById('vigenere-result').value = vigenereCipher(text, key, true);
}

function vigenereDecrypt() {
    const text = document.getElementById('vigenere-text').value;
    const key = document.getElementById('vigenere-key').value;
    document.getElementById('vigenere-result').value = vigenereCipher(text, key, false);
}

// --- C. Vigenère Cipher (Đã sửa) ---
function vigenereCipher(text, key, encrypt) {
    let result = '';
    const textUpper = text.toUpperCase();
    
    // Tiền xử lý: Chỉ giữ lại chữ cái trong khóa và chuyển thành chữ hoa
    const keyUpper = key.toUpperCase().replace(/[^A-Z]/g, ''); 

    if (keyUpper.length === 0) return "[LỖI]: Khóa không được để trống hoặc chỉ chứa ký tự không phải chữ cái.";

    let keyIndex = 0;
    for (let i = 0; i < textUpper.length; i++) {
        let char = textUpper[i];
        let P = getCharIndex(char); // Lấy vị trí bản rõ (hoặc bản mã khi giải mã)

        if (P !== null) {
            // Lấy vị trí chữ cái trong khóa (K)
            let K = getCharIndex(keyUpper[keyIndex % keyUpper.length]); 
            let C;

            if (encrypt) {
                // Mã hóa: C = (P + K) mod 26
                C = (P + K) % ALPHABET_SIZE;
            } else {
                // Giải mã: P = (C - K) mod 26
                let shift = (P - K);
                // Xử lý modulo cho số âm: (C - K) mod 26
                C = (shift % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
            }
            
            result += String.fromCharCode(C + A_CODE);
            keyIndex++; // Tăng chỉ số khóa CHỈ KHI ký tự được mã hóa/giải mã
        } else {
            result += char; // Giữ nguyên ký tự không phải chữ cái
        }
    }
    return result;
}

function vigenereEncrypt() {
    const text = document.getElementById('vigenere-text').value;
    const key = document.getElementById('vigenere-key').value;
    document.getElementById('vigenere-result').value = vigenereCipher(text, key, true);
}

function vigenereDecrypt() {
    const text = document.getElementById('vigenere-text').value;
    const key = document.getElementById('vigenere-key').value;
    document.getElementById('vigenere-result').value = vigenereCipher(text, key, false);
}

// --- D. Playfair Cipher (Chỉ logic cốt lõi) ---
// Hàm tìm vị trí (hàng, cột) của một ký tự trong ma trận
function findPosition(matrix, char) {
    for (let row = 0; row < 5; row++) {
        for (let col = 0; col < 5; col++) {
            if (matrix[row][col] === char) {
                return [row, col];
            }
        }
    }
    return null; 
}

function playfairCipher(text, key, encrypt) {
    if (!key) return "[LỖI]: Khóa Playfair không được để trống.";
    
    // 1. Tạo ma trận và Tiền xử lý bản rõ
    const matrix = generatePlayfairMatrix(key);
    const processedText = preprocessPlayfairText(text);
    let result = '';
    
    // Tùy chỉnh bước dịch chuyển (Shift)
    // Mã hóa: dịch chuyển +1 (bên phải/dưới)
    // Giải mã: dịch chuyển -1 (bên trái/trên)
    const shift = encrypt ? 1 : 4; // shift=1 cho mã hóa, shift=4 (-1 mod 5) cho giải mã

    for (let i = 0; i < processedText.length; i += 2) {
        const char1 = processedText[i];
        const char2 = processedText[i + 1];
        
        const [r1, c1] = findPosition(matrix, char1);
        const [r2, c2] = findPosition(matrix, char2);

        // 2. Áp dụng 3 quy tắc mã hóa/giải mã
        if (r1 === r2) {
            // Cùng hàng: Thay bằng chữ bên phải (hoặc trái nếu giải mã)
            const new_c1 = (c1 + shift) % 5;
            const new_c2 = (c2 + shift) % 5;
            result += matrix[r1][new_c1] + matrix[r2][new_c2];
            
        } else if (c1 === c2) {
            // Cùng cột: Thay bằng chữ phía dưới (hoặc trên nếu giải mã)
            const new_r1 = (r1 + shift) % 5;
            const new_r2 = (r2 + shift) % 5;
            result += matrix[new_r1][c1] + matrix[new_r2][c2];
            
        } else {
            // Hình chữ nhật: Thay bằng chữ cùng hàng, cột của chữ kia
            // (Quy tắc này không thay đổi cho mã hóa và giải mã)
            result += matrix[r1][c2] + matrix[r2][c1];
        }
    }
    return result;
}


function playfairEncrypt() {
    const text = document.getElementById('playfair-text').value;
    const key = document.getElementById('playfair-key').value;
    document.getElementById('playfair-result').value = playfairCipher(text, key, true);
    showPlayfairMatrix(key);
}

function playfairDecrypt() {
    const text = document.getElementById('playfair-text').value;
    const key = document.getElementById('playfair-key').value;
    document.getElementById('playfair-result').value = playfairCipher(text, key, false);
    showPlayfairMatrix(key);
}

// Cập nhật hàm hiển thị ma trận để dùng hàm generatePlayfairMatrix mới
function showPlayfairMatrix(key) {
    const matrix = generatePlayfairMatrix(key);
    let output = "Ma trận Playfair 5x5:\n";
    matrix.forEach(row => {
        output += row.join(' ') + '\n';
    });
    document.getElementById('playfair-matrix').innerText = output;
}

// --- E. Columnar Transposition (Hoán vị Cột) (Đã sửa cho Khóa Số) ---

function columnarCipher(text, key, encrypt) {
    if (!key) return "[LỖI]: Khóa không được để trống.";
    
    // 1. Chuẩn bị bản rõ và khóa
    const cleanText = text.toUpperCase().replace(/[^A-Z]/g, ''); // Chỉ giữ lại chữ cái
    
    // SỬA: Dùng hàm getNumericKeyOrder cho khóa số
    const keyOrder = getNumericKeyOrder(key); 
    
    if (!keyOrder) {
        // Thông báo lỗi nếu khóa không phải là hoán vị chuẩn
        return "[LỖI]: Khóa số không hợp lệ. Khóa phải là hoán vị của các số từ 1 đến độ dài khóa (ví dụ: '3142' cho khóa dài 4).";
    }
    
    const keyLength = keyOrder.length;
    if (keyLength === 0) return "[LỖI]: Khóa không hợp lệ.";

    const textLength = cleanText.length;
    const numRows = Math.ceil(textLength / keyLength);
    const PADDING_CHAR = 'X'; // Ký tự độn

    // 2. Mã hóa (Điền theo Hàng, Đọc theo Cột/Khóa)
    if (encrypt) {
        let matrix = [];
        let charIndex = 0;
        
        // Điền ma trận (theo hàng)
        for (let r = 0; r < numRows; r++) {
            let row = [];
            for (let c = 0; c < keyLength; c++) {
                // Thêm ký tự hoặc ký tự độn ('X')
                row.push(charIndex < textLength ? cleanText[charIndex++] : PADDING_CHAR);
            }
            matrix.push(row);
        }

        let cipherText = '';
        // Đọc theo cột theo thứ tự khóa
        for (let i = 0; i < keyLength; i++) {
            // keyOrder[i] là index cột gốc để đọc (0 đến keyLength-1)
            const columnIndexToRead = keyOrder[i]; 
            for (let r = 0; r < numRows; r++) {
                cipherText += matrix[r][columnIndexToRead];
            }
        }
        return cipherText;
        
    } else {
        // 3. Giải mã (Điền theo Cột/Khóa, Đọc theo Hàng)
        let matrix = Array(numRows).fill(0).map(() => Array(keyLength).fill(''));
        let charIndex = 0;

        // Tính toán kích thước cột thực tế để điền
        const totalCells = keyLength * numRows;
        const numShortColumns = totalCells - textLength; // Số ô độn
        const numLongColumns = keyLength - numShortColumns; // Số cột có đủ numRows

        // Điền bản mã vào ma trận theo thứ tự khóa (ĐIỀN THEO CỘT)
        for (let i = 0; i < keyLength; i++) {
            // keyOrder[i] là index cột thực tế cần điền (0 đến keyLength-1)
            const columnIndexToWrite = keyOrder[i]; 
            
            // Chiều dài cột thực tế (Long = numRows, Short = numRows-1)
            const columnLength = (columnIndexToWrite < numLongColumns) ? numRows : numRows - 1;
            
            for (let r = 0; r < columnLength; r++) {
                if (charIndex < textLength) {
                    matrix[r][columnIndexToWrite] = cleanText[charIndex++];
                }
            }
        }

        // Đọc ma trận theo hàng (thứ tự tự nhiên)
        let plainText = '';
        for (let r = 0; r < numRows; r++) {
            for (let c = 0; c < keyLength; c++) {
                 // Chỉ đọc các ô đã được điền
                 if (matrix[r][c] !== '') { 
                    plainText += matrix[r][c];
                }
            }
        }
        
        // Loại bỏ ký tự độn ('X') cuối cùng
        if (plainText.length > 0 && plainText[plainText.length - 1] === PADDING_CHAR) {
            // Logic loại bỏ ký tự độn: Thường chỉ loại bỏ X nếu nó là ký tự cuối cùng và được chèn vào
            if (numShortColumns > 0 || textLength % keyLength !== 0) {
                 plainText = plainText.substring(0, plainText.length - 1);
            }
        }
        
        return plainText;
    }
}

function columnarEncrypt() {
    const text = document.getElementById('columnar-text').value;
    const key = document.getElementById('columnar-key').value;
    document.getElementById('columnar-result').value = columnarCipher(text, key, true);
}

function columnarDecrypt() {
    const text = document.getElementById('columnar-text').value;
    const key = document.getElementById('columnar-key').value;
    document.getElementById('columnar-result').value = columnarCipher(text, key, false);
}