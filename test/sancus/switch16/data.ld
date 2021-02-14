sllvm_pm_switch16_data_start = .;
*(.sllvm.data.switch16)
*(.sllvm.data.lr1addr.switch16) /* Make sure ptr to local sp is put last */
sllvm_pm_switch16_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
