import ctypes

# TODO: Don't hardcode these config values
KEY_BITSIZE = 64
KEY_BYTESIZE = ((KEY_BITSIZE + 7) // 8)
libname = '/usr/local/share/sancus-compiler/libsancus-crypto.so'

_clib = ctypes.cdll.LoadLibrary(libname)

def compute_sancus_mac(key, msg):
  mac = bytes(KEY_BYTESIZE)
  clen = ctypes.c_ulonglong(len(msg))
  _clib.sancus_mac(key, msg, clen, mac)
  return mac

def compute_sancus_hash(msg):
  return compute_sancus_mac(bytes(KEY_BYTESIZE), msg)

