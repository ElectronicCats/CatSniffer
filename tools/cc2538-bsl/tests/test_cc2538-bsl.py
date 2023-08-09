from scripttest import TestFileEnvironment
import shutil

# Init test environment
env = TestFileEnvironment('./test-output')

# Tests

# Make sure there is help output
def test_help_output():
	res = env.run('python', './../cc2538-bsl.py', '-h', '--help')

# Test for failure on no input file
# TODO needs better checking
def test_sanity_checks_no_input():
	res = env.run('python', './../cc2538-bsl.py', '-w', '-r', '-v', expect_error=1)

# Test for not implemented feature of verify after read
# TODO needs better checking
def test_sanity_checks_verify_after_read():
	res = env.run('python', './../cc2538-bsl.py', '-r', '-v', expect_error=1)

# Test for version output
def test_version():
	res = env.run('python', './../cc2538-bsl.py', '--version')

# Clean up after tests
def teardown_module(module):
    print ("Removing test-output folder")
    shutil.rmtree('./test-output')