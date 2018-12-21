add_llvm_library(SLLVM 
  MODULE
  Analysis.cpp
  Transformation.cpp
  SancusTransformation.cpp
  SGXTransformation.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
