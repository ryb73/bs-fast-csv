open Js.Promise;

Node.Fs.createWriteStream "out.csv" Js.undefined (fun fileStream => {
    Some (FastCsv.createWriteStream Js.undefined (fun csvStream => {
        FastCsv.pipe csvStream fileStream;
        FastCsv.write csvStream [%bs.obj { one: "ok", two: "hey" }];
        /*Js.Exn.raiseError "This is an error";*/
        None;
    }));
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