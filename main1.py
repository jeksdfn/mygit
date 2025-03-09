import sqlite3
import customtkinter as Ctk
from tkinter import messagebox
conn = sqlite3.connect('user_credentials.db')
c = conn.cursor()
c.execute('''CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT)''')
conn.commit()


def store_things():
    username = entry_username.get()
    password = entry_password.get()
    
    if username and password:
        c.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, password))
        conn.commit()
        messagebox.showinfo("Erfolg")

    else:
        messagebox.showwarning("Fehler", "Bitte alle Felder ausfüllen")

def check_credentials(username_entry, password_entry):
    username = username_entry.get()
    password = password_entry.get()

    if username and password:
        c.execute("SELECT * FROM users WHERE username = ? AND password = ?", (username, password))
        result = c.fetchone()

        if result:
            messagebox.showinfo("Willkommen!")
        else:
            messagebox.showerror("Fehler", "Falsche daten")
    else:
        messagebox.showwarning("Fehler", "alle Felder ausfüllen!")

def windowTwo2():
    newWindow2 = Ctk.CTkToplevel(root)
    newWindow2.title("Login")
    newWindow2.geometry("300x250")
    
    Ctk.CTkLabel(newWindow2, text="Login", font=("Arial", 18)).pack(pady=10)
    
    entry_username2 = Ctk.CTkEntry(newWindow2, width=200, placeholder_text="Benutzername")
    entry_username2.pack(pady=5)
    
    entry_password2 = Ctk.CTkEntry(newWindow2, width=200, placeholder_text="Passwort", show="*")
    entry_password2.pack(pady=5)
    
    login_button = Ctk.CTkButton(newWindow2, text="Login", command=lambda: check_credentials(entry_username2, entry_password2))
    login_button.pack(pady=6)
def windowTwo():
    newWindow = Ctk.CTkToplevel(root)
    newWindow.title("Login")
    newWindow.geometry("300x250")
    
    Ctk.CTkLabel(newWindow, text="Login", font=("Arial", 18)).pack(pady=10)
    
    entry_username2 = Ctk.CTkEntry(newWindow, width=200, placeholder_text="Benutzername")
    entry_username2.pack(pady=5)
    
    entry_password2 = Ctk.CTkEntry(newWindow, width=200, placeholder_text="Passwort", show="*")
    entry_password2.pack(pady=5)
    
    login_button = Ctk.CTkButton(newWindow, text="Login", command=lambda: check_credentials(entry_username2, entry_password2))
    login_button.pack(pady=6)



Ctk.set_appearance_mode("dark")  
Ctk.set_default_color_theme("blue")

root = Ctk.CTk()
root.title("Benutzer")
root.geometry("400x400")

Ctk.CTkLabel(root, text="Registrierung", font=("Arial", 20)) .pack(pady=10)

entry_username = Ctk.CTkEntry(root, width=250, placeholder_text="Benutzername")
entry_username.pack(pady=5)

entry_password = Ctk.CTkEntry(root, width=250, placeholder_text="Passwort", show="*")
entry_password.pack(pady=5)

submit_button = Ctk.CTkButton(root, text="Registrieren", command=store_things)
submit_button.pack(pady=20)

login_button = Ctk.CTkButton(root, text="Login", command=windowTwo)
login_button.pack(pady=10)

root.mainloop()

conn.close()
