# AWS CI/CD Pipeline Lab Workbook

This workbook guides you through building a complete continuous integration pipeline on AWS using CodeCommit, CodeBuild, CodePipeline, and SNS. Complete the labs in order — each one builds on the previous.

> **Lab assets:** You need two files from the lab assets folder: `demo.cpp` (Lab 2) and `sns-access-policy.json` (Lab 5). Both are reproduced in full in the labs below in case you do not have the assets folder.

> **Region:** Create every resource in the **same AWS region** (for example, `us-east-1`). CodePipeline and the notification rule can only see resources in the region you are working in.

## Table of Contents
1. [Creating a Repository in AWS CodeCommit](#lab-1-creating-a-repository-in-aws-codecommit)
2. [Committing a File to AWS CodeCommit](#lab-2-committing-a-file-to-aws-codecommit)
3. [Creating and Committing a buildspec.yml File](#lab-3-creating-and-committing-a-buildspecyml-file)
4. [Creating an AWS CodeBuild Project](#lab-4-creating-an-aws-codebuild-project)
5. [Creating an SNS Topic for Build Failure Notifications](#lab-5-creating-an-sns-topic-for-build-failure-notifications)
6. [Integrating CodeBuild with SNS](#lab-6-integrating-codebuild-with-sns-for-build-failure-notifications)
7. [Creating a CI Pipeline with AWS CodePipeline](#lab-7-creating-a-ci-pipeline-with-aws-codepipeline)
8. [Testing Build Failure Notifications](#lab-8-testing-build-failure-notifications)
9. [Cleaning Up AWS Resources](#lab-9-cleaning-up-aws-resources)

---

# Lab 1: Creating a Repository in AWS CodeCommit

## Objective
In this lab, you will create a new source code repository in AWS CodeCommit. This repository will store your application code and will later be connected to an automated continuous integration (CI) pipeline, so that any new code pushed to the repository is automatically built and any build failure is reported by email.

## Prerequisites
- An active AWS account with access to the AWS Management Console
- Sufficient IAM permissions to create and manage CodeCommit repositories. Your IAM user or role must have at minimum the `AWSCodeCommitFullAccess` policy or equivalent permissions attached

> **Note:** AWS CodeCommit is a fully managed source control service hosted by AWS. It works similarly to GitHub or GitLab but integrates natively with other AWS services such as CodePipeline, CodeBuild, and CodeDeploy.

## Instructions

**Step 1 — Open the CodeCommit service.**
In the AWS Management Console, click the search bar at the top of the page, type `CodeCommit`, and select **CodeCommit** from the results to open the service.

**Step 2 — Navigate to the Repositories page.**
In the CodeCommit console, click **Repositories** in the left navigation panel. This page lists all existing repositories in your AWS account for the current region.

**Step 3 — Begin creating a new repository.**
Click the **Create repository** button in the top-right area of the Repositories page.

**Step 4 — Enter a repository name.**
On the Create repository page, enter a name in the **Repository name** field. For this lab, use:
```
DemoRepo
```
Repository names must meet the following requirements:
- Between 1 and 100 characters
- Only letters, numbers, hyphens (`-`), and underscores (`_`) are allowed
- Must be unique within your AWS account and region

**Step 5 — Add a description (optional).**
You may enter a brief description in the **Description** field to help identify the repository's purpose. For example:
```
Application source code repository for the deployment pipeline demo.
```

**Step 6 — Create the repository.**
Click the **Create** button to finish. AWS will provision the repository and redirect you to its details page.

**Step 7 — Verify the repository was created.**
On the repository details page, confirm the following:
- The repository name shows **DemoRepo**
- A **Clone URL** is available (both HTTPS and SSH options are shown)
- The repository is empty and ready to receive code

## Deliverables
Submit a screenshot showing:
- Step 2 — the Repositories page before creating the repository
- Step 4 — the Create repository form with the name `DemoRepo` filled in
- Step 7 — the repository details page confirming the repository was created successfully, including the Clone URL

## Review Questions
1. What is AWS CodeCommit, and how does it differ from GitHub or GitLab?
2. Why would a team choose to use CodeCommit instead of a third-party Git hosting service?
3. What does the Clone URL allow you to do, and what is the difference between the HTTPS and SSH options?
4. Why is it important that the repository is empty when first created before pushing existing local code to it?
5. How does connecting CodeCommit to a pipeline enable automated builds, and what would be needed to extend it to automated deployments to EC2 instances?

---

# Lab 2: Committing a File to AWS CodeCommit

## Objective
In this lab, you will add the sample C++ source file provided by the development team to your CodeCommit repository. You will use the built-in CodeCommit file editor in the AWS Console, which allows you to create and commit files directly without needing to clone the repository locally. By the end of this lab, the `demo.cpp` file will be committed and visible in the `DemoRepo` repository.

## Prerequisites
- Lab 1 is complete and the `DemoRepo` repository exists in AWS CodeCommit
- The `demo.cpp` file is available on your local workstation (from the lab assets, or create it from the listing below)

## Lab Asset: demo.cpp
If you do not have the file, create `demo.cpp` with exactly this content. The file **must end with the closing brace `}` of `main()` on its own line**, because Lab 8 removes that line to cause a build failure.

```cpp
#include <iostream>
#include <string>

int main() {
    std::string appName = "CI/CD Pipeline Demo";
    int buildNumber = 1;

    std::cout << "Hello from " << appName << "!" << std::endl;
    std::cout << "Build number: " << buildNumber << std::endl;
    std::cout << "If you can read this, CodeBuild compiled the code successfully." << std::endl;

    return 0;
}
```
- You have sufficient IAM permissions to commit files to the repository

## Instructions

**Step 1 — Open your repository.**
In the AWS Management Console, search for `CodeCommit` and open the service. On the **Repositories** page, locate and click on **DemoRepo** to open it.

**Step 2 — Open the file editor.**
Inside the repository, click the **Add file** button and select **Create file** from the dropdown menu.

> **Note:** Because `DemoRepo` is empty, this first commit also creates the repository's default branch, **`main`**. Later labs depend on this branch name. After committing, confirm the branch selector at the top of the repository page shows `main`.

> **Tip:** If you prefer to upload the file directly rather than copying its contents, select **Upload file** from the same dropdown instead and skip to Step 4.

**Step 3 — Copy the file contents.**
On your local workstation, open `demo.cpp` in a text editor such as Notepad (Windows) or TextEdit (macOS). Select all of the content, copy it, and paste it into the CodeCommit code editor on the screen.

**Step 4 — Fill in the file details.**
Scroll to the bottom of the page and complete the following fields:

- **File name:** `demo.cpp`
- **Author name:** Enter your full name
- **Email address:** Enter your email address
- **Commit message:**
```
Add initial C++ demo application source file
```

**Step 5 — Commit the file.**
Click the **Commit changes** button. CodeCommit will save the file and create a new commit in the repository.

**Step 6 — Verify the file was committed.**
You will be redirected to the repository home page. Confirm that `demo.cpp` is now listed as a file in the **DemoRepo** repository. Click on the file name to open it and verify that the contents match the original `demo.cpp` file from your workstation.

## Deliverables
Submit a screenshot showing:
- Step 2 — the Add file dropdown with the Create file or Upload file option visible
- Step 4 — the bottom of the commit form showing the file name, author name, email, and commit message filled in
- Step 6 — the repository home page showing `demo.cpp` listed as a committed file
- Step 6 — the file contents view confirming the code matches the original

## Review Questions
1. What is the difference between using the CodeCommit built-in editor and cloning the repository locally to commit files?
2. Why is a commit message important, and what makes a good commit message?
3. In a real development workflow, what tool would developers typically use to commit code to CodeCommit instead of the AWS Console editor?
4. What information is captured as part of a commit in CodeCommit, and why is the author name and email address recorded?
5. If the wrong file contents were committed by mistake, what options does CodeCommit provide to correct the error?

---

# Lab 3: Creating and Committing a buildspec.yml File

## Objective
In this lab, you will create a `buildspec.yml` file that provides AWS CodeBuild with the instructions it needs to compile your C++ application. You will then commit this file to your CodeCommit repository alongside the `demo.cpp` source file. CodeBuild will automatically reference this file when executing a build job.

## Prerequisites
- Labs 1 and 2 are complete
- The `DemoRepo` repository exists in CodeCommit and already contains `demo.cpp`
- You have sufficient IAM permissions to commit files to the repository

## Background: What is a buildspec.yml?
A `buildspec.yml` is a YAML-formatted file that tells AWS CodeBuild exactly what commands to run during a build. It is divided into **phases**, each representing a stage of the build process. CodeBuild reads this file from the root of your repository and executes the commands in order.

The three most common phases are:

| Phase | Purpose |
|---|---|
| `install` | Install any dependencies or tools needed before building |
| `build` | Run the commands that compile or package the application |
| `post_build` | Run any steps after the build, such as packaging or notifications |

## Instructions

**Step 1 — Review the buildspec.yml content.**
Based on the requirements provided by the development team, the `buildspec.yml` file for this project is as follows. Review it carefully before committing it to the repository:

```yaml
version: 0.2

phases:
  install:
    commands:
      - apt-get update -y
      - apt-get install -y build-essential
  build:
    commands:
      - g++ demo.cpp -o democode
```

Here is what each section does:

- `version: 0.2` — specifies the buildspec format version. Version 0.2 is the current standard
- `install` phase — updates the package list and installs `build-essential`, which provides the `g++` C++ compiler and related build tools. (The CodeBuild Standard image already includes `g++`; this step guarantees it and shows how dependencies are installed. Commands run as root, so `sudo` is not needed.)
- `build` phase — compiles `demo.cpp` using `g++` and outputs an executable named `democode`

**Step 2 — Open your repository.**
In the AWS Management Console, search for `CodeCommit` and open the service. On the **Repositories** page, click on **DemoRepo** to open it.

**Step 3 — Open the file editor.**
Inside the repository, click the **Add file** button and select **Create file** from the dropdown menu.

**Step 4 — Enter the file contents.**
In the CodeCommit code editor, carefully type or paste the `buildspec.yml` content from Step 1.

> **Important:** YAML files are sensitive to indentation. Use spaces, not tabs, and make sure each level of indentation is consistent. Incorrect indentation will cause CodeBuild to fail when reading the file.

**Step 5 — Fill in the file details.**
Scroll to the bottom of the page and complete the following fields:

- **File name:** `buildspec.yml`
- **Author name:** Enter your full name
- **Email address:** Enter your email address
- **Commit message:**
```
Add buildspec.yml with install and build instructions for CodeBuild
```

**Step 6 — Commit the file.**
Click the **Commit changes** button. CodeCommit will save the file and create a new commit in the repository.

**Step 7 — Verify the file was committed.**
You will be redirected to the repository home page. Confirm that both `demo.cpp` and `buildspec.yml` are now listed as files in the **DemoRepo** repository. Click on `buildspec.yml` to open it and verify that the contents and indentation match exactly what was entered in Step 1.

## Deliverables
Submit a screenshot showing:
- Step 3 — the Add file dropdown with Create file selected
- Step 5 — the bottom of the commit form showing the file name, author name, email, and commit message filled in
- Step 7 — the repository home page showing both `demo.cpp` and `buildspec.yml` listed as committed files
- Step 7 — the file contents view of `buildspec.yml` confirming the content and indentation are correct

## Review Questions
1. What is the purpose of the `buildspec.yml` file, and where must it be placed in the repository for CodeBuild to find it automatically?
2. Why does the `install` phase run `apt-get update` before installing `build-essential`?
3. What does the `g++ demo.cpp -o democode` command do, and what is the significance of the `-o` flag?
4. What would happen if the `buildspec.yml` file contained incorrect indentation?
5. In a more complex project, what kinds of commands might you add to a `post_build` phase after the application has been compiled?

---

# Lab 4: Creating an AWS CodeBuild Project

## Objective
In this lab, you will create an AWS CodeBuild project that connects to your CodeCommit repository and uses the `buildspec.yml` file to automatically compile the C++ application. By the end of this lab, CodeBuild will be configured to fetch the source code from `DemoRepo` and run the build process successfully.

## Prerequisites
- Labs 1, 2, and 3 are complete
- The `DemoRepo` repository contains both `demo.cpp` and `buildspec.yml`
- You have sufficient IAM permissions to create CodeBuild projects and service roles

## Background: What is AWS CodeBuild?
AWS CodeBuild is a fully managed continuous integration service that compiles source code, runs tests, and produces deployable software packages. It eliminates the need to manage your own build servers. CodeBuild reads the `buildspec.yml` file from your repository and executes the defined commands in an isolated, temporary build environment.

## Instructions

**Step 1 — Open the CodeBuild service.**
In the AWS Management Console, click the search bar at the top of the page, type `CodeBuild`, and select **CodeBuild** from the results.

**Step 2 — Begin creating a build project.**
On the CodeBuild landing page, click **Create build project**. If you are taken to an information page first, click the **Create build project** button displayed there. Alternatively, in the left navigation pane, expand **Build**, choose **Build projects**, and then click **Create build project**.

---

### Project Configuration

**Step 3 — Enter the project name.**
In the **Project configuration** section, fill in the following:

- **Project name:** `DemoBuild`
- **Description** *(optional):* `CodeBuild project to compile the C++ demo application from DemoRepo`

---

### Source

**Step 4 — Configure the source provider.**
In the **Source** section, fill in the following details to connect CodeBuild to your CodeCommit repository:

- **Source provider:** `AWS CodeCommit`
- **Repository:** `DemoRepo`
- **Reference type:** `Branch`
- **Branch:** `main`

> **Note:** Selecting the `main` branch means CodeBuild will always pull the latest code from that branch when a build is triggered.

---

### Environment

**Step 5 — Configure the build environment.**
In the **Environment** section, fill in the following details. These settings define the virtual machine that CodeBuild will use to run your build:

- **Provisioning model:** `On-demand`
- **Environment image:** `Managed image`
- **Compute:** `EC2`
- **Operating system:** `Ubuntu`
- **Runtime(s):** `Standard`
- **Image:** `aws/codebuild/standard:7.0`
- **Image version:** `Always use the latest image for this runtime version`
- **Service role:** `New service role`

> **Note:** Older versions of this lab used `aws/codebuild/standard:5.0`. That image is deprecated and no longer listed in the console. Use `standard:7.0`. If your console shows a newer Standard image (such as `8.0`), it will also work. Some console versions show an **Environment type** field instead of **Compute**; choose `Linux` / `Linux EC2`.

> **Note:** Choosing **New service role** allows AWS to automatically create an IAM role with the permissions CodeBuild needs to access your CodeCommit repository and write build logs to CloudWatch. The role is named `codebuild-DemoBuild-service-role` by default; note this name, because you will delete it in Lab 9.

---

### Buildspec

**Step 6 — Select the buildspec option.**
In the **Buildspec** section, ensure that **Use a buildspec file** is selected.

> This tells CodeBuild to look for a file named `buildspec.yml` in the root of your repository. Because you committed this file in Lab 3, CodeBuild will find it automatically when the build runs — no additional path configuration is needed.

---

### Artifacts, Batch Configuration, and Logs

**Step 7 — Leave remaining sections at their defaults.**
For this lab, the **Artifacts**, **Batch configuration**, and **Logs** sections do not require changes. Leave them at their default values and proceed to the bottom of the page.

---

**Step 8 — Create the build project.**
Scroll to the bottom of the page and click the **Create build project** button. AWS will provision the project and redirect you to the CodeBuild project console.

**Step 9 — Verify the project was created.**
On the project console page, confirm the following:
- The project name shows **DemoBuild**
- The source shows **DemoRepo** on the **main** branch
- The environment shows the **Ubuntu Standard 7.0** managed image
- The buildspec shows **buildspec.yml**

## Deliverables
Submit a screenshot showing:
- Step 4 — the Source section with CodeCommit, DemoRepo, and the main branch selected
- Step 5 — the Environment section showing the Ubuntu managed image and new service role settings
- Step 6 — the Buildspec section with "Use a buildspec file" selected
- Step 9 — the completed CodeBuild project console page showing all configuration details

## Review Questions
1. What is AWS CodeBuild, and how does it differ from running builds on a local machine or a self-managed build server?
2. Why is it important to select the correct branch in the Source configuration when setting up a CodeBuild project?
3. What is the purpose of the IAM service role that is created for CodeBuild, and what permissions does it typically need?
4. What is a managed image in CodeBuild, and what advantage does it offer over a custom build environment?
5. What would happen if the `buildspec.yml` file were missing from the root of the repository when a build is triggered?

---

# Lab 5: Creating an SNS Topic for Build Failure Notifications

## Objective
In this lab, you will create an Amazon Simple Notification Service (SNS) topic that enables AWS CodeBuild to send email notifications to developers when a build fails. You will configure a custom access policy that grants AWS CodeStar Notifications (the service that delivers CodeBuild notification rules) permission to publish messages to the topic, add your email address as a subscriber, and confirm the subscription. By the end of this lab, your notification pipeline will be ready to alert the team of any build failures.

## Prerequisites
- Labs 1 through 4 are complete and the `DemoBuild` CodeBuild project exists
- You have sufficient IAM permissions to create SNS topics and subscriptions
- You have access to the `sns-access-policy.json` file from the lab assets
- You have access to the email inbox you will use as the notification endpoint

## Background: What is Amazon SNS?
Amazon Simple Notification Service (SNS) is a fully managed messaging service that enables applications and services to send notifications to subscribers. In this lab, SNS acts as the bridge between CodeBuild and your email inbox. When a CodeBuild build fails, the notification rule you create in Lab 6 sends the event through AWS CodeStar Notifications, which publishes a message to the SNS topic. SNS then delivers that message to all confirmed subscribers. Because the publisher is the CodeStar Notifications service (`codestar-notifications.amazonaws.com`), not CodeBuild itself, the topic's access policy must allow that service to publish.

## Instructions

### Part A: Create the SNS Topic

**Step 1 — Open the SNS service.**
In the AWS Management Console, click the search bar at the top of the page, type `SNS`, and select **Simple Notification Service** from the results.

**Step 2 — Begin creating a topic.**
Depending on whether topics have been created in your account before, follow the appropriate path:

- **If no topics exist yet:** A topic name field will be displayed on the SNS home page. Enter `fail-build-topic` in the field and click **Next step**.
- **If topics already exist:** In the left navigation panel, click **Topics**, then click **Create topic** on the Topics page.

**Step 3 — Configure the topic details.**
On the **Create topic** page, fill in the following fields in the **Details** section:

- **Type:** `Standard`
- **Name:** `fail-build-topic`

> **Note:** Standard topics support best-effort message ordering and at-least-once delivery, which is appropriate for notifications like build alerts. FIFO topics are used when strict message ordering is required and are not needed here.

---

### Part B: Configure the Access Policy

**Step 4 — Open the Access Policy section.**
Scroll down the Create topic page to the **Access policy** section and expand it by clicking on the section header.

**Step 5 — Switch to the Advanced editor.**
Under **Choose method**, click **Advanced** to open the JSON policy editor.

**Step 6 — Enter the custom access policy.**
In the JSON editor, select all of the existing policy and replace it with the custom policy from the `sns-access-policy.json` file in your lab assets (reproduced below). Then replace every placeholder:

- `REGION` (2 places) — the AWS region where your resources are located (for example, `us-east-1`)
- `ACCOUNT_ID` (4 places) — your 12-digit AWS account number, with no dashes

```json
{
  "Version": "2008-10-17",
  "Id": "fail-build-topic-policy",
  "Statement": [
    {
      "Sid": "__default_statement_ID",
      "Effect": "Allow",
      "Principal": {
        "AWS": "*"
      },
      "Action": [
        "SNS:GetTopicAttributes",
        "SNS:SetTopicAttributes",
        "SNS:AddPermission",
        "SNS:RemovePermission",
        "SNS:DeleteTopic",
        "SNS:Subscribe",
        "SNS:ListSubscriptionsByTopic",
        "SNS:Publish"
      ],
      "Resource": "arn:aws:sns:REGION:ACCOUNT_ID:fail-build-topic",
      "Condition": {
        "StringEquals": {
          "AWS:SourceOwner": "ACCOUNT_ID"
        }
      }
    },
    {
      "Sid": "AWSCodeStarNotifications_publish",
      "Effect": "Allow",
      "Principal": {
        "Service": [
          "codestar-notifications.amazonaws.com"
        ]
      },
      "Action": "SNS:Publish",
      "Resource": "arn:aws:sns:REGION:ACCOUNT_ID:fail-build-topic",
      "Condition": {
        "StringEquals": {
          "aws:SourceAccount": "ACCOUNT_ID"
        }
      }
    }
  ]
}
```

The first statement is the SNS default policy, which lets your own account manage the topic. The second statement is the one this lab needs: it allows the CodeStar Notifications service to publish to this topic, but only on behalf of your account. After replacing the placeholders, both `Resource` values should look like this:
```
arn:aws:sns:us-east-1:123456789012:fail-build-topic
```

> **Tip:** To quickly find your Account ID and the correct ARN format, temporarily switch back to the **Basic** method view — a sample policy will be displayed with these values pre-filled. Copy them, then switch back to **Advanced** to apply them to your custom policy.

---

### Part C: Create the Topic

**Step 7 — Create the topic.**
Scroll to the bottom of the Create topic page and click **Create topic**. You will see a success message and be redirected to the topic's detail page.

**Step 8 — Note the Topic ARN.**
On the topic detail page, locate and copy the **Topic ARN**. It will look similar to the following:
```
arn:aws:sns:us-east-1:123456789012:fail-build-topic
```
You will need this ARN when connecting the topic to CodeBuild in the next lab.

---

### Part D: Add an Email Subscription

**Step 9 — Open the Subscriptions tab.**
On the topic detail page, scroll to the bottom and click the **Subscriptions** tab. Then click **Create subscription**.

**Step 10 — Configure the subscription.**
On the Create subscription page, fill in the following:

- **Topic ARN:** This should already be pre-filled with the ARN of `fail-build-topic`
- **Protocol:** `Email`
- **Endpoint:** Enter the email address where you want to receive build failure notifications

**Step 11 — Create the subscription.**
Scroll to the bottom of the page and click **Create subscription**. You will see a success message confirming the subscription was created. Its status will show as **Pending confirmation** until you verify your email address.

---

### Part E: Confirm the Subscription

**Step 12 — Check your email inbox.**
Open the email inbox you provided in Step 10 and look for an email from `AWS Notifications`. The subject line will read:
```
AWS Notification - Subscription Confirmation
```

**Step 13 — Confirm the subscription.**
Inside the email, click the **Confirm subscription** link. You will be redirected to a confirmation page in your browser showing a success message.

**Step 14 — Verify the subscription status.**
Return to the AWS Console, navigate back to your `fail-build-topic` topic, and click the **Subscriptions** tab. The status of your email subscription should now show as **Confirmed**.

## Deliverables
Submit a screenshot showing:
- Step 3 — the Create topic form with the type set to Standard and the name `fail-build-topic`
- Step 6 — the JSON access policy editor showing your completed custom policy
- Step 7 — the success message after the topic was created
- Step 10 — the Create subscription form showing Email as the protocol and your email address as the endpoint
- Step 14 — the Subscriptions tab showing the subscription status as **Confirmed**

## Review Questions
1. What is Amazon SNS, and how does it differ from Amazon SQS?
2. Why is a custom access policy required, and why does it name `codestar-notifications.amazonaws.com` as the principal rather than CodeBuild?
3. What does the `Resource` element in the SNS access policy represent, and why must it be specific to your topic ARN?
4. Why does the email subscription start in a **Pending confirmation** state, and what security purpose does the confirmation step serve?
5. In a real development environment, what other types of endpoints besides email could be used as SNS subscribers to receive build failure notifications?

---

# Lab 6: Integrating CodeBuild with SNS for Build Failure Notifications

## Objective
In this lab, you will create a notification rule inside your CodeBuild project that integrates with the SNS topic created in Lab 5. This integration will automatically send an email to subscribed developers whenever a build fails. By the end of this lab, the end-to-end notification pipeline will be fully configured and active.

## Prerequisites
- All previous labs are complete
- The `DemoBuild` CodeBuild project exists and is connected to `DemoRepo`
- The `fail-build-topic` SNS topic exists and your email subscription is confirmed
- You have sufficient IAM permissions to create CodeBuild notification rules

## Background: What are CodeBuild Notification Rules?
AWS CodeBuild notification rules allow you to define which build events should trigger notifications and where those notifications should be sent. Notifications are delivered through AWS CodeStar Notifications, which acts as the bridge between CodeBuild events and your chosen SNS topic. Once configured, the rule monitors your build project and automatically publishes a message to SNS whenever a matching event occurs.

## Instructions

### Part A: Create the Notification Rule

**Step 1 — Navigate to your CodeBuild project.**
In the AWS Management Console, search for `CodeBuild` and open the service. In the left navigation panel, expand **Build** and choose **Build projects**. Click on **DemoBuild** to open the project.

**Step 2 — Open the notification menu.**
On the **DemoBuild** project page, click the **Notify** dropdown at the top of the page.

**Step 3 — Begin creating a notification rule.**
From the **Notify** dropdown, choose **Create notification rule**.

> **Alternative:** You can also open **Settings → Notification rules** in the CodeBuild left navigation and click **Create notification rule**. If you go this way, set the **Source** to the `DemoBuild` project.

---

### Part B: Configure the Notification Rule

**Step 4 — Enter the rule name and detail type.**
In the notification rule configuration form, fill in the following:

- **Notification name:** `fail-build-notification`
- **Detail type:** `Full`

> **Note:** Choosing **Full** means the notification message will include complete details about the build event, such as the project name, build ID, build status, and the failed phase. The **Basic** option sends a shorter summary with fewer details.

**Step 5 — Select the trigger events.**
In the **Events that trigger notifications** section, select the following events:

Under **Build state:**
- ☑ `Failed`

Under **Build phase:**
- ☑ `Failure`

> **Note:** Selecting events under both **Build state** and **Build phase** ensures comprehensive coverage. A **Build state** event of `Failed` is triggered when the overall build job fails. A **Build phase** event of `Failure` is triggered when a specific phase within the build — such as the `install` or `build` phase — fails. Together, they ensure no failure goes unnoticed. Expect a single failure to produce **two** emails: one for the phase failure and one for the build failure.

Leave all other events unchecked for this lab.

---

### Part C: Configure the Notification Target

**Step 6 — Add the SNS topic as the target.**
In the **Targets** section, under **Configured targets**, fill in the following:

- **Target type:** `SNS topic`
- **Target:** Select `fail-build-topic` from the dropdown list

**Step 7 — Submit the notification rule.**
Click the **Submit** button at the bottom of the page to create the notification rule.

---

### Part D: Verify the Notification Rule

**Step 8 — Confirm the rule was created.**
After submitting, you will be redirected to the notification rule detail page. Confirm the following:

- The rule name shows **fail-build-notification**
- The detail type shows **Full**
- The selected events include **Failed** (Build state) and **Failure** (Build phase)
- The **Notification status** shows **Sending notifications**

> If the notification status shows anything other than **Sending notifications**, review the rule configuration and ensure the correct events were selected.

**Step 9 — Verify the notification target status.**
Scroll to the bottom of the notification rule detail page and locate the **Notification targets** section. Confirm that the target status for `fail-build-topic` is set to **Active**.

> **Troubleshooting:** If the target status shows **Unreachable**, this indicates a problem with the SNS access policy configured in Lab 5. Return to the `fail-build-topic` SNS topic and verify that the access policy contains the `AWSCodeStarNotifications_publish` statement with `codestar-notifications.amazonaws.com` as the principal. Pay close attention to the `Resource` ARN, `Account ID`, and `Region` values in the policy.

## Deliverables
Submit a screenshot showing:
- Step 5 — the Events section with **Failed** and **Failure** checkboxes selected
- Step 6 — the Targets section showing `fail-build-topic` selected as the SNS topic target
- Step 8 — the notification rule detail page showing the status as **Sending notifications**
- Step 9 — the Notification targets section showing the target status as **Active**

## Review Questions
1. What is the difference between a **Build state** event and a **Build phase** event in CodeBuild notifications?
2. Why is the **Full** detail type preferable over **Basic** for a build failure notification sent to developers?
3. What does a target status of **Unreachable** indicate, and what is the most likely cause?
4. What AWS service acts as the bridge between CodeBuild events and the SNS topic in this notification setup, and where does it appear in the SNS access policy?
5. In a production environment, how might you extend this notification setup to alert different teams for different types of events?

---

# Lab 7: Creating a CI Pipeline with AWS CodePipeline

## Objective
In this lab, you will create a continuous integration pipeline using AWS CodePipeline that automatically detects code changes in your CodeCommit repository and triggers a CodeBuild job to compile the application. By the end of this lab, you will have a fully functional CI pipeline that runs end to end from source code to a successful build.

## Prerequisites
- All previous labs are complete
- `DemoRepo` exists in CodeCommit and contains both `demo.cpp` and `buildspec.yml`
- The `DemoBuild` CodeBuild project exists and is connected to `DemoRepo`
- The `fail-build-notification` notification rule is active
- You have sufficient IAM permissions to create CodePipeline pipelines and service roles

## Background: What is AWS CodePipeline?
AWS CodePipeline is a fully managed continuous integration and continuous delivery (CI/CD) service that automates the steps required to release software. A pipeline is made up of stages — in this lab you will configure a **Source** stage that watches for code changes in CodeCommit and a **Build** stage that triggers CodeBuild to compile the application. Whenever new code is pushed to the repository, the pipeline detects the change and automatically kicks off the entire process without any manual intervention.

## Instructions

### Part A: Create the Pipeline

**Step 1 — Open the CodePipeline service.**
In the AWS Management Console, click the search bar at the top of the page, type `CodePipeline`, and select **CodePipeline** from the results.

**Step 2 — Begin creating a pipeline.**
On the CodePipeline home page, click the **Create pipeline** button.

**Step 2a — Choose a creation option.**
If the console asks how you want to create the pipeline, choose **Build custom pipeline** and click **Next**. (Do not choose a template.)

---

### Part B: Configure Pipeline Settings

**Step 3 — Enter the pipeline name and service role.**
In the **Pipeline settings** section, fill in the following:

- **Pipeline name:** `MyFirstPipeline`
- **Execution mode:** `Queued` (default; leave as-is if shown)
- **Service role:** `New service role`

> **Note:** Selecting **New service role** allows AWS to automatically create an IAM role that grants CodePipeline the permissions it needs to interact with CodeCommit, CodeBuild, and other AWS services used in the pipeline.

**Step 4 — Leave advanced settings at their defaults.**
Scroll down to the **Advanced settings** section and leave all options at their default values. Click **Next** to proceed.

---

### Part C: Add the Source Stage

**Step 5 — Configure the source provider.**
In the **Add source stage** step, fill in the following details:

- **Source provider:** `AWS CodeCommit`
- **Repository name:** `DemoRepo`
- **Branch name:** `main`

After selecting the repository and branch, a message will appear confirming that an Amazon EventBridge rule will be created for this pipeline. This rule enables CodePipeline to automatically detect changes when new code is pushed to the `main` branch.

Under **Change detection options**, leave the default setting as **Amazon EventBridge (recommended)**. Leave **Output artifact format** at its default (**CodePipeline default**).

> **Note:** Amazon EventBridge (formerly called Amazon CloudWatch Events) is the preferred detection method because it triggers the pipeline immediately when a change is detected, rather than polling the repository on a schedule. This means your pipeline will start within seconds of a code push.

Click **Next** to proceed.

---

### Part D: Add the Build Stage

**Step 6 — Configure the build provider.**
In the **Add build stage** step, fill in the following details:

- **Build provider:** `AWS CodeBuild` (in newer consoles, choose **Other build providers**, then select **AWS CodeBuild** from the dropdown)
- **Region:** Ensure the correct AWS region is selected — it must match the region where `DemoBuild` was created
- **Project name:** `DemoBuild`

Click **Next** to proceed.

> **Note:** If the console shows an **Add test stage** step, click **Skip test stage** and confirm.

---

### Part E: Skip the Deploy Stage

**Step 7 — Skip the deploy stage.**
In the **Add deploy stage** step, click **Skip deploy stage**.

A confirmation popup will appear asking you to confirm that you want to skip this stage. Click **Skip** to confirm.

> **Note:** For this lab, the pipeline ends at the build stage. In a full CI/CD workflow, a deploy stage would be added here to automatically push the compiled application to EC2 instances or another deployment target.

---

### Part F: Review and Create

**Step 8 — Review the pipeline configuration.**
On the **Review** page, check all of the pipeline settings before creating it:

| Setting | Value |
|---|---|
| Pipeline name | MyFirstPipeline |
| Service role | New service role |
| Source provider | AWS CodeCommit |
| Repository | DemoRepo |
| Branch | main |
| Build provider | AWS CodeBuild |
| Build project | DemoBuild |
| Deploy stage | Skipped |

If everything looks correct, click **Create pipeline**.

---

### Part G: Verify the Pipeline

**Step 9 — Monitor the pipeline execution.**
After the pipeline is created, you will see a success message and the pipeline will start running automatically. Watch the progress of each stage on the pipeline console page:

- The **Source** stage will turn green and show **Succeeded** once CodePipeline has successfully pulled the latest code from `DemoRepo`
- The **Build** stage will turn green and show **Succeeded** once CodeBuild has successfully compiled the application using `buildspec.yml`

**Step 10 — Confirm both stages succeeded.**
Once the pipeline completes, confirm that both stages show a **Succeeded** status. This confirms the full CI pipeline is working end to end.

> **Troubleshooting:** If the Build stage fails on the first run, click **View details** to read the error. If it says `The provided role cannot be assumed: Access denied when attempting to assume the role`, click the **Release change** button on the pipeline page to retry the execution. This is a known timing issue that occurs occasionally when a new service role has just been created and IAM permissions have not fully propagated yet.

## Deliverables
Submit a screenshot showing:
- Step 5 — the Source stage configuration showing CodeCommit, DemoRepo, and the main branch selected
- Step 6 — the Build stage configuration showing CodeBuild and DemoBuild selected
- Step 8 — the Review page showing the complete pipeline configuration before creation
- Step 10 — the pipeline console showing **Succeeded** status for both the Source and Build stages

## Review Questions
1. What is the purpose of the EventBridge rule that is created when configuring the Source stage, and how does it differ from polling?
2. Why is a new IAM service role created for CodePipeline, and what AWS services does it typically need permission to access?
3. What would happen to the pipeline if a developer pushed code with a syntax error in `demo.cpp`? Which stage would fail, and what notification would be triggered?
4. What is the difference between a CI pipeline and a CD pipeline, and what would need to be added to `MyFirstPipeline` to make it a full CI/CD pipeline?
5. In a team environment with multiple developers, what are the benefits of having an automated pipeline compared to each developer manually building and deploying the application?

---

# Lab 8: Testing Build Failure Notifications

## Objective
In this lab, you will intentionally introduce a syntax error into the `demo.cpp` source file to trigger a build failure in the CI pipeline. You will then monitor the pipeline to confirm the failure is detected and verify that the SNS email notification is delivered to your inbox. By the end of this lab, you will have validated the complete end-to-end notification workflow.

## Prerequisites
- All previous labs are complete
- `MyFirstPipeline` ran successfully and both the Source and Build stages show **Succeeded**
- Your email subscription to `fail-build-topic` is confirmed
- The `fail-build-notification` rule is active and the notification target status is **Active**

## Background
In a real development environment, build failures can occur when a developer accidentally commits code with syntax errors, missing dependencies, or incorrect configurations. The notification pipeline you configured in the previous labs ensures that the team is alerted immediately when this happens, so issues can be investigated and resolved quickly without manual monitoring of the pipeline.

## Instructions

### Part A: Introduce a Syntax Error into demo.cpp

**Step 1 — Open the CodeCommit repository.**
In the AWS Management Console, search for `CodeCommit` and open the service. On the Repositories page, click on **DemoRepo** to open it.

**Step 2 — Open the demo.cpp file.**
On the repository home page, click on `demo.cpp` to view its contents.

**Step 3 — Edit the file.**
Click the **Edit** button in the top-right area of the file view to open the file in the CodeCommit editor.

**Step 4 — Introduce the syntax error.**
Locate the last line of the file, which contains a closing curly brace:
```cpp
}
```
Delete this line. Removing the closing brace introduces a syntax error that will cause the C++ compiler to fail during the build phase.

> **Note:** In C++, every opening curly brace `{` must have a matching closing brace `}`. Removing the final `}` leaves the main function body unclosed, which the compiler will reject with an error similar to `error: expected '}'  at end of input`.

**Step 5 — Fill in the commit details.**
Scroll to the bottom of the editor page and complete the following fields:

- **Author name:** Enter your full name
- **Email address:** Enter your email address
- **Commit message:**
```
Test: remove closing brace to trigger build failure notification
```

**Step 6 — Commit the change.**
Click **Commit changes**. You will be redirected to the repository home page confirming the commit was successful.

---

### Part B: Monitor the Pipeline for a Build Failure

**Step 7 — Open the CodePipeline console.**
In the AWS Management Console, search for `CodePipeline` and open the service. Click on **MyFirstPipeline** to open the pipeline.

**Step 8 — Watch the pipeline execute.**
The pipeline will automatically detect the new commit on the `main` branch via the EventBridge rule and usually begins executing within a minute. Watch the progress of each stage:

- The **Source** stage should turn green and show **Succeeded**, confirming CodePipeline successfully pulled the updated code from `DemoRepo`
- The **Build** stage should turn red and show **Failed**, confirming CodeBuild encountered the syntax error during compilation

**Step 9 — Review the build failure details.**
Click on the **Details** link within the failed Build stage to open the CodeBuild build log. Review the error output to confirm the failure was caused by the missing closing brace. You should see a compiler error similar to the following:
```
demo.cpp: In function 'int main()':
demo.cpp:12:14: error: expected '}' at end of input
demo.cpp:4:12: note: to match this '{'
```
The log will then show `COMMAND_EXECUTION_ERROR` for the BUILD phase.

---

### Part C: Verify the Email Notification

**Step 10 — Check your email inbox.**
Open the email inbox you subscribed with during Lab 5. Look for a new notification email from **AWS Notifications**. The email should arrive within a few minutes of the build failure.

**Step 11 — Review the notification contents.**
You should receive **two** emails: one from the **Build phase: Failure** event and one from the **Build state: Failed** event. SNS email delivers the notification as a JSON message, so it is not formatted like a normal email. Look through the JSON and find the following details:

- The name of the CodeBuild project (`"project-name": "DemoBuild"`)
- The build status (`"build-status": "FAILED"`)
- The build phase that failed (`"completed-phase": "BUILD"`, in the phase-failure email)
- The build ID, which you can use to find the build logs in the CodeBuild console
- The date and time of the failure (`"time"`)

> **Tip:** If no email arrives within about 10 minutes, check your spam folder, then confirm the notification target in Lab 6, Step 9 still shows **Active**.

---

### Part D: Restore the Original Code

**Step 12 — Fix the syntax error.**
Now that the failure and notification have been verified, restore the `demo.cpp` file to its original working state. Return to the `DemoRepo` repository in CodeCommit, open `demo.cpp`, click **Edit**, and add the missing closing brace `}` back at the end of the file.

**Step 13 — Commit the fix.**
Scroll to the bottom of the editor and complete the commit details:

- **Commit message:**
```
Fix: restore closing brace to resolve build failure
```
Click **Commit changes**.

**Step 14 — Verify the pipeline recovers.**
Return to the CodePipeline console and confirm that `MyFirstPipeline` runs again automatically and that both the Source and Build stages return to a **Succeeded** status.

## Deliverables
Submit a screenshot showing:
- Step 4 — the edited `demo.cpp` file in the CodeCommit editor with the closing brace removed
- Step 5 — the commit form showing the author details and commit message filled in
- Step 8 — the CodePipeline console showing the Source stage as **Succeeded** and the Build stage as **Failed**
- Step 9 — the CodeBuild log output showing the compiler error message
- Step 11 — the SNS notification email in your inbox showing the build failure details
- Step 14 — the pipeline console showing both stages returning to **Succeeded** after the fix was committed

## Review Questions
1. Why did the Source stage succeed while the Build stage failed, even though the error was introduced in the source code?
2. How quickly did the pipeline detect the new commit and begin executing? What AWS service is responsible for this detection?
3. What information in the SNS notification email would be most useful to a developer trying to diagnose and fix the build failure quickly?
4. In a team environment, why is it important to restore a broken build as quickly as possible rather than leaving it in a failed state?
5. Beyond syntax errors, what are three other common reasons a CodeBuild build might fail in a real project?

---

# Lab 9: Cleaning Up AWS Resources

## Objective
In this lab, you will delete all AWS resources created during the CI pipeline proof of concept. Cleaning up resources is an essential practice to avoid unnecessary charges and keep your AWS account organized. You will delete the notification rule, CodeCommit repository, CodePipeline pipeline and its artifact bucket, CodeBuild project and its logs, SNS topic, and the IAM roles and policies that were automatically created during the setup.

## Prerequisites
- All previous labs are complete
- You have sufficient IAM permissions to delete the resources listed below

## Important Warning
> **Caution:** All deletions in this lab are permanent and cannot be undone. Double-check each resource name before confirming any deletion. Any code committed to CodeCommit that has not been backed up elsewhere will be permanently lost.

## Resources to Delete

| Resource | Name |
|---|---|
| Notification Rule | fail-build-notification |
| CodeCommit Repository | DemoRepo |
| CodePipeline Pipeline | MyFirstPipeline |
| EventBridge Rule | codepipeline-DemoRe-main-*-rule (created by the pipeline) |
| S3 Artifact Bucket | codepipeline-<region>-* (created by the pipeline) |
| CodeBuild Project | DemoBuild |
| CloudWatch Log Group | /aws/codebuild/DemoBuild |
| SNS Topic | fail-build-topic |
| IAM Roles | AWSCodePipelineServiceRole-<region>-MyFirstPipeline, codebuild-DemoBuild-service-role, and (if present) cwe-role-<region>-MyFirstPipeline |
| IAM Policies | AWSCodePipelineServiceRole-<region>-MyFirstPipeline, CodeBuildBasePolicy-DemoBuild-<region>, and (if present) start-pipeline-execution-<region>-MyFirstPipeline |

## Instructions

### Step 0 — Delete the Notification Rule

**0.1 —** Open the CodeBuild console, open the **DemoBuild** project, and choose **Notify → Manage notification rules** (or open **Settings → Notification rules** in the left navigation).

**0.2 —** Select **fail-build-notification**, click **Delete**, type `delete` if prompted, and confirm.

> **Note:** Delete the rule first. A rule left behind after its project and topic are deleted keeps appearing in the notification rules list.

---

### Step 1 — Delete the CodeCommit Repository

**1.1 — Open the CodeCommit console.**
In the AWS Management Console, search for `CodeCommit` and open the service.

**1.2 — Select the repository.**
On the Repositories page, click on **DemoRepo** to open it.

**1.3 — Delete the repository.**
Click **Settings** in the left navigation panel, then click **Delete repository**. A confirmation dialog will appear. Type `delete` in the confirmation field and click **Delete**.

> **Note:** Deleting the repository permanently removes all committed files, commit history, and branches. This action cannot be undone.

---

### Step 2 — Delete the CodePipeline Pipeline

**2.1 — Open the CodePipeline console.**
In the AWS Management Console, search for `CodePipeline` and open the service.

**2.2 — Select the pipeline.**
In the left navigation panel, click **Pipelines**. Select the checkbox next to **MyFirstPipeline**.

**2.3 — Delete the pipeline.**
Click the **Delete pipeline** button. A confirmation dialog will appear. Type `delete` in the confirmation field and click **Delete**.

> **Note:** Deleting the pipeline does not delete the associated source repository, build project, artifact bucket, or (in some cases) the EventBridge rule. Those must be deleted separately in the steps below.

**2.4 — Delete the EventBridge rule (if it remains).**
Open the **Amazon EventBridge** console and click **Rules**. If a rule whose name starts with `codepipeline-DemoRe-main` is listed, select it, click **Delete**, type `delete`, and confirm.

**2.5 — Delete the pipeline artifact bucket.**
Open the **S3** console. Find the bucket whose name starts with `codepipeline-<region>-` (for example, `codepipeline-us-east-1-123456789012`). Select it and click **Empty**, type `permanently delete`, and confirm. Then select it again, click **Delete**, type the bucket name, and confirm.

> **Caution:** If you have other pipelines in this region, they may share this bucket. Only delete it if `MyFirstPipeline` was your only pipeline.

---

### Step 3 — Delete the CodeBuild Project

**3.1 — Open the CodeBuild console.**
In the AWS Management Console, search for `CodeBuild` and open the service.

**3.2 — Select the build project.**
In the left navigation panel, expand **Build** and click **Build projects**. Select the checkbox next to **DemoBuild**.

**3.3 — Delete the project.**
Click **Actions → Delete** (labeled **Delete build project** in some console versions). Type `delete` in the confirmation field and click **Delete**.

**3.4 — Delete the build log group.**
Open the **CloudWatch** console, click **Logs → Log groups**, select `/aws/codebuild/DemoBuild`, and choose **Actions → Delete log group(s)**. Confirm the deletion.

---

### Step 4 — Delete the SNS Topic

**4.1 — Open the SNS console.**
In the AWS Management Console, search for `SNS` and open the Simple Notification Service.

**4.2 — Navigate to Topics.**
In the left navigation panel, click **Topics**.

**4.3 — Select the topic.**
On the Topics page, select the checkbox next to **fail-build-topic**.

**4.4 — Delete the topic.**
Click the **Delete** button. A confirmation dialog will appear. Type `delete me` in the confirmation field and click **Delete**.

> **Note:** Deleting the SNS topic will also remove all associated subscriptions, including the email subscription confirmed in Lab 5.

---

### Step 5 — Delete the IAM Roles

**5.1 — Open the IAM console.**
In the AWS Management Console, search for `IAM` and open the service.

**5.2 — Navigate to Roles.**
In the left navigation panel, click **Roles**.

**5.3 — Find the CodePipeline service role.**
In the search bar, type `AWSCodePipelineServiceRole` to filter the list. Locate the role whose name begins with:
```
AWSCodePipelineServiceRole-
```

**5.4 — Delete the role.**
Select the checkbox next to the role name and click **Delete**. A confirmation dialog will appear. Confirm the role name in the field provided and click **Delete**.

**5.5 — Delete the CodeBuild service role.**
Clear the search bar, type `codebuild-DemoBuild`, and delete the role `codebuild-DemoBuild-service-role` the same way.

**5.6 — Delete the EventBridge role (if present).**
Clear the search bar, type `cwe-role`, and delete the role ending in `MyFirstPipeline`, if one exists.

---

### Step 6 — Delete the IAM Policies

**6.1 — Navigate to Policies.**
In the IAM console, click **Policies** in the left navigation panel.

**6.2 — Find the CodePipeline policy.**
In the search bar, type `AWSCodePipelineServiceRole` to filter the list. Locate the policy whose name matches the following pattern, replacing the region and pipeline name with your own values:
```
AWSCodePipelineServiceRole-<region>-MyFirstPipeline
```
For example:
```
AWSCodePipelineServiceRole-us-east-1-MyFirstPipeline
```

> **Note:** If you created the pipeline in a different region, substitute the correct region code in the policy name.

**6.3 — Delete the policy.**
Select the policy, click **Delete**, type the policy name in the confirmation field, and click **Delete**.

**6.4 — Delete the remaining policies.**
Repeat the search and deletion for each policy below:
- `CodeBuildBasePolicy-DemoBuild-<region>`
- `start-pipeline-execution-<region>-MyFirstPipeline`, if present

> **Note:** Because you deleted the roles in Step 5, these policies are no longer attached to anything and can be deleted directly.

---

### Step 7 — Verify All Resources Have Been Deleted

After completing all deletions, perform a final check to confirm that no resources remain:

| Service | Verification |
|---|---|
| CodeCommit | DemoRepo no longer appears on the Repositories page |
| CodePipeline | MyFirstPipeline no longer appears on the Pipelines page |
| CodeBuild | DemoBuild no longer appears on the Build projects page |
| SNS | fail-build-topic no longer appears on the Topics page |
| CodeBuild notification rules | fail-build-notification no longer appears |
| EventBridge | No rule starting with `codepipeline-DemoRe-main` remains |
| S3 | No `codepipeline-<region>-*` artifact bucket remains (unless used by another pipeline) |
| CloudWatch Logs | `/aws/codebuild/DemoBuild` no longer appears |
| IAM Roles | No `AWSCodePipelineServiceRole-*`, `codebuild-DemoBuild-service-role`, or `cwe-role-*-MyFirstPipeline` role remains |
| IAM Policies | No `AWSCodePipelineServiceRole-*-MyFirstPipeline`, `CodeBuildBasePolicy-DemoBuild-*`, or `start-pipeline-execution-*-MyFirstPipeline` policy remains |

## Deliverables
Submit a screenshot showing:
- Step 1.3 — the CodeCommit deletion confirmation dialog with `delete` typed in
- Step 2.3 — the CodePipeline deletion confirmation dialog with `delete` typed in
- Step 0.2 — the notification rule deletion confirmation
- Step 2.5 — the S3 artifact bucket deletion confirmation
- Step 3.3 — the CodeBuild project deletion confirmation
- Step 4.4 — the SNS topic deletion confirmation dialog with `delete me` typed in
- Step 7 — each service console confirming the deleted resources no longer appear

## Review Questions
1. Why is it important to clean up AWS resources after completing a lab or proof of concept?
2. Why must the IAM role and policy be deleted separately from the pipeline, even though they were created automatically during pipeline setup?
3. What could happen if the IAM role created for CodePipeline were left in place after all other resources were deleted?
4. In a production environment, what steps would you take before deleting a CodeCommit repository to ensure no important code is lost?
5. What AWS service or feature could you use to set up automated resource cleanup or cost alerts to prevent unused resources from accumulating charges?
