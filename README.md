
# PhoXo

![OS](https://img.shields.io/badge/OS-Windows%2010%2F11-blue)
![C++](https://img.shields.io/badge/C%2B%2B-MFC-orange)
![UI](https://img.shields.io/badge/UI-BCGControlBar-lightgrey)
![License](https://img.shields.io/badge/License-MPL--2.0-green)

Native **Windows MFC** application.

---

## Build Requirements

### 1. Visual Studio 2022

All projects in this repository are built using **Visual Studio 2022**  

---

### 2. BCGControlBar Pro for MFC

- Only the **`PhoxoEdit`** project depends on **BCGControlBar Pro**.
- You can download an **evaluation version** of BCGControlBar Pro from the official website.

#### Notes for Evaluation Version Users

If you are using the **BCGControlBar evaluation version**, please update your project settings as follows:

1. **Use MFC in a Shared DLL**
2. **Use the CRT in DLL form** (`/MD` or `/MDd`)

> Static MFC builds are **not supported** when using the BCG evaluation version.

---

## Clone with Submodules

This repository has **two submodules**: `phoxo-core-lib` and `UIStone`.  

> ⚠️ **Important:** When cloning, **check "recursive"** to get submodules.  

Or via command line:

```bash
git clone --recursive git@github.com:atphoxo/PhoXo.git
