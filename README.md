# Security Enhanced LLVM

This is the main source code respository for SLLVM.

## Building from Source

1. Make sure you have installed the dependencies:

   * `g++` 4.7 or later
   * GNU `make` 3.81 or later
   * `cmake` 3.4.3 or later
   * `curl`
   * `git`
   * `svn`

2. Clone the source with git:

   ```sh
   $ git clone https://github.com/hanswinderix/sllvm.git
   $ cd sllvm
   ```

3. Create a Makefile.local for local build configuration

   * Edit Makefile.local
   * Set BASEDIR to current working directory (absolute path)

4. Fetch other repositories and configure the build

   ```sh
   $ make sllvm-checkout
   $ make sllvm-configure
   $ make sllvm-apply-full-patch
   ```
7. Build and install:

   ```sh
   $ make sllvm-install
   ```

## Compiling Sancus PMs and SGX enclaves

See [Makefile](sllvm/test/Makefile) in [test directory](sllvm/test) for build
instructions.

## License

See [LICENSE](LICENSE) for details.
