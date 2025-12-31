
# PhoXo

![OS](https://img.shields.io/badge/OS-Windows%2010%2F11-blue)
![C++](https://img.shields.io/badge/C%2B%2B-MFC-orange)
![UI](https://img.shields.io/badge/UI-BCGControlBar-lightgrey)
![License](https://img.shields.io/badge/License-MPL--2.0-green)

Native **Windows MFC** application.

---

## Build Requirements

### 1. Visual Studio 2022
All projects in this repository are built using **Visual Studio 2022**.

### 2. BCGControlBar Pro for MFC
- Only the `PhoXoEdit` project depends on **BCGControlBar Pro**.  
- You can download an **evaluation version** from the official website.


## Clone with Submodules

This repository has **two submodules**: `phoxo-core-lib` and `UIStone`.  

> ⚠️ **Important:** When cloning, **check "recursive"** to get submodules.  

Or via command line:

```bash
git clone --recursive git@github.com:atphoxo/PhoXo.git
