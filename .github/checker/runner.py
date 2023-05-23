import importlib
import sys
import os

module_source_folder = os.path.dirname(sys.argv[0])
os.chdir(module_source_folder)

import vera

vera.loadTokens(open(sys.argv[1]).read(), sys.argv[2])
vera.setSourceFileNames([sys.argv[2]])

modules = [
    "C-A3",
    "C-C1",
    "C-C3",
    "C-F2",
    "C-F3",
    "C-F4",
    "C-F5",
    "C-F6",
    "C-F8",
    "C-F9",
    "C-G1",
    "C-G2",
    "C-G3",
    "C-G4",
    "C-G5",
    "C-G6",
    "C-G7",
    "C-G8",
    "C-H1",
    "C-H2",
    "C-L2",
    "C-L3",
    "C-L4",
    "C-O1",
    "C-O3",
    "C-O4",
    "C-V1"
]
for module in modules:
    if module[-1] == '\n':
        module = module[:-1]
    # if module != "C-L3" and module != "C-L4":
    #     continue
    importlib.import_module(module)
vera.logReports()