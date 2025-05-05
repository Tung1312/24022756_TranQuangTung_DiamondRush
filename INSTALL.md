Hướng dẫn chi tiết để thiết lập và biên dịch trò chơi Diamond Rush trên cả nền tảng Linux và Windows.

### Yêu cầu hệ thống
- Trình biên dịch C++ (g++ hoặc MinGW-w64)
- Thư viện SDL2 (SDL2, SDL2_image, SDL2_ttf, SDL2_mixer)

##  Windows

### Cài đặt MinGW, SDL2 và các thư viện liên quan qua MSYS2

1. Tải và cài đặt [MSYS2](https://www.msys2.org/)
2. Mở MSYS2 MinGW 64-bit terminal và chạy các lệnh sau:

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-pkg-config
pacman -S mingw-w64-x86_64-SDL2
pacman -S mingw-w64-x86_64-SDL2_image
pacman -S mingw-w64-x86_64-SDL2_ttf
pacman -S mingw-w64-x86_64-SDL2_mixer
pacman -S mingw-w64-x86_64-make
```

3. Thêm đường dẫn `/C:/msys64/mingw64/bin/` vào biến môi trường PATH:
   - Chuột phải vào This PC/My Computer > Properties > Advanced system settings
   - Chọn Environment Variables
   - Trong System variables, tìm và chọn Path, nhấn Edit
   - Nhấn New và thêm `C:\msys64\mingw64\bin`
   - Nhấn OK để lưu thay đổi

### 3. Biên dịch dự án

1. Mở Command Prompt hoặc PowerShell
2. Di chuyển đến thư mục dự án: `cd "c:\Diamond Rush"`
3. Chạy lệnh biên dịch:

```
mingw32-make windows
```

4. Chạy game:

```
.\DiamondRush.exe
```

## Cài đặt trên Linux

### 1. Cài đặt các thư viện cần thiết

Cho Ubuntu/Debian:
```bash
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

Cho Fedora:
```bash
sudo dnf install gcc-c++ SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

Cho Arch Linux:
```bash
sudo pacman -S gcc sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### 2. Biên dịch dự án

```bash
make linux
```

### 3. Chạy game

```bash
./DiamondRush
```