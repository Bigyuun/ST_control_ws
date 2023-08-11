import socket

# 서버 설정
HOST = '127.0.0.1'
PORT = 12345

def server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen()

    print("서버 대기중...")

    conn, addr = server_socket.accept()
    print(f"연결됨: {addr}")

    while True:
        received_data = conn.recv(512).decode()
        print(f"수신한 메시지: {received_data}")

        if received_data == 'exit':
            break

    conn.close()

if __name__ == '__main__':
    server()