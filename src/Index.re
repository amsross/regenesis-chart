// Entry point

[@bs.val] [@bs.scope "window"] external baseUrl: string = "baseUrl";
[@bs.val] external document: Js.t({..}) = "document";

let body = document##createElement("div");
document##body##appendChild(body) |> ignore;

let makeContainer = text => {
  let container = document##createElement("article");
  container##className #= "center mw9 ph3-ns";

  let title = document##createElement("h1");
  title##className #= "f4 bg-near-white mv0 pv2 ph3";
  title##innerText #= text;

  body##appendChild(container) |> ignore;

  container;
};

ReactDOMRe.render(<Main baseUrl />, makeContainer("Main"));
