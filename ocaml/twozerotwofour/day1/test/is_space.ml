let l = [ "3"; " "; " "; "4" ]

let test_is_space =
  let t = Dopo.is_space " " in
  Printf.printf "test_is_space: %b\n" t

let test_is_not_space =
  let t = Dopo.is_space "hello" in
  Printf.printf "test_is_not_space: %b\n" t

let test_filter =
  let nums = List.filter Dopo.is_space l in
  Printf.printf "test_filter: %d\n" (List.length nums)

let () =
  test_is_space;
  test_is_not_space;
  test_filter
