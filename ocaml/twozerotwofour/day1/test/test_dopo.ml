let l = [ "3"; ""; ""; ""; "4" ]

let test_empty =
  let t = Dopo.empty "" in
  Printf.printf "test_empty: %b\n" t

let test_not_empty =
  let t = Dopo.not_empty "hello" in
  Printf.printf "test_not_empty: %b\n" t

let test_filter =
  let nums = List.filter Dopo.not_empty l in
  Printf.printf "test_filter: %d\n" (List.length nums)

let () =
  test_empty;
  test_not_empty;
  test_filter
