import os
import subprocess
import sys

FORCE_REBUILD = False
DEBUG = False
TS_ERROR_CODE = 10


ROOT_DIR = sys.argv[1]
TEST_NAME = sys.argv[2]
BUILD_DIR = sys.argv[3]

TEST_REF_SRC_CPP = os.path.join(ROOT_DIR, TEST_NAME) + '.cc'
TEST_REF_SRC_PY = os.path.join(ROOT_DIR, TEST_NAME) + '.py'
CODE_BIN = os.path.join(BUILD_DIR, 'bin', TEST_NAME)

TMP_REF_BIN = '/tmp/algos_ref_bin_{}'.format(TEST_NAME)

CXX = "clang++"

# Diverse utils functions
class Utils:
    def __init__(self):
        pass

    @staticmethod
    def run_cmd(cmd, stdin_path=None):
        if DEBUG:
            if stdin_path is None:
                print("Run command '{}'".format(' '.join(cmd)))
            else:
                print("Run command '{}' < {}".format(' '.join(cmd), stdin_path))
        
        if stdin_path is not None:
            stdin_path = open(stdin_path, 'rb')

        p = subprocess.Popen(cmd,
                             stdin=stdin_path,
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        out, err = p.communicate()
        ret = p.returncode

        if ret != 0:
            print('Failed to run command: {} (exit status {})'.format(' '.join(cmd), ret))
            print('OUTPUT: <>{}<>'.format(out.decode('ascii')))
            print(' ERROR: <>{}<>'.format(err.decode('ascii')))
            sys.exit(TS_ERROR_CODE)
    
        return out

    @staticmethod
    def read_file_bin(path):
        if DEBUG:
            print('Read binary file {}'.format(path))

        with open(path, 'rb') as f:
            return f.read()

    @staticmethod            
    def write_file_bin(path, data):
        if DEBUG:
            print('Writing binary file {}'.format(path))

        with open(path, 'wb') as f:
            f.write(data)

    @staticmethod
    def ts_err(mess):
        print('Error loading the test: {}'.format(mess))
        sys.exit(TS_ERROR_CODE)



# Manage build, exec, read and write ref files
class RefBuilder():
    def __init__(self):
        pass

    # returns path to the source file of the ref (either C++ or Python)
    def ref_src_path():
        srcs = [TEST_REF_SRC_CPP, TEST_REF_SRC_PY]
        for p in srcs:
            if os.path.isfile(p):
                return p
        Utils.ts_err("Cannot find ref source file for {}".format(TEST_NAME))        


    @staticmethod
    def prepare_cpp(src_path):
        if os.path.isfile(TMP_REF_BIN) and not FORCE_REBUILD:
            return
        Utils.run_cmd([CXX, src_path, '-o', TMP_REF_BIN])

    @staticmethod
    def prepare_py(src_path):
        pass
        
    # make sure the ref is ready to be executed
    @staticmethod
    def prepare():
        src_path = RefBuilder.ref_src_path()
        ext = os.path.splitext(src_path)[1]
        if ext == '.cc':
            RefBuilder.prepare_cpp(src_path)
        elif ext == '.py':
            RefBuilder.prepare_py(src_path)
        

    # prepare the command needed to build the ref file
    @staticmethod
    def get_cmd():
        src_path = RefBuilder.ref_src_path()
        ext = os.path.splitext(src_path)[1]
        if ext == '.cc':
            return [TMP_REF_BIN]
        else:
            return ['python3', src_path]

    # build the ref file `ref_path` if it doesn't exist, using the input `input_path` (None if no input)
    @staticmethod
    def build_ref_file(ref_path, input_path):
        if os.path.isfile(ref_path) and not FORCE_REBUILD:
            return

        out = Utils.run_cmd(RefBuilder.get_cmd(), stdin_path=input_path)
        Utils.write_file_bin(ref_path, out)

    @staticmethod
    # build and read ref file, returns byte array
    def get_ref(ref_path, input_path):
        RefBuilder.build_ref_file(ref_path, input_path)
        return Utils.read_file_bin(ref_path)


class TestRunner():
    def __init__(self):
        pass

    # Check if expected and actual output are the same, and exit if they are not
    @staticmethod
    def check_output(code_out, ref_out):
        if code_out == ref_out:
            return

        print('Test {} Failed: output differs'.format(TEST_NAME))
        print('Expected:\n<BEG>{}<END>'.format(ref_out.decode('ascii')))
        print('Actual:\n<BEG>{}<END>'.format(code_out.decode('ascii')))
        sys.exit(1)

    # Run an inctual test. input_file is optional (none)
    def run_test(input_file=None):
        if DEBUG:
            if input_file is None:
                print('Running test {}'.format(TEST_NAME))
            else:
                print('Running test {} < {}'.format(TEST_NAME, input_file))

        if input_file is None:
            ref_path = os.path.join(ROOT_DIR, TEST_NAME) + '.out'
            input_path = None
        else:
            ref_path = os.path.join(ROOT_DIR, TEST_NAME) + '_' + input_file + '.out'
            input_path = os.path.join(ROOT_DIR, input_file) + '.input'

        code_out = Utils.run_cmd([CODE_BIN], stdin_path=input_path)
        ref_out = RefBuilder.get_ref(ref_path, input_path)
        TestRunner.check_output(code_out, ref_out)



        
def list_input_files():
    res = []
    for name in os.listdir(ROOT_DIR):
        if name.endswith('.input'):
            res.append(os.path.splitext(name)[0])
    return res

RefBuilder.prepare()

inputs = list_input_files()
if len(inputs) == 0:
    TestRunner.run_test(None)
else:
    for f in inputs:
        TestRunner.run_test(f)

sys.exit(0)
