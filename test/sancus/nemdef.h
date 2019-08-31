#ifndef NEMDEF_H
#define NEMDEF_H

#define _eentry __attribute__((eentry))
#define _secret __attribute__((secret))

#ifdef SANCUS_STEP_ENABLED 
#define ATTACK(_f_, ...)                 \
  __ss_mount(#_f_ "(" #__VA_ARGS__ ")"); \
  _f_(__VA_ARGS__);
#else
#define ATTACK(_f_, ...)                          \
  printf("attack: " #_f_ "(" #__VA_ARGS__ ")\n"); \
  _f_(__VA_ARGS__);
#endif

#endif
