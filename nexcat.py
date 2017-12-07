import sys, time, os
from mechanize import Browser
 
def main():
    br = Browser()                # Create a browser
    br.open("https://www.nexcat.com/nexcat/login-nexcat.php")            # Open the login page
    br.select_form(name="f")  # Find the login form
if __name__ == "__main__":
    main()
    