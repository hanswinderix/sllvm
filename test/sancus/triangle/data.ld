sllvm_pm_triangle_data_start = .;
*(.sllvm.data.triangle)
*(.sllvm.data.lr1addr.triangle) /* Make sure pointer to local sp is put last */
sllvm_pm_triangle_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
