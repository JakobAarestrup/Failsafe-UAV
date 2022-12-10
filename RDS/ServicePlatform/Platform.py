from tkinter import *
from tkinter import ttk
from functools import partial

def validateLogin(username, password):
    print("username entered :", username.get())
    print("password entered :", password.get())
    if username.get() == "Jakob" and password.get() == "PPWORLD":   
        print("Login succesful")
        main_window.destroy()
            
    else:
        return

#window
main_window = Tk()
main_window.geometry('400x200')
main_window.title('RDS Login Side')


#username label and text entry box
usernameLabel = Label(main_window, text="User Name").grid(row=0, column=0)
username = StringVar()
usernameEntry = Entry(main_window, width = 20, borderwidth = 1,textvariable=username).grid(row=0, column=1)

#usernameEntry = Entry(service_window, width = 20, borderwidth = 1,textvariable=username).grid(row=0, column=1)

#password label and password entry box
passwordLabel = Label(main_window,text="Password").grid(row=1, column=0)
password = StringVar()
passwordEntry = Entry(main_window, textvariable=password, show='*').grid(row=1, column=1)

validateLogin = partial(validateLogin, username, password)

#login button
loginButton = Button(main_window, text="Login", command=validateLogin).grid(row=4, column=1)


main_window.mainloop()