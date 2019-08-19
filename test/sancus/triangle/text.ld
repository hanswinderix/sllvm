sllvm_pm_triangle_text_start = .;
*(.sllvm.text.dispatch.triangle)  /* Make sure the dispatcher is put first */
*(.sllvm.text.triangle)
sllvm_pm_triangle_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
