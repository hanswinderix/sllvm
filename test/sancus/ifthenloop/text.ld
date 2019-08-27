sllvm_pm_ifthenloop_text_start = .;
*(.sllvm.text.dispatch.ifthenloop)  /* Make sure the dispatcher is put first */
*(.sllvm.text.ifthenloop)
sllvm_pm_ifthenloop_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
