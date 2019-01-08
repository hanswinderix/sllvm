# Security Enhanced LLVM

This is the main source code repository for [Security Enhanced LLVM](https://distrinet.cs.kuleuven.be/software/sancus/publications/winderix18thesis.pdf) 
(SLLVM).

## Building from Source

This section documents the current best way to build and install the SLLVM
toolchain from source. The GNU make based build system has been developed to 
work on a fresh Ubuntu 18.04.1 LTS installation, but it should be fairly 
straightforward to port to other GNU/Linux distributions.

1. Make sure you have installed the following dependencies.

   * GNU **make** 3.81 or later
   * **git**

   ```sh
   sudo apt install make git
   ```

2. Clone the Git repository.

   ```sh
   git clone https://github.com/hanswinderix/sllvm.git
   cd sllvm
   ```

3. Install the following additional dependencies.

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
   * **clang-sancus**

   ```sh
   sudo -H make install-deps
   ```

   Note: The msp430-gcc and clang-sancus dependencies are only required to 
   build the legacy sancus compiler. These dependencies will disappear in the 
   future.

4. (Optional) Create and edit Makefile.local for local build configuration.

   The following GNU make variables can be set.
   
   | Variable name    | Purpose                                            |
   |------------------|----------------------------------------------------|
   | JOBS             | Specifies the number of jobs to run simultaneously |
   | BUILD\_TYPE      | *Debug* or *Release* build                         |
   | SANCUS\_SECURITY | Sancus security level can be *64* or *128* bits    |
   | SANCUS\_KEY      | Sancus master key in hexadecimal notation          |

5. Fetch other repositories and configure the build. 

   ```sh
   $ make fetch
   $ make configure
   ```
   
6. Build and install.

   ```sh
   $ make install
   ```

## Running the Sancus examples

   ```sh
   $ make test-sancus
   ```

## Compiling Sancus Protected Modules

See [Sancus Examples](https://github.com/sancus-pma/sancus-examples) for 
details on how to compile Sancus PMs with the SLLVM-based toolchain.

## Compiling Intel SGX enclaves

The [Security Enhanced LLVM](https://distrinet.cs.kuleuven.be/software/sancus/publications/winderix18thesis.pdf) master's thesis contains information on how
to compiler Intel SGX enclaves.

## License

See [LICENSE](LICENSE) for details.
