type locations = { left : int list; right : int list }

let parse_line lines =
  let rec parse_line left right lines =
    match lines with
    | [] -> { left = List.sort compare left; right = List.sort compare right }
    | x :: xs ->
        let parts = String.split_on_char ' ' x in
        parse_line
          ((List.nth parts 0 |> int_of_string) :: left)
          ((List.nth parts 3 |> int_of_string) :: right)
          xs
  in
  parse_line [] [] lines

let part1 { left; right } =
  List.fold_left2 (fun acc a b -> acc + (a - b |> abs)) 0 left right

let part2 { left; right } =
  let number_counts =
    List.fold_left
      (fun acc b ->
        match List.assoc_opt b acc with
        | Some num -> (b, num + 1) :: acc
        | None -> (b, 1) :: acc)
      [] right
  in
  List.fold_left
    (fun acc a ->
      match List.assoc_opt a number_counts with
      | Some num -> acc + (a * num)
      | None -> acc)
    0 left

let () =
  let lines = Aoc.read_lines () in
  let locations = parse_line lines in
  let part1_result = part1 locations in
  let part2_result = part2 locations in
  Printf.printf "Part1 Result: %d\nPart2 Result: %d\n" part1_result part2_result
