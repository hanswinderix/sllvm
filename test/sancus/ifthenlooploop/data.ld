sllvm_pm_ifthenlooploop_data_start = .;
*(.sllvm.data.ifthenlooploop)
*(.sllvm.data.lr1addr.ifthenlooploop) /* Make sure ptr to local sp is last */
sllvm_pm_ifthenlooploop_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
