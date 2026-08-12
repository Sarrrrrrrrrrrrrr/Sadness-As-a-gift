# 02 — Format String Vulnerability

## The bug

```c
fgets(buf, sizeof(buf), stdin);
printf(buf);   // <- should be printf("%s", buf)
```

`printf` treats its first argument as a **format string** and scans it for
`%` conversion specifiers, pulling one "argument" off the stack/registers
for each one it finds. Here, the format string *is* our input. If we type
`%p`, printf doesn't print the literal characters `%p` — it thinks we
passed it a pointer argument and reads one off the stack to print, even
though we never passed anything. Type enough of them, and printf walks
further and further through memory it was never meant to expose,
one 8-byte word per specifier.

`feedback_form()` also has a local array `secret_flag[]`, declared right
before `buf[]`, that's never printed anywhere in the code. Our goal: read
it anyway, purely through the format string bug.

## Build it

```sh
./build.sh
```

## Find the leak yourself

Run `./vulnerable` and, at the prompt, paste a chain of positional
specifiers to dump the stack:

```
%1$p|%2$p|%3$p|%4$p|%5$p|%6$p|%7$p|%8$p|...|%25$p
```

(`%N$p` means "print argument N as a pointer" — the `N$` lets you pick
*which* stack slot to read instead of just taking them in order, which
makes probing much easier.)

If you sweep far enough you'll notice two things:
1. A run of values that spell out **your own input** in hex — that's
   `buf` itself being read back, because it's sitting on the stack right
   where printf is looking.
2. Right after `buf`'s slots run out, the values stop looking like your
   input and start looking like something else — that's `secret_flag[]`,
   which lives in the very next stack slots.

In this build, `buf` occupies argument slots 6–21 (128 bytes / 8 =
16 slots), so `secret_flag` starts at **argument 22**. You can confirm this
yourself by comparing the hex output of `%6$p` against your own input, or
by attaching `gdb` and checking where `secret_flag` actually lives relative
to `buf` (`p &secret_flag`, `p &buf`).

## The exploit

```sh
python3 exploit.py
```

Each `%p` gives us 8 raw bytes of stack memory, printed as a hex number.
Since x86-64 is little-endian, reversing those bytes back into ASCII
reconstructs the original string. `exploit.py` sends `%22$p|%23$p|...`,
parses each hex value with `int(token, 16).to_bytes(8, "little")`, and
concatenates the results — which spells out the flag.

This is an **information disclosure**, not a crash: the program runs to
completion normally. That's what makes format string bugs dangerous in
real services — they can leak canaries, pointers (defeating ASLR), or
secrets silently, with no obvious sign anything happened.

(The stronger version of this bug uses `%n`, which tells `printf` to
*write* the number of bytes output so far into a pointer you control —
turning a leak into an arbitrary memory write. It's not used in this lab
because reliably placing a writable target address as a "real" stack
argument — without a stray NUL byte in that address truncating the format
string before your `%n` directive is even reached — depends on the exact
stack layout and libc build. The `%p` leak here is the reliable, portable
version of the same root cause.)

## How to fix it

- Never pass user input as the format string. Always write
  `printf("%s", buf)`.
- Compile with `-Wformat -Wformat-security -Werror=format-security` —
  GCC/Clang will refuse to compile `printf(buf)` with a non-literal format
  string when these are on. (This lab's `build.sh` explicitly disables that
  warning with `-Wno-format-security` so it compiles — try removing that
  flag and rebuilding to see the warning GCC would normally give you.)
