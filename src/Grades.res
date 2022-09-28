module Dataset = ChartJs.Data.Dataset
module MapStr = Belt.Map.String

type t = {
  partition_key: string,
  sort_key: string,
  studentid: int,
  schoolyear: string,
  mp: int,
  course: string,
  unixstamp: float,
  grade: float,
}

let decode: Js.Json.t => array<t> = {
  open Json.Decode
  array(json => {
    partition_key: json |> field("partition_key", string),
    sort_key: json |> field("sort_key", string),
    studentid: json |> field("studentid", oneOf(list{int, json => int_of_string(string(json))})),
    schoolyear: json |> field("schoolyear", string),
    mp: json |> field("mp", oneOf(list{int, json => int_of_string(string(json))})),
    course: json |> field("course", string),
    unixstamp: json |> field(
      "unixstamp",
      oneOf(list{float, json => Js.Float.fromString(string(json))}),
    ),
    grade: json |> field("grade", oneOf(list{float, json => Js.Float.fromString(string(json))})),
  })
}

let groupByCourse: array<t> => array<(string, array<t>)> = arr =>
  arr
  |> Array.to_list
  |> List.fold_left(
    (a, {course} as v) =>
      MapStr.get(a, course)->Belt.Option.mapWithDefault(list{v}, List.append(list{v}))
        |> MapStr.set(a, course),
    MapStr.empty,
  )
  |> MapStr.toArray
  |> Array.map(((course, grades)) => (course, Array.of_list(grades)))

let toDatum = ({unixstamp, grade}) =>
  Dataset.Datum.make(~x=Js.Date.fromFloat(unixstamp), ~y=grade, ())

let toData = grades => {
  Random.init(319904352)

  groupByCourse(grades) |> Array.map(((course, grades)) => {
    let color = {
      open Random
      (int(256), int(256), int(256), 0.5)
    }

    Dataset.make(
      ~label=course,
      ~borderColor=Color.RGBA.show(color),
      ~borderDash=(5, 2),
      ~backgroundColor={
        open Color.RGBA
        show(alpha(1.0, color))
      },
      ~lineTension=0.,
      ~fill=false,
      ~data=Array.map(toDatum, grades),
      (),
    )
  })
}
