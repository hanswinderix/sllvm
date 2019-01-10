sllvm_pm_hello_data_start = .;
*(.sllvm.data.hello)
*(.sllvm.data.lr1addr.hello) /* Make sure the pointer to local sp is put last */
sllvm_pm_hello_data_end = .;
