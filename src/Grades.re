module Dataset = BsChartjs.Data.Dataset;
module MapStr = Belt.Map.String;
let (>.) = BsAbstract.Function.Infix.(>.);
let ((<#>), (|?)) = BsAbstract.Option.Infix.((<#>), (|?));

type t = {
  partition_key: string,
  sort_key: string,
  studentid: int,
  schoolyear: string,
  mp: int,
  course: string,
  unixstamp: float,
  grade: float,
};

external decode: Js.Json.t => array(t) = "%identity";

let groupByCourse: array(t) => array((string, array(t))) =
  Array.to_list
  >. List.fold_left(
       (a, {course} as v) =>
         [v]
         |? (MapStr.get(a, course) <#> List.append([v]))
         |> MapStr.set(a, course),
       MapStr.empty,
     )
  >. MapStr.toArray
  >. Array.(map(((course, grades)) => (course, of_list(grades))));

let toDatum = ({unixstamp, grade}) =>
  Dataset.Datum.make(~x=Js.Date.fromFloat(unixstamp), ~y=grade, ());

let toData = grades => {
  Random.init(319904352);

  groupByCourse(grades)
  |> Array.map(((course, grades)) => {
       let color = Random.(int(256), int(256), int(256), 0.5);

       Dataset.make(
         ~label=course,
         ~borderColor=Color.RGBA.show(color),
         ~borderDash=(5, 2),
         ~backgroundColor=Color.RGBA.(show(alpha(1.0, color))),
         ~lineTension=0.,
         ~fill=false,
         ~data=Array.map(toDatum, grades),
         (),
       );
     });
};
