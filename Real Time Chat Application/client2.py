import asyncio
import websockets
import json

async def chat_client():
    uri = "ws://127.0.0.1:8080"
    
    try:
        async with websockets.connect(uri) as ws:
            print("Connected.")
            
            # 1. Register
            await ws.send(json.dumps({
                "type": "register",
                "username": "tester3",
                "email": "test3@test.com",
                "password": "password123"
            }))
            print(f"Register: {await ws.recv()}")
            
            # 2. Login
            await ws.send(json.dumps({
                "type": "login",
                "email": "test3@test.com",
                "password": "password123"
            }))
            login_resp = json.loads(await ws.recv())
            print(f"Login: {login_resp}")
            
            token = login_resp.get("token")
            if not token:
                print("No token received.")
                return
            
            # 3. Auth
            await ws.send(json.dumps({
                "type": "auth",
                "token": token
            }))
            auth_resp = await ws.recv()
            print(f"Auth: {auth_resp}")

            # 4. Send Message
            test_msg = {
                "type": "send_message",
                "payload": {
                    "conversation_id": "11111111-1111-1111-1111-111111111111",
                    "content": "hello from tester3"
                }
            }
            await ws.send(json.dumps(test_msg))
            print("Test message sent.")

            # Keep alive. Wait for messages.
            while True:
                print(f"Message: {await ws.recv()}")
                
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    asyncio.run(chat_client())