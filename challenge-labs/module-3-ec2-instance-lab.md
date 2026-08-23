# Lab Assignment: Launching and Managing an Amazon EC2 Instance (Linux)

## Table of Contents
1. [Overview](#overview)
2. [Objectives](#objectives)
3. [Creating an Amazon EC2 Instance](#creating-an-amazon-ec2-instance)
4. [Configuring Security Groups](#configuring-security-groups)
5. [Connecting to the Instance via SSH](#connecting-to-the-instance-via-ssh)
6. [PEM vs. PPK: Understanding Private Key Formats](#pem-vs-ppk-understanding-private-key-formats)
7. [Stop, Reboot, and Terminate Operations](#stop-reboot-and-terminate-operations)
8. [Review Questions](#review-questions)

---

## Overview
Amazon Elastic Compute Cloud (Amazon EC2) is a web service that lets you create and manage virtual servers in the AWS Cloud. With EC2, you can configure your own operating system and applications to meet your specific requirements.

An EC2 instance is a virtual server running on AWS. When you launch an instance, it is secured with two key components: a **key pair**, which verifies your identity when connecting, and a **security group**, which acts as a virtual firewall to control incoming and outgoing traffic. When connecting to your instance via SSH, you must provide the private key from the key pair you specified at launch.

In this lab, you will launch a Linux virtual server on Amazon EC2 and gain hands-on experience with core cloud computing concepts that you can apply to your own projects.

---

## Objectives
By the end of this lab, you will know how to:
- Create an EC2 instance (t2.micro)
- Configure a security group for SSH access
- Connect to the instance through SSH
- Perform Stop, Reboot, and Terminate operations on an instance

---

## Creating an Amazon EC2 Instance

**Step 1 — Open the EC2 Dashboard.**
In the AWS Management Console, type `EC2` in the search bar and click the result to open the EC2 Dashboard.

**Step 2 — Begin launching an instance.**
Click the **Launch Instance** button.

**Step 3 — Add a name and tags.**
In the **Name and tags** section, you can provide a name for your instance and add optional tags as key/value pairs. Tagging resources is recommended in production environments to stay organized, but it is not required for this lab. You may skip this section if you prefer.

**Step 4 — Select an Amazon Machine Image (AMI).**
An AMI is a pre-configured operating system template that serves as the foundation for your instance. For this lab, select **Ubuntu**.

**Step 5 — Choose an instance type.**
For the instance type, select **t2.micro**. This is a small, general-purpose instance suitable for this lab and eligible for the AWS Free Tier.

**Step 6 — Create a key pair.**
In the **Key pair** section, click **Create new key pair**. Enter `MyKeyPair` as the name, keep the default values for key pair type and private key file format, and click **Create key pair**. A file named `MyKeyPair.pem` will automatically download to your local machine. This file contains your private key and is required to connect to your instance via SSH. Store it in a safe location, as it cannot be downloaded again.

**Step 7 — Configure the security group.**
In the **Network Settings** section, ensure that the **Allow SSH traffic from** checkbox is checked and that **Anywhere** is selected.

> **AWS Security Note:** The default security group configuration will allow SSH access from any source IP address (`0.0.0.0/0`). This is acceptable for lab purposes, but production environments should use more restrictive rules that limit access to known IP addresses.

**Step 8 — Configure storage.**
In the **Configure storage** section, confirm the default values are selected: **8 GiB** and **gp2** Root volume. No changes are needed.

**Step 9 — Review advanced settings.**
Click **Advanced Details** to expand the section and take a moment to review the additional configuration options available. No changes are required for this lab.

**Step 10 — Review all settings.**
Before proceeding, review all of the settings you have configured to make sure everything is correct.

**Step 11 — Launch the instance.**
Click the **Launch instance** button. A confirmation page will appear to let you know the instance is being created. It may take a few moments for the instance to reach a running state.

---

## Configuring Security Groups

**Step 1 — Open your running instances.**
From the EC2 Dashboard, click **Instances (running)** under the Resources section.

**Step 2 — Select your instance.**
Check the checkbox next to the instance you want to configure.

**Step 3 — Navigate to the security group.**
Click the **Security** tab, then click the security group ID listed there. Security group IDs begin with `sg-`.

**Step 4 — Add an inbound rule.**
To allow SSH access to your instance, add an inbound rule to the security group. When defining the rule, specify a source IP address rather than using a broad range.

> **Security Best Practice:** Avoid using `0.0.0.0/0` (all IPv4 addresses) or `::/0` (all IPv6 addresses) as the source, as either setting allows anyone on the internet to attempt a connection. Instead, specify your own IP address or a narrow range of trusted addresses to limit exposure.

---

## Connecting to the Instance via SSH

**Step 1 — Wait for the instance to be ready.**
After launching, it may take a few minutes before your instance is ready to accept connections. Monitor its status in the EC2 Dashboard until it shows a **Running** state and passes its status checks.

**Step 2 — Find the public DNS name or IP address.**
In the EC2 Dashboard, select your instance and locate its public DNS name or public IP address in the instance details panel.

**Step 3 — Confirm SSH is available on your local machine.**
Open a terminal and type `ssh`. If the command is not recognized, install an SSH client before proceeding.

**Step 4 — Connect to your instance.**
Use the `ssh` command, providing the path to your private key file, the instance username, and the public DNS name or IP address:
```
ssh -i "path/to/your/key.pem" ubuntu@your-instance-public-dns
```

---

## PEM vs. PPK: Understanding Private Key Formats

When connecting to an EC2 instance, the format of your private key file depends on the SSH client you are using. The two most common formats are PEM and PPK.

**PEM (Privacy Enhanced Mail)**
PEM is a Base64-encoded container format for storing private keys and certificates. It is the standard format used by Linux, macOS, and Windows PowerShell users. AWS provides your key pair as a `.pem` file at the time of creation — this is a one-time download and cannot be retrieved again. To use a PEM file with SSH, pass it with the `-i` option:
```
ssh -i mykey.pem ubuntu@your-instance-public-dns
```

**PPK (PuTTY Private Key)**
PPK is a proprietary format used by PuTTY, a popular SSH client for Windows. PuTTY does not support `.pem` files directly, so you must convert your `.pem` file to `.ppk` format using a companion tool called PuTTYgen. Once converted, load the `.ppk` file in PuTTYgen and save it as a private key to use with PuTTY.

| | PEM | PPK |
|---|---|---|
| **Used by** | Linux, macOS, Windows PowerShell | PuTTY (Windows) |
| **Provided by AWS** | Yes, at key pair creation | No — converted from PEM |
| **SSH command support** | Yes, via `-i` flag | No — requires PuTTY |
| **Conversion needed** | No | Yes, using PuTTYgen |

> **Important:** Keep your private key file secure at all times. Never share it with unauthorized individuals. If your private key is compromised, anyone who obtains it can access your instance.

---

## Stop, Reboot, and Terminate Operations

Once your EC2 instance is running, you have three ways to change its state: stopping it temporarily, rebooting it, or terminating it permanently. Understanding the difference between these operations — and what data each one preserves or destroys — is essential for managing your instances safely.

### Stopping an Instance

Stopping an instance is similar to shutting down a computer. The instance is powered off but not deleted, and you can start it again later.

**Steps:**
1. Navigate to the EC2 Dashboard and select the instance you want to stop.
2. Click the **Instance state** dropdown menu.
3. Click **Stop**.

**What is lost when you stop an instance:**
- Data stored in RAM
- Data stored on instance store volumes
- The public IPv4 address automatically assigned at launch (to keep a permanent public IP, associate an Elastic IP address with your instance)

**What persists when you stop an instance:**
- All attached Amazon EBS volumes and the data on them
- Private IPv4 addresses
- IPv6 addresses
- Any Elastic IP addresses associated with the instance

> **Note:** Elastic IP addresses that remain associated with a stopped instance are still billed, even while the instance is not running.

---

### Rebooting an Instance

Rebooting is equivalent to restarting the operating system. It is the least disruptive state change and typically completes within a few minutes.

**Steps:**
1. Navigate to the EC2 Dashboard and select the instance you want to reboot.
2. Click the **Instance state** dropdown menu.
3. Click **Reboot**.

**What is preserved during a reboot:**
- Public DNS name (IPv4)
- Public and private IPv4 addresses
- IPv6 address (if applicable)
- Data on instance store volumes

> **Note:** A reboot does not start a new billing period. Unlike stopping and starting an instance, which incurs a new minimum one-minute charge, a reboot is billed continuously as part of the same running period.

---

### Terminating an Instance

Termination is permanent. Once an instance is terminated, it cannot be recovered. Use this option only when you no longer need the instance.

**Steps:**
1. Navigate to the EC2 Dashboard and select the instance you want to terminate.
2. Click the **Instance state** dropdown menu.
3. Click **Terminate**.

**Effects of termination:**
- The instance is permanently shut down and removed
- All data stored locally on the instance is lost
- Attached EBS volumes are detached and deleted unless they were configured to persist after termination
- You will no longer be charged for instance usage after termination

---

### Quick Comparison

| | **Stop** | **Reboot** | **Terminate** |
|---|---|---|---|
| **Instance recoverable** | Yes | Yes | No |
| **Public IPv4 retained** | No | Yes | No |
| **Private IPv4 retained** | Yes | Yes | No |
| **EBS volumes retained** | Yes | Yes | Only if set to persist |
| **Instance store data** | Lost | Retained | Lost |
| **Billing** | No instance charge while stopped | Continuous | Ends immediately |

---

## Deliverables
Take a screenshot of each of the following and include them in your submission:
- The AMI and instance type selections (Creating an Instance, Steps 4 and 5)
- The Network Settings section showing SSH access enabled (Creating an Instance, Step 7)
- The confirmation page after clicking Launch instance (Creating an Instance, Step 11)
- The EC2 Instances list showing your instance in a **Running** state
- The security group inbound rule showing your IP address as the SSH source
- The terminal output confirming a successful SSH connection

---

## Review Questions
1. What is the purpose of an Amazon Machine Image (AMI), and how does it differ from an EC2 instance?
2. Why is the key pair file (`.pem`) important, and what happens if you lose it?
3. What is a security group, and how does it differ from a traditional firewall?
4. Why is allowing SSH access from `0.0.0.0/0` considered a security risk in production environments?
5. What is the difference between a PEM file and a PPK file, and when would you need to use each?
6. What tool do you use to convert a `.pem` file to `.ppk` format, and why is this conversion necessary?
7. What is the main difference between stopping and terminating an EC2 instance?
8. Why might you associate an Elastic IP address with an instance rather than relying on the automatically assigned public IPv4 address?
9. In what situation would you choose to reboot an instance rather than stop and start it?
10. What happens to data stored on an instance store volume when an instance is stopped? How does this differ from a reboot?
11. If an EBS volume is attached to an instance that is terminated, what determines whether the volume is deleted or retained?
