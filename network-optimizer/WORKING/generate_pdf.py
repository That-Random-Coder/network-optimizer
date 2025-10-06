import sys
import os
import subprocess

# Take dot file path as argument
dot_file = sys.argv[1]

if not os.path.exists(dot_file):
    raise FileNotFoundError(f"DOT file not found: {dot_file}")

# Incremental PDF naming
base = os.path.splitext(os.path.basename(dot_file))[0]  # e.g., "graph"
i = 0
while True:
    pdf_file = f"{base}{i}.pdf"
    if not os.path.exists(pdf_file):
        break
    i += 1

# Full path to dot.exe (change if installed elsewhere)
graphviz_dot = r"C:\Program Files\Graphviz\bin\dot.exe"
if not os.path.exists(graphviz_dot):
    raise FileNotFoundError(f"dot.exe not found at {graphviz_dot}")

# Generate PDF
subprocess.run([graphviz_dot, "-Tpdf", dot_file, "-o", pdf_file], check=True)
print(f"Generated {pdf_file}")
