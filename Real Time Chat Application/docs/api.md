# api.md

## WebSocket Event Payload (JSON)

### 1. Client -> Server: Send Message
{
  "type": "send_message",
  "payload": {
    "sender_id": "uuid",
    "group_id": "uuid",
    "content": "string",
    "client_timestamp": 1690000000
  }
}

### 2. Server -> Client: Receive Message
{
  "type": "new_message",
  "payload": {
    "message_id": "uuid",
    "sender_id": "uuid",
    "group_id": "uuid",
    "content": "string",
    "server_sequence_id": 1690000005
  }
}