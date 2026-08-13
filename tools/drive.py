"""pty-based smoke test driver.

Feeds a scripted keystroke sequence to the editor binary under a real
pseudo-terminal (ncurses requires one) and reports how the process exited -
a clean exit, or the signal that killed it (e.g. 11 for SIGSEGV).

Usage: python3 drive.py <path-to-binary> "<keys to type>" [--yes-to-prompts]

Used as a regression check: the crash fixed in commit e1308b4 (double space
triggering an unsigned-underflow in the spell checker) is reproduced by
`python3 drive.py ./build/texteditor "This  is"`.
"""
import os
import pty
import sys
import time
import select
import signal


def main():
    if len(sys.argv) < 2:
        print("usage: drive.py <binary> [keys] [--cwd DIR]")
        return 1

    binpath = os.path.abspath(sys.argv[1])
    keys = sys.argv[2] if len(sys.argv) > 2 else ""
    cwd = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # repo root

    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(cwd)
        os.execvp(binpath, [binpath])
        return 0  # unreachable

    time.sleep(0.5)
    for ch in keys:
        os.write(fd, ch.encode())
        time.sleep(0.03)
    time.sleep(0.3)
    os.write(fd, b"\x1b")  # ESC to quit
    time.sleep(0.2)
    os.write(fd, b"y")     # confirm "unsaved changes" prompt if it appears
    time.sleep(0.3)

    output = b""
    try:
        while True:
            r, _, _ = select.select([fd], [], [], 0.5)
            if not r:
                break
            data = os.read(fd, 65536)
            if not data:
                break
            output += data
    except OSError:
        pass

    wpid, status = os.waitpid(pid, os.WNOHANG)
    if wpid == 0:
        time.sleep(1)
        wpid, status = os.waitpid(pid, os.WNOHANG)

    print("=== STATUS ===")
    if wpid == 0:
        print("still running, killing")
        os.kill(pid, signal.SIGKILL)
        os.waitpid(pid, 0)
        return 1
    if os.WIFSIGNALED(status):
        print("Killed by signal:", os.WTERMSIG(status))
        return 1
    if os.WIFEXITED(status):
        print("Exited with code:", os.WEXITSTATUS(status))
        return os.WEXITSTATUS(status)
    return 1


if __name__ == "__main__":
    sys.exit(main())
