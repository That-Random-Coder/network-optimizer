@echo off
if not exist experiments mkdir experiments
gen_graph.exe 100 150 100 experiments\g1.txt
gen_graph.exe 200 400 100 experiments\g2.txt
gen_graph.exe 400 1600 100 experiments\g3.txt
echo file,V,E,kruskal_ms,dijkstra_ms > experiments\results.csv
for %%f in (experiments\g1.txt experiments\g2.txt experiments\g3.txt) do (
  echo Running %%f
  netopt.exe %%f < sample_inputs\sample_graph_input.txt > tmp_out.txt
  for /f "tokens=3" %%a in ('findstr /C:"Kruskal time" tmp_out.txt') do set kr=%%a
  for /f "tokens=3" %%b in ('findstr /C:"Dijkstra time" tmp_out.txt') do set dij=%%b
  for /f "tokens=1,2 delims= " %%x in ('type "%%f" ^| findstr /R "^[0-9]" ^| more +0 ^| findstr /N "." ^| findstr /R "^1:"') do (
  )
  for /f "tokens=1,2" %%x in ('more +0 "%%f" ^| findstr /R "^[0-9]" ^| set /p line= ^& echo !line!') do (
  )
  for /f "usebackq tokens=1,2 delims= " %%x in ("%%f") do (
    set Vv=%%x
    set Ee=%%y
    goto :gotVE
  )
  :gotVE
  echo %%f,%Vv%,%Ee%,%kr%,%dij% >> experiments\results.csv
)
del tmp_out.txt
echo Results written to experiments\results.csv
