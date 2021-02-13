sllvm_pm_multiply_text_start = .;
*(.sllvm.text.dispatch.multiply)  /* Make sure the dispatcher is put first */
*(.sllvm.text.multiply)
*(.sllvm.text.multiply.*)         /* Internalized runtime functions */
sllvm_pm_multiply_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;

/* Wrap information */
*(.sllvm.wrap.*)

/* Symbols for secure linking */
*(.sllvm.slink.*)
