# Security Enhanced LLVM

This is the main source code respository for SLLVM.

## Building from Source

1. Make sure you have installed the dependencies:

2. Clone the source with git:

   ```sh
   $ git clone https://github.com/hanswinderix/sllvm.git
   $ cd sllvm
   ```

3. Create a Makefile.local for local build configuration

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

## License

See [LICENSE](LICENSE) for details.
