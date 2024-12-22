let read_file filename =
  In_channel.with_open_bin filename In_channel.input_lines
