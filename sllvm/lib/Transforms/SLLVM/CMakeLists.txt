add_llvm_loadable_module(SLLVM
  Analysis.cpp
  Transformation.cpp
  SancusTransformation.cpp
  SGXTransformation.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
