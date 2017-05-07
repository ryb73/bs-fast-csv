open Js.Promise;

Node.Fs.createWriteStream "out.csv" Js.undefined (fun fileStream => {
    FastCsv.createWriteStream Js.undefined (fun csvStream => {
        FastCsv.pipe csvStream fileStream;
        FastCsv.write csvStream [%bs.obj { one: "ok", two: "hey" }];
        FastCsv.write csvStream [%bs.obj { one: "two", two: "three" }];
        /*ignore (Js.Exn.raiseError "This is an error");*/

        resolve ();
    });
})
    |> catch (fun err => {
        Js.log "caught an error:";
        Js.log err;
        resolve ();
    })
    |> then_ (fun _ => {
        Js.log "after";
        resolve ();
    });