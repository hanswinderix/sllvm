sllvm_pm_mulhi3_text_start = .;
*(.sllvm.text.dispatch.mulhi3)  /* Make sure the dispatcher is put first */
*(.sllvm.text.mulhi3)
sllvm_pm_mulhi3_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
