[@bs.val]
external fetch:
  string =>
  Js.Promise.t({. [@bs.meth] "json": unit => Js.Promise.t(Js.Json.t)}) =
  "fetch";

module type Config = {
  type t;
  let decode: Js.Json.t => t;
};

type t('t) =
  | MissingParams
  | Loading
  | ErrorFetching(Js.Promise.error)
  | Loaded('t);

module Make = (Config: Config) => {
  let fetch: string => Js.Promise.t(t(Config.t)) =
    url =>
      Js.Promise.(
        fetch(url)
        |> then_(response => response##json())
        |> then_(json => resolve(Loaded(Config.decode(json))))
        |> catch(err => resolve(ErrorFetching(err)))
      );

  let effect = setter => {
    let cancelled = ref(false);

    let withoutURL = () => {
      setter(_ => MissingParams);
      None;
    };

    let withURL = (url, ()) => {
      setter(_ => Loading);

      fetch(url)
      |> Js.Promise.(
           then_(v => {
             if (! cancelled^) {
               setter(_ => v);
             };

             resolve();
           })
         )
      |> ignore;

      Some(() => cancelled := true);
    };

    fun
    | None => withoutURL
    | Some(url) => withURL(url);
  };
};
