name := "ipbm"

build:
    gcc -Wall -o ./target/{{name}} src/*.c

[default]
test mode="l" address="10": build
    ./target/{{name}} {{mode}} {{address}}
    hexdump -C code.ips

