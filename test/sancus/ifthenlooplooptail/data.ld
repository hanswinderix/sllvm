sllvm_pm_ifthenlooplooptail_data_start = .;
*(.sllvm.data.ifthenlooplooptail)
*(.sllvm.data.lr1addr.ifthenlooplooptail) /* ptr to local sp comes last */
sllvm_pm_ifthenlooplooptail_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure ptr to local sp is put last */
sllvm_pm_sancus_step_data_end = .;
