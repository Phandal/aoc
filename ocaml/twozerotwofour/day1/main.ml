let compare_parts acc f s =
  let difference = Int.abs (f - s) in
  acc + difference

let process_lists (f, s) =
  let fs = List.sort Int.compare f in
  let ss = List.sort Int.compare s in
  List.fold_left2 compare_parts 0 fs ss

let reduce_parts (fl, sl) parts =
  match parts with Ok (f, s) -> (f :: fl, s :: sl) | Error _ -> (fl, sl)

let parse_line s =
  let parts = String.split_on_char ' ' s |> List.filter Dopo.not_empty in
  let length = List.length parts in
  if length <> 2 then
    Error
      (Printf.sprintf "line \"%s\" must contain exactly 2 numbers: %d given" s
         length)
  else Ok (List.nth parts 0 |> int_of_string, List.nth parts 1 |> int_of_string)

let day1 f =
  let lists =
    Aoc.read_file f |> List.map parse_line
    |> List.fold_left reduce_parts ([], [])
  in
  process_lists lists

(*let () = day1 "input/input_day_01_test.txt"*)
let () =
  let result = day1 "input/input_day_01.txt" in
  Printf.printf "Day 1 result: %d\n" result
