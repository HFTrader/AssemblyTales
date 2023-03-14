import json
import matplotlib.pyplot as plt
import os, sys
import re

if len(sys.argv)<=1:
    print("Usage: compile_benchmark_results.py <json> [<json> ...]")
    sys.exit(0)

tests=[]
benchmarks = {}
for filename in sys.argv[1:]:
    with open(filename,'r') as fin:
        jstext = fin.read()
        results = json.loads( jstext )
    test_name = re.match('^.*_(.*).json',filename)[1]
    tests.append( test_name )
    ghz = float(results['context']['mhz_per_cpu'])/1000.
    stats = {}
    for bench in results['benchmarks']:
        bench_name = bench['name']
        cputime = float(bench['cpu_time'])
        cycles = cputime * ghz
        stats.setdefault(bench_name,[]).append( cycles )        
    for bench_name,values in stats.items():
        min_value = min(values)
        benchmarks.setdefault(bench_name,[]).append( min_value )

benchmark_names = list(benchmarks.keys())
num_benchmarks = len(benchmark_names)
num_tests = len(tests)
print( "Benchmarks:", benchmark_names )
print( "Benchmarks:%d Tests:%d" %(num_benchmarks, num_tests) )
width = 0.5 #1./(1+num_benchmarks)
fig, axes = plt.subplots(num_benchmarks,layout='constrained',figsize=(8,num_benchmarks*4), sharex=True)
if num_benchmarks==1:
    axes = [ axes ]
for ax,bench_name in zip(axes,benchmark_names):
    items = benchmarks[bench_name]
    label_positions = range(num_tests) 
    rects = ax.bar( label_positions, items, width, label=bench_name)
    ax.bar_label(rects, padding=3)
    ax.set_xticks(label_positions,tests )
    max_value = max(items)
    min_value = min(items)
    value_range = max_value - min_value
    margin = 0.1*value_range
    ax.legend(loc='upper left', ncols=3)
    ax.set_ylabel('Cycles')
    ax.set_ylim( [min_value-margin,max_value+2*margin ] )
plt.savefig('graph_cycles.png')

