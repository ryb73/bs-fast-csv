open Js.Promise;
open Node.Fs;

module CsvFormatter = FastCsv.Format({
    type t = Js.t {. one: string, two: string };
});

FileWriteStream.create "out.csv" Js.undefined (fun fileStream => {
    CsvFormatter.create Js.undefined (fun csvStream => {
        CsvFormatter.pipe csvStream (FileWriteStream.writeable fileStream);
        CsvFormatter.write csvStream { "one": "ok", "two": "hey" };
        CsvFormatter.write csvStream { "one": "two", "two": "three" };
        /*ignore (Js.Exn.raiseError "This is an error");*/

        resolve ();
    });
})
    |> then_ (fun _ => {
        Js.log "after write";
        resolve ();
    })
    |> catch (fun err => {
        Js.log "caught an error:";
        Js.log err;
        resolve ();
    });

let fileStream = FileReadStream.create "out.csv" Js.undefined;
let csvStream = FastCsv.parse ();

FileReadStream.pipe fileStream (FastCsv.Parse.writeable csvStream);

FastCsv.Parse.on csvStream @@ `data (fun data => {
    Js.log data;
});

FastCsv.Parse.on csvStream @@ `end_ (fun () => {
    Js.log "done reading";
});

/* This shouldn't typecheck */
/*FastCsv.Read.pipe csvStream (FastCsv.Read.writeable csvStream);*/