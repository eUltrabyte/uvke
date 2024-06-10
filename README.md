![uvke icon](https://github.com/eUltrabyte/uvke/blob/dev/Resource/Textures/uvke-icon.png?raw=true "uvke icon")
![uvke logo](https://github.com/eUltrabyte/uvke/blob/dev/Resource/Textures/uvke.png?raw=true "uvke logo")

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/github/license/eUltrabyte/uvke)](https://github.com/eUltrabyte/uvke/blob/main/LICENSE)
[![Pull Requests](https://img.shields.io/github/issues-pr/eUltrabyte/uvke)](https://github.com/eUltrabyte/uvke/pulls)
[![Forks](https://img.shields.io/github/forks/eUltrabyte/uvke?style=flat)](https://github.com/eUltrabyte/uvke)

#### uvke ( Ultrabyte Vulkan Engine ) is open source framework for Vulkan based apps development written in C++ 20.

Main goal of uvke is to make using vulkan simple. I want to use it for myself but usage by someone else would be nice.

### Libraries
[glfw](https://github.com/glfw/glfw) - Open source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.

[Vulkan SDK](https://lunarg.com/vulkan-sdk/) - Vulkan is a new generation graphics and compute API that provides high-efficiency, cross-platform access to modern graphics processing units, which are used in a wide variety of devices from PCs and consoles to mobile phones and embedded platforms.

[imgui](https://github.com/ocornut/imgui) - Bloat-free graphical user interface library for C++. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline-enabled application. It is fast, portable, renderer agnostic, and self-contained (no external dependencies).

[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - Image loading/decoding from file/memory: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC.

[portaudio](https://github.com/PortAudio/portaudio) - Portable audio I/O library designed for cross-platform support of audio. It uses either a callback mechanism to request audio processing, or blocking read/write calls to buffer data between the native audio subsystem and the client.

[tinyobjloader](https://github.com/tinyobjloader/tinyobjloader/blob/release/tiny_obj_loader.h) - tiny but powerful single file wavefront obj loader written in C++03. No dependency except for C++ STL. It can parse over 10M polygons with moderate memory and time.

### Installation
```bash
git clone https://github.com/eUltrabyte/uvke.git
cd uvke && mkdir Build
cd Build && cmake ..
```

### Bugs
Bugs are reported to [issue tracker](https://github.com/eUltrabyte/uvke/issues)
#### Report template look like this
```
Operating System:
CPU:
GPU:
Branch:
Short description:
```