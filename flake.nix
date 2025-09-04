{
  description = "Pi Pico dev flake.";

  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  
  outputs = { self, nixpkgs, utils }: utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShell = with pkgs; mkShell {
        packages = [
          tio
        ];
        buildInputs = [
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
      };
    }
  );
}