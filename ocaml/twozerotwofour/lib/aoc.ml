let read_lines () =
  let rec _read_lines lines =
    try _read_lines (read_line () :: lines) with End_of_file -> lines
  in
  _read_lines []
