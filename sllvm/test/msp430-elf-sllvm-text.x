sllvm_pm_sensor_text_start = .;
*(.sllvm.text.dispatch.sensor)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sensor)
*(.sllvm.text.sensor.*)         /* Internalized runtime functions */
sllvm_pm_sensor_text_end = .;

sllvm_pm_reader_text_start = .;
*(.sllvm.text.dispatch.reader)  /* Make sure the dispatcher is put first */
*(.sllvm.text.reader)
*(.sllvm.text.reader.*)         /* Internalized runtime functions */
sllvm_pm_reader_text_end = .;

sllvm_pm_hello_text_start = .;
*(.sllvm.text.dispatch.hello)  /* Make sure the dispatcher is put first */
*(.sllvm.text.hello)
*(.sllvm.text.hello.*)         /* Internalized runtime functions */
sllvm_pm_hello_text_end = .;

sllvm_foo_text_section_start = .;
*(.sllvm.text.dispatch.foo)  /* Make sure the dispatcher is put first */
*(.sllvm.text.foo)
*(.sllvm.text.foo.*)         /* Internalized runtime functions */
sllvm_foo_text_section_end = .;

sllvm_bar_text_section_start = .;
*(.sllvm.text.dispatch.bar)  /* Make sure the dispatcher is put first */
*(.sllvm.text.bar)
*(.sllvm.text.bar.*)         /* Internalized runtime functions */
sllvm_bar_text_section_end = .;

/* Hashes for secure linking */
*(.sllvm.hash.reader.*)
