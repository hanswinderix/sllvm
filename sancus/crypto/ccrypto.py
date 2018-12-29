import ctypes

# TODO: Don't hardcode these config values
KEY_BITSIZE = 64
KEY_BYTESIZE = ((KEY_BITSIZE + 7) // 8)
libname = '/home/hans/devel/sllvm.git/install/share/sancus-compiler/libsancus-crypto.so'

clib = ctypes.cdll.LoadLibrary(libname)

def sancus_wrap(key, ad, body):
  cipher = bytes(len(body))
  tag = bytes(KEY_BYTESIZE)
  ad_clen = ctypes.c_ulonglong(len(ad))
  body_clen = ctypes.c_ulonglong(len(body))
  if clib.sancus_wrap(key, ad, ad_clen, body, body_clen, cipher, tag):
    return cipher, tag
  return None

def sancus_wrap_sm_nonce(name, text):
  return sancus_wrap(bytes(0), bytes(name, 'utf-8'), text)[1][:2][::-1]
  
def compute_sancus_mac(key, msg):
  mac = bytes(KEY_BYTESIZE)
  clen = ctypes.c_ulonglong(len(msg))
  clib.sancus_mac(key, msg, clen, mac)
  return mac

def compute_sancus_hash(msg):
  return compute_sancus_mac(bytes(KEY_BYTESIZE), msg)

