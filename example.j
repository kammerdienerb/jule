fn
    A arg
    do
        println "in A" # I'm printing something
        println arg

fn
    B arg
    do
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

fn
    true
    do
        println "TRUE"
        1

fn
    false
    do
        println "FALSE"
        0

println
    and true false

if 0
     println "hi"
     println "bye"

set my-object
    object
        . "test" 777

fn
    test object field
    do
        println field
        if
            in object field
            println "object has the field!"
            println "object does not have the field"

test my-object "test"
test my-object "foo"

println
    field my-object "test"

set i 0

while
    < i 10
    do
        println "hi"
        set i
            + 1 i

set thing
    repeat i 0
        println "bye"
println "-----------"
println thing
while
    != nil thing
    println "foo"

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
fn
    fib n
    if
        <= n 2
        != n 0
        +
            fib
                - n 1
            fib
                - n 2

println
    fib 6

fn
    fact n
    if
        == 1 n
        n
        *   n
            fact
                - n 1
println
    fact 6

eval-file "foo.j"
test:foo
