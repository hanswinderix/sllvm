sllvm_pm_switch16_text_start = .;
*(.sllvm.text.dispatch.switch16)  /* Make sure the dispatcher is put first */
*(.sllvm.text.switch16)
sllvm_pm_switch16_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
