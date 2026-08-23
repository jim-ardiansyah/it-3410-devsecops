# Linux System Administration Lab Workbook

This workbook contains a series of hands-on labs covering essential Linux system administration skills. Complete the labs in order, as some build on the users, groups, and files created in earlier exercises. After each step, observe the output and confirm the result before moving on.

## Table of Contents
1. [Organizing Files and Directories for Project Management](#lab-1-organizing-files-and-directories-for-project-management)
2. [Managing User Accounts and Permissions](#lab-2-managing-user-accounts-and-permissions)
3. [Configuring File Permissions and Ownership](#lab-3-configuring-file-permissions-and-ownership)
4. [Configuring and Troubleshooting Network Settings](#lab-4-configuring-and-troubleshooting-network-settings)
5. [Setting and Managing Environment Variables](#lab-5-setting-and-managing-environment-variables)
6. [Monitoring and Managing Processes](#lab-6-monitoring-and-managing-processes)
7. [Installing and Using the `jq` JSON Parser](#lab-7-installing-and-using-the-jq-json-parser)
8. [Updating System Software with `yum`](#lab-8-updating-system-software-with-yum)

---

# Lab 1: Organizing Files and Directories for Project Management

## Objective
Your team is working on a project that involves managing various files and directories on a server. In this lab, you will set up a folder structure and perform basic file operations to keep project resources organized. By completing these tasks, you will practice essential Linux commands for creating, listing, renaming, copying, and deleting files.

## Instructions
Complete the following steps in order. After each operation, record the command you used and observe the output.

**Step 1 — Create a project directory.**
In your `home` directory, create a new directory named `project_files`:
```
mkdir project_files
```

**Step 2 — Enter the new directory.**
Navigate into the directory you just created:
```
cd project_files
```

**Step 3 — Create three empty files.**
Use the `touch` command to create three empty files named `report.txt`, `data.csv`, and `notes.txt`:
```
touch report.txt
touch data.csv
touch notes.txt
```

**Step 4 — Verify the files were created.**
List the contents of the directory in long format to confirm all three files exist:
```
ls -l
```

**Step 5 — Rename a file.**
Rename `notes.txt` to `important_notes.txt` using the `mv` command:
```
mv notes.txt important_notes.txt
```

**Step 6 — Verify the rename.**
List the directory contents again to confirm the file name changed:
```
ls -l
```

**Step 7 — Create a backup copy.**
Make a copy of `report.txt` named `report_backup.txt` using the `cp` command:
```
cp report.txt report_backup.txt
```

**Step 8 — Verify the copy.**
List the directory contents to confirm the backup file was created:
```
ls -l
```

**Step 9 — Delete a file.**
Remove `data.csv` using the `rm` command. If prompted for confirmation, type `y` and press Enter:
```
rm data.csv
```

**Step 10 — Confirm the deletion.**
List the contents one final time to validate that `data.csv` no longer exists:
```
ls -l
```

## Deliverables
Submit a document containing the commands you ran for each step along with a screenshot or copy of the `ls -l` output from Steps 4, 6, 8, and 10. Your final directory should contain `report.txt`, `report_backup.txt`, and `important_notes.txt`.

## Review Questions
1. What is the difference between the `mv` and `cp` commands?
2. Why is it useful to run `ls -l` after each file operation?
3. What does the `-l` flag add to the output of the `ls` command?
4. Why might a team create backup copies of important files during a project?

---

# Lab 2: Managing User Accounts and Permissions

## Objective
As your team grows, so does the need to manage user accounts and permissions on the server. In this lab, you will create a new user named `bryan` and a group called `projects` for a new project team, then add the user to that group. By completing these tasks, you will practice essential Linux commands for user and group administration, including creating accounts, setting passwords, managing group membership, and switching between users.

## Prerequisites
Some of these commands require administrative privileges. Make sure you are logged in with an account that has the necessary permissions (such as `root` or an account with `sudo` access) before you begin.

## Instructions
Complete the following steps in order. After each operation, observe the output and confirm the result before moving on.

**Step 1 — Change to your home directory.**
Return to your home directory to start from a known location:
```
cd ~
```

**Step 2 — Create a new user.**
Create a user named `bryan` with a home directory of `/home/bryan`:
```
useradd -d /home/bryan bryan
```

**Step 3 — Set the user's password.**
Set a password for `bryan`. When prompted, enter `Linux-User1`:
```
passwd bryan
```
*Note: If the `passwd` command is not available, install it with* `yum install -y passwd` *and then try again.*

**Step 4 — Check the user information.**
Display information about the new user to confirm the account was created:
```
id bryan
```

**Step 5 — Create a new group.**
Create a group named `projects`:
```
groupadd projects
```

**Step 6 — Add the user to the group.**
Add `bryan` to the `projects` group:
```
usermod -a -G projects bryan
```

**Step 7 — Verify group membership.**
Display the user information again to confirm `bryan` now belongs to the `projects` group:
```
id bryan
```

**Step 8 — Switch to the new user.**
Switch your session to the `bryan` user:
```
su bryan
```

**Step 9 — Confirm the current user.**
Verify that you are now logged in as `bryan`:
```
whoami
```

**Step 10 — Move to the user's home directory.**
Change into the home directory for the current user:
```
cd ~
```

**Step 11 — Confirm your location.**
Print the working directory to confirm you are in `/home/bryan`, the home directory set when the account was created:
```
pwd
```

**Step 12 — Return to your original account.**
Exit the `bryan` session to switch back to your original account:
```
exit
```

**Step 13 — Confirm the current user.**
Verify that you are logged in as `root`:
```
whoami
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from the two `id bryan` commands (Steps 4 and 7) and the `whoami` and `pwd` output from Steps 9, 11, and 13.

## Review Questions
1. What does the `-d` option do in the `useradd` command?
2. Why is the `-a` option important when using `usermod -a -G` to add a user to a group?
3. What is the difference between a user's primary group and a supplementary group?
4. What is the purpose of the `id` command, and what information does it display?
5. Why does switching users with `su` and then typing `exit` return you to your previous account?

---

# Lab 3: Configuring File Permissions and Ownership

## Objective
You've been assigned to configure file permissions and ownership on the server to ensure data security and access control. In this lab, you will set up a directory structure and apply appropriate permissions and ownership settings to secure project data. By completing these tasks, you will practice using `chmod` and `chown` to control who can read, write, and execute files, and you will learn how to interpret the permission and ownership information shown by `ls -l`.

## Prerequisites
This lab builds on the user and group created in Lab 2. Make sure the user `bryan` and the group `projects` already exist, and that you are logged in with an account that has administrative privileges (such as `root` or an account with `sudo` access).

## Instructions
Complete the following steps in order. Pay close attention to the `ls -l` output after each change so you can see how the permissions and ownership evolve.

**Step 1 — Create a secure directory.**
In your home directory, create a directory named `secure_data`:
```
mkdir secure_data
```

**Step 2 — Enter the directory.**
Navigate into the `secure_data` directory:
```
cd secure_data
```

**Step 3 — Create three files.**
Use the `touch` command to create three files named `confidential.txt`, `restricted.txt`, and `public.txt`:
```
touch confidential.txt
touch restricted.txt
touch public.txt
```

**Step 4 — View the initial permissions.**
List the directory contents in long format to view the default permissions and ownership of each file. Take note of this output so you can compare it later:
```
ls -l
```

**Step 5 — Restrict a file to the owner only.**
Set permissions on `confidential.txt` so that only the owner can read, write, and execute it:
```
chmod 700 confidential.txt
```

**Step 6 — Allow owner and group access.**
Set permissions on `restricted.txt` so that the owner and group can read and write, but others have no access:
```
chmod 660 restricted.txt
```

**Step 7 — Allow limited public access.**
Set permissions on `public.txt` so that the owner and group can read and write, and others can read only:
```
chmod 664 public.txt
```

**Step 8 — Verify the permission changes.**
List the directory contents again and compare this output with the output from Step 4 to observe how the permissions have changed:
```
ls -l
```

**Step 9 — Change file ownership.**
Change the ownership of `confidential.txt` to the user `bryan` and the group `projects`:
```
chown bryan:projects confidential.txt
```

**Step 10 — Verify the ownership change.**
List the directory contents again and compare `confidential.txt` with the other files, which are still owned by `root`:
```
ls -l
```

**Step 11 — Create a script file.**
Create a new file for a script named `script.sh`:
```
touch script.sh
```

**Step 12 — Confirm the file is not executable.**
List the directory contents and note that `script.sh` is not executable, as shown by the missing `x` in its permissions:
```
ls -l
```

**Step 13 — Make the script executable.**
Add executable permission to `script.sh`:
```
chmod +x script.sh
```

**Step 14 — Confirm the file is now executable.**
List the directory contents one final time and note that `script.sh` now includes an `x` in its permissions, indicating it is executable:
```
ls -l
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the `ls -l` output from Steps 4, 8, 10, 12, and 14. Be sure your submission clearly shows the permission and ownership changes at each stage.

## Review Questions
1. In the numeric permission `700`, what access does each digit represent, and who does it apply to?
2. What is the difference between the permission settings `660` and `664`?
3. In the `chown bryan:projects confidential.txt` command, what do `bryan` and `projects` each refer to?
4. How can you tell from `ls -l` output whether a file is executable?
5. Why is it a good security practice to give the `confidential.txt` file more restrictive permissions than `public.txt`?

---

# Lab 4: Configuring and Troubleshooting Network Settings

## Objective
The web team is deploying a new web application, and you've been tasked with configuring the networking settings on the server. In this lab, you will validate the hostname, change it both temporarily and permanently, install common networking tools, and use those tools to inspect network interfaces, test connectivity, and verify DNS resolution. By completing these tasks, you will practice essential Linux commands for network configuration and troubleshooting.

## Prerequisites
Several of these commands require administrative privileges and access to the internet to download packages. Make sure you are logged in with an account that has the necessary permissions (such as `root` or an account with `sudo` access) before you begin.

## Instructions
Complete the following steps in order. Observe the output of each command and confirm the result before moving on.

**Step 1 — Check the current hostname.**
Display the current hostname of the Linux host:
```
hostname
```

**Step 2 — Change the hostname temporarily.**
Change the hostname to `server-1` for the current session. This change will not survive a reboot:
```
hostname server-1
```

**Step 3 — Open the hostname file for a permanent change.**
To make the change permanent, edit the `/etc/hostname` file with a text editor such as `vi`. This change takes effect on the next reboot:
```
vi /etc/hostname
```

**Step 4 — Edit the file.**
Press the `i` key to enter insert mode, then change the contents of the file to `server-1`.

**Step 5 — Save and exit.**
Press `ESC` to leave insert mode, then type `:wq` and press Enter to write the file and close the editor. The command appears in the bottom-left corner of the editor.

**Step 6 — Verify the temporary hostname change.**
Confirm that the hostname is now `server-1` for the current session:
```
hostname
```

**Step 7 — Update installed packages.**
Update the system's packages before installing new tools:
```
yum update -y
```

**Step 8 — Install networking tools.**
Install the networking utilities you will use in the following steps:
```
yum install -y net-tools bind
```

**Step 9 — Display network interface information.**
Use `ifconfig` to view details about the system's network interfaces:
```
ifconfig
```

**Step 10 — Identify the active interface.**
Examine the `ifconfig` output and identify the active network interface and its assigned IP address. Make a note of both for your submission.

**Step 11 — Test connectivity with ping.**
Use `ping` to test connectivity to the default gateway. If the gateway address does not respond, test the loopback address instead:
```
ping 172.17.0.1
```
*If that address does not work, try the loopback address:* `ping 127.0.0.1`

**Step 12 — Stop the ping.**
Press `CTRL-C` to stop the continuous ping and return to the prompt.

**Step 13 — Display network connections and routing.**
Use `netstat` to display active network connections and the routing table:
```
netstat
```

**Step 14 — Check the DNS configuration.**
Display the contents of the DNS configuration file:
```
cat /etc/resolv.conf
```

**Step 15 — Test DNS resolution.**
Use `nslookup` to query a domain name of your choice and confirm that DNS resolution is working:
```
nslookup redhat.com
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from Steps 6, 9, 11, 13, 14, and 15. In your submission, clearly identify the active network interface and its IP address from Step 10.

## Review Questions
1. What is the difference between changing the hostname with the `hostname` command and editing the `/etc/hostname` file?
2. Why does the change made in the `/etc/hostname` file only take effect after a reboot?
3. What kind of information does the `ifconfig` command display about a network interface?
4. What is the purpose of the loopback address `127.0.0.1`, and when might you ping it?
5. What does the `/etc/resolv.conf` file contain, and how does it relate to the `nslookup` command?

---

# Lab 5: Setting and Managing Environment Variables

## Objective
The application team has requested that environment variables be set on the host, as a new application requires them for specific configuration settings. In this lab, you will inspect existing environment variables, create and modify variables, add a directory to the `PATH`, remove a variable, and use command history to restore a previous setting. By completing these tasks, you will practice managing environment variables and working efficiently with the shell's command history.

## Instructions
Complete the following steps in order. Observe the output of each command and confirm the result before moving on.

**Step 1 — View the current environment variables.**
Display all of the environment variables currently set in your session:
```
env
```

**Step 2 — Check the PATH variable.**
Filter the environment variables to display only the `PATH` variable, which defines the directories where executable files are located:
```
env | grep PATH
```

**Step 3 — Set a new environment variable.**
Create a new environment variable named `VAR_DATABASE` with the value `prod-db-01` for the new application:
```
export VAR_DATABASE="prod-db-01"
```

**Step 4 — Verify the new variable.**
Confirm that `VAR_DATABASE` has been set correctly:
```
echo $VAR_DATABASE
```

**Step 5 — Add a directory to PATH.**
Use `export` to append a directory to the existing `PATH` variable:
```
export PATH=$PATH:/home/apps
```

**Step 6 — Confirm the PATH change.**
Display the `PATH` variable to confirm the new directory was added:
```
echo $PATH
```

**Step 7 — Remove the variable.**
The application team has switched to a different database. Remove the `VAR_DATABASE` variable:
```
unset VAR_DATABASE
```

**Step 8 — Confirm the variable was removed.**
Check that `VAR_DATABASE` no longer has a value. The output should be empty:
```
echo $VAR_DATABASE
```

**Step 9 — View your command history.**
The application team has decided to keep the original configuration after all. Display the last 15 commands you have executed:
```
history 15
```

**Step 10 — Locate the export command.**
Search through the command history for the `export` command that set `VAR_DATABASE` to `prod-db-01`. Note the number listed next to it (for example, `25`).

**Step 11 — Re-run the command from history.**
Run that command again by typing `!` followed by its history number. For example:
```
!25
```
*Replace `25` with the number that appears next to the command in your own history.*

**Step 12 — Verify the variable is set again.**
Confirm that `VAR_DATABASE` has been restored to its original value:
```
echo $VAR_DATABASE
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from Steps 4, 6, 8, 9, and 12. Be sure your submission shows the variable being set, removed, and then restored from command history.

## Review Questions
1. What is the difference between an environment variable and a regular shell variable?
2. Why is the `PATH` variable important, and what happens when you add a directory to it?
3. When you set a variable with `export` in a terminal session, does it persist after you close the terminal? Why or why not?
4. What does the `unset` command do, and how can you confirm that a variable has been removed?
5. How does the `!` followed by a history number (such as `!25`) work, and why is it useful?

---

# Lab 6: Monitoring and Managing Processes

## Objective
A new Linux system administrator needs to learn how to manage processes on Linux servers. In this lab, you will demonstrate how to use the `ps` and `kill` commands to monitor running processes and terminate them as needed. To provide a realistic example, you will install and start the Apache `httpd` web server, locate its process, and then terminate it. By completing these tasks, you will practice identifying processes by name and process ID (PID) and controlling them from the command line.

## Prerequisites
Several of these commands require administrative privileges and internet access to download packages. Make sure you are logged in with an account that has the necessary permissions (such as `root` or an account with `sudo` access) before you begin.

## Instructions
Complete the following steps in order. Observe the output of each command and confirm the result before moving on.

**Step 1 — Install the Apache web server.**
Install the `httpd` package:
```
yum install -y httpd
```

**Step 2 — Start the service.**
Start the `httpd` service so there is a running process to work with:
```
systemctl start httpd
```

**Step 3 — List all running processes.**
Use `ps aux` to display all processes currently running on the machine:
```
ps aux
```

**Step 4 — Identify the httpd process.**
Examine the output and locate the `httpd` process. Note its process ID (PID) and any other relevant details.

**Step 5 — Filter for the httpd process.**
Narrow your search to processes matching `httpd` and note the PID of the first `httpd` entry:
```
ps -ef | grep httpd
```

**Step 6 — Terminate the process.**
Use the `kill` command followed by the PID you identified to terminate the process. For example:
```
kill 113
```
*Replace `113` with the actual PID from your own output.*

**Step 7 — Confirm the process was terminated.**
Re-run one of the process-listing commands to verify that the `httpd` process is no longer listed:
```
ps -ef | grep httpd
```

**Step 8 — Check the service status.**
Confirm that the `httpd` service is no longer running:
```
systemctl status httpd
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from Steps 5, 7, and 8. In your submission, clearly identify the PID of the `httpd` process you terminated.

## Review Questions
1. What is the difference between the `ps aux` and `ps -ef` command formats?
2. What is a process ID (PID), and why is it needed to terminate a process?
3. Why might you use `grep` together with `ps` when looking for a specific process?
4. What is the difference between stopping a service with `kill` and stopping it with `systemctl stop`?
5. After using `kill` to terminate the `httpd` process, what does the `systemctl status httpd` output tell you about the service's state?

---

# Lab 7: Installing and Using the `jq` JSON Parser

## Objective
As a Linux administrator, you're responsible for ensuring that the Data Team has the tools they need to process and analyze JSON data efficiently. In this lab, you will check whether the `jq` JSON parser is installed, install it using the package manager, verify the installation, and then use `jq` to create and parse a sample JSON file. By completing these tasks, you will practice installing software with a package manager and using `jq` to extract data from JSON files.

## Prerequisites
Installing packages requires administrative privileges and internet access. Make sure you are logged in with an account that has the necessary permissions (such as `root` or an account with `sudo` access) before you begin.

## Instructions
Complete the following steps in order. Observe the output of each command and confirm the result before moving on.

**Step 1 — Check whether `jq` is installed.**
Check for an existing `jq` installation. Because it is not yet installed, this command should return a "command not found" error:
```
jq --version
```

**Step 2 — Install `jq`.**
Install the `jq` package using your package manager:
```
yum install -y jq
```

**Step 3 — Verify the installation.**
Once the installation completes, confirm that `jq` is installed by displaying its version:
```
jq --version
```

**Step 4 — Locate the `jq` binary.**
Use the `which` command to identify where `jq` was installed:
```
which jq
```

**Step 5 — Create a JSON file.**
Create an empty file named `employee-list.json`:
```
touch employee-list.json
```

**Step 6 — Add JSON data to the file.**
Use a here-document with the `cat` command to write JSON-formatted data into the file. Type the following exactly as shown:
```
cat <<-EOF > employee-list.json
{
"employees": [
{
"name": "John Doe",
"age": 30,
"position": "Software Engineer"
}
]
}
EOF
```

**Step 7 — Confirm the data was written.**
Display the contents of the file to confirm the JSON data was saved correctly:
```
cat employee-list.json
```

**Step 8 — Parse the file with `jq`.**
Use `jq` to parse the `employees` array and confirm the tool is working as expected:
```
cat employee-list.json | jq '.employees'
```

**Step 9 — Extract a specific field.**
Use `jq` to retrieve just the name of the employee from the JSON data:
```
cat employee-list.json | jq '.employees[].name'
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from Steps 3, 4, 7, 8, and 9. Be sure your submission shows both the full parsed `employees` array and the extracted employee name.

## Review Questions
1. Why is it useful to run `jq --version` both before and after installing the tool?
2. What does the `which` command tell you about an installed program?
3. What is a here-document (the `<<-EOF ... EOF` syntax), and what is it used for in Step 6?
4. In the expression `.employees[].name`, what does each part (`.employees`, `[]`, and `.name`) do?
5. How could a tool like `jq` be useful when working with JSON data returned from an API?

---

# Lab 8: Updating System Software with `yum`

## Objective
You have been tasked with keeping your system current by applying the latest security patches and software updates. In this lab, you will use the `yum` package manager to review the packages installed on the system, identify which packages have updates available, and apply those updates. By completing these tasks, you will practice performing routine system maintenance to ensure a server runs the latest supported versions of its installed software.

## Prerequisites
These commands require administrative privileges and internet access to reach the package repositories. Make sure you are logged in with an account that has the necessary permissions (such as `root` or an account with `sudo` access) before you begin.

## Instructions
Complete the following steps in order. Observe the output of each command and confirm the result before moving on.

**Step 1 — List installed packages.**
Display the packages that are already installed on the system:
```
yum list installed
```

**Step 2 — View available updates.**
Display the packages that have updates available in the repositories:
```
yum list available
```

**Step 3 — Apply the updates.**
Upgrade the installed packages to their latest available versions. Depending on the current state of the system, there may be no packages available to update:
```
yum upgrade
```

## Deliverables
Submit a document containing the commands you ran for each step, along with a screenshot or copy of the output from Steps 1, 2, and 3. In your submission, note whether any packages were available to update and, if so, how many were upgraded.

## Review Questions
1. Why is it important to keep a system's software packages up to date?
2. What is the difference between `yum list installed` and `yum list available`?
3. What is the difference between `yum upgrade` and `yum update`?
4. Why might the `yum upgrade` command report that there are no packages to update?
5. Why do package management commands like `yum upgrade` typically require administrative privileges?
