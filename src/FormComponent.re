module Input = {
  [@react.component]
  let make = (~label, ~type_, ~value, ~setState) => {
    <div className="mt2">
      <label className="db fw4 lh-copy f2 f4-ns f6-m">
        {React.string(label)}
      </label>
      <input
        className="pa2 input-reset ba bg-transparent w-100 measure"
        type_
        value
        onChange={evt =>
          evt->ReactEvent.Form.target##value
          ->((v, _) => v == "" ? None : Some(v))
          ->setState
        }
      />
    </div>;
  };
};

module Select = {
  [@react.component]
  let make = (~label, ~options, ~value, ~setState) => {
    <div className="mt2">
      <label className="db fw4 lh-copy f2 f4-ns f6-m">
        {React.string(label)}
      </label>
      <select
        className="pa2 input-reset ba bg-transparent w-100 measure"
        defaultValue=value
        onChange={evt =>
          evt->ReactEvent.Form.target##value
          ->((v, _) => v == "" ? None : Some(v))
          ->setState
        }>
        <option />
        {options
         |> Array.map(o => {<option key=o> {React.string(o)} </option>})
         |> ReasonReact.array}
      </select>
    </div>;
  };
};

[@react.component]
let make = (~children) =>
  <form onSubmit={evt => evt->ReactEvent.Form.preventDefault}>
    <fieldset className="ba b--transparent ph0 mh0"> children </fieldset>
  </form>;
