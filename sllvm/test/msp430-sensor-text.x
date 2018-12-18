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

*(.sllvm.hash.reader.*)         /* Hashes for secure linking */
