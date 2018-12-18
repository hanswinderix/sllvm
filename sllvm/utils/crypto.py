import sys
import elf
import loader
import shutil
import ccrypto

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

assert len(sys.argv) > 2
loader = loader.Loader(sys.argv[1])
fill_hashes(loader, sys.argv[2])

#master_key=bytes.fromhex("deadbeefcafebabe")
#vendor_id=0x1234.to_bytes(2, byteorder='little')
#vendor_key = compute_sancus_mac(master_key, vendor_id)
#print('vendor', vendor_key.hex())
#for pm in l.get_protected_modules():
#  print(pm.name, compute_sancus_mac(vendor_key, pm.get_identity()).hex())
