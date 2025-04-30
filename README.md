# Dự án làm game bằng thu viện SDL2
Dự án game dựa trên tựa game Diamond Rush cổ điển của Nokia, được phát triển bằng C++ và SDL2.
*Trần Quang Tùng - 24022756*

## Nguồn tài nguyên tham khảo tham khảo
[Hướng dẫn làm game bằng SDL của LazyFoo' Productions](https://lazyfoo.net/tutorials/SDL/index.php)


## Hướng dẫn cài đặt và biên dịch
### Yêu cầu hệ thống
- Compiler C++ (g++ hoặc MinGW-w64)
- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer
### Biên dịch
#### Cho cả hai nền tảng:
```bash
make all
```
#### Cho riêng Linux:
```bash
make linux
```
#### Cho riêng Windows:
```bash
make windows
```
#### Dọn dẹp file biên dịch:
```bash
make clean
```

## Cách chơi game
### Điều khiển
- Mũi tên lên/xuống/trái/phải: Di chuyển nhân vật
- ESC: Tạm dừng game
- R: Khởi động lại màn chơi
### Mục tiêu
- Thu thập tất cả kim cương trong mỗi màn chơi
- Né tránh chướng ngại vật và kẻ địch
- Giải các câu đố để mở đường đi
- Tìm chìa khóa để mở cửa và hoàn thành màn chơi

## Checklist tính năng
### Tính năng cơ bản
- [ ] Hiển thị nhân vật và di chuyển
- [ ] Thu thập kim cương
- [ ] Hiển thị số kim cương đã thu thập
- [ ] Hiệu ứng âm thanh khi thu thập vật phẩm
- [ ] Bản đồ các màn chơi cơ bản
- [ ] Hệ thống va chạm với tường và chướng ngại vật
- [ ] Hệ thống hiển thị điểm số
### Giao diện
- [ ] Màn hình chính (Menu)
- [ ] Màn hình chọn màn chơi
- [ ] Màn hình tạm dừng (Pause)
- [ ] Màn hình kết thúc màn chơi
- [ ] Hướng dẫn chơi trong game
- [ ] Hiệu ứng chuyển cảnh
### Vật lý game
- [ ] Trọng lực (rơi từ trên cao xuống)
- [ ] Đẩy các khối đá
- [ ] Các bẫy (gai nhọn, hố, v.v.)
- [ ] Cơ chế dịch chuyển (teleport)
- [ ] Hệ thống nước và bơi
### Tính năng nâng cao
- [ ] Kẻ địch di chuyển
- [ ] Các vật phẩm đặc biệt (siêu năng lực, bất tử tạm thời)
- [ ] Các câu đố phức tạp (nút bấm, cần gạt, cơ chế thời gian)
- [ ] Lưu và tải game
- [ ] Bảng xếp hạng điểm cao
- [ ] Chế độ chơi thời gian
- [ ] Hỗ trợ điều khiển gamepad
- [ ] Cài đặt tùy chỉnh (âm lượng, độ phân giải)
- [ ] Hệ thống thành tựu
- [ ] Chế độ màn chơi tự tạo

## Ghi chú phát triển
Các tính năng sẽ được cập nhật dần dần trong quá trình làm việc.

---
*Lưu ý: Đây là dự án giáo dục không nhằm mục đích thương mại. Diamond Rush là thương hiệu của Gameloft và dự án này không liên kết hoặc được xác nhận bởi Gameloft.*