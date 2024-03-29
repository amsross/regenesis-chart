@val @scope("window") external innerHeight: float = "innerHeight"
@val @scope("window") external innerWidth: float = "innerWidth"

let makeChart = (title, datasets) => {
  open ChartJs

  let yAxes = [
    {
      open Options.Options.Scales.Axis
      make(
        ~_type=#linear,
        ~ticks=Ticks.make(~suggestedMin=70, ~suggestedMax=105, ~stepSize=5, ()),
        ~display=true,
        (),
      )
    },
  ]

  let xAxes = [
    {
      open Options.Options.Scales.Axis
      make(
        ~_type=#time,
        ~distribution=#linear,
        ~time=Time.make(~round=true, ~_unit="day", ()),
        ~display=true,
        (),
      )
    },
  ]

  let options = {
    open Options
    make(
      ~_type=#line,
      ~data=Data.make(~datasets, ()),
      ~options={
        open Options
        make(
          ~responsive=true,
          ~maintainAspectRatio=true,
          ~aspectRatio=0.75 *. innerWidth /. innerHeight,
          ~title=Title.make(~display=true, ~text=title, ()),
          ~scales={
            open Scales
            make(~xAxes, ~yAxes, ())
          },
          (),
        )
      },
      (),
    )
  }

  canvas => ChartJs.make(canvas, options)
}

let handleCanvas = (title, grades, canvas) =>
  Js.Nullable.toOption(canvas)->Belt.Option.map(makeChart(title, grades))->ignore

@react.component
let make = (~grades: array<(string, array<Grades.t>)>) => {
  Array.sort(((left, _), (right, _)) => compare(right, left), grades)

  <div>
    {React.array(
      grades |> Array.map(((title, grades)) =>
        <canvas
          ref={ReactDOM.Ref.callbackDomRef(handleCanvas(title, Grades.toData(grades)))}
          key=title
          width="1000"
        />
      ),
    )}
  </div>
}
