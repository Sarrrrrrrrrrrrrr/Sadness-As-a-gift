# 01 — Stack Buffer Overflow

## The bug

```c
void vulnerable_function() {
    char buffer[64];
    printf("Enter your name: ");
    scanf("%s", buffer);       // <- no size limit
    printf("Hello, %s!\n", buffer);
}
```

`scanf("%s", buffer)` copies input into `buffer` until it hits whitespace —
it never checks that the input fits in the 64 bytes we allocated. On the
stack, `buffer` sits right below the saved base pointer and the **return
address** (the address the CPU jumps back to when the function ends). Write
past byte 64 and you start overwriting those — including the return
address. Control where it points, and you control where the program jumps
next.

There's also a `win()` function that main() never calls. Our goal: get the
CPU to jump there anyway.

## Build it

```sh
./build.sh
```

This compiles with `-fno-stack-protector -no-pie -z execstack` — i.e. with
the compiler's usual defenses turned off on purpose, so the classic
technique works cleanly. (Modern binaries ship with stack canaries, ASLR,
and non-executable stacks by default specifically to stop this attack —
see "How to fix it" below.)

## Try it yourself first

Run `./vulnerable` and just type a very long string at the prompt. It'll
crash (`Segmentation fault`) — you're already smashing the stack, you just
haven't aimed it yet.

To aim it, you need to know exactly how many bytes separate the start of
`buffer` from the return address (the "offset"). The standard way to find
it:

```sh
python3 -c "from pwn import *; print(cyclic(200))" # a De Bruijn pattern, no substring repeats
# feed that into ./vulnerable, let it crash, then in gdb:
#   (gdb) x/wx $rsp        # or check the crash address
# then:
python3 -c "from pwn import *; print(cyclic_find(0x6161616c))"  # whatever bytes were at the crash point
```

For this binary the offset is **72 bytes** (64-byte buffer + 8-byte saved
`rbp`), then the next 8 bytes are the return address.

## The exploit

```sh
python3 exploit.py
```

`exploit.py` builds a payload of `72` junk bytes followed by the address of
`win()` (read straight out of the binary with `pwntools`' `ELF()`), packed
as a little-endian 8-byte value with `p64()`. That overwrites the return
address, so when `vulnerable_function()` returns, execution jumps into
`win()` instead of back to `main()`.

Expect a segfault *after* the flag prints — that's normal. We only
controlled the return address, not what comes after `win()` returns, so
the CPU jumps into garbage next. In a real exploit you'd chain further
addresses (ROP) to land somewhere clean; for this lab, printing the flag
is the win condition.

## How to fix it

- Use bounded input: `scanf("%63s", buffer)` or `fgets(buffer, sizeof(buffer), stdin)`.
- Compile with a stack protector (`-fstack-protector-strong`, on by default
  on most modern toolchains) so a canary value gets checked before the
  function returns, catching the overwrite before it's used.
- Enable ASLR and NX (non-executable stack) at the OS/toolchain level —
  they don't stop the overflow but make it much harder to weaponize.
