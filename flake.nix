{
  description = "Simple Window Manager";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, ... }@inputs: inputs.utils.lib.eachSystem [
    "x86_64-linux"
    "i686-linux"
    "aarch64-linux"
  ]
    (system:
      let
        pkgs = import nixpkgs {
          inherit system;

          overlays = [ ];
        };
      in
      {
        devShells.default = pkgs.mkShell rec {
          name = "swm";

          packages = with pkgs; [
            # Build tools
            gnumake

            # LLVM
            llvmPackages.lldb
            llvmPackages.clang
            llvmPackages.llvm
            clang-tools
            llvmPackages.libclang

            # Libs
            xorg.libX11
            xorg.libXft
            xorg.libXinerama

            # Tools
            valgrind
          ];
        };

        packages.default = pkgs.callPackage ./default.nix { };
      });
}
