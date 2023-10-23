use-package "file"
use-package "sys"

fn (err msg)
    println msg
    exit 1

if (< (len sys:argv) 2) (err "missing file argument")

local path (sys:argv 1)

if (== nil (local ifile (file:open-rd path)))
    err (fmt "error opening file '%'" path)

local lines (file:read-lines ifile)
local n     (len lines)
local dig   0

while (> n 0)
    local n (// n 10)
    ++ dig

local i 1
foreach line lines
    println (fmt " % │ %" (pad dig (++ i)) line)

file:close ifile
