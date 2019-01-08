import ctypes
import config

# TODO: KEY_BITSIZE should be the default value. It should be possible to 
#       override it via the commandline

clib = ctypes.cdll.LoadLibrary(config.libname)

def sancus_wrap(key, ad, body):
  cipher = bytes(len(body))
  tag = bytes(len(key))
  ad_clen = ctypes.c_ulonglong(len(ad))
  body_clen = ctypes.c_ulonglong(len(body))
  if clib.sancus_wrap(key, ad, ad_clen, body, body_clen, cipher, tag):
    return cipher, tag
  return None

def sancus_unwrap(key, ad, cipher, tag):
  body = bytes(len(cipher))
  ad_clen = ctypes.c_ulonglong(len(ad))
  cipher_clen = ctypes.c_ulonglong(len(cipher))
  if clib.sancus_unwrap(key, ad, ad_clen, cipher, cipher_clen, tag, body):
    return body
  return None

def sancus_wrap_sm_nonce(name, text):
  return sancus_wrap(bytes(0), bytes(name, 'utf-8'), text)[1][:2][::-1]
  
def compute_sancus_mac(key, msg):
  mac = bytes(len(key))
  clen = ctypes.c_ulonglong(len(msg))
  clib.sancus_mac(key, msg, clen, mac)
  return mac

def compute_sancus_hash(size, msg):
  return compute_sancus_mac(bytes(size), msg)
