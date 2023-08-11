import socket
import threading

# 송신부 설정
SEND_HOST = '127.0.0.1'
SEND_PORT = 12345

# 수신부 설정
RECV_HOST = '127.0.0.1'
RECV_PORT = 12345

def sender():
    sender_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sender_socket.connect((SEND_HOST, SEND_PORT))

    while True:
        message = input("송신할 메시지 입력 (끝내려면 'exit' 입력): ")
        sender_socket.send(message.encode())

        if message == 'exit':
            break

    sender_socket.close()

def receiver():
    receiver_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    receiver_socket.bind((RECV_HOST, RECV_PORT))
    receiver_socket.listen()

    print("수신 대기중...")

    conn, addr = receiver_socket.accept()
    print(f"연결됨: {addr}")

    while True:
        received_data = conn.recv(1024).decode()
        print(f"수신한 메시지: {received_data}")

        if received_data == 'exit':
            break

    conn.close()

# 스레드 생성 및 실행
send_thread = threading.Thread(target=sender)
recv_thread = threading.Thread(target=receiver)

send_thread.start()
recv_thread.start()

send_thread.join()
recv_thread.join()

print("프로그램 종료")