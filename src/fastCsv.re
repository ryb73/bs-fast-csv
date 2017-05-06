type t;

type createWriteStreamOptions;

external createWriteStream : Js.undefined createWriteStreamOptions => t = "" [@@bs.module "fast-csv"];
external pipe : t => Node.Fs.WriteStream.t => unit = "" [@@bs.send];
external write : t => Js.t {..} => unit = "" [@@bs.send];
external close : t => unit = "end" [@@bs.send];