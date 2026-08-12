# Sadness-As-a-gift

Hands-on cybersecurity practice labs. Each folder has a small, deliberately
vulnerable program, a README explaining *why* it's vulnerable, and a working
exploit against it. The goal is to build real intuition for common bug
classes by breaking them yourself, not just reading about them.

## ⚠️ Scope & rules

- Everything here is **intentionally vulnerable**. Never deploy any of it,
  never run it against a target you don't own, never expose it to a network
  you don't control.
- All labs are meant to run **locally**, against **localhost**, for
  **learning only**.
- Binary exploitation labs assume Linux (compiled with protections disabled
  on purpose — see each lab's README).

## Labs

| # | Topic | Bug class | Language |
|---|-------|-----------|----------|
| [01](01-buffer-overflow/) | Stack buffer overflow | Memory corruption | C |
| [02](02-format-string/) | Format string vulnerability | Memory corruption / info leak | C |
| [03](03-sql-injection/) | SQL injection (login bypass + data exfil) | Web / injection | Python (Flask) |
| [04](04-command-injection/) | OS command injection | Web / injection | Python (Flask) |

## Suggested order

1. Start with **01** and **02** if you want to understand memory corruption
   at the C/assembly level — they build on each other (stack layout, then
   how format strings abuse the same stack).
2. Then **03** and **04** for web-side injection bugs — same root cause
   (untrusted input reaching a place that interprets it as code/commands),
   different surface.

Each lab README follows the same shape: **what the bug is**, **how to run
it**, **how to exploit it**, **how to fix it**. Try to find the exploit
yourself before reading the "how to exploit it" section.

## Requirements

- `gcc` for the C labs (01, 02)
- `python3` + `pip install flask requests pwntools` for the exploit scripts
  and the web labs (03, 04)
