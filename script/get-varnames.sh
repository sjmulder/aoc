find . -name '*.c' |
xargs clang -Xclang -ast-dump=json -fsyntax-only |
jq -r --unbuffered '
  .. |
  objects |
  select(.kind == "VarDecl" or .kind == "ParamVarDecl") |
  select(.loc != null) |
  select(.loc.includedFrom == null) |
  .name'
