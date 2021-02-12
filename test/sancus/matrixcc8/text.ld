sllvm_pm_matrixcc8_text_start = .;
*(.sllvm.text.dispatch.matrixcc8)  /* Make sure the dispatcher is put first */
*(.sllvm.text.matrixcc8)
*(.sllvm.text.matrixcc8.*)         /* Internalized runtime functions */
sllvm_pm_matrixcc8_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;

/* Wrap information */
*(.sllvm.wrap.*)

/* Symbols for secure linking */
*(.sllvm.slink.*)
