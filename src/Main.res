module Option = Belt.Option

type grades =
  | MissingParams
  | LoadingGrades
  | ErrorFetchingGrades(Js.Promise.error)
  | LoadedGrades(array<Grades.t>)
type segments = array<(string, array<Grades.t>)>
type studentID = option<int>
type schoolYear = option<string>
type markingPeriod = option<int>
type schoolYears = list<string>
type markingPeriods = list<int>

module SetStr = Belt.Set.String
module SetInt = Belt.Set.Int
module MapStr = Belt.Map.String
module GradeFetcher = Fetcher.Make({
  type t = array<Grades.t>
  let decode = Grades.decode
})

@react.component
let make = (~baseUrl) => {
  let (grades, setGrades) = React.useState(() => Fetcher.MissingParams)
  let (segments: segments, setSegments) = React.useState(() => [])
  let (studentID: studentID, setStudentID) = React.useState(() => None)
  let (schoolYear, setSchoolYear) = React.useState(() => None)
  let (markingPeriod: markingPeriod, setMarkingPeriod) = React.useState(() => None)
  let (schoolYears, setSchoolYears) = React.useState(() => [])
  let (markingPeriods, setMarkingPeriods) = React.useState(() => [])

  let url = studentID->Option.map(studentID => baseUrl ++ "/" ++ string_of_int(studentID))

  /* fetch the grades for the indicated student */
  React.useEffect1(GradeFetcher.effect(setGrades, url), [studentID])

  /* get the available school years from the grades */
  React.useEffect1(() => {
    switch grades {
    | Fetcher.Loaded(grades) =>
      let schoolYears =
        grades
        |> Array.fold_left(
          (a, {Grades.schoolyear: schoolyear}) => SetStr.add(a, schoolyear),
          SetStr.empty,
        )
        |> SetStr.toArray

      if !Array.exists(y => Some(y) == schoolYear, schoolYears) {
        setSchoolYear(_ => None)
      }

      setSchoolYears(_ => schoolYears)
    | _ =>
      setSchoolYear(_ => None)
      setSchoolYears(_ => [])
    }
    None
  }, [grades])

  /* get the available marking periods from the grades */
  React.useEffect2(() => {
    switch grades {
    | Fetcher.Loaded(grades) =>
      let markingPeriods =
        grades
        |> Array.fold_left(
          (a, {Grades.mp: mp, schoolyear}) =>
            schoolYear == Some(schoolyear) ? SetInt.add(a, mp) : a,
          SetInt.empty,
        )
        |> SetInt.toArray

      if !Array.exists(y => Some(y) == markingPeriod, markingPeriods) {
        setMarkingPeriod(_ => None)
      }

      setMarkingPeriods(_ => Array.map(string_of_int, markingPeriods))
    | _ =>
      setMarkingPeriod(_ => None)
      setMarkingPeriods(_ => [])
    }
    None
  }, (schoolYear, schoolYears))

  /* filter the current grades by school year and marking period */
  React.useEffect3(() => {
    switch grades {
    | Fetcher.Loaded(grades) =>
      let segments = grades |> Array.fold_left((map, grades: Grades.t) => {
          let key = grades.schoolyear ++ (" MP" ++ string_of_int(grades.mp))

          switch (markingPeriod, schoolYear) {
          | (Some(markingPeriod), _) if markingPeriod != grades.mp => map
          | (_, Some(schoolYear)) if schoolYear != grades.schoolyear => map
          | _ =>
            MapStr.get(map, key)
            ->Option.map(List.append(list{grades}))
            ->Option.getWithDefault(list{grades}) |> MapStr.set(map, key)
          }
        }, MapStr.empty) |> MapStr.toArray |> Array.map(((title, grades)) => (
          title,
          Array.of_list(grades),
        ))

      setSegments(_ => segments)
    | _ => setSegments(_ => [])
    }

    None
  }, (grades, schoolYear, markingPeriod))

  <article className="cf ph2-ns pa4-ns black-80">
    <div className="fl w-100 w-third-ns pa2">
      <FormComponent>
        <FormComponent.Input
          label="Student ID:"
          type_="number"
          value={studentID->Option.mapWithDefault("", string_of_int)}
          setState={/* convert the value to an int */
          f => setStudentID(_ => f()->Option.map(int_of_string))}
        />
        {Array.length(schoolYears) > 0
          ? <FormComponent.Select
              label="School Year:"
              options=schoolYears
              value={schoolYear->Option.getWithDefault("")}
              setState=setSchoolYear
            />
          : React.null}
        {Array.length(markingPeriods) > 0
          ? <FormComponent.Select
              label="Marking Period:"
              options=markingPeriods
              value={markingPeriod->Option.mapWithDefault("", string_of_int)}
              setState={/* convert the value to an int */
              f => setMarkingPeriod(_ => f()->Option.map(int_of_string))}
            />
          : React.null}
      </FormComponent>
    </div>
    <div className="fl w-100 w-two-thirds-ns pa2"> <ChartComponent grades=segments /> </div>
  </article>
}
