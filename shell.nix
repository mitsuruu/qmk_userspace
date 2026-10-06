{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShellNoCC {
  packages = with pkgs; [
    clang
    dos2unix
    just
    qmk
  ];
}
