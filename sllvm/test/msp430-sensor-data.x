sllvm_sensor_data_section_start = .;
*(.sllvm.data.sensor)
sllvm_sensor_data_section_end = .;

sllvm_reader_data_section_start = .;
*(.sllvm.data.reader)
sllvm_reader_data_section_end = .;
