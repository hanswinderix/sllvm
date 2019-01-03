# Security Enhanced LLVM

This is the main source code repository for Security Enhanced LLVM (SLLVM).

## Building from Source

This section documents the current best way to build and install the SLLVM
toolchain from source. The GNU make based build system has been developed to 
work on a fresh Ubuntu 18.04.1 LTS installation, but it should be fairly 
straightforward to port to other GNU/Linux distributions.

1. Make sure you have installed the following dependencies:

   * GNU **make** 3.81 or later
   * **git**
   * **curl**
   * **cmake** 3.4.3 or later
   * **g++** 4.7 or later
   * **texinfo**
   * **Python 3**
   * **pip3** (for Python 3+)
   * **pyelftools** (Python 3+)
   * **msp430-gcc** 4.6 or later
   * **expect**
   * **tcl**
   * **iverilog**

   ```sh
   sudo apt install make git curl cmake g++ texinfo python3 python3-pip gcc-msp430 expect tcl iverilog
   pip3 install pyelftools
   ```

   or alternatively, if you already downloaded the source code and have GNU 
   **make** installed

   ```sh
   sudo -H make install-deps
   ```

   Note: The msp430-gcc dependency is only required to build the legacy sancus
   compiler. This dependency will disappear in the future.

2. Clone the source with git:

   ```sh
   $ git clone https://github.com/hanswinderix/sllvm.git
   $ cd sllvm
   ```

3. (Optional) Create a Makefile.local for local build configuration

   The following GNU make variables can be set.
   
   | Variable name    | Purpose                                            |
   |------------------|----------------------------------------------------|
   | JOBS             | Specifies the number of jobs to run simultaneously |
   | BUILD\_TYPE      | *Debug* or *Release* build                         |
   | SANCUS\_SECURITY | Sancus security level can be *64* or *128* bits    |
   | SANCUS\_KEY      | Sancus master key in hexadecimal notation          |

4. Fetch other repositories and configure the build. 

   ```sh
   $ make fetch
   $ make configure
   ```
   
   Note: This step might ask for the sudo password because the legacy sancus 
   compiler needs clang-sancus to be installed. In the future, this dependency 
   will disappear.

5. Build and install:

   ```sh
   $ make install
   ```

## Compiling Sancus PMs

TODO

## Compiling Intel SGX enclaves

TODO

## License

See [LICENSE](LICENSE) for details.
