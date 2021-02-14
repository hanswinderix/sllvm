sllvm_pm_math32_text_start = .;
*(.sllvm.text.dispatch.math32)  /* Make sure the dispatcher is put first */
*(.sllvm.text.math32)
*(.sllvm.text.math32.*)         /* Internalized runtime functions */
sllvm_pm_math32_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
