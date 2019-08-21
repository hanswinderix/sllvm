sllvm_pm_bsl_text_start = .;
*(.sllvm.text.dispatch.bsl)  /* Make sure the dispatcher is put first */
*(.sllvm.text.bsl)
sllvm_pm_bsl_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
