from datetime import datetime
import os


def readFile(filename):
    filehandle = open(filename)
    print(filehandle.read())
    filehandle.close()


fileDir = os.path.dirname(os.path.realpath('__file__'))
fn = os.path.join(fileDir, 'test_pages/our_site/dependencies/header.html')
readFile(fn)

current_datetime = datetime.now()
print("<br><br>Hello from python <br><br>")
print("<h3>Current date and time:<h3>")
print("<h4>" + str(current_datetime) + "</h4>")

fn = os.path.join(fileDir, 'test_pages/our_site/dependencies/footer.html')
readFile(fn)
