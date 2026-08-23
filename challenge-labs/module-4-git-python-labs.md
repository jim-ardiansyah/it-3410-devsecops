# Git and Python Application Lab Workbook

This workbook guides you through installing Git on your local machine, writing a Python web application, testing it locally, and pushing your code to GitHub. Complete the labs in order — each one builds on the previous.

## Table of Contents
1. [Installing Git on Your Local Machine](#lab-1-installing-git-on-your-local-machine)
2. [Creating a Python Flask Web Server](#lab-2-creating-a-python-flask-web-server)
3. [Testing the Application Locally](#lab-3-testing-the-application-locally)
4. [Initializing a Git Repository and Making Your First Commit](#lab-4-initializing-a-git-repository-and-making-your-first-commit)
5. [Pushing Code to GitHub](#lab-5-pushing-code-to-github)

---

# Lab 1: Installing Git on Your Local Machine

## Objective
In this lab, you will install Git on your local machine and configure it with your name and email address. By the end of this lab, Git will be ready to use for all subsequent exercises.

## Prerequisites
- A computer running Windows, macOS, or Linux
- An internet connection
- Administrator or sudo access on your machine

## Instructions

### Windows

**Step 1 — Download the Git installer.**
Open your browser and go to:
```
https://git-scm.com/download/win
```
The download will start automatically. If it does not, click the link for the latest version.

**Step 2 — Run the installer.**
Open the downloaded `.exe` file and follow the installation wizard. The default options are suitable for this lab — click **Next** through each screen and then click **Install**.

**Step 3 — Open Git Bash.**
Once installation is complete, open the **Start menu**, search for **Git Bash**, and open it. Git Bash is a terminal that gives you access to Git commands on Windows. Use it for all commands in this workbook.

**Step 4 — Verify the installation.**
In Git Bash, run the following command to confirm Git was installed correctly:
```
git --version
```
You should see output similar to:
```
git version 2.x.x.windows.x
```

---

### macOS

**Step 1 — Open the Terminal.**
Press `Cmd + Space`, type `Terminal`, and press Enter to open it.

**Step 2 — Install Git.**
Run the following command. If Git is not installed, macOS will prompt you to install the Xcode Command Line Tools, which includes Git:
```
git --version
```
Follow the on-screen prompts to complete the installation. Alternatively, if you have Homebrew installed, you can run:
```
brew install git
```

**Step 3 — Verify the installation.**
Once installed, confirm Git is available:
```
git --version
```
You should see output similar to:
```
git version 2.x.x
```

---

### Linux (Ubuntu / Debian)

**Step 1 — Open the Terminal.**
Press `Ctrl + Alt + T` to open a terminal window.

**Step 2 — Update the package list.**
```
sudo apt update
```

**Step 3 — Install Git.**
```
sudo apt install git -y
```

**Step 4 — Verify the installation.**
```
git --version
```
You should see output similar to:
```
git version 2.x.x
```

---

## Configure Git (All Platforms)

After installing Git, configure your identity. These details will be attached to every commit you make.

**Step 1 — Set your name.**
Replace `"Your Name"` with your actual name:
```
git config --global user.name "Your Name"
```

**Step 2 — Set your email address.**
Use the same email address you will use for your GitHub account:
```
git config --global user.email "your-email@example.com"
```

**Step 3 — Verify your configuration.**
```
git config --list
```
You should see your name and email listed in the output:
```
user.name=Your Name
user.email=your-email@example.com
```

## Deliverables
Submit a screenshot of the terminal showing:
- The output of `git --version`
- The output of `git config --list` showing your name and email

## Review Questions
1. Why is it important to configure your name and email in Git before making commits?
2. What does the `--global` flag do in the `git config` command?
3. What is the difference between Git and GitHub?
4. Why do Windows users use Git Bash instead of the standard Command Prompt?

---

# Lab 2: Creating a Python Flask Web Server

## Objective
In this lab, you will create a project folder, write a simple Python web application using the Flask framework, and install Flask on your local machine using PIP. By the end of this lab, you will have a working Flask application ready to run and test.

## Prerequisites
- Git is installed and configured (Lab 1 complete)
- Python 3 is installed on your machine. Verify this by running:
```
python3 --version
```
If Python is not installed, download it from [https://www.python.org/downloads](https://www.python.org/downloads) and install it before continuing. On Windows, make sure to check **Add Python to PATH** during installation.

> **Note:** On Windows, you may need to use `python` instead of `python3`. Try both if one does not work.

## Instructions

**Step 1 — Choose a working directory.**
Open your terminal (Git Bash on Windows, Terminal on macOS/Linux) and navigate to a location where you want to create your project. For example, to work on your Desktop:
```
cd ~/Desktop
```

**Step 2 — Create the project folder.**
Create a new folder named `my-python-app`:
```
mkdir my-python-app
```

**Step 3 — Enter the project folder.**
```
cd my-python-app
```

**Step 4 — Verify you are in the right place.**
```
pwd
```
The output should show a path ending in `my-python-app`.

**Step 5 — Create the application file.**
Create a new empty file named `app.py`:
```
touch app.py
```
> **Windows users:** If `touch` is not recognized, use `echo. > app.py` instead.

**Step 6 — Open the file in a text editor.**
Open `app.py` in a text editor of your choice:

- **VS Code:** `code app.py` (if VS Code is installed)
- **nano (macOS/Linux):** `nano app.py`
- **Notepad (Windows):** `notepad app.py`

**Step 7 — Add the application code.**
Type or paste the following code into `app.py`:
```python
from flask import Flask

app = Flask(__name__)

@app.route("/")
def info():
    return "<h1>First version of the Python app</h1>"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=7000)
```
Save the file and close the editor.

**Step 8 — Attempt to run the application.**
Try running the application to see what happens:
```
python3 app.py
```
You should see an error similar to the following, because Flask is not yet installed:
```
ModuleNotFoundError: No module named 'flask'
```
This is expected. Continue to the next step to install Flask.

**Step 9 — Install Flask using PIP.**
Install the Flask package using PIP, Python's package installer:
```
pip install flask
```
Wait for the installation to complete. You should see a confirmation message ending with `Successfully installed flask-x.x.x`.

> **Note:** If `pip` is not recognized, try `pip3` instead.

**Step 10 — Verify Flask is installed.**
Confirm that Flask was installed successfully:
```
pip show flask
```
You should see details about the Flask package including its version and install location.

## Deliverables
Submit a screenshot of the terminal showing:
- Step 8 — the `ModuleNotFoundError` confirming Flask was not yet installed
- Step 9 — the PIP output confirming Flask was successfully installed
- Step 10 — the output of `pip show flask`

## Review Questions
1. What is Flask, and why is it a popular choice for building web servers in Python?
2. What does PIP stand for, and what is its purpose?
3. Why did the application fail when you first tried to run it in Step 8?
4. What does `host="0.0.0.0"` mean in the Flask `app.run()` call?
5. What is the difference between `pip install` and `pip show`?

---

# Lab 3: Testing the Application Locally

## Objective
In this lab, you will start your Flask application and send HTTP requests to it from your local machine to confirm it is working correctly. You will test it using both a web browser and the command line.

## Prerequisites
- Lab 2 is complete and Flask is installed
- You are inside the `my-python-app` folder

## Instructions

**Step 1 — Confirm your location.**
Make sure you are inside the project folder:
```
pwd
```
The output should end in `my-python-app`. If not, navigate there:
```
cd ~/Desktop/my-python-app
```

**Step 2 — Start the Flask application.**
Run the application:
```
python3 app.py
```
You should see output similar to the following, confirming the server is running on port `7000`:
```
 * Running on http://0.0.0.0:7000
 * Running on http://127.0.0.1:7000
Press CTRL+C to quit
```
Keep this terminal open and running. The server will continue to run until you stop it.

**Step 3 — Test using a web browser.**
Open your web browser and navigate to:
```
http://127.0.0.1:7000
```
You should see a page displaying:
```
First version of the Python app
```
This confirms your Flask server is running and responding to HTTP requests.

**Step 4 — Test using curl.**
Open a **second terminal window** (leave the first one running the server) and run:
```
curl http://127.0.0.1:7000
```
You should see the following HTML response:
```html
<h1>First version of the Python app</h1>
```

> **Windows users:** `curl` is available in Git Bash and in PowerShell on Windows 10 and later. If it is not available, skip this step and rely on the browser test in Step 3.

**Step 5 — Stop the application.**
Return to the terminal window running the Flask server and press `Ctrl + C` to stop it. You should return to the command prompt.

## Deliverables
Submit a screenshot showing:
- Step 2 — the terminal output confirming the Flask server started on port `7000`
- Step 3 — the browser showing "First version of the Python app" at `http://127.0.0.1:7000`
- Step 4 — the `curl` output (if applicable)

## Review Questions
1. What does `127.0.0.1` refer to, and why is it used when testing a server locally?
2. Why do you need to open a second terminal window to run `curl` while the Flask server is running?
3. What happens to the Flask server when you press `Ctrl + C`?
4. What is the difference between testing an application with a browser and testing it with `curl`?

---

# Lab 4: Initializing a Git Repository and Making Your First Commit

## Objective
In this lab, you will initialize a Git repository inside your project folder, stage your application file, and create your first commit. You will also rename the default branch from `master` to `main` to follow modern Git conventions.

## Prerequisites
- Labs 1, 2, and 3 are complete
- You are inside the `my-python-app` folder with `app.py` present

## Instructions

**Step 1 — Navigate to the project folder.**
```
cd ~/Desktop/my-python-app
```

**Step 2 — Confirm the project contents.**
List the files in the directory to confirm that `app.py` is present:
```
ls
```
You should see `app.py` in the output.

**Step 3 — Initialize a new Git repository.**
Set up an empty Git repository inside the project folder:
```
git init
```
You should see output similar to:
```
Initialized empty Git repository in /Users/yourname/Desktop/my-python-app/.git/
```

**Step 4 — Verify the `.git` folder was created.**
List all files including hidden ones to confirm Git created its tracking folder:
```
ls -la
```
You should see a `.git` entry in the output. This hidden folder is where Git stores all version history and configuration for your project.

**Step 5 — Create a `.gitignore` file.**
Before staging your files, create a `.gitignore` to tell Git which files it should not track. This prevents unnecessary or sensitive files from being committed:
```
touch .gitignore
```
Open `.gitignore` in your text editor and add the following lines:
```
__pycache__/
*.pyc
.env
```
Save and close the file.

**Step 6 — Check the repository status.**
View the current state of the repository:
```
git status
```
You should see that you are on the `master` branch, there are no commits yet, and both `app.py` and `.gitignore` appear as untracked files.

**Step 7 — Stage all files.**
Add all files in the project folder to the staging area:
```
git add .
```

**Step 8 — Verify the staging area.**
Check the repository status again to confirm both files are staged:
```
git status
```
Both `app.py` and `.gitignore` should now appear under "Changes to be committed".

**Step 9 — Create your first commit.**
Commit the staged files with a descriptive message:
```
git commit -m "Initial commit: add Flask web application"
```
You should see output confirming the commit was created, including the branch name, a short commit hash, and your message.

**Step 10 — Verify the repository is clean.**
Check the status one more time to confirm there are no remaining changes:
```
git status
```
The output should read "nothing to commit, working tree clean".

**Step 11 — Review the commit history.**
View the commit log to inspect your first commit:
```
git log --oneline
```
You should see one entry showing the short commit hash and your commit message.

**Step 12 — Rename the branch to main.**
Rename the current branch from `master` to `main`:
```
git branch -m main
```

**Step 13 — Confirm the rename.**
List all branches to verify the change:
```
git branch
```
You should see `* main` as the only branch, with the asterisk indicating it is the active branch.

## Deliverables
Submit a screenshot of the terminal showing:
- Step 3 — the `git init` confirmation output
- Step 6 — the `git status` output showing untracked files
- Step 8 — the `git status` output after staging
- Step 9 — the commit confirmation output
- Step 11 — the `git log --oneline` output
- Step 13 — the `git branch` output showing `* main`

## Review Questions
1. What is the difference between an untracked file and a staged file in Git?
2. What does `git add .` do, and when might you want to stage individual files instead?
3. What is a `.gitignore` file, and why is it important to create one before your first commit?
4. What is a commit hash, and why does Git use it to identify commits?
5. Why is it becoming standard practice to name the primary branch `main` instead of `master`?

---

# Lab 5: Pushing Code to GitHub

## Objective
In this lab, you will create a free GitHub account (if you do not already have one), create a remote repository, connect it to your local repository, and push your committed code to GitHub. By the end of this lab, your application code will be safely stored in the cloud and accessible from anywhere.

## Prerequisites
- Labs 1 through 4 are complete
- You have at least one commit on the `main` branch
- You have an internet connection

## Instructions

### Part A: Set Up Your GitHub Account

**Step 1 — Create a GitHub account.**
If you do not already have a GitHub account, go to [https://github.com](https://github.com) and sign up for a free account. Use the same email address you configured in Git during Lab 1.

**Step 2 — Verify your email address.**
GitHub will send a verification email. Click the link in the email to activate your account before continuing.

---

### Part B: Create a Personal Access Token

GitHub requires a Personal Access Token (PAT) instead of a password for Git operations over HTTPS. You will need this in Part D.

**Step 1 — Go to token settings.**
While logged in to GitHub, click your profile photo in the top-right corner, then go to:
```
Settings → Developer settings → Personal access tokens → Tokens (classic) → Generate new token
```

**Step 2 — Configure the token.**
- Enter a name for the token, such as `my-python-app-token`
- Set an expiration (30 days is sufficient for this lab)
- Under **Select scopes**, check the `repo` checkbox

**Step 3 — Generate and copy the token.**
Click **Generate token**. Copy the token immediately and save it somewhere safe — GitHub will only show it once. You will use this token as your password when pushing in Part D.

---

### Part C: Create a Remote Repository on GitHub

**Step 1 — Create a new repository.**
While logged in to GitHub, click the **+** icon in the top-right corner and select **New repository**.

**Step 2 — Configure the repository.**
- Set the repository name to `my-python-app`
- Leave it set to **Public**
- **Do not** check any of the initialization options (no README, no `.gitignore`, no license) — the repository must be completely empty to avoid conflicts with your local repository

**Step 3 — Create the repository.**
Click **Create repository**. GitHub will display a setup page — keep it open, as you will need the repository URL in the next section.

---

### Part D: Connect and Push Your Local Repository

**Step 1 — Navigate to your project folder.**
Open your terminal and make sure you are inside the `my-python-app` folder:
```
cd ~/Desktop/my-python-app
```

**Step 2 — Confirm your local repository is ready.**
Check that you have at least one commit on the `main` branch:
```
git log --oneline
```
You should see your commit from Lab 4.

**Step 3 — Add the remote repository.**
Link your local repository to the GitHub remote. Replace `your-username` with your actual GitHub username:
```
git remote add origin https://github.com/your-username/my-python-app.git
```
The name `origin` is the conventional alias for the primary remote repository.

**Step 4 — Verify the remote was added.**
```
git remote -v
```
You should see two entries pointing to your GitHub repository URL:
```
origin  https://github.com/your-username/my-python-app.git (fetch)
origin  https://github.com/your-username/my-python-app.git (push)
```

**Step 5 — Push your code to GitHub.**
Push the `main` branch to the remote repository. The `-u` flag sets `origin main` as the default upstream so that future pushes only require `git push`:
```
git push -u origin main
```
When prompted, enter your GitHub **username** and paste your **Personal Access Token** as the password.

**Step 6 — Confirm the push was successful.**
You should see output similar to the following:
```
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Writing objects: 100% (4/4), 390 bytes | 390.00 KiB/s, done.
Total 4 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/your-username/my-python-app.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```

**Step 7 — Verify on GitHub.**
Open your browser and go to:
```
https://github.com/your-username/my-python-app
```
You should see your `app.py` and `.gitignore` files listed in the repository along with your commit message. This confirms your code has been successfully pushed to GitHub.

---

## Quick Reference: Day-to-Day Git Workflow

After your initial setup, use this workflow whenever you make changes to your project:
```
git add .
git commit -m "Describe what you changed"
git push
```

---

## Deliverables
Submit a screenshot showing:
- Part B — the GitHub Personal Access Token settings page (before closing it)
- Part C Step 3 — the empty repository setup page on GitHub
- Part D Step 4 — the `git remote -v` output with your GitHub URL
- Part D Step 6 — the terminal output confirming a successful push
- Part D Step 7 — your GitHub repository page showing `app.py` and `.gitignore`

## Review Questions
1. What is the difference between a local repository and a remote repository?
2. What does `git remote add origin` do, and why is `origin` the conventional name?
3. What is the purpose of the `-u` flag in `git push -u origin main`?
4. Why does GitHub require a Personal Access Token instead of your account password?
5. If a teammate cloned your GitHub repository, would they need to run `git remote add origin`? Why or why not?
6. What would happen if you had initialized your GitHub repository with a README before pushing your local code?
