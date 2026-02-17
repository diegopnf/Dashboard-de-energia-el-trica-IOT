output "dynamodb_table_name" {
  value = aws_dynamodb_table.telemetria.name
}

output "iot_thing_name" {
  value = aws_iot_thing.esp32c6_medidor.name
}

output "s3_bucket_ota" {
  value = aws_s3_bucket.firmware.id
}