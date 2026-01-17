# Atom Eye

## Purpose



## Environment

Install MSYS2 from: https://www.msys2.org/

Open the MSYS2 MinGW64 shell (not the MSYS shell)

Install dependencies
```
pacman -S --needed \
  mingw-w64-x86_64-gcc \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-glfw \
  mingw-w64-x86_64-glm
```

## Compilation

```bash
cmake -S . -B build -G Ninja
cmake --build build
```