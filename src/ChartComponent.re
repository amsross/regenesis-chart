module Option = BsAbstract.Option;
let (>.) = BsAbstract.Function.Infix.(>.);

let handleCanvas = (title, grades, canvas) =>
  Option.Infix.(
    Js.Nullable.toOption(canvas) <#> Chart.make(title, grades) |> ignore
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
