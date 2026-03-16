alias c := compile
alias ca := compile-all
alias cdb := compile-db
alias f := flash

[private]
default:
    @just --list --unsorted

[private]
builder goal board *args:
    qmk {{ goal }} -km mitsuruu -kb {{ board }} {{ args }}

[group('compile')]
compile board *args: (builder "compile" board args)

[group('compile')]
compile-db board *args: (builder "compile" board "--compiledb" args)

[group('compile')]
compile-all *args:
    qmk userspace-compile {{ args }}

[group('flash')]
flash board *args: (builder "flash" board args)

[group('util')]
clean:
    qmk clean
    rm *.bin
    rm *.uf2
    rm *.hex
