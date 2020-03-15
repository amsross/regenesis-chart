module RGBA = {
  type t = (int, int, int, float);

  let alpha: (float, t) => t = (a, (r, g, b, _)) => (r, g, b, a);

  let show: t => string =
    ((r, g, b, a)) =>
      "rgba("
      ++ string_of_int(r)
      ++ ","
      ++ string_of_int(g)
      ++ ","
      ++ string_of_int(b)
      ++ ","
      ++ Js.Float.toString(a)
      ++ ")";
};

type t =
  | Red
  | Orange
  | Yellow
  | Green
  | Blue
  | Indigo
  | Violet;

let toRGB: t => RGBA.t =
  fun
  | Red => (255, 0, 0, 1.0)
  | Orange => (255, 127, 0, 1.0)
  | Yellow => (255, 255, 0, 1.0)
  | Green => (0, 255, 0, 1.0)
  | Blue => (0, 0, 255, 1.0)
  | Indigo => (75, 0, 130, 1.0)
  | Violet => (148, 0, 211, 1.0);

let show: t => string = color => toRGB(color) |> RGBA.show;

let fade: (float, t) => string =
  (a, color) => toRGB(color) |> RGBA.alpha(a) |> RGBA.show;
