sllvm_pm_ifcompound_text_start = .;
*(.sllvm.text.dispatch.ifcompound)  /* Make sure the dispatcher is put first */
*(.sllvm.text.ifcompound)
*(.sllvm.text.ifcompound.*)         /* Internalized runtime functions */
sllvm_pm_ifcompound_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
