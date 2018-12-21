add_llvm_library(SancusModuleCreator
  MODULE
  SancusModuleCreator.cpp
  FunctionCcInfo.cpp
  SancusModuleInfo.cpp
  AnnotationParser.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
