## Getting Started

Visual Studio 2022/2026 is recommended, The Game Engine has been tested on:
* **Windows 10/11** with **VS2022/VS2026**.
* **Linux (Ubuntu)** with **CLion**.

### Prerequisites
* A compiler with full **C++23** support (e.g., MSVC 19.30+, GCC 13+, or Clang 16+).
* **CMake 3.20** or higher.

### Cloning the Repository
You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/Qczer/GameEngine`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

If you already cloned it without submodules, run: `git submodule update --init --recursive`

## Generate Project

The engine uses **CMake**, which means most IDEs will handle the configuration automatically without the need for manual project generation.

### Windows
**Recommended IDE:** Visual Studio 2022 or 2026.

**How to open and run:**
1. Open Visual Studio.
2. Select **"Open a local folder"**.
3. Choose the root directory of the repository.
4. Visual Studio will automatically detect `CMakeLists.txt` and configure the project.
5. Select **GameEngine-Editor.exe** as the startup item and press **F5** to build and run.

### Linux
**Recommended IDE:** CLion or VS Code.

**Requirements:** Ensure you have `cmake`, `gtkmm-4.0` and a compiler supporting **C++23** (e.g., `gcc-13` or `clang-16`) installed.

**How to build:**
1. Open the project folder in **CLion**.
2. CLion will automatically initialize the CMake profile.
3. Build the **GameEngine-Editor** target.
4. Click the **Play** button to start the engine.