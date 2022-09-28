module Promise = Js.Promise

module Fetch = {
  module Response = {
    type t = {
      ok: bool,
      status: int,
    }

    @send external json: t => Promise.t<Js.Json.t> = "json"
  }

  @val external fetch: string => Promise.t<Response.t> = "fetch"
}

module type Config = {
  type t
  let decode: Js.Json.t => t
}

type t<'t> =
  | MissingParams
  | Loading
  | ErrorFetching(Promise.error)
  | Loaded('t)

module Make = (Config: Config) => {
  let fetch: string => Promise.t<t<Config.t>> = url => {
    Fetch.fetch(url)
    |> Promise.then_(response => response->Fetch.Response.json)
    |> Promise.then_(json => Promise.resolve(Loaded(Config.decode(json))))
    |> Promise.catch(err => Promise.resolve(ErrorFetching(err)))
  }

  let effect = setter => {
    let cancelled = ref(false)

    let withoutURL = () => {
      setter(_ => MissingParams)
      None
    }

    let withURL = (url, ()) => {
      setter(_ => Loading)

      fetch(url)
      |> Promise.then_(v => {
        if !cancelled.contents {
          setter(_ => v)
        }

        Promise.resolve()
      })
      |> ignore

      Some(() => cancelled := true)
    }

    x =>
      switch x {
      | None => withoutURL
      | Some(url) => withURL(url)
      }
  }
}
