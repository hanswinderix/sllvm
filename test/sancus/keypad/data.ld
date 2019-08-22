sllvm_pm_keypad_data_start = .;
*(.sllvm.data.keypad)
*(.sllvm.data.lr1addr.keypad) /* Make sure pointer to local sp is put last */
sllvm_pm_keypad_data_end = .;

sllvm_pm_keypad_mmio_data_start = .;
*(.sllvm.data.keypad_mmio)
*(.sllvm.data.lr1addr.keypad_mmio) /* Make sure ptr to local sp is put last */
sllvm_pm_keypad_mmio_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
