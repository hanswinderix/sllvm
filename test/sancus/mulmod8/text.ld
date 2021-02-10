sllvm_pm_mulmod8_text_start = .;
*(.sllvm.text.dispatch.mulmod8)  /* Make sure the dispatcher is put first */
*(.sllvm.text.mulmod8)
*(.sllvm.text.mulmod8.*)         /* Internalized runtime functions */
sllvm_pm_mulmod8_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
