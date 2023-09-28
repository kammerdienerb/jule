load-package "packages/file"
load-package "packages/sys"

fn (err msg)
    do
        println msg
        exit 1

if (< (len sys:argv) 3) (err "missing file argument")

local path (sys:argv 2)

if (== nil (local ifile (file:open-rd path)))
    err (fmt "error opening file '%'" path)

local lines (file:read-lines ifile)
local n     (len lines)
local dig   0

while (> n 0)
    do
        local n (// n 10)
        ++ dig

local ofile (file:open-wr "/dev/stdout")

local i 1
foreach line lines
    file:write ofile (fmt " % │ %\n" (pad dig (++ i)) line)

file:close ofile

file:close ifile
