let read_file filename =
  try Ok (In_channel.with_open_bin filename In_channel.input_lines)
  with Sys_error m -> Error m

let default_file df arr = try arr.(1) with _ -> df
