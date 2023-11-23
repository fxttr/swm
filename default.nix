{ lib
, llvmPackages
, gnumake
, xorg
}:

llvmPackages.stdenv.mkDerivation rec {
  pname = "swm";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ gnumake ];

  buildInputs = [
    xorg.libX11
    xorg.libXft
    xorg.libXinerama
  ];

  installPhase = ''                                                                               
    mkdir -p $out/bin
    cp swm $out/bin
  '';

  meta = with lib; {
    homepage = "https://github.com/fxttr/swm";
    description = ''
      A simple window manager for X11";
    '';
    licencse = licenses.mit;
    platforms = with platforms; linux;
    maintainers = [ maintainers.fxttr ];
  };
}
