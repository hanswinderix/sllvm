for ifc in `ls -d *.ifc`; do
  # No -Os in xxx.ifc
  grep "\-O" $ifc/Makefile.*

  # No *after*.dot in xxx.ifc
  ls $ifc | grep -i after\.dot

  # No SM violations
  grep -i "SM violation" `find . -name \*.sim`
done

# Manual checks
#
# - meld xxx xxx.ifc
#
#   The only differences should be in the name of the executable and in
#   the if-converted enclave code
#
# - Check CSV that everything executes in constant-time
