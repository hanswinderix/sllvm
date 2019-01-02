import os

KEY_BITSIZE  = 64
KEY_BYTESIZE = ((KEY_BITSIZE + 7) // 8)

libname = os.path.join(os.path.dirname((os.path.abspath(__file__))), 
        "../../install/share/sancus-compiler/libsancus-crypto.so")
