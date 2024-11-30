file(REMOVE_RECURSE
  "libGameEngine.a"
  "libGameEngine.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/GameEngine.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
