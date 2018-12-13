sllvm_sensor_text_section_start = .;
*(.sllvm.text.dispatch.sensor)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sensor)
*(.sllvm.text.sensor.*)         /* Internalized runtime functions */
sllvm_sensor_text_section_end = .;

sllvm_reader_text_section_start = .;
*(.sllvm.text.dispatch.reader)  /* Make sure the dispatcher is put first */
*(.sllvm.text.reader)
*(.sllvm.text.reader.*)         /* Internalized runtime functions */
sllvm_reader_text_section_end = .;
