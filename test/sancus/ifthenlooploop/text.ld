sllvm_pm_ifthenlooploop_text_start = .;
*(.sllvm.text.dispatch.ifthenlooploop)  /* Make sure dispatcher comes first */
*(.sllvm.text.ifthenlooploop)
sllvm_pm_ifthenlooploop_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
