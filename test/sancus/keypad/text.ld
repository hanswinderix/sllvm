sllvm_pm_keypad_text_start = .;
*(.sllvm.text.dispatch.keypad)  /* Make sure the dispatcher is put first */
*(.sllvm.text.keypad)
sllvm_pm_keypad_text_end = .;

sllvm_pm_keypad_mmio_text_start = .;
*(.sllvm.text.dispatch.keypad_mmio)  /* Make sure the dispatcher is put first */
*(.sllvm.text.keypad_mmio)
sllvm_pm_keypad_mmio_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
