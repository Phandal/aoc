val read_file : string -> (string list, string) result
(** [read_file f] reads all lines from file [f] into a [string list]. *)

val default_file : string -> string array -> string
(** [default_file f args] returns filename supplied in program [args] or returns [f]. *)
