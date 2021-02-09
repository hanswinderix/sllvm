sllvm_pm_sharevalue_text_start = .;
*(.sllvm.text.dispatch.sharevalue)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sharevalue)
*(.sllvm.text.sharevalue.*)         /* Internalized runtime functions */
sllvm_pm_sharevalue_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
