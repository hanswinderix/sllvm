sllvm_pm_kruskal_data_start = .;
*(.sllvm.data.kruskal)
*(.sllvm.data.lr1addr.kruskal) /* Pointer to local sp must be last */
sllvm_pm_kruskal_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
