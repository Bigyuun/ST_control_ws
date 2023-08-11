import socket

# 클라이언트 설정
HOST = '127.0.0.1'
PORT = 12345

def client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))

    while True:
        message = input("송신할 메시지 입력 (끝내려면 'exit' 입력): ")
        client_socket.send(message.encode())

        if message == 'exit':
            break

    client_socket.close()

if __name__ == '__main__':
    client()
