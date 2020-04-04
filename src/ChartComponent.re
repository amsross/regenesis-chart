module Option = BsAbstract.Option;
let (>.) = BsAbstract.Function.Infix.(>.);

[@bs.val] [@bs.scope "window"] external innerHeight: float = "innerHeight";
[@bs.val] [@bs.scope "window"] external innerWidth: float = "innerWidth";

let makeChart = (title, datasets) => {
  open ChartJs;

  let yAxes = [|
    Options.Options.Scales.Axis.(
      make(
        ~_type=`linear,
        ~ticks=
          Ticks.make(~suggestedMin=70, ~suggestedMax=105, ~stepSize=5, ()),
        ~display=true,
        ~scaleLabel=ScaleLabel.make(~display=true, ~labelString="Grade", ()),
        (),
      )
    ),
  |];

  let xAxes = [|
    Options.Options.Scales.Axis.(
      make(
        ~_type=`time,
        ~distribution=`linear,
        ~time=Time.make(~round=true, ~_unit="day", ()),
        ~display=true,
        ~scaleLabel=ScaleLabel.make(~display=true, ~labelString="Date", ()),
        (),
      )
    ),
  |];

  let options =
    Options.(
      make(
        ~_type=`line,
        ~data=Data.make(~datasets, ()),
        ~options=
          Options.(
            make(
              ~responsive=true,
              ~maintainAspectRatio=true,
              ~aspectRatio=0.75 *. innerWidth /. innerHeight,
              ~title=Title.make(~display=true, ~text=title, ()),
              ~scales=Scales.(make(~xAxes, ~yAxes, ())),
              (),
            )
          ),
        (),
      )
    );

  canvas => ChartJs.make(canvas, options);
};

let handleCanvas = (title, grades, canvas) =>
  Option.Infix.(
    Js.Nullable.toOption(canvas) <#> makeChart(title, grades) |> ignore
  );

[@react.component]
let make = (~grades: array((string, array(Grades.t)))) => {
  Array.sort(((left, _), (right, _)) => compare(right, left), grades);

  <div>
    {React.array(
       grades
       |> Array.map(((title, grades)) =>
            <canvas
              ref={ReactDOMRe.Ref.callbackDomRef(
                handleCanvas(title, Grades.toData(grades)),
              )}
              key=title
              width="1000"
            />
          ),
     )}
  </div>;
};
