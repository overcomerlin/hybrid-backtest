cmake_minimum_required(VERSION 3.14)
project(HybridBacktest)

# Set C++ standard to 17 (required for many modern quantitative libraries)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python installation
# Includes the Interpreter for running scripts and Development for header files/libraries
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Manage Pybind11 dependency via FetchContent
# This ensures the project remains portable without requiring 'pip install pybind11' on the host
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.12.0 # Using a stable tagged version is recommended for reproducibility
)
FetchContent_MakeAvailable(pybind11)

# Define the Python module (.so on Linux/macOS, .pyd on Windows)
# The first argument 'quant_engine' is the name you will use to 'import' in Python
pybind11_add_module(quant_engine src/main.cpp)

# Link the Python development libraries to the module
# PRIVATE visibility is used to keep the link interface clean
target_link_libraries(quant_engine PRIVATE Python3::Module)