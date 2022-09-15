import os
import sys

def readFile(filename):
    filehandle = open(filename)
    print(filehandle.read())
    filehandle.close()

fileDir = os.path.dirname(os.path.realpath('__file__'))
fn = os.path.join(fileDir, 'test_pages/our_site/dependencies/header.html')
readFile(fn)

for item, value in os.environ.items():
    print('{}: {}'.format(item, value))
    print("<br>")

print("<br><h1>Post body </h1>")

data = sys.stdin.read()
print(data)

fn = os.path.join(fileDir, 'test_pages/our_site/dependencies/footer.html')
readFile(fn)
