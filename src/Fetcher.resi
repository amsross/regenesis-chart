type t<'t> =
  | MissingParams
  | Loading
  | ErrorFetching(Js.Promise.error)
  | Loaded('t)

module type Config = {
  type t
  let decode: Js.Json.t => t
}

module Make: (Config: Config) =>
{
  let effect: ((t<Config.t> => t<Config.t>) => unit, option<string>, unit) => option<unit => unit>
}
