sllvm_pm_mulmod8_data_start = .;
*(.sllvm.data.mulmod8)
*(.sllvm.data.lr1addr.mulmod8) /* Make sure ptr to local sp is put last */
sllvm_pm_mulmod8_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
