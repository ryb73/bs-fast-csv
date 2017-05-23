open Node.Fs;
open Js.Promise;

module Write = {
    type t;

    type t2 = t; /* Create t2 as alias for t to prevent compile error */
    include Readable.Make({
        type t = t2;
    });

    external write : t => Js.t {..} => unit = "" [@@bs.send];

    type createOptions;
    external _create : Js.undefined createOptions => t = "createWriteStream" [@@bs.module "fast-csv"];

    let create options scope => {
        try {
            let stream = _create options;
            scope stream
            |> then_ (fun _ => {
                Stream._end (toStream stream);
                resolve ();
            });
        } {
            | err => reject err;
        }
    };
};

module Read = {
    type t;

    type t2 = t;
    include Writeable.Make({
        type t = t2;
    });

    external parse : unit => t = "" [@@bs.module "fast-csv"];

    type event = [
        | `data (array string => unit)
        | `end_ (unit => unit)
    ];

    external _on : t => string => ('a => 'b) => unit = "on" [@@bs.send];

    let on stream (handler : event) => switch handler {
        | `data func => _on stream "data" func
        | `end_ func => _on stream "end" func
    };
};
