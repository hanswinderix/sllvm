sllvm_foo_text_section_start = .;
*(.sllvm.text.dispatch.foo)  /* Make sure the dispatcher is put first */
*(.sllvm.text.foo)
sllvm_foo_text_section_end = .;

sllvm_bar_text_section_start = .;
*(.sllvm.text.dispatch.bar)  /* Make sure the dispatcher is put first */
*(.sllvm.text.bar)
sllvm_bar_text_section_end = .;
