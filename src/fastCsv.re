open Js.Promise;

type t;

external pipe : t => Node.Fs.WriteStream.t => unit = "" [@@bs.send];
external write : t => Js.t {..} => unit = "" [@@bs.send];

external _end : t => unit = "end" [@@bs.send];

type createWriteStreamOptions;
external _createWriteStream : Js.undefined createWriteStreamOptions => t = "createWriteStream" [@@bs.module "fast-csv"];

let createWriteStream options scope => {
    try {
        let stream = _createWriteStream options;
        switch (scope stream) {
            | None => Js.Promise.resolve @@ _end stream
            | Some promise => {
                promise
                    |> then_ (fun _ => {
                        _end stream;
                        Js.Promise.resolve ();
                    });
            }
        };
    } {
        | err => reject err;
    }
};