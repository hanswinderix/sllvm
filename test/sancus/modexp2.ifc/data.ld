sllvm_pm_modexp2_data_start = .;
*(.sllvm.data.modexp2)
*(.sllvm.data.lr1addr.modexp2) /* Make sure ptr to local sp is put last */
sllvm_pm_modexp2_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
