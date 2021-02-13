sllvm_pm_ffilter_text_start = .;
*(.sllvm.text.dispatch.ffilter)  /* Make sure the dispatcher is put first */
*(.sllvm.text.ffilter)
*(.sllvm.text.ffilter.*)         /* Internalized runtime functions */
sllvm_pm_ffilter_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
