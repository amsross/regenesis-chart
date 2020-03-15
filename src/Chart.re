type ctx;
type t;

[@bs.send] external getContext: (Dom.element, string) => ctx = "getContext";
[@bs.val] [@bs.scope "window"] external innerHeight: float = "innerHeight";
[@bs.val] [@bs.scope "window"] external innerWidth: float = "innerWidth";

module Options = {
  type dataset = {
    .
    "label": string,
    "lineTension": float,
    "backgroundColor": string,
    "borderColor": string,
    "borderDash": Js.Undefined.t((int, int)),
    "fill": bool,
    "data":
      array({
        .
        "x": Js.Date.t,
        "y": float,
      }),
  };

  type axes =
    array({
      .
      "type": Js.Undefined.t(string),
      "distribution": Js.Undefined.t(string),
      "time":
        Js.Undefined.t({
          .
          "round": bool,
          "unit": string,
        }),
      "ticks": {
        .
        "suggestedMin": Js.Undefined.t(int),
        "suggestedMax": Js.Undefined.t(int),
        "stepSize": Js.Undefined.t(int),
      },
      "display": bool,
      "scaleLabel": {
        .
        "display": bool,
        "labelString": string,
      },
    });

  type options = {
    .
    "responsive": bool,
    "maintainAspectRatio": bool,
    "aspectRatio": float,
    "title": {
      .
      "display": bool,
      "text": string,
    },
    "scales": {
      .
      "xAxes": axes,
      "yAxes": axes,
    },
  };

  type t = {
    .
    "type": string,
    "data": {. "datasets": array(dataset)},
    "options": options,
  };
};

[@bs.new] external newChart: (ctx, Options.t) => t = "Chart";

let make = (title, datasets, canvas) =>
  canvas
  ->getContext("2d")
  ->newChart({
      "type": "line",
      "data": {
        "datasets": datasets,
      },
      "options": {
        "responsive": true,
        "maintainAspectRatio": true,
        "aspectRatio": 0.75 *. innerWidth /. innerHeight,
        "title": {
          "display": true,
          "text": title,
        },
        "scales": {
          "xAxes": [|
            {
              "type": Js.Undefined.return("time"),
              "distribution": Js.Undefined.return("linear"),
              "time": Js.Undefined.return({"round": true, "unit": "day"}),
              "ticks": {
                "suggestedMin": Js.Undefined.empty,
                "suggestedMax": Js.Undefined.empty,
                "stepSize": Js.Undefined.empty,
              },
              "display": true,
              "scaleLabel": {
                "display": true,
                "labelString": "Date",
              },
            },
          |],
          "yAxes": [|
            {
              "type": Js.Undefined.return("linear"),
              "distribution": Js.Undefined.empty,
              "time": Js.Undefined.empty,
              "ticks": {
                "suggestedMin": Js.Undefined.return(70),
                "suggestedMax": Js.Undefined.return(105),
                "stepSize": Js.Undefined.return(5),
              },
              "display": true,
              "scaleLabel": {
                "display": true,
                "labelString": "Grade",
              },
            },
          |],
        },
      },
    });
