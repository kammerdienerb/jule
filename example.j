fn
    A arg
    print "in A" # I'm printing something
    print arg

fn
    B arg
    print arg
    A "bar"
    print arg

# comment on a line

B "foo" # no space

print
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
    print "TRUE"
    1

fn
    false
    print "FALSE"
    0

print
    and true false

if 0
     print "hi"
     print "bye"

set my-object
    object
        . "test" 777

fn
    test object field
    print field
    if
        in object field
        print "object has the field!"
        print "object does not have the field"

test my-object "test"
test my-object "foo"

print
    field my-object "test"

set i 0

while
    < i 10
    print "hi"
    set i
        + 1 i

set thing
    repeat 0
        print "bye"
print "-----------"
print thing
while
    != nil thing
    print "foo"

print
    set OBJ
        object
            . "foo" object

print
    set OBJ
        insert OBJ "my-field" 789
print
    set OBJ
        delete OBJ "foo"

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

print
    fib 6

fn
    fact n
    if
        == 1 n
        n
        *   n
            fact
                - n 1
print
    fact 6
