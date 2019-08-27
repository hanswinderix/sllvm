sllvm_pm_ifthenloop_data_start = .;
*(.sllvm.data.ifthenloop)
*(.sllvm.data.lr1addr.ifthenloop) /* Make sure ptr to local sp is put last */
sllvm_pm_ifthenloop_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
