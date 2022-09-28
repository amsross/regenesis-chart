@val @scope("window") external baseUrl: Js.Nullable.t<string> = "baseUrl"

switch (ReactDOM.querySelector("#root"), baseUrl->Js.Nullable.toOption) {
| (Some(root), Some(baseUrl)) => ReactDOM.render(<Main baseUrl />, root)
| _ => Js.Console.log("something went wrong")
}
