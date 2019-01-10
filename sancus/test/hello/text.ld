sllvm_pm_hello_text_start = .;
*(.sllvm.text.dispatch.hello)  /* Make sure the dispatcher is put first */
*(.sllvm.text.hello)
sllvm_pm_hello_text_end = .;
