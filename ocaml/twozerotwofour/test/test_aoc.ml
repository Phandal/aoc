let test_read_file () =
  let known =
    Result.fold
      ~ok:(List.fold_left (fun acc i -> Printf.sprintf "%s, %s" acc i) "")
      ~error:Fun.id
      (Aoc.read_file "test/test.txt")
  in
  Printf.printf "known: %s\n" known;

  let unknown =
    Result.fold
      ~ok:(List.fold_left (fun acc i -> Printf.sprintf "%s, %s" acc i) "")
      ~error:Fun.id
      (Aoc.read_file "test/unknown.txt")
  in
  Printf.printf "unknown: %s\n" unknown;
  ()

let test_default_file () =
  let filled_array = Array.make 3 "one" in
  let file = Aoc.default_file "test" filled_array in
  Printf.printf "supplied: %s\n" file;

  let empty_array = Array.make 0 "" in
  let file = Aoc.default_file "test" empty_array in
  Printf.printf "not supplied: %s\n" file;
  ()

let () =
  test_read_file ();
  test_default_file ();
  Printf.printf "%s\n" (Sys.getcwd ())
