
# PhoXo

![PhoXo](https://img.shields.io/badge/PhoXo-Windows-blue)
![C++](https://img.shields.io/badge/C%2B%2B-MFC-orange)
![UI](https://img.shields.io/badge/UI-BCGControlBar%20Pro-lightgrey)

Native **Windows MFC** application.

---

## Build Requirements

### 1. Visual Studio 2022
All projects in this repository are built using **Visual Studio 2022**.

### 2. BCGControlBar Pro for MFC
- Only the `PhoXoEdit` project depends on **BCGControlBar Pro**.  
- You can download an **evaluation version** from the official website.

### 3. phoxo_core_lib
- C++20 header-only library for high-performance image processing on Windows.  
- Used by **all projects**.  
- GitHub: [https://github.com/atphoxo/phoxo-core-lib](https://github.com/atphoxo/phoxo-core-lib)  
- **Note:** Add this library to Visual Studio's **C++ Common Include Directories** before building.

### 4. UIStone
- C++20 header-only library for Win32 SDK and MFC, providing extended utilities for efficiency.  
- Used by **all projects**.  
- GitHub: [https://github.com/atphoxo/UIStone](https://github.com/atphoxo/UIStone)  
- **Note:** Add this library to Visual Studio's **C++ Common Include Directories** before building.
