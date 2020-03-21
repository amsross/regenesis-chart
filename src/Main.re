module Option = {
  include BsAbstract.Option.Infix;
  let map = BsAbstract.Option.Functor.map;
};
let ((<|>), (<#>), (<$>), (|?)) = Option.((<|>), (<#>), (<$>), (|?));

let (>.) = (f, g, x) => g(f(x));
[@bs.val] external fetch: string => Js.Promise.t('a) = "fetch";

type grades =
  | MissingParams
  | LoadingGrades
  | ErrorFetchingGrades(Js.Promise.error)
  | LoadedGrades(array(Grades.t));
type segments = array((string, array(Grades.t)));
type studentID = option(int);
type schoolYear = option(string);
type markingPeriod = option(int);
type schoolYears = list(string);
type markingPeriods = list(int);

let buildUrl = baseUrl => (<$>)(string_of_int >. (++)(baseUrl ++ "/"));

module SetStr = Belt.Set.String;
module SetInt = Belt.Set.Int;
module MapStr = Belt.Map.String;
module GradeFetcher =
  Fetcher.Make({
    type t = array(Grades.t);
    let decode = Grades.decode;
  });

[@react.component]
let make = (~baseUrl) => {
  let (grades, setGrades) = React.useState(() => Fetcher.MissingParams);
  let (segments: segments, setSegments) = React.useState(() => [||]);
  let (studentID: studentID, setStudentID) = React.useState(() => None);
  let (schoolYear, setSchoolYear) = React.useState(() => None);
  let (markingPeriod: markingPeriod, setMarkingPeriod) =
    React.useState(() => None);
  let (schoolYears, setSchoolYears) = React.useState(() => [||]);
  let (markingPeriods, setMarkingPeriods) = React.useState(() => [||]);

  let url = buildUrl(baseUrl, studentID);

  /* fetch the grades for the indicated student */
  React.useEffect1(GradeFetcher.effect(setGrades, url), [|studentID|]);

  /* get the available school years from the grades */
  React.useEffect1(
    () => {
      switch (grades) {
      | Fetcher.Loaded(grades) =>
        let schoolYears =
          grades
          |> Array.fold_left(
               (a, Grades.{schoolyear}) => SetStr.add(a, schoolyear),
               SetStr.empty,
             )
          |> SetStr.toArray;

        if (!Array.exists(y => Some(y) == schoolYear, schoolYears)) {
          setSchoolYear(_ => None);
        };

        setSchoolYears(_ => schoolYears);
      | _ =>
        setSchoolYear(_ => None);
        setSchoolYears(_ => [||]);
      };
      None;
    },
    [|grades|],
  );

  /* get the available marking periods from the grades */
  React.useEffect2(
    () => {
      switch (grades) {
      | Fetcher.Loaded(grades) =>
        let markingPeriods =
          grades
          |> Array.fold_left(
               (a, Grades.{mp, schoolyear}) =>
                 schoolYear == Some(schoolyear) ? SetInt.add(a, mp) : a,
               SetInt.empty,
             )
          |> SetInt.toArray;

        if (!Array.exists(y => Some(y) == markingPeriod, markingPeriods)) {
          setMarkingPeriod(_ => None);
        };

        setMarkingPeriods(_ => Array.map(string_of_int, markingPeriods));
      | _ =>
        setMarkingPeriod(_ => None);
        setMarkingPeriods(_ => [||]);
      };
      None;
    },
    (schoolYear, schoolYears),
  );

  /* filter the current grades by school year and marking period */
  React.useEffect3(
    () => {
      switch (grades) {
      | Fetcher.Loaded(grades) =>
        let segments =
          grades
          |> Array.to_list
          |> List.filter((Grades.{mp}) => mp |? markingPeriod == mp)
          |> List.filter((Grades.{schoolyear}) =>
               schoolyear |? schoolYear == schoolyear
             )
          |> List.fold_left(
               (a, v: Grades.t) => {
                 let key = v.schoolyear ++ " MP" ++ string_of_int(v.mp);

                 [v]
                 |? (MapStr.get(a, key) <#> List.append([v]))
                 |> MapStr.set(a, key);
               },
               MapStr.empty,
             )
          |> MapStr.toArray
          |> Array.map(((title, grades)) => (title, Array.of_list(grades)));

        setSegments(_ => segments);
      | _ => setSegments(_ => [||])
      };

      None;
    },
    (grades, schoolYear, markingPeriod),
  );

  <article className="cf ph2-ns pa4-ns black-80">
    <div className="fl w-100 w-third-ns pa2">
      <FormComponent>
        <FormComponent.Input
          label="Student ID:"
          type_="number"
          value={"" |? (string_of_int <$> studentID)}
          /* convert the value to an int */
          setState={f => setStudentID(_ => int_of_string <$> f())}
        />
        {Array.length(schoolYears) > 0
           ? <FormComponent.Select
               label="School Year:"
               options=schoolYears
               value={"" |? schoolYear}
               setState=setSchoolYear
             />
           : React.null}
        {Array.length(markingPeriods) > 0
           ? <FormComponent.Select
               label="Marking Period:"
               options=markingPeriods
               value={"" |? (string_of_int <$> markingPeriod)}
               /* convert the value to an int */
               setState={f => setMarkingPeriod(_ => int_of_string <$> f())}
             />
           : React.null}
      </FormComponent>
    </div>
    <div className="fl w-100 w-two-thirds-ns pa2">
      <ChartComponent grades=segments />
    </div>
  </article>;
};
