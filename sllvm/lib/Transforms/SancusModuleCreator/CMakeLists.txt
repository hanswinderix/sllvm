add_llvm_loadable_module(SancusModuleCreator
  SancusModuleCreator.cpp
  FunctionCcInfo.cpp
  SancusModuleInfo.cpp
  AnnotationParser.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
