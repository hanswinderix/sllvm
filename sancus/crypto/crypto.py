#!/usr/bin/env python3

import sys
import elf
import loader
import shutil
import ccrypto
import argparse

# Required for secure linking
def fill_hashes(loader, fname):
  elf = loader.get_ELF()
  shutil.copy(elf.get_name(), fname)
  with open(fname, 'rb+') as f:
    for pm1 in loader.get_protected_modules():
      pattern = 'sllvm_hash_%s_(\w+)' % pm1.get_name()
      for match, sym in elf.match_symbols(pattern):
        pm2 = loader.find_protected_module_by_symbol_name(match.group(1))
        assert pm2
        assert pm1 != pm2
        hash = pm2.get_hash()
        size = sym['st_size']
        assert size == ccrypto.KEY_BYTESIZE
        assert size == len(hash)
        offset = elf.get_offset_of_text_addr_in_file(sym['st_value'])
        f.seek(offset)
        f.write(hash)

def wrap_text(loader, fname, key):
  elf = loader.get_ELF()
  shutil.copy(elf.get_name(), fname)
  with open(fname, 'rb+') as f:
    for pm in loader.get_protected_modules():
      sym_nonce = elf.find_symbol_by_name('sllvm_nonce_%s' % pm.get_name())
      #TODO: assert sym_nonce != None
      sym_tag = elf.find_symbol_by_name('sllvm_tag_%s' % pm.get_name())
      #TODO: assert sym_tag != None
      if sym_nonce != None and sym_tag != None:
        # 1) Compute nonce, tag and wrap
        nonce = ccrypto.sancus_wrap_sm_nonce(pm.get_name(), pm.get_text())
        wrap, tag = ccrypto.sancus_wrap(key, nonce, pm.get_text())

        # 1) Write wrapped text
        offset = elf.get_offset_of_text_addr_in_file(pm.get_text_start())
        f.seek(offset)
        f.write(wrap)

        # 2) Write nonce
        offset = elf.get_offset_of_text_addr_in_file(sym_nonce['st_value'])
        f.seek(offset)
        f.write(nonce)

        # 3) Write tag
        offset = elf.get_offset_of_text_addr_in_file(sym_tag['st_value'])
        f.seek(offset)
        f.write(tag)

parser = argparse.ArgumentParser()
parser.add_argument('-k', '--key', type=lambda x: bytes.fromhex(x))
#parser.add_argument('-i', '--id', type=lambda x: int(x, 16))
#parser.add_argument('-v', '--gen-vendor-key', action='store_true')
parser.add_argument('-g', '--gen-vendor-key', type=lambda x: int(x, 16))
parser.add_argument('-w', '--wrap-sm-text-sections', action='store_true')
#parser.add_argument('-w', '--wrap-text', action='store_true')
#parser.add_argument('-f', '--fill-hashes', action='store_true')
parser.add_argument('-f', '--fill-macs', action='store_true')
parser.add_argument('-o', '--output-file')
parser.add_argument('-s', '--gen-sm-key')
parser.add_argument('-u', '--unwrap', nargs=3)
parser.add_argument('-v', '--verbose', action='store_true')
parser.add_argument('infile', nargs='?')
args = parser.parse_args()

if args.infile:
  loader = loader.Loader(args.infile)

if args.unwrap:
  l = [bytes.fromhex(x) for x in args.unwrap]
  ad, cipher, tag = tuple(l)
  body = ccrypto.sancus_unwrap(args.key, ad, cipher, tag)
  assert body
  #TODO: print(body.hex())
  sys.stdout.buffer.write(body)

if args.gen_sm_key:
  assert args.infile
  pm = loader.find_protected_module_by_name(args.gen_sm_key)
  assert pm
  sm_key = ccrypto.compute_sancus_mac(args.key, pm.get_identity())
  #TODO: print(sm_key.hex())
  sys.stdout.buffer.write(sm_key)

if args.gen_vendor_key:
  id = args.gen_vendor_key.to_bytes(2, byteorder='little')
  vendor_key = ccrypto.compute_sancus_mac(args.key, id)
  #TODO: print(vendor_key.hex())
  sys.stdout.buffer.write(vendor_key)

if args.wrap_sm_text_sections:
  assert args.infile
  wrap_text(loader, args.output_file, args.key)

if args.fill_macs:
  assert args.infile
  fill_hashes(loader, args.output_file)
