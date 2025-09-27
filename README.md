# Sencuri_bai1
### TÌM HIỂU CÁC PHƯƠNG PHÁP MÃ HOÁ CỔ ĐIỂN
Caesar
Affine
Hoán vị
Vigenère
Playfair

### Với mỗi phương pháp, hãy tìm hiểu:
Tên gọi
Thuật toán mã hoá, thuật toán giải mã
Không gian khóa
Cách phá mã (mà không cần khoá)
Cài đặt thuật toán mã hoá và giải mã bằng code C++ và bằng html+css+javascript

============================
###  Mật mã Caesar.
Nguyên lý: Mỗi chữ cái trong bản rõ sẽ được dịchh chuyển đi một số bước nhất định trong bảng chữ cái.
Mã hóa: Lấy vị trí chữ cái trong bản rõ "cộng" với khóa rồi lấy phần dư chia cho 26 để ra chữ cái trong bản mã.
Giải mã: Lấy vị trí chữ cái trong bản mã trừ đi khóa, cộng thêm 26 nếu âm, rồi chia dư cho 26 để ra chữ cái gốc.
Không gian khóa: 26 (từ 0 đến 25).
Cách phá mã: Thử tất cả 26 khóa (brute force).

#### Hình ảnh kiểm thử
#### Mã hóa:
<img width="1080" height="960" alt="image" src="https://github.com/user-attachments/assets/ed3df442-b41a-4ab7-885f-8d324a9cb0b0" />
#### Giải mã:
<img width="915" height="930" alt="image" src="https://github.com/user-attachments/assets/630204f5-c029-4ef6-8cb9-bf4ca769a8a2" />

### Mật mã Affine.
Nguyên lý: Mỗi chữ cái trong bản rõ được biến đổi tuyến tính bằng hai tham số khóa là a và b.
Mã hóa: Nhân vị trí chữ cái trong bản rõ với a rồi cộng b, sau đó lấy phần dư chia 26 để ra chữ cái trong bản mã.
Giải mã: Lấy vị trí chữ cái trong bản mã trừ b, sau đó nhân với nghịch đảo của a (theo modulo 26), rồi lấy phần dư chia 26 để ra chữ cái gốc.
Không gian khóa: 312 (có 12 giá trị hợp lệ của a nhân với 26 giá trị của b).
Cách phá mã: Dùng phân tích tần suất hoặc thử brute force toàn bộ 312 khóa.

#### Hình ảnh kiểm thử
#### Mã hóa:
 <img width="925" height="891" alt="image" src="https://github.com/user-attachments/assets/2751dd0a-1417-44ea-8410-ae89cf0b449f" />
#### Giải mã:
<img width="918" height="866" alt="image" src="https://github.com/user-attachments/assets/8196b7bd-eb81-41d3-97c7-5b6a4991e8e0" />

### Mật mã Vigenère.
Nguyên lý: Dùng một chuỗi khóa lặp lại để dịch chuyển từng chữ cái trong bản rõ.
Mã hóa: Lấy vị trí chữ cái bản rõ cộng với vị trí chữ cái trong khóa, rồi lấy phần dư chia 26 để ra chữ cái bản mã.
Giải mã: Lấy vị trí chữ cái bản mã trừ đi vị trí chữ cái trong khóa, cộng thêm 26 nếu âm, rồi lấy phần dư chia 26 để ra chữ cái gốc.
Không gian khóa: 26 mũ độ dài khóa.
Cách phá mã: Dùng phương pháp Kasiski hoặc Friedman để đoán độ dài khóa, sau đó tách thành nhiều Caesar để phá.

#### Hình ảnh kiểm thử
#### Mã hóa:
 <img width="923" height="854" alt="image" src="https://github.com/user-attachments/assets/eda15454-b51c-40e8-ad2e-cbe39aeb6f03" />
#### Giải mã:
<img width="940" height="851" alt="image" src="https://github.com/user-attachments/assets/96b74b13-b485-421e-9c01-2e7bf82f8672" />

### Mật mã Playfair.
Nguyên lý: Mã hóa theo cặp chữ cái bằng bảng 5x5 sinh từ khóa (gộp I và J).
Nếu hai chữ cái cùng hàng → thay mỗi chữ bằng chữ bên phải nó.
Nếu hai chữ cái cùng cột → thay mỗi chữ bằng chữ phía dưới nó.
Nếu khác hàng và cột → thay mỗi chữ bằng chữ cùng hàng nhưng ở cột của chữ kia (hình chữ nhật).
Khi chuẩn bị bản rõ: nếu hai chữ cái trong một cặp trùng nhau thì chèn chữ X vào giữa, nếu số chữ lẻ thì thêm X ở cuối.
Không gian khóa: xấp xỉ 25! (rất lớn).
Cách phá mã: Dùng phân tích tần suất cặp chữ cái (digraph frequency analysis).

#### Hình ảnh kiểm thử
#### Mã hóa:
 <img width="1061" height="895" alt="_C__Users_Dell_Desktop_Cong20thong%20tin_Mon_Sencurity_index html" src="https://github.com/user-attachments/assets/e2af9a44-652f-4087-8cee-6418fe538a3b" />
#### Giải mã:
<img width="1114" height="880" alt="_C__Users_Dell_Desktop_Cong20thong%20tin_Mon_Sencurity_indexhtml" src="https://github.com/user-attachments/assets/dc6384f0-ca7d-4ec9-936b-42c14d7e757e" />

### Mật mã hoán vị.
Nguyên lý: Không thay đổi chữ cái, chỉ thay đổi vị trí của chúng theo một khóa là một hoán vị.
Mã hóa: Sắp xếp lại các chữ cái theo thứ tự được chỉ định bởi khóa.
Giải mã: Sắp xếp ngược lại theo hoán vị đảo.
Không gian khóa: n! (n giai thừa), với n là số vị trí cần hoán đổi. Ví dụ 3142 hợp lệ
Cách phá mã: Với n nhỏ thì có thể brute force, với n lớn thì phân tích mẫu từ và tần suất để suy ra.

#### Hình ảnh kiểm thử
#### Mã hóa:
 <img width="1088" height="881" alt="image" src="https://github.com/user-attachments/assets/a50ff826-bd37-420e-8a5b-a52a94466b00" />
#### Giải mã:
<img width="1046" height="903" alt="image" src="https://github.com/user-attachments/assets/af1025b9-3455-4e52-80f3-8295b34d692f" />
