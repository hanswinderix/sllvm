#ifndef NEMDEF_H
#define NEMDEF_H

#define _eentry __attribute__((eentry))
#define _secret __attribute__((secret))

#define SS_MOUNT(_f_, ...)               \
  __ss_mount(#_f_ "(" #__VA_ARGS__ ")"); \
  _f_(__VA_ARGS__);

#endif
