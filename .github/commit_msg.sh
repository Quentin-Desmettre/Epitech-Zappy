#!/bin/env python3

import sys, re, os

TYPES = ['doc', 'feat', 'fix', 'style', 'test', 'chore', 'ci']
REGEXP = "^[.0-9]+$|^([Rr]evert|[Mm]erge):? .*$|^("

def build_regex():
    global REGEXP
    global TYPES
    for type in TYPES:
        REGEXP += type + "|"
    REGEXP = REGEXP[:-1]
    REGEXP += ")(\(.+\))?: "

def get_commit_message():
    commit_message = ""
    with open(sys.argv[1], "r") as f:
        commit_message = f.readline()
    return commit_message

def print_error(commit_message):
    message = \
    """
    echo -e '\\n\\e[31m[Invalid Commit Message]'
    echo -e '------------------------\\033[0m\\e[0m'
    echo -e 'Valid types: \\e[36m{}\\033[0m'
    echo -e '\\e[37mActual commit message: \\e[33m\"{}\"\\033[0m'
    echo -e '\\e[37mExample valid commit message: \\e[36m\"fix(subject): message\"\\033[0m'
    echo -e '\\e[37mRegex: \\e[33m\"{}\"\\033[0m\\n'
    """.format(TYPES, commit_message[:-1], REGEXP)
    with open("/tmp/commit_msg_error", "w+") as f:
        f.write(message)
    os.system("cat /tmp/commit_msg_error | bash")

if __name__ == "__main__":
    commit_message = get_commit_message()
    build_regex()
    if not re.match(REGEXP, commit_message):
        print_error(commit_message)
        sys.exit(1)
    sys.exit(0)
