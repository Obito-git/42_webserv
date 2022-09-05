from datetime import datetime
print(open("../dependencies/header.html", "r").read())
current_datetime = datetime.now()
print("<br><br>\n<h3>")
print(current_datetime)
print("</h3>")
print(open("../dependencies/footer.html", "r").read())
