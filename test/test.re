open Js.Promise;
open Node.Fs;

FileWriteStream.create "out.csv" Js.undefined (fun fileStream => {
    FastCsv.Write.create Js.undefined (fun csvStream => {
        FastCsv.Write.pipe csvStream (FileWriteStream.writeable fileStream);
        FastCsv.Write.write csvStream { "one": "ok", "two": "hey" };
        FastCsv.Write.write csvStream { "one": "two", "two": "three" };
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
let csvStream = FastCsv.Read.parse ();

FileReadStream.pipe fileStream (FastCsv.Read.writeable csvStream);

FastCsv.Read.on csvStream @@ `data (fun data => {
    Js.log data;
});

FastCsv.Read.on csvStream @@ `end_ (fun () => {
    Js.log "done reading";
});
