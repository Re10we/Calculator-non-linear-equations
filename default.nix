with import <nixpkgs> {};
  
    stdenv.mkDerivation {
      name = "my_prog";
      src = ./.;
      
      buildInputs = [
       pkg-config
       qt6.full
       ninja
                
       lld
       clang-tools
       clang
 
     ];

      shellHook = '' 
        export PATH="${qt6.full}/libexec:$PATH"
        export PATH="${qt6.full}/lib/cmake:$PATH"
      '';      
   }