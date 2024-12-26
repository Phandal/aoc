let parts line =
  let filtered =
    String.split_on_char ' ' line |> List.filter (fun x -> x <> "")
  in
  (List.nth filtered 0 |> int_of_string, List.nth filtered 1 |> int_of_string)

let fold_parts (fa, sa) line =
  let f, s = parts line in
  (f :: fa, s :: sa)

let parse_lines lines = List.fold_left fold_parts ([], []) lines

let abs_difference (f, s) =
  let f, s = (List.sort compare f, List.sort compare s) in
  List.fold_left2 (fun acc f s -> acc + abs (f - s)) 0 f s

let assoc_left_right (f, s) =
  let alist =
    List.fold_left
      (fun acc i ->
        let len = List.find_all (fun ss -> ss == i) s |> List.length in
        (i, len) :: acc)
      [] f
  in
  List.fold_left (fun acc i -> acc + (i * List.assoc i alist)) 0 f

let part1 fn =
  fn |> Aoc.read_file |> Result.map parse_lines |> Result.map abs_difference

let part2 fn =
  fn |> Aoc.read_file |> Result.map parse_lines |> Result.map assoc_left_right

let () =
  let filename = Aoc.default_file "input/input_day_01.txt" Sys.argv in
  let part1_result =
    Result.fold
      ~ok:(fun acc -> string_of_int acc)
      ~error:(fun e -> Printf.sprintf "Error: %s" e)
      (part1 filename)
  in

  let part2_result =
    Result.fold
      ~ok:(fun acc -> string_of_int acc)
      ~error:(fun e -> Printf.sprintf "Error: %s" e)
      (part2 filename)
  in

  Printf.printf "Part1: %s\nPart2: %s\n" part1_result part2_result
