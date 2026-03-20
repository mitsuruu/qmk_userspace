alias c := compile
alias ca := compile-all
alias cdb := compile-db
alias f := flash

[private]
default:
    @just --list --unsorted

[private]
qmk goal board *args:
    qmk {{ goal }} -km mitsuruu -kb {{ board }} {{ args }}

[group('compile')]
compile board *args: (qmk "compile" board args)

[group('compile')]
compile-db board *args: (qmk "compile" board "--compiledb" args)

[group('compile')]
compile-all *args:
    qmk userspace-compile {{ args }}

[group('flash')]
flash board *args: (qmk "flash" board args)

[group('util')]
clean:
    qmk clean
    find . -name '*.bin' -delete
    find . -name '*.uf2' -delete
    find . -name '*.hex' -delete
