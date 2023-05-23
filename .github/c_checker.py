#!/usr/bin/env python3

import os, websocket

working_dir = os.getcwd()
hasError = False
filesReceived = 0
TOKENS_TXT = working_dir + "/.github/checker/tokens.txt"
# Get all files to parse

def isIgnore(filename):
    """
    -not -path './bonus/*' -not -path './tests/*' -not -path './libs/myteams/*'
    """
    patterns = ["./bonus/", "./tests/", "./libs/"]
    for pattern in patterns:
        if filename.startswith(pattern):
            return True
    return False


def getFiles():
    files = []
    final_files = []
    # exclude files in bonus and tests folders
    os.system("find . -name '*.c' -o -name '*.h' > files.txt")
    with open("files.txt", "r") as f:
        files = f.read().split("\n")
        for (i, file) in enumerate(files):
            if not (len(file) == 0 or isIgnore(file)):
                final_files.append(working_dir + "/" + file)
    os.system("rm files.txt")
    return final_files
files = getFiles()

def on_message(ws: websocket.WebSocketApp, message):
    global filesReceived
    global hasError
    filesReceived += 1

    with open(TOKENS_TXT, "w+") as f:
        f.write(message)

    os.system(f"python3 .github/checker/runner.py {TOKENS_TXT} {files[filesReceived - 1]}")
    with open(".github/checker/logs.log", "r") as f:
        tokens = f.read()
        if len(tokens) != 0:
            print("Error(s) found in file " + files[filesReceived - 1] + ".")
            hasError = True

    if filesReceived == len(files):
        ws.close()

def on_error(ws, error):
    print(f"Connection error: {error}")
    exit(1)

def on_close(ws):
    exit(0)

def on_open(ws: websocket.WebSocketApp):
    for file in files:
        with open(file, "r") as f:
            ws.send(f.read())
    if len(files) == 0:
        print("No error found")
        ws.close()
        exit(0)

def checkCodingStyle():
    # websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://54.36.183.139:8081/",
                                on_message=on_message,
                                on_error=on_error,
                                on_close=on_close)
    ws.on_open = on_open
    ws.timeout = 10
    ws.run_forever()

    if not hasError:
        print("No error found")
        exit(0)
    exit(1)

import signal

def signal_handler(signum, frame):
    print("Timed out!")
    exit(1)

signal.signal(signal.SIGALRM, signal_handler)
signal.alarm(30)   # 30 seconds
try:
    checkCodingStyle()
except Exception as e:
    print(e)
    exit(1)
