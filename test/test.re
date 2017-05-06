let csvStream = FastCsv.createWriteStream Js.undefined;
let fileStream = Node.Fs.createWriteStream "out.csv" Js.undefined;

FastCsv.pipe csvStream fileStream;

FastCsv.write csvStream [%bs.obj { one: "ok", two: "yeaah" }];

FastCsv.close csvStream;