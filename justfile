name := "ipbm"

build:
    gcc -Wall -o ./target/{{name}} src/*.c

[default]
test address="10": build
    ./target/{{name}} {{address}}
    hexdump -C code.ips

