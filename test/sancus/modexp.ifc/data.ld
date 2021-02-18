sllvm_pm_modexp_data_start = .;
*(.sllvm.data.modexp)
*(.sllvm.data.lr1addr.modexp) /* Make sure ptr to local sp is put last */
sllvm_pm_modexp_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
