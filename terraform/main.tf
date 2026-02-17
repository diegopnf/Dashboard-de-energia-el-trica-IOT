# 1. Tabela DynamoDB para Telemetria
resource "aws_dynamodb_table" "telemetria" {
  name           = "${var.project_name}-dados"
  billing_mode   = "PAY_PER_REQUEST"
  hash_key       = "device_id"
  range_key      = "timestamp"

  attribute {
    name = "device_id"
    type = "S"
  }

  attribute {
    name = "timestamp"
    type = "N"
  }

  tags = {
    Project = var.project_name
  }
}

# 2. Bucket S3 para Firmware (OTA)
resource "aws_s3_bucket" "firmware" {
  bucket = "${var.project_name}-firmware-repo"
}

# 3. AWS IoT Thing (O Objeto no IoT Core)
resource "aws_iot_thing" "esp32c6_medidor" {
  name = var.device_id
}

# 4. IoT Policy (Princípio do Privilégio Mínimo)
resource "aws_iot_policy" "esp32_policy" {
  name = "${var.project_name}-policy"

  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Action   = ["iot:Connect"]
        Effect   = "Allow"
        Resource = ["*"]
      },
      {
        Action   = ["iot:Publish"]
        Effect   = "Allow"
        Resource = ["arn:aws:iot:${var.aws_region}:*:topic/energia/${var.device_id}/dados"]
      }
    ]
  })
}