# Terraform Labs: Provisioning AWS Resources

Oct 22, 2026 · @Kyle Anderson

This workbook guides you through provisioning AWS infrastructure as code with Terraform, starting from a fresh Windows workstation. Complete the labs in order — each one builds on the previous.

> **Lab assets:** All Terraform files used in these labs are reproduced in full below. Create each lab in its own folder under `C:\tf-labs` (for example, `C:\tf-labs\lab2`).

> **Region:** Create every resource in the **same AWS region** (for example, `us-east-1`). Terraform only manages resources in the region set in the provider block.

> **Cost:** Every lab ends with `terraform destroy`. A forgotten EC2 instance keeps billing until it is destroyed.

## Table of Contents

1. Installing Terraform and the AWS CLI on Windows
2. Your First Terraform Project: Creating an S3 Bucket
3. Using Variables and Outputs to Build a Secure S3 Bucket
4. Provisioning a VPC, Security Group, and EC2 Instance
5. Creating a Least-Privilege IAM Role and Remote State
6. Scanning Terraform Code for Security Misconfigurations
7. Cleaning Up AWS Resources

---

## Lab 1: Installing Terraform and the AWS CLI on Windows

### Objective

In this lab, you will install Terraform and the AWS Command Line Interface (CLI) on a Windows workstation and connect them to your AWS account. By the end of this lab, the `terraform` and `aws` commands will work in PowerShell and Terraform will be able to authenticate to AWS.

### Prerequisites

- Windows 10 (version 1809 or later) or Windows 11, with local administrator rights
- An active AWS account with access to the AWS Management Console
- An IAM user with permissions to manage S3, EC2, VPC, and IAM resources. Do not use the AWS account root user
- Visual Studio Code (recommended) or another code editor

### Background: What is Terraform?

Terraform is an Infrastructure as Code (IaC) tool from HashiCorp. You describe the infrastructure you want in `.tf` configuration files, and Terraform creates, changes, or deletes cloud resources to match. Terraform is distributed as a single executable file, `terraform.exe`. Installing it on Windows means placing that file in a folder listed in your `PATH` environment variable.

### Instructions

#### Part A: Install Terraform

Choose **one** of the three methods below. Method 1 is recommended.

**Step 1 — Open PowerShell.** Click **Start**, type `PowerShell` (or `Terminal`), and open **Windows PowerShell** or **Windows Terminal**.

**Step 2 — Install Terraform.**

*Method 1 — winget (recommended).* winget is included with Windows 11 and current Windows 10 builds. It adds Terraform to your `PATH` automatically. Confirm winget is available, then install Terraform:

```powershell
winget --version
winget install --id Hashicorp.Terraform --exact
```

*Method 2 — Chocolatey.* Use this if your workstation already uses Chocolatey. Run PowerShell **as Administrator**:

```powershell
choco install terraform -y
```

*Method 3 — Manual install.* Go to [developer.hashicorp.com/terraform/install](https://developer.hashicorp.com/terraform/install) and download the **Windows AMD64** zip file. Create the folder `C:\Tools\Terraform`, extract `terraform.exe` into it, and add the folder to your user `PATH`:

```powershell
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
[Environment]::SetEnvironmentVariable("Path", "$userPath;C:\Tools\Terraform", "User")
```

> **Note:** As a GUI alternative, click **Start**, search for **Edit environment variables for your account**, select **Path**, click **Edit**, click **New**, and enter `C:\Tools\Terraform`.

**Step 3 — Restart the terminal.** Close and reopen PowerShell so the updated `PATH` is loaded.

**Step 4 — Verify the installation.** Run the following commands:

```powershell
terraform -version
terraform -help
```

Confirm the output shows a version similar to `Terraform v1.x.x on windows_amd64`.

> **Tip:** If `terraform -version` reports that your version is out of date, run `winget upgrade --id Hashicorp.Terraform --exact` (or rerun your install method from Step 2).

---

#### Part B: Install the AWS CLI

**Step 5 — Install AWS CLI version 2.** In PowerShell, run:

```powershell
winget install --id Amazon.AWSCLI --exact
```

Alternatively, download and run the MSI installer from [aws.amazon.com/cli](https://aws.amazon.com/cli/).

**Step 6 — Verify the installation.** Close and reopen PowerShell, then run:

```powershell
aws --version
```

---

#### Part C: Configure AWS Credentials

**Step 7 — Create an access key.** In the AWS Management Console, open **IAM**, click **Users**, and select your lab user. On the **Security credentials** tab, click **Create access key**, choose **Command Line Interface (CLI)**, and complete the wizard. Copy the **Access key ID** and **Secret access key**.

> **Caution:** The secret access key is shown only once. Never paste it into a Terraform file, commit it to Git, or share it in chat.

**Step 8 — Configure the AWS CLI.** Run `aws configure` and enter the following values:

- **AWS Access Key ID:** your access key ID
- **AWS Secret Access Key:** your secret access key
- **Default region name:** `us-east-1`
- **Default output format:** `json`

```powershell
aws configure
```

Credentials are saved in `%USERPROFILE%\.aws\credentials`. Terraform reads them from this file automatically.

> **Note:** If your organization uses AWS IAM Identity Center, run `aws configure sso` instead. Short-lived credentials are the more secure option.

**Step 9 — Verify your identity.** Run the following command and confirm it returns your account ID and IAM user ARN:

```powershell
aws sts get-caller-identity
```

---

#### Part D: Install the VS Code Extension (Optional)

**Step 10 — Install the HashiCorp Terraform extension.** In VS Code, open the **Extensions** view, search for `HashiCorp Terraform`, and click **Install**. The extension provides syntax highlighting, formatting, and autocomplete for `.tf` files.

### Deliverables

Submit a screenshot showing:

- Step 2 — the PowerShell window showing the Terraform installation completing
- Step 4 — the output of `terraform -version`
- Step 6 — the output of `aws --version`
- Step 9 — the output of `aws sts get-caller-identity` (you may blur the account ID)

### Review Questions

1. What is Infrastructure as Code, and what advantages does it offer over creating resources manually in the AWS Console?
2. Why does Terraform need to be in the `PATH` environment variable, and what error appears if it is not?
3. What are the advantages of installing Terraform with winget compared to the manual zip method?
4. Why should you never use the AWS account root user for day-to-day work or Terraform automation?
5. Why are short-lived credentials from IAM Identity Center more secure than long-lived access keys?

---

## Lab 2: Your First Terraform Project: Creating an S3 Bucket

### Objective

In this lab, you will write your first Terraform configuration and use it to create an Amazon S3 bucket. You will run the complete Terraform workflow — `init`, `plan`, `apply`, and `destroy` — and see how Terraform uses a state file to track the resources it manages.

### Prerequisites

- Lab 1 is complete, and `terraform -version` and `aws sts get-caller-identity` both succeed
- Your IAM user has permissions to create and delete S3 buckets

### Background: The Terraform Workflow

Every Terraform project follows the same core workflow:

| Command | Purpose |
| --- | --- |
| `terraform init` | Downloads the providers (plugins) the configuration needs |
| `terraform plan` | Compares the code with what exists and shows the changes it would make |
| `terraform apply` | Makes the changes shown in the plan |
| `terraform destroy` | Deletes every resource managed by the configuration |

Terraform records what it created in a **state file** named `terraform.tfstate`. It compares that file with your code and with the real resources in AWS to decide what to change.

### Instructions

#### Part A: Create the Configuration

**Step 1 — Create a project folder.** In PowerShell, run:

```powershell
mkdir C:\tf-labs\lab2
cd C:\tf-labs\lab2
code .
```

**Step 2 — Create the main.tf file.** In VS Code, create a file named `main.tf` with the following content. Replace `<yourinitials>` with your initials in lowercase.

```hcl
terraform {
  required_version = ">= 1.10"

  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 6.0"
    }
  }
}

provider "aws" {
  region = "us-east-1"
}

resource "aws_s3_bucket" "lab2" {
  bucket = "devsecops-lab2-<yourinitials>-2026"

  tags = {
    Course = "DevSecOps"
    Lab    = "2"
  }
}
```

Here is what each block does:

- `terraform` block — sets the minimum Terraform version and the AWS provider version to download
- `provider "aws"` block — tells the AWS provider which region to create resources in
- `resource "aws_s3_bucket" "lab2"` block — declares one S3 bucket. `aws_s3_bucket` is the resource type and `lab2` is the local name Terraform uses to refer to it

> **Note:** S3 bucket names must be globally unique across all AWS accounts. If the name is already taken, `apply` fails with `BucketAlreadyExists`. Add more digits to make it unique.

---

#### Part B: Run the Terraform Workflow

**Step 3 — Initialize the project.** Run `terraform init`. Confirm the output ends with `Terraform has been successfully initialized!`.

**Step 4 — Format and validate the code.** Run the following commands. `fmt` rewrites the code in the standard style, and `validate` checks syntax and references.

```powershell
terraform fmt
terraform validate
```

**Step 5 — Review the plan.** Run `terraform plan`. Confirm the output shows `Plan: 1 to add, 0 to change, 0 to destroy.` Nothing has been created yet.

**Step 6 — Apply the configuration.** Run `terraform apply`. Review the plan, type `yes` when prompted, and press **Enter**. Confirm the output shows `Apply complete! Resources: 1 added`.

---

#### Part C: Inspect the Results

**Step 7 — Inspect the Terraform state.** Run the following commands:

```powershell
terraform state list
terraform state show aws_s3_bucket.lab2
aws s3 ls
```

**Step 8 — Verify in the AWS Console.** In the AWS Management Console, open **S3** and click your bucket. On the **Properties** tab, confirm the **Course** and **Lab** tags are listed.

**Step 9 — Observe configuration drift.** On the bucket's **Properties** tab, add a new tag `Owner = manual` and save it. Return to PowerShell and run `terraform plan` again. Confirm that Terraform plans to remove the tag.

> **Note:** Terraform treats the code as the source of truth. Any change made manually in the Console is called **drift**, and Terraform reverts it on the next `apply`.

---

#### Part D: Destroy the Resources

**Step 10 — Destroy the bucket.** Run `terraform destroy`, type `yes` when prompted, and confirm the output shows `Destroy complete! Resources: 1 destroyed`.

### Deliverables

Submit a screenshot showing:

- Step 3 — the output of `terraform init`
- Step 5 — the plan output showing `1 to add`
- Step 6 — the apply output showing `Apply complete!`
- Step 8 — the S3 console showing the bucket and its tags
- Step 9 — the plan output showing Terraform removing the manually added tag
- Step 10 — the destroy output showing `Destroy complete!`

### Review Questions

1. What files and folders did `terraform init` create, and what is the purpose of `.terraform.lock.hcl`?
2. What do the `+`, `~`, and `-` symbols mean in the output of `terraform plan`?
3. What is the Terraform state file, and why should it never be committed to a Git repository?
4. What is configuration drift, and why is it a security concern in a production environment?
5. Why is it important to run `terraform plan` and review its output before running `terraform apply`?

---

## Lab 3: Using Variables and Outputs to Build a Secure S3 Bucket

### Objective

In this lab, you will organize a Terraform project into multiple files, parameterize it with input variables, and return values with outputs. You will then apply four security controls that every production S3 bucket should have: blocked public access, versioning, encryption at rest, and encryption in transit.

### Prerequisites

- Labs 1 and 2 are complete
- Your IAM user has permissions to create S3 buckets and bucket policies

### Background: Variables, Outputs, and File Layout

Terraform loads every `.tf` file in a folder as one configuration, so you can split code into files by purpose:

| File | Purpose |
| --- | --- |
| `providers.tf` | Terraform settings, provider versions, and default tags |
| `variables.tf` | Input variables, so the same code can be reused with different values |
| `main.tf` | The resources to create |
| `outputs.tf` | Values Terraform prints after `apply`, such as names and ARNs |
| `terraform.tfvars` | The values assigned to the variables for this run |

### Instructions

#### Part A: Create the Configuration Files

**Step 1 — Create a project folder.** Run `mkdir C:\tf-labs\lab3`, then `cd C:\tf-labs\lab3`, and open the folder in VS Code.

**Step 2 — Create providers.tf.** Create `providers.tf` with the following content. The `default_tags` block applies the same tags to every resource automatically.

```hcl
terraform {
  required_version = ">= 1.10"
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 6.0"
    }
  }
}

provider "aws" {
  region = var.region

  default_tags {
    tags = {
      Course      = "DevSecOps"
      Environment = var.environment
      ManagedBy   = "Terraform"
    }
  }
}
```

**Step 3 — Create variables.tf.** Create `variables.tf` with the following content. The `validation` block rejects any value other than `dev`, `test`, or `prod`.

```hcl
variable "region" {
  type    = string
  default = "us-east-1"
}

variable "environment" {
  type    = string
  default = "dev"

  validation {
    condition     = contains(["dev", "test", "prod"], var.environment)
    error_message = "environment must be dev, test or prod."
  }
}

variable "student_id" {
  description = "Lowercase initials plus digits, used in bucket names"
  type        = string
}
```

**Step 4 — Create main.tf.** Create `main.tf` with the following content:

```hcl
resource "aws_s3_bucket" "secure" {
  bucket        = "devsecops-${var.student_id}-${var.environment}"
  force_destroy = true # lab only: lets destroy remove a non-empty bucket
}

# Control 1: Block all public access
resource "aws_s3_bucket_public_access_block" "secure" {
  bucket                  = aws_s3_bucket.secure.id
  block_public_acls       = true
  block_public_policy     = true
  ignore_public_acls      = true
  restrict_public_buckets = true
}

# Control 2: Versioning protects against accidental deletes
resource "aws_s3_bucket_versioning" "secure" {
  bucket = aws_s3_bucket.secure.id
  versioning_configuration {
    status = "Enabled"
  }
}

# Control 3: Encryption at rest
resource "aws_s3_bucket_server_side_encryption_configuration" "secure" {
  bucket = aws_s3_bucket.secure.id
  rule {
    apply_server_side_encryption_by_default {
      sse_algorithm = "AES256"
    }
  }
}

# Control 4: Deny any request that does not use TLS
data "aws_iam_policy_document" "tls_only" {
  statement {
    sid     = "DenyInsecureTransport"
    effect  = "Deny"
    actions = ["s3:*"]
    principals {
      type        = "*"
      identifiers = ["*"]
    }
    resources = [
      aws_s3_bucket.secure.arn,
      "${aws_s3_bucket.secure.arn}/*",
    ]
    condition {
      test     = "Bool"
      variable = "aws:SecureTransport"
      values   = ["false"]
    }
  }
}

resource "aws_s3_bucket_policy" "tls_only" {
  bucket     = aws_s3_bucket.secure.id
  policy     = data.aws_iam_policy_document.tls_only.json
  depends_on = [aws_s3_bucket_public_access_block.secure]
}
```

**Step 5 — Create outputs.tf.** Create `outputs.tf` with the following content:

```hcl
output "bucket_name" {
  value = aws_s3_bucket.secure.bucket
}

output "bucket_arn" {
  value = aws_s3_bucket.secure.arn
}
```

**Step 6 — Create terraform.tfvars.** Create `terraform.tfvars` and set your student ID (for example, your initials plus two digits):

```hcl
student_id = "jd42"
```

> **Important:** `.tfvars` files often hold environment-specific or sensitive values. Add `*.tfvars` to your `.gitignore` file so they are never committed.

---

#### Part B: Deploy the Bucket

**Step 7 — Initialize, plan, and apply.** Run the following commands and type `yes` when prompted:

```powershell
terraform init
terraform plan
terraform apply
```

Confirm the output shows `Resources: 5 added` and lists the `bucket_name` and `bucket_arn` outputs.

**Step 8 — Verify the security controls in the Console.** Open **S3**, click your bucket, and confirm the following:

- On the **Permissions** tab, **Block all public access** shows **On**, and the bucket policy contains `DenyInsecureTransport`
- On the **Properties** tab, **Bucket Versioning** shows **Enabled** and **Default encryption** shows **SSE-S3**
- On the **Properties** tab, the tags **Course**, **Environment**, and **ManagedBy** are listed

---

#### Part C: Test the Configuration

**Step 9 — Test variable validation.** Run `terraform plan -var="environment=qa"`. Confirm Terraform rejects the value with the message `environment must be dev, test or prod.`

**Step 10 — Test versioning.** Upload the same file twice, then list its versions:

```powershell
"hello" | Out-File test.txt
$BUCKET = terraform output -raw bucket_name
aws s3 cp test.txt "s3://$BUCKET/"
aws s3 cp test.txt "s3://$BUCKET/"
aws s3api list-object-versions --bucket $BUCKET
```

Confirm the output lists two versions of `test.txt`, each with a different `VersionId`.

**Step 11 — Destroy the resources.** Run `terraform destroy` and type `yes` when prompted.

### Deliverables

Submit a screenshot showing:

- Step 7 — the apply output showing `5 added` and both output values
- Step 8 — the bucket's Permissions tab showing public access blocked and the bucket policy
- Step 8 — the bucket's Properties tab showing versioning and default encryption enabled
- Step 9 — the validation error for `environment=qa`
- Step 10 — the `list-object-versions` output showing two versions

### Review Questions

1. What are the benefits of splitting a Terraform project into `providers.tf`, `variables.tf`, `main.tf`, and `outputs.tf`?
2. Which of the four S3 controls in this lab protect confidentiality, and which protect integrity or availability?
3. Why does the bucket policy use a `Deny` statement with the `aws:SecureTransport` condition rather than an `Allow` statement?
4. What does `default_tags` save you from doing, and why are consistent tags important for security audits and cost tracking?
5. Why is `force_destroy = true` acceptable in a lab but dangerous in production?

---

## Lab 4: Provisioning a VPC, Security Group, and EC2 Instance

### Objective

In this lab, you will use Terraform to build a small network and a hardened web server. You will create a VPC, a public subnet, an internet gateway, a route table, a security group, and an EC2 instance running Amazon Linux 2023. By the end of this lab, you will reach a web page served by the instance and understand how Terraform orders resources by their dependencies.

### Prerequisites

- Labs 1 through 3 are complete
- Your IAM user has permissions to create VPC and EC2 resources

### Background: How Terraform Orders Resources

When one resource refers to another — for example, `vpc_id = aws_vpc.lab.id` — Terraform knows it must create the VPC first. It builds a **dependency graph** from these references and creates independent resources in parallel. This lab builds the following resources:

| Resource | Value |
| --- | --- |
| VPC | `10.0.0.0/16` |
| Public subnet | `10.0.1.0/24` |
| Internet gateway and route table | Route `0.0.0.0/0` to the internet gateway |
| Security group | HTTP (port 80) from your IP address only |
| EC2 instance | `t3.micro`, Amazon Linux 2023, IMDSv2, encrypted root volume |

### Instructions

#### Part A: Create the Configuration Files

**Step 1 — Create a project folder.** Create the folder `C:\tf-labs\lab4` and open it in VS Code. Copy `providers.tf` from Lab 3 into this folder.

**Step 2 — Find your public IP address.** Run the following command and note the IP address it returns:

```powershell
(Invoke-RestMethod -Uri "https://checkip.amazonaws.com").Trim()
```

**Step 3 — Create variables.tf.**

```hcl
variable "region" {
  type    = string
  default = "us-east-1"
}

variable "environment" {
  type    = string
  default = "dev"
}

variable "my_ip" {
  description = "Your public IP in CIDR form, e.g. 203.0.113.10/32"
  type        = string
}

variable "instance_type" {
  type    = string
  default = "t3.micro"
}
```

**Step 4 — Create network.tf.**

```hcl
data "aws_availability_zones" "available" {
  state = "available"
}

resource "aws_vpc" "lab" {
  cidr_block           = "10.0.0.0/16"
  enable_dns_hostnames = true

  tags = { Name = "lab4-vpc" }
}

resource "aws_subnet" "public" {
  vpc_id                  = aws_vpc.lab.id
  cidr_block              = "10.0.1.0/24"
  availability_zone       = data.aws_availability_zones.available.names[0]
  map_public_ip_on_launch = true

  tags = { Name = "lab4-public" }
}

resource "aws_internet_gateway" "igw" {
  vpc_id = aws_vpc.lab.id
  tags   = { Name = "lab4-igw" }
}

resource "aws_route_table" "public" {
  vpc_id = aws_vpc.lab.id
  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = aws_internet_gateway.igw.id
  }
  tags = { Name = "lab4-public-rt" }
}

resource "aws_route_table_association" "public" {
  subnet_id      = aws_subnet.public.id
  route_table_id = aws_route_table.public.id
}
```

**Step 5 — Create security.tf.**

```hcl
resource "aws_security_group" "web" {
  name        = "lab4-web-sg"
  description = "HTTP from approved IP only"
  vpc_id      = aws_vpc.lab.id
}

resource "aws_vpc_security_group_ingress_rule" "http" {
  security_group_id = aws_security_group.web.id
  description       = "HTTP from my IP"
  cidr_ipv4         = var.my_ip
  from_port         = 80
  to_port           = 80
  ip_protocol       = "tcp"
}

resource "aws_vpc_security_group_egress_rule" "all" {
  security_group_id = aws_security_group.web.id
  description       = "Outbound for package updates"
  cidr_ipv4         = "0.0.0.0/0"
  ip_protocol       = "-1"
}
```

> **Note:** This security group has **no SSH rule**. Leaving port 22 open to the internet is one of the most common audit findings. If you need a shell on the instance, use EC2 Instance Connect or Session Manager instead.

**Step 6 — Create compute.tf.**

```hcl
# Latest Amazon Linux 2023 AMI, looked up at plan time
data "aws_ssm_parameter" "al2023" {
  name = "/aws/service/ami-amazon-linux-latest/al2023-ami-kernel-default-x86_64"
}

resource "aws_instance" "web" {
  ami                    = data.aws_ssm_parameter.al2023.insecure_value
  instance_type          = var.instance_type
  subnet_id              = aws_subnet.public.id
  vpc_security_group_ids = [aws_security_group.web.id]

  metadata_options {
    http_tokens = "required" # enforce IMDSv2
  }

  root_block_device {
    volume_size = 8
    encrypted   = true
  }

  user_data = <<-EOF
    #!/bin/bash
    dnf install -y httpd
    echo "<h1>Deployed by Terraform - ${var.environment}</h1>" > /var/www/html/index.html
    systemctl enable --now httpd
  EOF

  # Rebuild the instance when the startup script changes
  user_data_replace_on_change = true

  tags = { Name = "lab4-web" }
}
```

**Step 7 — Create outputs.tf.**

```hcl
output "web_url" {
  value = "http://${aws_instance.web.public_ip}"
}
```

---

#### Part B: Deploy the Infrastructure

**Step 8 — Initialize and plan.** Run the following commands, replacing `203.0.113.10` with the IP address from Step 2:

```powershell
terraform init
terraform plan -var="my_ip=203.0.113.10/32"
```

Confirm the plan shows `Plan: 9 to add`.

**Step 9 — Apply the configuration.** Run `terraform apply -var="my_ip=203.0.113.10/32"` and type `yes` when prompted.

**Step 10 — Open the web page.** Wait about two minutes for the instance to finish installing the web server. Copy the `web_url` output into your browser and confirm the page shows **Deployed by Terraform - dev**.

> **Troubleshooting:** If the page does not load, confirm you used `http://` and not `https://`, and that the IP address in `my_ip` matches your current public IP.

**Step 11 — Verify in the AWS Console.** Open **EC2**, click **Instances**, and select **lab4-web**. On the **Details** tab, confirm **IMDSv2** shows **Required**. On the **Storage** tab, confirm the root volume shows **Encrypted**.

---

#### Part C: Explore Dependencies and Changes

**Step 12 — View the dependency graph.** Run `terraform graph`, copy the output, and paste it into [dreampuf.github.io/GraphvizOnline](https://dreampuf.github.io/GraphvizOnline) to see the order Terraform creates resources in.

**Step 13 — Compare an update with a replacement.** Run `terraform plan -var="my_ip=203.0.113.10/32" -var="instance_type=t3.small"`. Note whether the plan shows `~ update in-place` or `-/+ must be replaced`. Then change the text in `user_data` and run the plan again. Compare the two results. Do not apply either change.

---

#### Part D: Destroy the Resources

**Step 14 — Destroy the infrastructure.** Run `terraform destroy -var="my_ip=203.0.113.10/32"` and type `yes` when prompted. Confirm the output shows `Destroy complete! Resources: 9 destroyed`.

> **Caution:** EC2 instances bill while they are running. Always confirm the instance shows **Terminated** in the EC2 console after destroying.

### Deliverables

Submit a screenshot showing:

- Step 8 — the plan output showing `9 to add`
- Step 10 — the web page in your browser showing **Deployed by Terraform - dev**
- Step 11 — the EC2 instance details showing IMDSv2 required and the encrypted root volume
- Step 12 — the rendered dependency graph
- Step 13 — the two plan outputs comparing an in-place update with a replacement
- Step 14 — the destroy output showing `9 destroyed`

### Review Questions

1. Why did Terraform create the VPC before the subnet without being told to, and how does it decide which resources can be created in parallel?
2. Why is the security group's HTTP rule restricted to `my_ip` instead of `0.0.0.0/0`?
3. What attack does requiring IMDSv2 help prevent?
4. Why does a data source look up the AMI ID instead of hard-coding it in the configuration?
5. Why did changing `instance_type` and changing `user_data` produce different plan results?

---

## Lab 5: Creating a Least-Privilege IAM Role and Remote State

### Objective

In this lab, you will move the Terraform state file off your workstation and into an encrypted, versioned S3 bucket with state locking. You will then create an IAM role that allows an EC2 instance to read objects from one specific bucket and nothing else. By the end of this lab, you will understand how teams share state safely and how to apply the principle of least privilege in Terraform.

### Prerequisites

- Labs 1 through 4 are complete
- Your IAM user has permissions to create S3 buckets, IAM roles, IAM policies, and instance profiles

### Background: Remote State and Least Privilege

By default, Terraform stores state in a local `terraform.tfstate` file. This causes problems for teams: each person has a different copy, two people can run `apply` at the same time, and the file may contain sensitive values in plain text. A **remote backend** stores state in a shared location, and **state locking** stops two runs from changing it at once. Since Terraform 1.10, the S3 backend supports native locking with `use_lockfile = true`, replacing the older DynamoDB lock table.

The **principle of least privilege** means giving an identity only the permissions it needs to do its job. An IAM role for an application should name specific actions and specific resources, never `"Action": "*"` or `"Resource": "*"`.

### Instructions

#### Part A: Create the State Bucket

**Step 1 — Create the state bucket with the AWS CLI.** The backend bucket must exist before Terraform can use it. In PowerShell, run the following commands, replacing `jd42` with your student ID:

```powershell
$B = "tfstate-devsecops-jd42"
aws s3api create-bucket --bucket $B --region us-east-1
aws s3api put-bucket-versioning --bucket $B --versioning-configuration Status=Enabled
aws s3api put-public-access-block --bucket $B --public-access-block-configuration "BlockPublicAcls=true,IgnorePublicAcls=true,BlockPublicPolicy=true,RestrictPublicBuckets=true"
```

**Step 2 — Verify the state bucket.** Open **S3** in the AWS Console and confirm `tfstate-devsecops-jd42` is listed with **Bucket Versioning** enabled.

---

#### Part B: Configure the Remote Backend

**Step 3 — Create backend.tf.** Create the folder `C:\tf-labs\lab5` and add `backend.tf` with the following content. Replace `jd42` with your student ID.

```hcl
terraform {
  required_version = ">= 1.10"

  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 6.0"
    }
  }

  backend "s3" {
    bucket       = "tfstate-devsecops-jd42"
    key          = "lab5/terraform.tfstate"
    region       = "us-east-1"
    encrypt      = true
    use_lockfile = true # native S3 state locking (Terraform 1.10+)
  }
}

provider "aws" {
  region = "us-east-1"
}
```

> **Note:** Backend blocks cannot use variables. The bucket name must be typed directly into the file.

---

#### Part C: Create the Least-Privilege Role

**Step 4 — Create iam.tf.** Add `iam.tf` with the following content:

```hcl
variable "student_id" {
  type = string
}

resource "aws_s3_bucket" "app_data" {
  bucket        = "devsecops-appdata-${var.student_id}"
  force_destroy = true
}

# Trust policy: who may assume the role (only the EC2 service)
data "aws_iam_policy_document" "ec2_trust" {
  statement {
    actions = ["sts:AssumeRole"]
    principals {
      type        = "Service"
      identifiers = ["ec2.amazonaws.com"]
    }
  }
}

# Permissions policy: what the role may do (read one bucket, nothing else)
data "aws_iam_policy_document" "read_app_data" {
  statement {
    sid       = "ListBucket"
    actions   = ["s3:ListBucket"]
    resources = [aws_s3_bucket.app_data.arn]
  }
  statement {
    sid       = "ReadObjects"
    actions   = ["s3:GetObject"]
    resources = ["${aws_s3_bucket.app_data.arn}/*"]
  }
}

resource "aws_iam_role" "app" {
  name               = "lab5-app-role-${var.student_id}"
  assume_role_policy = data.aws_iam_policy_document.ec2_trust.json
}

resource "aws_iam_role_policy" "read_app_data" {
  name   = "read-app-data"
  role   = aws_iam_role.app.id
  policy = data.aws_iam_policy_document.read_app_data.json
}

resource "aws_iam_instance_profile" "app" {
  name = "lab5-app-profile-${var.student_id}"
  role = aws_iam_role.app.name
}

output "instance_profile" {
  value = aws_iam_instance_profile.app.name
}
```

Here is what each part does:

- **Trust policy** (`ec2_trust`) — controls *who* can assume the role. Only the EC2 service is allowed
- **Permissions policy** (`read_app_data`) — controls *what* the role can do. It can list one bucket and read its objects
- **Instance profile** — the container that attaches an IAM role to an EC2 instance

---

#### Part D: Deploy and Verify

**Step 5 — Initialize with the remote backend.** Run `terraform init`. Confirm the output shows `Successfully configured the backend "s3"!`.

**Step 6 — Apply the configuration.** Run `terraform apply -var="student_id=jd42"` and type `yes` when prompted. Confirm the output shows `Resources: 4 added`.

**Step 7 — Confirm state is stored remotely.** Run `aws s3 ls s3://tfstate-devsecops-jd42/lab5/` and confirm `terraform.tfstate` is listed. Then confirm there is no `terraform.tfstate` file in your `C:\tf-labs\lab5` folder. (The small `.terraform\terraform.tfstate` file only records backend settings, not your resources.)

**Step 8 — Review the role in the IAM Console.** Open **IAM**, click **Roles**, and select `lab5-app-role-jd42`. On the **Permissions** tab, expand `read-app-data` and confirm it allows only `s3:ListBucket` and `s3:GetObject` on your app bucket. On the **Trust relationships** tab, confirm the principal is `ec2.amazonaws.com`.

---

#### Part E: Test State Locking

**Step 9 — Trigger a state lock.** Open two PowerShell windows in `C:\tf-labs\lab5`. In the first window, run `terraform apply -var="student_id=jd42"` and leave it waiting at the `Enter a value:` prompt. In the second window, run `terraform plan -var="student_id=jd42"`.

Confirm the second window fails with `Error acquiring the state lock`. Type `no` in the first window to release the lock.

> **Optional extension:** Attach the AWS managed policy `AmazonSSMManagedInstanceCore` to the role (Session Manager requires it), then add `iam_instance_profile = "lab5-app-profile-jd42"` to `aws_instance.web` in Lab 4 and apply. Connect with **Session Manager** and confirm `aws s3 ls s3://devsecops-appdata-jd42` succeeds while `aws s3 ls s3://tfstate-devsecops-jd42` returns **AccessDenied**.

---

#### Part F: Destroy the Resources

**Step 10 — Destroy the resources.** Run `terraform destroy -var="student_id=jd42"` and type `yes` when prompted.

> **Note:** Keep the `tfstate-devsecops-jd42` bucket. You will delete it in Lab 7.

### Deliverables

Submit a screenshot showing:

- Step 5 — the `terraform init` output showing the S3 backend was configured
- Step 7 — the `aws s3 ls` output showing the state file stored in S3
- Step 8 — the IAM role's Permissions tab showing the `read-app-data` policy
- Step 8 — the IAM role's Trust relationships tab showing `ec2.amazonaws.com`
- Step 9 — the second PowerShell window showing `Error acquiring the state lock`

### Review Questions

1. What problems does a remote backend with state locking solve for a team of engineers?
2. Why is the state bucket created with the AWS CLI instead of in the same Terraform configuration that uses it?
3. What is the difference between an IAM role's trust policy and its permissions policy?
4. Why is `s3:ListBucket` granted on the bucket ARN while `s3:GetObject` is granted on `arn/*`?
5. If the policy used `"s3:*"` on `"*"`, what principle would it violate, and what could an attacker do if the EC2 instance were compromised?

---

## Lab 6: Scanning Terraform Code for Security Misconfigurations

### Objective

In this lab, you will use static analysis tools to find security misconfigurations in Terraform code before it is deployed. You will scan an intentionally insecure configuration with Checkov and Trivy, fix the findings, and add the scan to a GitHub Actions pipeline so insecure code is blocked automatically. By the end of this lab, you will understand how Infrastructure as Code scanning supports a shift-left DevSecOps approach.

### Prerequisites

- Labs 1 through 5 are complete
- Python 3.9 or later is installed. If it is missing, run `winget install --id Python.Python.3.12 --exact`
- A GitHub account (for Part D only)

### Background: What is IaC Scanning?

Infrastructure as Code scanners read `.tf` files and compare each resource against a library of security rules, such as "S3 buckets must be encrypted" or "security groups must not allow SSH from 0.0.0.0/0". They run without touching AWS, so they can check code on a laptop or in a pull request before anything is deployed. This is called **shifting left**: finding problems earlier, when they are cheapest to fix.

| Tool | Description |
| --- | --- |
| `terraform validate` | Checks syntax and references only. It does not check security |
| Checkov | Open-source scanner from Prisma Cloud with over 1,000 built-in policies |
| Trivy | Open-source scanner from Aqua Security. The former tfsec project is now part of Trivy |

### Instructions

#### Part A: Install the Scanners

**Step 1 — Install Checkov.** In PowerShell, run:

```powershell
pip install checkov
checkov --version
```

**Step 2 — Install Trivy.** Download the Windows zip file from the [Trivy GitHub releases page](https://github.com/aquasecurity/trivy/releases). Extract `trivy.exe` to `C:\Tools\Trivy`, and add that folder to your `PATH` as in Lab 1, Step 2, Method 3. Alternatively, if you use Chocolatey, run `choco install trivy -y`. Close and reopen PowerShell, then run:

```powershell
trivy --version
```

---

#### Part B: Create an Insecure Configuration

**Step 3 — Create the insecure main.tf.** Create the folder `C:\tf-labs\lab6` and add `main.tf` with the following content. This configuration is insecure on purpose — **do not apply it**.

```hcl
provider "aws" {
  region = "us-east-1"
}

resource "aws_s3_bucket" "bad" {
  bucket = "devsecops-insecure-example"
}

resource "aws_security_group" "bad" {
  name = "wide-open"
  ingress {
    from_port   = 22
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

resource "aws_instance" "bad" {
  ami                    = "ami-12345678"
  instance_type          = "t3.micro"
  vpc_security_group_ids = [aws_security_group.bad.id]
}
```

**Step 4 — Run the built-in Terraform checks.** Run the following commands:

```powershell
terraform init -backend=false
terraform fmt -check
terraform validate
```

Confirm `terraform validate` reports `Success! The configuration is valid.`

> **Note:** Valid code is not the same as secure code. `validate` checks that the configuration is written correctly, not that it is safe.

---

#### Part C: Scan and Remediate

**Step 5 — Scan with Checkov.** Run `checkov -d .`. For each **FAILED** check, record the check ID (for example, `CKV_AWS_24`), the resource it flags, and the description.

**Step 6 — Scan with Trivy.** Run `trivy config .`. Record each finding's ID and severity (**CRITICAL**, **HIGH**, **MEDIUM**, or **LOW**).

Expect findings similar to the following:

- The security group allows SSH (port 22) from `0.0.0.0/0`
- The EC2 instance does not require IMDSv2
- The EC2 root volume is not encrypted
- The S3 bucket has no public access block, versioning, or encryption

**Step 7 — Fix the findings.** Edit `main.tf` using the secure patterns you built in Lab 3 (S3 controls) and Lab 4 (security group restricted to `my_ip`, IMDSv2, encrypted root volume). Rerun the scan until all six required checks below pass, then rerun `trivy config .` and record which findings remain:

```powershell
checkov -d . --check CKV_AWS_24,CKV_AWS_23,CKV_AWS_79,CKV_AWS_8,CKV2_AWS_6,CKV_AWS_21
```

| Check | Requirement |
| --- | --- |
| `CKV_AWS_24` | No security group allows SSH (port 22) from `0.0.0.0/0` |
| `CKV_AWS_23` | Every security group and rule has a description |
| `CKV_AWS_79` | The EC2 instance requires IMDSv2 |
| `CKV_AWS_8` | The EC2 root volume is encrypted |
| `CKV2_AWS_6` | The S3 bucket has a public access block |
| `CKV_AWS_21` | The S3 bucket has versioning enabled |

> **Note:** A full `checkov -d .` scan still reports lower-priority findings such as S3 access logging, KMS encryption, and cross-region replication. Real teams decide which checks are mandatory; this lab requires the six above, and the CI pipeline in Part D enforces the same list.

**Step 8 — Document an accepted risk.** If a finding does not apply to your use case, suppress it with a written reason so the decision is visible in code review. For example:

```hcl
resource "aws_s3_bucket" "logs" {
  # checkov:skip=CKV_AWS_144: cross-region replication not required for lab data
  bucket = "devsecops-logs-jd42"
}
```

**Step 9 — Scan your earlier labs.** Run `checkov -d C:\tf-labs\lab3` and `checkov -d C:\tf-labs\lab4`. Note which findings remain and decide whether each should be fixed or accepted with a documented skip.

---

#### Part D: Add Scanning to a CI Pipeline

**Step 10 — Push your lab code to GitHub.** Create a new private GitHub repository and push the contents of `C:\tf-labs` to it, so `lab3`, `lab4`, and `lab6` are folders at the repository root. Make sure `.gitignore` excludes `.terraform/`, `*.tfstate`, and `*.tfvars` before you commit.

**Step 11 — Create the workflow file.** In the repository, create `.github/workflows/terraform-security.yml` with the following content:

```yaml
name: terraform-security
on: [pull_request]

jobs:
  scan:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: hashicorp/setup-terraform@v3
      - name: Format check
        run: terraform fmt -check -recursive
      - name: Validate
        working-directory: lab3
        run: terraform init -backend=false && terraform validate
      - name: Checkov (required security checks)
        run: |
          pip install checkov
          checkov -d . --framework terraform \
            --skip-path lab2 --skip-path lab5 \
            --check CKV_AWS_24,CKV_AWS_23,CKV_AWS_79,CKV_AWS_8,CKV2_AWS_6,CKV_AWS_21
```

**Step 12 — Test the pipeline.** Create a new branch, add the insecure `lab6/main.tf` from Step 3, and open a pull request. Confirm the **Checkov (required security checks)** step fails and the pull request shows a red check. Then push the fixed version and confirm the check turns green.

### Deliverables

Submit a screenshot showing:

- Step 4 — the `terraform validate` output reporting the insecure code as valid
- Step 5 — the Checkov output showing the FAILED checks
- Step 6 — the Trivy output showing findings and their severities
- Step 7 — the Checkov and Trivy output after remediation
- Step 12 — the GitHub pull request showing the failed check, then the passing check after the fix

### Review Questions

1. Why did `terraform validate` pass even though the configuration contained serious security problems?
2. What does "shift left" mean, and why is it cheaper to fix a misconfiguration in a pull request than in production?
3. What are the risks of overusing `checkov:skip`, and who in a team should approve a skip?
4. Why is running scanners on every pull request more effective than asking engineers to run them manually?
5. Besides IaC scanning, what other security checks could be added to a Terraform CI/CD pipeline (for example, before or after `terraform plan`)?

---

## Lab 7: Cleaning Up AWS Resources

### Objective

In this lab, you will confirm that every AWS resource created during Labs 2 through 6 has been destroyed, delete the remote state bucket, and remove credentials you no longer need. Cleaning up resources is an essential practice to avoid unnecessary charges and to reduce your account's attack surface.

### Prerequisites

- All previous labs are complete
- You have sufficient IAM permissions to delete the resources listed below

### Important Warning

> **Caution:** All deletions in this lab are permanent and cannot be undone. Double-check each resource name before confirming any deletion. Deleting the state bucket also deletes the record of what Terraform manages — run `terraform destroy` in every lab folder **before** deleting it.

### Resources to Delete

| Resource | Name | Created in |
| --- | --- | --- |
| S3 Bucket | `devsecops-lab2-<yourinitials>-2026` | Lab 2 |
| S3 Bucket and controls | `devsecops-<student_id>-dev` | Lab 3 |
| VPC, subnet, internet gateway, route table | `lab4-vpc`, `lab4-public`, `lab4-igw`, `lab4-public-rt` | Lab 4 |
| Security Group | `lab4-web-sg` | Lab 4 |
| EC2 Instance | `lab4-web` | Lab 4 |
| S3 Bucket | `devsecops-appdata-<student_id>` | Lab 5 |
| IAM Role, Policy, Instance Profile | `lab5-app-role-<student_id>`, `read-app-data`, `lab5-app-profile-<student_id>` | Lab 5 |
| S3 State Bucket | `tfstate-devsecops-<student_id>` | Lab 5 (created with the AWS CLI) |
| IAM Access Key | Your lab user's access key | Lab 1 |

### Instructions

#### Part A: Destroy Terraform-Managed Resources

**Step 1 — Run terraform destroy in each lab folder.** For each folder below, open PowerShell in that folder and run `terraform destroy`, adding the same `-var` values you used for `apply`. Type `yes` when prompted.

```powershell
cd C:\tf-labs\lab2; terraform destroy
cd C:\tf-labs\lab3; terraform destroy
cd C:\tf-labs\lab4; terraform destroy -var="my_ip=203.0.113.10/32"
cd C:\tf-labs\lab5; terraform destroy -var="student_id=jd42"
```

Confirm each command reports either `Destroy complete!` or `No changes. No objects need to be destroyed.`

**Step 2 — Confirm the state files are empty.** In each folder, run `terraform state list`. Confirm the command returns no output.

> **Note:** Lab 6 was never applied, so it has no resources to destroy.

---

#### Part B: Delete the Remote State Bucket

**Step 3 — Empty the state bucket.** Open the **S3** console and select `tfstate-devsecops-<student_id>`. Click **Empty**, type `permanently delete`, and click **Empty**. Because versioning is enabled, this also removes all previous versions of the state file.

**Step 4 — Delete the state bucket.** Select the bucket again, click **Delete**, type the bucket name, and click **Delete bucket**.

---

#### Part C: Remove Lab Credentials

**Step 5 — Deactivate and delete the access key.** If you no longer need the access key created in Lab 1, open **IAM**, click **Users**, select your lab user, and open the **Security credentials** tab. Under **Access keys**, choose **Actions → Deactivate**, then **Actions → Delete**, and confirm.

**Step 6 — Remove the local credentials.** Open `%USERPROFILE%\.aws\credentials` in a text editor and delete the profile for the access key you deleted.

---

#### Part D: Verify All Resources Have Been Deleted

**Step 7 — Perform a final check.** Confirm that no lab resources remain in the `us-east-1` region:

| Service | Verification |
| --- | --- |
| S3 | No bucket starting with `devsecops-` or `tfstate-devsecops-` remains |
| EC2 | No `lab4-web` instance remains, or it shows **Terminated** |
| VPC | No `lab4-vpc` VPC remains |
| EC2 → Security Groups | No `lab4-web-sg` security group remains |
| IAM → Roles | No `lab5-app-role-*` role remains |
| IAM → Users | Your lab access key is deleted or deactivated |

> **Tip:** Open **Billing and Cost Management → Bills** the next day and confirm no unexpected charges appear.

### Troubleshooting Common Issues

| Symptom | Fix |
| --- | --- |
| `terraform : The term 'terraform' is not recognized` | Close and reopen the terminal. Check the folder is on `PATH` with `$env:Path -split ';'` |
| `winget` is not recognized | Install **App Installer** from the Microsoft Store |
| `No valid credential sources found` | Run `aws configure` again, then `aws sts get-caller-identity` |
| `BucketAlreadyExists` | S3 names are global. Change the bucket name or `student_id` |
| `Error acquiring the state lock` after a crash | Confirm nobody else is running Terraform, then run `terraform force-unlock <LOCK_ID>` |
| Antivirus blocks `terraform.exe` or provider downloads | Add an exclusion for `C:\Tools\Terraform` and the project's `.terraform` folder |
| `destroy` fails with `DependencyViolation` on a VPC | Wait one minute for the EC2 network interface to be released, then run `destroy` again |

### Recommended .gitignore

```text
.terraform/
*.tfstate
*.tfstate.*
*.tfvars
crash.log
```

> **Note:** Do commit `.terraform.lock.hcl`, so everyone on the team uses the same provider versions.

### Deliverables

Submit a screenshot showing:

- Step 1 — the `terraform destroy` output for Lab 4 showing `Destroy complete!`
- Step 3 — the S3 Empty bucket confirmation for the state bucket
- Step 4 — the S3 Delete bucket confirmation for the state bucket
- Step 5 — the IAM Security credentials tab showing the access key deleted or deactivated
- Step 7 — the S3, EC2, VPC, and IAM consoles confirming no lab resources remain

### Review Questions

1. Why is it important to clean up AWS resources after completing a lab or proof of concept?
2. Why must `terraform destroy` be run before the remote state bucket is deleted? What would happen if the order were reversed?
3. What security risk does an unused but active IAM access key create?
4. Why can `terraform destroy` sometimes fail on the first attempt with a dependency error, and how does Terraform decide the order in which to delete resources?
5. What AWS services or features could you use to set up cost alerts or detect forgotten resources automatically?

### Sources

- [HashiCorp: Install Terraform](https://developer.hashicorp.com/terraform/install)
- [KodeKloud: Install Terraform on Windows](https://notes.kodekloud.com/docs/HashiCorp-Certified-Terraform-Associate-004/Preparing-Your-Environment/Install-Terraform-on-Microsoft-Windows/page)
- [OneUptime: How to install Terraform on Windows step by step](https://oneuptime.com/blog/post/2026-02-23-how-to-install-terraform-on-windows-step-by-step/view)
