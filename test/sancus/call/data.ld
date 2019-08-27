sllvm_pm_call_data_start = .;
*(.sllvm.data.call)
*(.sllvm.data.lr1addr.call) /* Make sure ptr to local sp is put last */
sllvm_pm_call_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
