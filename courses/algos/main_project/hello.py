'''
from obts.cliprinter import CLIPrinter
from obts.testresult import TSResult
from obts.utestsimplebinary import UTestSimpleBinary

from obts.filesfinder import FilesFinder, FilterType

TESTS = ['/home/obs/rep/ml-notebooks/courses/algos/main_project/_build/bin/utest_balgosrbkw_04_directed.bin', '/home/obs/rep/ml-notebooks/courses/algos/main_project/_build/bin/utest_balgosrbkw_04_undirected.bin']

ts = TSResult('algos')
ts = CLIPrinter(ts)

ff = FilesFinder()
ff.filter_begin_with("utest_", FilterType.File)
ff.filter_end_with(".bin", FilterType.File)
ff.filter_no_dir()
#ff.filter_no_recursion()
files, dirs = ff.run("../_build/")
print(files)
print(dirs)

#UTestSimpleBinary(ts, 'directed', TESTS[0]).run()
#UTestSimpleBinary(ts, 'undirected', TESTS[1]).run()

#ts.summary()
'''

from obts.tsauto import TSAuto

ts = TSAuto("..")
ts.run()
