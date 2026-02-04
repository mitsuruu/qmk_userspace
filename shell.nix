let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShellNoCC {
    packages = [
      pkgs.qmk
      pkgs.dos2unix
    ];
  }
