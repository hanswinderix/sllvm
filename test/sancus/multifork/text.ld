sllvm_pm_multifork_text_start = .;
*(.sllvm.text.dispatch.multifork)  /* Make sure the dispatcher is put first */
*(.sllvm.text.multifork)
sllvm_pm_multifork_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
