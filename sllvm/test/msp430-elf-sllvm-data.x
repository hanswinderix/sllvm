sllvm_pm_sensor_data_start = .;
*(.sllvm.data.sensor)
sllvm_pm_sensor_data_end = .;

sllvm_pm_reader_data_start = .;
*(.sllvm.data.reader)
sllvm_pm_reader_data_end = .;

sllvm_pm_hello_data_start = .;
*(.sllvm.data.hello)
sllvm_pm_hello_data_end = .;

sllvm_foo_data_section_start = .;
*(.sllvm.data.foo)
sllvm_foo_data_section_end = .;

sllvm_bar_data_section_start = .;
*(.sllvm.data.bar)
sllvm_bar_data_section_end = .;
