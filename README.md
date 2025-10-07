
# Case Studies

Author: Zachary Rehg  
Organization: Three Haverim  
Version: 0.1.0  
Date: 10/7/2025

This program aims to simulate business school case studies in an interactive format for educational purposes.  
Curently, the program includes the following studies:  
- Hitman

## Table of Contents:

1. Installation
2. Dependencies
3. Licenses

## 1. Installation

Note: All given version numbers are tested. Other versions not mentioned are untested and my lead to unpredictable results.

1. Git Clone this repository
2. Download CMake GUI verison 3.25.3
3. Download and build SFML version 3.0.0 using CMake Gui
    1. Create a path entry "CMAKE_INSTALL_PREFIX" and set its value to the intended install location
4. Build project using CMake GUI
    1. Set Hitman directory as source
    2. Set Hitman/build directory as build
    3. Build according to environment makefiles and toolchain

## 2. Dependencies

- SFML version 3.0.0
    - This project uses SFML for graphics and UI elements
    - SFML internally uses the following libraries:
        - freetype
        - libjpeg
        - stb_image
        - OpenAL Soft (not used in this project) (NOTE: if this project is changed to include this dependency it must be PROPERLY LICENSED according to LGPL guidelines)
        - libogg
        - libvorbis
        - libflac
        - minimp3 (not used in this project)
- TGUI version 1.10.0
    - This project uses TGUI for interactive UI elements
- SQLite version 3.50.4
    - This project uses SQLite for basic data processing backend

## 3. Licenses

- SFML: zlib/png license (https://www.sfml-dev.org/license/)
- freetype: FTL license (https://freetype.org/license.html)
- libjpeg: public domain
- stb_image: public domain
- libogg, libvorbis, libflac: BSD 3 license (https://opensource.org/license/bsd-3-clause)
- TGUI: zlib license (https://opensource.org/license/Zlib)
- SQLite: public domain (https://www.sqlite.org/copyright.html)

Full license texts included with this distribution