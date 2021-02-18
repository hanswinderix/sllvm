sllvm_pm_bsl_data_start = .;
*(.sllvm.data.bsl)
*(.sllvm.data.lr1addr.bsl) /* Make sure pointer to local sp is put last */
sllvm_pm_bsl_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
