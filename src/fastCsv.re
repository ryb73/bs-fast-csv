open Node.Fs;
open Js.Promise;

module type Typed = {
    type t; /* I'd like to be able to restrict this to subtypes of Js.t {..} */
};

module Format(DataType : Typed) => {
    type t;

    include Writeable.Make({
        type stream = t;
        type data = DataType.t;
    });

    include Readable.Make({
        type stream = t;
        type data = string;
    });

    type createOptions;
    external _create : Js.undefined createOptions => t = "createWriteStream" [@@bs.module "fast-csv"];

    let create options scope => {
        try {
            let streamObj = _create options;
            scope streamObj
            |> then_ (fun _ => {
                Stream._end (stream streamObj);
                resolve ();
            });
        } {
            | err => reject err;
        }
    };
};

module Parse = {
    type t;

    include Writeable.Make({
        type stream = t;
        type data = Node.StringBuffer.t;
    });

    include Readable.Make({
        type stream = t;
        type data = array string; /* TODO: support "headers" mode */
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

let parse = Parse.parse;