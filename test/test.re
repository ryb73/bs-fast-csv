open Js.Promise;

open NodeFs;

module CsvFormatter =
  FastCsv.Format(
    {
      type t = {
        .
        "one": string,
        "two": string
      };
    }
  );

FileWriteStream.create(
  "out.csv",
  Js.undefined,
  (fileStream) =>
    CsvFormatter.create(
      Js.undefined,
      (csvStream) => {
        CsvFormatter.pipe(csvStream, FileWriteStream.writeable(fileStream));
        CsvFormatter.write(csvStream, {"one": "ok", "two": "hey"});
        CsvFormatter.write(csvStream, {"one": "two", "two": "three"});
        /*ignore (Js.Exn.raiseError "This is an error");*/
        resolve()
      }
    )
)
|> then_(
     (_) => {
       Js.log("after write");
       resolve()
     }
   )
|> catch(
     (err) => {
       Js.log("caught an error:");
       Js.log(err);
       resolve()
     }
   );

let fileStream = FileReadStream.create("out.csv", Js.undefined);

let csvStream = FastCsv.parse();

FileReadStream.pipe(fileStream, FastCsv.Parse.writeable(csvStream));

FastCsv.Parse.on(csvStream) @@ `data((data) => Js.log(data));

FastCsv.Parse.on(csvStream) @@ `end_(() => Js.log("done reading"));
/* This shouldn't typecheck */
/*FastCsv.Read.pipe csvStream (FastCsv.Read.writeable csvStream);*/
