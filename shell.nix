{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    buildInputs = with pkgs; [
      cmake
      gcc
      gcc-arm-embedded
      gnumake
      pico-sdk
      picotool
      python311
    ];
    shellHook = ''
      export PICO_SDK_DIR=${pkgs.pico-sdk}/lib/pico-sdk
      export PICO_SDK_FETCH_FROM_GIT=on
      export PICO_EXTRAS_FETCH_FROM_GIT=on
      export BOARD=pico
      export PICO_BOARD=pico
    '';
}