sllvm_text_section_start = .;
*(.sllvm.text.dispatch.*)  /* Make sure the dispatcher is put first */
*(.sllvm.text.*)
sllvm_text_section_end = .;
