# Dự án làm game bằng SDL2
Game được dựa trên Diamond Rush của Gameloft và được phát triển bằng C++ và SDL2.
> *Trần Quang Tùng - 24022756 - K69I-IS3*

![](https://media.discordapp.net/attachments/929223503704621056/1368665609554104431/Screenshot_20250505_020150.png?ex=68190cad&is=6817bb2d&hm=e89e5d8f6dc96a61bf005e5886cdd5481fa5be7687fc5a567af465d1d14c90a2&=&format=webp&quality=lossless)

*Cách cài đặt và biên dịch [tại đây](INSTALL.md) và danh sách đầy đủ các tính năng [tại đây](FEATURES.md).*

## Tài nguyên tham khảo
- Hướng dẫn làm game bằng SDL của [LazyFoo' Productions](https://lazyfoo.net/tutorials/SDL/index.php)
- Hướng dẫn lập trình game bằng [SDL2 của Let's Make Games](https://www.youtube.com/playlist?list=PL2RPjWnJduNmXHRYwdtublIPdlqocBoLS)
- [ChatGPT](https://chat.openai.com/) - Hỗ trợ về thuật toán và quản lý dự án

## Nguồn đồ họa và âm thanh
- [Hình ảnh game](https://www.vg-resource.com/thread-36977.html) - Trích xuất trực tiếp từ game gốc.
- [Âm nhạc/âm thanh](https://pixabay.com) - Nhạc nền và hiệu ứng âm thanh.

## Cách chơi game
### Điều khiển
- Mũi tên lên/xuống/trái/phải: Di chuyển nhân vật

    ![](https://media.discordapp.net/attachments/929223503704621056/1368666027164172298/Screenshot_20250505_020949.png?ex=68190d11&is=6817bb91&hm=41dd20804c7c348a95684d3dfa542ae3b1d650865504ed8ea6c0684fc22186a6&=&format=webp&quality=lossless)
- R: Khởi động lại màn chơi

    ![](https://media.discordapp.net/attachments/929223503704621056/1368667312453980281/Screenshot_20250505_021258.png?ex=68190e43&is=6817bcc3&hm=584e5693b52fb543c42a97bb0f976f2ebfb47049c1df6e0c64a51f6fa5752d49&=&format=webp&quality=lossless)
- F3: Hiển thị màn hình debug
- ~~ESC: Tạm dừng game~~

### Mục tiêu
- Thu thập tất cả kim cương trong mỗi màn chơi
- Né tránh chướng ngại vật ~~và kẻ địch~~ để mở đường đi
- ~~Tìm chìa khóa để mở cửa~~
- Hoàn thành màn chơi

### Tùy chỉnh cấu hình (Config)
Hỗ trợ tùy chỉnh thông qua `config.cfg`, cho phép người dùng thay đổi các thông số mà không cần biên dịch lại:

> Lưu ý: *Các giá trị liên quan đến vật lý và độ trễ (tốc độ rơi, độ trễ đẩy,...) 
chưa được tối ưu trong hệ thống cấu hình. Thay đổi các giá trị này có thể
gây lỗi gameplay hoặc hiệu ứng không mong muốn.*

![](https://media.discordapp.net/attachments/1304753378550874152/1368845352257585214/image.png?ex=6819b413&is=68186293&hm=fab4e8d7a57d5972c61465bb63ac48adf5978a0e0658822f513a74b34dfc52ac&=&format=webp&quality=lossless)

### Màn hình Debug (F3)
Chế độ debug hiển thị thông tin kỹ thuật khi chơi game, bao gồm:
- FPS
- Vị trí người chơi trên lưới (x, y)
- Số lượng kim cương đã thu thập
- Số lá đã phá hủy
- Trạng thái người chơi
- Trạng thái âm thanh (bật/tắt)

![](https://media.discordapp.net/attachments/929223503704621056/1368665797722898632/Screenshot_20250505_020234.png?ex=68190cda&is=6817bb5a&hm=f51a9799ec312917b220a7cf036d8b7fef76fd8b6874ea717852f4b214e3851a&=&format=webp&quality=lossless)

---
*Disclaimer: Dự án được phát triển với mục đích học tập; không phân phối ,sử dụng cho mục đích thương mại. Diamond Rush là thương hiệu của Gameloft và dự án này không liên kết hoặc được xác nhận bởi Gameloft.*