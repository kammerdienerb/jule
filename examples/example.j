fn (A arg)
    println "in A" # I'm printing something
    println arg

fn (B arg)
    println arg
    A "bar"
    println arg

# comment on a line

B "foo" # no space

println
    list
        object
            . "a" 123
            . "b" 456
            . "c" 789
        object
            . 123 "a"
            . 456 "b"
            . 789 "c"

fn (true)
    println "TRUE"
    1

fn (false)
    println "FALSE"
    0

println (and true false)

if 0 (println "hi")
else (println "bye")

set my-object
    object
        . "test" 777

fn (test object field)
    println field
    if (in object field)
        println "object has the field!"
    else
        println "object does not have the field"

test my-object "test"
test my-object "foo"

println (field my-object "test")
println (my-object "test")

set i 0

while (< i 10)
    println "hi"
    set i (+ 1 i)

set thing
    foreach i (range 0 10)
        println "bye"
println thing
set thing nil
println thing
while (!= nil thing)
    println "foo"
println "-----------"

set OBJ
    object
        . "foo" object
println OBJ
println "-----------"
insert OBJ "my-field" 789
println OBJ
println "-----------"
delete OBJ "foo"
println OBJ
println "-----------"


foreach i (list 1 2 3 4 5 6 7 8 9)
    println i


fn (fib n)
    select (<= n 2)
        != n 0
        + (fib (- n 1)) (fib (- n 2))
println (fib 6)

fn (fact n)
    select (== 1 n)
        n
        * n (fact (- n 1))
println (fact 6)

eval-file "examples/foo.j"
test:foo

println
    (lambda (x) (fmt "we also have %s" x))
        "lambda"
