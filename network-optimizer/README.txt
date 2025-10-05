# Network Cost Optimizer (C)

A command-line **C project** that optimizes network deployment cost by computing **Minimum Spanning Trees (MST)** and **shortest paths** in weighted graphs. Supports **timing experiments**, path reconstruction, and optional graph visualizations.

---

## Features

- **Minimum Spanning Tree (MST)** using **Kruskal + Union-Find**
- **Shortest Path Calculation** using **Dijkstra’s algorithm** with min-heap
- Console output for:
  - MST edges and total cost  
  - Shortest distances from a user-specified source  
  - Optional path to a target node  
- **Timing measurement** for MST and Dijkstra algorithms  
- Optional **graph visualization** (DOT + PDF):
  - MST edges (red)  
  - Shortest path edges (blue)  
  - Source node (green, labeled S)  
  - Target node (orange, labeled T)  
- Random **graph generator** for testing and experiments  
- Experiment scripts to measure algorithm scaling  

---
