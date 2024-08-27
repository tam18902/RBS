# RBS

## Overview

Server_Client Stimulate RBS RU_DU

## Prerequisites

Before building the project, ensure you have the following installed:

- CMake
- A C++ compiler (e.g., GCC, Clang, MSVC)

## Building the Project

Follow these steps to build the project using CMake:

### Clone the Repository

First, clone the repository to your local machine:

```bash
git clone https://github.com/tam18902/RBS.git
cd RBS
```

### Create a Build Directory for library

Build shared library which can be installed into lib/ folder:
```bash
mkdir shared/build
cd shared/build
```
### Configure and Build the library

Run CMake to configure the project and generate build files:
```bash
cmake ..
make install
```
Now lib/ directory is automatically generated.

### Create a Build Directory for the project
Back to RBS/ top project. Create a directory where the build files will be generated:
```bash
cd -
mkdir build
cd build
```
### Configure and Build the project

Run CMake to configure the project and generate build files:
```bash
cmake ..
make
```

### Run Project
```bash
#for server
./server/server
# for client
./client/client
```