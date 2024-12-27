{
  description = "Simple Window Manager";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    nix-vscode-extensions.url = "github:nix-community/nix-vscode-extensions";

    nix-code = {
      url = "github:fxttr/nix-code";
      inputs.extensions.follows = "nix-vscode-extensions";
    };
  };

  outputs = { self, nixpkgs, flake-utils, ... }@inputs: flake-utils.lib.eachSystem [
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
            nixpkgs-fmt

            (inputs.nix-code.vscode.${system} {
              extensions = with inputs.nix-code.extensions.${system}; [
                bbenoist.nix
                jnoortheen.nix-ide
                mkhl.direnv
                llvm-vs-code-extensions.vscode-clangd
              ];
            })
          ];
        };

        defaultPackage = pkgs.llvmPackages.stdenv.mkDerivation
          rec {
            pname = "swm";
            version = "0.1.0";

            src = ./.;

            prePatch = ''
              sed -i "s@/usr@$out@" Makefile
            '';

            nativeBuildInputs = with pkgs; [ gnumake ];

            buildInputs = with pkgs; [
              xorg.libX11
              xorg.libXft
              xorg.libXinerama
            ];

            meta = with nixpkgs.lib; {
              homepage = "https://github.com/fxttr/swm";
              description = ''
                A simple window manager for X11";
              '';
              licencse = licenses.mit;
              platforms = with platforms; linux;
              maintainers = [ maintainers.fxttr ];
            };
          };
      });
}

